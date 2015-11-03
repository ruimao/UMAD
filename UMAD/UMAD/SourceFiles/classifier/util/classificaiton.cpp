/**
 *@ Filename: classificaiton.cpp
 *@ Implement training model and the test of classification algorithm
 *@ Author: Ping Li
 *@ Version: 2014-09-28
*/

#include "../../../HeaderFiles/metricdata/MetricData.h"
#include "../../../HeaderFiles/metricdata/DoubleVector.h"
#include "../../../HeaderFiles/metricdata/Image.h"
#include "../../../HeaderFiles/metricdata/StringObject.h"
#include "../../../HeaderFiles/metricdata/DNA.h"
#include "../../../HeaderFiles/metricdata/Peptide.h"

#include "../../../HeaderFiles/metricdistance/MetricDistance.h"
#include "../../../HeaderFiles/metricdistance/EuclideanDistance.h"
#include "../../../HeaderFiles/metricdistance/EditDistance.h"
#include "../../../HeaderFiles/metricdistance/ImageMetric.h"
#include "../../../HeaderFiles/metricdistance/DNAMetric.h"
#include "../../../HeaderFiles/metricdistance/PeptideMetric.h"

#include "../../../HeaderFiles/index/PivotSelectionMethod.h"
#include "../../../HeaderFiles/index/FFTPivotSelectionMethod.h"
#include "../../../HeaderFiles/index/PCAOnFFT.h"
#include "../../../HeaderFiles/index/IncrementalSelection.h"

#include "../../../HeaderFiles/classifier/Train_Knn.h"
#include "../../../HeaderFiles/classifier/Test_Knn.h"
#include "../../../HeaderFiles/classifier/util/NavieBayes.h"
#include "../../../HeaderFiles/classifier/Train_NavieBayes.h"
#include "../../../HeaderFiles/classifier/Test_NavieBayes.h"

#include "../../../HeaderFiles/classifier/util/DatasetInMetricSpace.h"
#include "../../../HeaderFiles/classifier/util/MetricDataFormat.h"
#include "../../../HeaderFiles/classifier/util/ReadLabel.h"
#include "../../../HeaderFiles/classifier/Train.h"
#include "../../../HeaderFiles/classifier/Test.h"

#include "../../../HeaderFiles//metricdistance/CountedMetric.h"

#include<memory>
#include<iostream>
#include<string>
#include<vector>

using namespace std;

extern void joinCharArray(char*&dest,char *sor);

void Train(char *dataType,char *disfun,char *pivotSelectionMethod,int numPivot,char *classifyMethod,char *trainDataFileName,int initialSize,int dim,char *pivotsAndTrainModelFileName)
{
	char *newTrainDataFileName="SourceFiles/util/data/";
	joinCharArray(newTrainDataFileName,trainDataFileName);
	char *newPivotsAndTrainModelFileName="SourceFiles/util/result/";
	joinCharArray(newPivotsAndTrainModelFileName,pivotsAndTrainModelFileName);

	vector<shared_ptr<CMetricData> > *traindata=0; //store the traindata
	if(strcmp(dataType,"vector")==0)
	{
		traindata=CDoubleVector::loadData(newTrainDataFileName,initialSize,dim);
	}
	else if(strcmp(dataType,"string")==0)
	{
		traindata=CStringObject::loadData(newTrainDataFileName,initialSize);
	}
	else if(strcmp(dataType,"image")==0)
	{
		traindata=CImage::loadData(newTrainDataFileName,initialSize,dim);
	}
	else if(strcmp(dataType,"dna")==0)
	{
		traindata=CDNA::loadData(newTrainDataFileName,initialSize,dim);
	}
	else if(strcmp(dataType,"peptide")==0)
	{
		traindata=CPeptide::loadData(newTrainDataFileName,initialSize,dim);
	}

	CReadLabel readTrainDataLabel;
	vector <string> trainDataLabel; //store the traindata label
	trainDataLabel=readTrainDataLabel.loadLabel(newTrainDataFileName,initialSize);

	CMetricDistance *metric=0;
	if(strcmp(disfun,"EuclideanDistance")==0)
	{
		metric = new CEuclideanDistance;
	}
	else if(strcmp(disfun,"EditDistance")==0)
	{
		metric = new CEditDistance;
	}
	else if(strcmp(disfun,"ImageMetric")==0)
	{
		metric = new CImageMetric;
	}
	else if(strcmp(disfun,"DNAMetric")==0)
	{
		metric = new CDNAMetric;
	}
	else if(strcmp(disfun,"PeptideMetric")==0)
	{
		metric = new CPeptideMetric;
	}

	CountedMetric *cmetric=new CountedMetric(metric);
	CPivotSelectionMethod *pivotselectionmethod=0;
	if(strcmp(pivotSelectionMethod,"fft")==0)
	{
		pivotselectionmethod = new CFFTPivotSelectionMethod;
	}
	else if(strcmp(pivotSelectionMethod,"pcaonfft")==0)
	{
		pivotselectionmethod = new CPCAOnFFT(2);
	}
	else if(strcmp(pivotSelectionMethod,"incremental")==0)
	{
		pivotselectionmethod = new CIncrementalSelection(1,2);
	}
	

	if(strcmp(classifyMethod,"knn")==0)
	{
		CTrain_Knn train_knn;
		train_knn.TrainModel(classifyMethod,traindata,trainDataLabel,cmetric,pivotselectionmethod,numPivot,newPivotsAndTrainModelFileName,dim);
	}
	else if(strcmp(classifyMethod,"naviebayes")==0)
	{
		CTrain_NavieBayes train_naviebayes;
		train_naviebayes.TrainModel(classifyMethod,traindata,trainDataLabel,cmetric,pivotselectionmethod,numPivot,newPivotsAndTrainModelFileName,newTrainDataFileName,dim);
	}
	
}

void Test(char *dataType,char *disfun,char *classifyMethod,char *testDataFileName,int finalSize,int dim,char *pivotsAndTrainModelFileName,char *testModelFileName,int status,int k)
{
	char *newTestDataFileName="SourceFiles/util/data/";
	joinCharArray(newTestDataFileName,testDataFileName);
	char *newPivotsAndTrainModelFileName="SourceFiles/util/result/";
	joinCharArray(newPivotsAndTrainModelFileName,pivotsAndTrainModelFileName);
	char *newTestModelFileName="SourceFiles/util/result/";
	joinCharArray(newTestModelFileName,testModelFileName);

	vector<shared_ptr<CMetricData> > *testdata=0; //store the testdata
	if(strcmp(dataType,"vector")==0)
	{
		testdata=CDoubleVector::loadData(newTestDataFileName,finalSize,dim);
	}
	else if(strcmp(dataType,"string")==0)
	{
		testdata=CStringObject::loadData(newTestDataFileName,finalSize);
	}
	else if(strcmp(dataType,"image")==0)
	{
		testdata=CImage::loadData(newTestDataFileName ,finalSize, dim);
	}
	else if(strcmp(dataType,"dna")==0)
	{
		testdata=CDNA::loadData(newTestDataFileName,finalSize,dim);
	}
	else if(strcmp(dataType,"peptide")==0)
	{
		testdata=CPeptide::loadData(newTestDataFileName,finalSize,dim);
	}

	CReadLabel readTestDataLabel;
	vector <string> testDataLabel;
	testDataLabel=readTestDataLabel.loadLabel(newTestDataFileName,finalSize);

	CMetricDistance *metric=0;
	if(strcmp(disfun,"EuclideanDistance")==0)
	{
		metric = new CEuclideanDistance;
	}
	else if(strcmp(disfun,"EditDistance")==0)
	{
		metric = new CEditDistance;
	}
	else if(strcmp(disfun,"ImageMetric")==0)
	{
		metric = new CImageMetric;
	}
	else if(strcmp(disfun,"DNAMetric")==0)
	{
		metric = new CDNAMetric;
	}
	else if(strcmp(disfun,"PeptideMetric")==0)
	{
		metric = new CPeptideMetric;
	}
	
	CountedMetric *cmetric=new CountedMetric(metric);
	if(strcmp(classifyMethod,"knn")==0)
	{
		CTest_Knn test_knn;
		test_knn.TestModel(classifyMethod,testdata,testDataLabel,cmetric,newPivotsAndTrainModelFileName,newTestModelFileName,status,k);
	}
	else if(strcmp(classifyMethod,"naviebayes")==0)
	{
		CTest_NavieBayes test_naviebayes;
		test_naviebayes.TestModel(classifyMethod,testdata,testDataLabel,cmetric,newPivotsAndTrainModelFileName,newTestModelFileName,status);
	}
}