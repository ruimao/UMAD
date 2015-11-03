#include "../../HeaderFiles/metricdata/DoubleVector.h"
//#include  "../../HeaderFiles/metricdata/TandemSpectra.h"
#include "../../HeaderFiles/metricdistance/MetricDistance.h"
#include "../../HeaderFiles/metricdistance/EuclideanDistance.h"
#include "../../HeaderFiles/index/Index.h"
#include "../../HeaderFiles/index/MVPIndex.h"
#include "../../HeaderFiles/index/ListIndex.h"
#include "../../HeaderFiles/index/PartitionMethod.h"
#include "../../HeaderFiles/index/PivotSelectionMethod.h"
#include "../../HeaderFiles/index/PCAOnFFT.h"
#include "../../HeaderFiles/index/FFTPivotSelectionMethod.h"
#include "../../HeaderFiles/index/IncrementalSelection.h"
#include "../../HeaderFiles/index/BalancePartitionMethod.h"
#include "../../HeaderFiles/index/query/Query.h"
#include "../../HeaderFiles/index/query/RangeQuery.h"
#include "../../HeaderFiles/metricdistance/DNAMetric.h"
#include "../../HeaderFiles//metricdistance/CountedMetric.h"
#include "../../HeaderFiles/metricdistance/EditDistance.h"
#include "../../HeaderFiles/metricdistance/ImageMetric.h"
//#include "../../HeaderFiles/metricdistance/MSMSMetric.h"
#include "../../HeaderFiles/metricdistance/RNAMetric.h"
#include "../../HeaderFiles/metricdistance/PeptideMetric.h"
#include "../../HeaderFiles/index/query/SearchTask.h"
#include "../../HeaderFiles/metricdata/Image.h"
#include "../../HeaderFiles/metricdata/DNA.h"
#include "../../HeaderFiles/metricdata/Peptide.h"
#include "../../HeaderFiles/metricdata/StringObject.h"
#include "../../HeaderFiles/util/MemMonitor.h"
#include "../../HeaderFiles/index/ExcludedMiddleForest.h"
#include "../../HeaderFiles/index/ExcludedMiddlePartitionMethod.h"
#include "../../HeaderFiles/cluster/PivotWisePartitionMethod.h"

#include<vector>
using namespace std;

#ifdef __GNUC__
#include <sys/timeb.h>
#include "/usr/include/sys/times.h"
#include <memory>
#else
#ifdef _WIN32
#include <Windows.h>
#include <psapi.h>
#include <memory>
#pragma comment(lib,"psapi.lib")
#endif
#endif

extern void joinCharArray(char*&dest, const char *sor);

char * produceFileName(char *filename, int kind)
{
	char *newFileName="../../../SourceFiles/util/";
#ifdef _WIN32
	newFileName = "./SourceFiles/util/";
#endif

	switch(kind){
	case 0:joinCharArray(newFileName,"data/") ;break;//data
	case 1:joinCharArray(newFileName,"database/") ;break;//database
	case 2:joinCharArray(newFileName,"result/") ;break;//result
	default :break;
	}
	if(filename!=NULL) joinCharArray(newFileName,filename);
	return newFileName ;
}

void loadDataByType(char * dataType, vector<shared_ptr<CMetricData> > *&dataList, char *dataFileName,int size,int dim,int fragmentLength)
{
	if(!strcmp(dataType,"vector"))
	{
		CObjectFactory factory();
		CObjectFactory::objectRegister("vector",CDoubleVector::getConstructor());
		
		dataList=CDoubleVector::loadData(dataFileName,size,dim);
	}
	else if(!strcmp(dataType,"spectra"))
	{
		//rawData = CTandemSpectra::loadData(newDataFileName,size);
		return ;
	}
	else if(!strcmp(dataType,"string"))
	{
		CObjectFactory factory();
		CObjectFactory::objectRegister("string",CStringObject::getConstructor());
		dataList = CStringObject::loadData(dataFileName,size);
	}
	else if(!strcmp(dataType,"rna"))
	{
		dataList = CRNA::loadData(dataFileName, size,fragmentLength);
	}
	else if(!strcmp(dataType,"image"))
	{
		CObjectFactory factory();
		CObjectFactory::objectRegister("image",CImage::getConstructor());
		dataList = CImage::loadData(dataFileName ,size, dim);
	}
	else if(!strcmp(dataType,"dna"))
	{
		CObjectFactory factory();
		CObjectFactory::objectRegister("dna",CDNA::getConstructor());
		dataList = CDNA::loadData(dataFileName,size,fragmentLength);
	}
	else if(!strcmp(dataType,"peptide"))
	{
		CObjectFactory factory();
		CObjectFactory::objectRegister("peptide",CPeptide::getConstructor());
		dataList = CPeptide::loadData(dataFileName,size,fragmentLength);
	}
}


void getMetricByType(CMetricDistance *&metric,char *dataType)
{
	if(!strcmp(dataType,"vector")) 
	{
		metric = new CEuclideanDistance;
	}
	else if(!strcmp(dataType,"spectra")) 
	{
		//metric = new CMSMSMetric;
	}
	else if(!strcmp(dataType,"string")) 
	{
		metric = new CEditDistance;
	}
	else if(!strcmp(dataType,"rna")) 
	{
		metric = new CRNAMetric ;
	}
	else if(!strcmp(dataType,"image")) 
	{
		metric = new CImageMetric ;	
	}
	else if(!strcmp(dataType,"dna")) 
	{
		metric = new CDNAMetric ;
	}
	else if(!strcmp(dataType,"peptide"))
	{
		metric = new CPeptideMetric;
	}
}


void getPivotSelectionMethod(char *pivotSelectionMethod,CPivotSelectionMethod *&psm,int fftscale,int setA,int setN)
{
	if(!strcmp(pivotSelectionMethod,"fft"))
	{
		psm=new CFFTPivotSelectionMethod;
	}
	else if(!strcmp(pivotSelectionMethod,"incremental"))
	{
		psm = new CIncrementalSelection(setA,setN) ;
	}
	else if(!strcmp(pivotSelectionMethod,"pcaonfft"))
	{
		psm = new CPCAOnFFT(fftscale);
	}
	else
	{
		cout<<"invalid pivot selection method"<<endl;
	}
}

void getPartitionMethod(char *partitionMethod,CPartitionMethod *&pm)
{
	if(!strcmp(partitionMethod,"balanced"))
	{
		pm=new CBalancePartitionMethod;
	}
	else if (!strcmp(partitionMethod,"ExcludedMiddle"))
	{
		pm=new CExcludedMiddlePartitionMethod;
	}
	else if (!strcmp(partitionMethod,"PivotWise"))
	{
		pm=new PivotWisePartitionMethod;
	}
}


void getIndexType(CMetricDistance *metric,CountedMetric *cmetric,vector<shared_ptr<CMetricData> > *rawData,CPivotSelectionMethod *psm,CPartitionMethod *pm,CIndex *&index,char * dataType,char *indexType,char *newIndexName,int &numPivot,int &singlePivotFanout,int &maxLeafSize,double MF_maxRadius,double MF_middleProportion,int buildMode,int bThreadNum,double trisectionRadius,char* selectOptimalPivots,char* partitionByOnePivot)
{
	if (!strcmp(indexType,"MVPIndex"))
	{
		index=new CMVPIndex(*rawData,cmetric,psm,pm,numPivot,singlePivotFanout,maxLeafSize,buildMode,bThreadNum,trisectionRadius,selectOptimalPivots,partitionByOnePivot);
	}
	else if(!strcmp(indexType,"ListIndex"))
	{
		index=new CListIndex(cmetric);
	}
	else if(!strcmp(indexType,"ForestIndex"))
	{
        MF_middleProportion = 0.3;
		index=new CExcludedMiddleForest(*rawData,cmetric,psm,pm,numPivot,singlePivotFanout,maxLeafSize,0,MF_maxRadius,MF_middleProportion,trisectionRadius,selectOptimalPivots,partitionByOnePivot);
	}

	index->setFileName(newIndexName);
	index->setDataType(dataType);
}