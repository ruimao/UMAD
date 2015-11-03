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

#include <iomanip>
#include <sstream>
#include <float.h>
#include <fstream>

#ifdef __GNUC__
#include <sys/timeb.h>
#include "/usr/include/sys/times.h"
#else

#include <Windows.h>
#include <psapi.h>
#pragma comment(lib,"psapi.lib")
FILETIME kernalStartTime,kernalEndTime;
FILETIME userStartTime,userEndTime;
FILETIME ftDummy;
PROCESS_MEMORY_COUNTERS pmc;
#endif




std::unordered_map<long,std::pair<char*,CIndexNode*> > indexCache;
extern void cacheIndex(int *height,char* indexfilename,string dataType,unordered_map<long,std::pair<char*,CIndexNode*> > *cache,bool all);
extern void cacheForestIndex(int* height,long* &rootAddress,int& numTree,int* treeHeights,char* indexfilename,string dataType,unordered_map<long,std::pair<char*,CIndexNode*> > *cache,bool all);
extern void getMVPIndexStructureInfo(char *&indexName,char *& dataType,char *& outputFileName);
void multiThreadsInInterAndInner(vector<shared_ptr<CMetricData> >* queryObjectsList,char *&dataType,char *&indexFileName,double maximumSearchRadius,double minimumSearchRadius,char *&resultFileName,int dataSize,int cacheHeight,int maxsearchthread,int maxthreadnumberforchildren);
extern void globalMultithreadSearch(vector<shared_ptr<CMetricData> > *rawData,vector<shared_ptr<CMetricData> > *queryObjectsList,char *dataType,char *indexFileName,bool verify,double maxRadius,double minRadius,double step,CountedMetric *&cmetric, char *resultFileName,int dataBaseSize,int cacheHeight,int threadNum);
extern void globalForestMultithreadSearch(vector<shared_ptr<CMetricData> > *rawData,vector<shared_ptr<CMetricData> > *queryObjectsList,char *dataType,char *indexFileName,int& numTree,long* rootAddress,bool verify,double maxRadius,double minRadius,double step,CountedMetric *&cmetric, char *resultFileName,int dataBaseSize,int cacheHeight,int threadNum);
extern void joinCharArray(char*&dest,const char *sor);


extern char * produceFileName(char *filename, int kind);
extern void loadDataByType(char * dataType, vector<shared_ptr<CMetricData> > *&dataList, char *dataFileName,int size,int dim,int fragmentLength);
extern void getMetricByType(CMetricDistance *&metric,char *dataType);
extern void getPivotSelectionMethod(char *pivotSelectionMethod,CPivotSelectionMethod *&psm,int fftscale,int setE,int setC);
extern void getPartitionMethod(char *partitionMethod,CPartitionMethod *&pm);
extern void getIndexType(CMetricDistance *metric,CountedMetric *cmetric,vector<shared_ptr<CMetricData> > *rawData,CPivotSelectionMethod *psm,CPartitionMethod *pm,CIndex *&index,char * dataType,char *indexType,char *newIndexName,int &numPivot,int &singlePivotFanout,int &maxLeafSize,double MF_maxRadius,double MF_middleProportion,int buildMode,int bThreadNum,double trisectionRadius,char* selectOptimalPivots,char* partitionByOnePivot);
void _search(vector<shared_ptr<CMetricData>> *queryList,CountedMetric *cmetric,char *resultFileName,char *indexFileName,char *rawDataFile,int dim,int fragmentLength,bool verify,char *dataType,double maxRadius,double minRadius,double step,int searchMode,int firstQuery,int lastQuery,int dataBaseSize,int cacheHeight,bool cacheAll,int maximumThreadsNumForSearching,int totalSearchingThreadForChildPathSearchingTaskInThreadpool);
void verifySearchResult(vector<shared_ptr<CMetricData> > *&rawData,CMetricDistance *metric,CRangeQuery q, list<shared_ptr<CMetricData> > *queryResultList,ofstream &output);

/*
*
* @This is the main entry point for building a VP Index. It can build more than one index by one time
* 
* The basic commandline options are: 
* @param dataFileName raw data file name. 
* @param numPivot number of pivots in an index node.
* @param setE the size of evaluation set in the incremental pivot selection.
* @param setC the size of candidate set in the incremental pivot selection.
* @param singlePivotFanout fanout of a pivot.
* @param fftscale use for pcaonfft pivot selection method.
* @param pivotSelectionMethod the pivot selection selection method: random, fft, center, pcaonfft, pca.
* @param partitionMethod data partition method: balanced, clusteringkmeans, clusteringboundary.
* @param maxLeafSize maximum number of children in leaf nodes.
* @param dataType data type, one of "protein", "dna", "vector", "image", "msms","string".
* @param initialSize size of smallest database.
* @param finalSize size of largest database.
* @param stepSize step size of databases.
* @param bucket whether bucketing will be used, 1: usebuildingInformationFile
* @param fragmentLength fragment length, only meaningful for sequences.
* @param dim dimension of vector data to load or number of features to be loaded.
* @param maxR maximum radius for partition.
* @param indexType [list,mvptree],the type of the index built later.
* @param fftOpt 0:use old version FFT, 1: use new version FFT but don't choose the third point got from FFT as the second pivot, 2:use new version FFT and choose the third point got from FFT as the second pivot. Default 0.  
*
*/
void batchBulkLoad(char *&dataFileName,int &numPivot,int setE,int setC,int &singlePivotFanout,int fftscale,char *&pivotSelectionMethod,char *&partitionMethod,int &maxLeafSize,char *&dataType,int initialSize,int finalSize,int stepSize,bool bucket,int &fragmentLength,int dim,double maxR,char *&indexType,int fftOpt,char *buildingInformationFile,int buildMode,char *indexName,double MF_maxRadius,double MF_middleProportion,bool putInfo,int bThreadNum,double trisectionRadius,char* selectOptimalPivots,char* partitionByOnePivot)
{
    vector<shared_ptr<CMetricData> > *rawData=0;

    CMetricDistance *metric=0;

    CPivotSelectionMethod *psm=0;

    CPartitionMethod *pm=0;

    CIndex *index =0;

    int indexNum = (finalSize - initialSize) / stepSize + 1;

    double *buildTimes=new double[indexNum];

    double *loadDataTimes=new double[indexNum];

    int *distanceCalculatedTimes=new int[indexNum];


#ifdef _WIN32
    char *newDataFileName="./SourceFiles/util/data/";
#else
    char *newDataFileName="../data/";
#endif

    joinCharArray(newDataFileName,dataFileName);


#ifdef _WIN32
    char *newIndexName="./SourceFiles/util/database/";
#else
    char *newIndexName="../database/";
#endif


    joinCharArray(newIndexName,indexName);

    //cout<<__FILE__<<__LINE__<<" build index file name:"<<newIndexName<<endl;


#ifdef _WIN32
    char *resultFileName="./SourceFiles/util/result/";
	
#else
    char *resultFileName="../result/";
#endif


    joinCharArray(resultFileName,buildingInformationFile);

    ofstream output(resultFileName,ofstream::app);

    int distanceFunctionCounterBeforSearch=0,distanceFunctionCounterAfterSearch=0;

    getMetricByType(metric,dataType);

    CountedMetric *cmetric=new CountedMetric(metric);

    for (int size = initialSize, i = 0; (size <= finalSize) & (i < indexNum); size += stepSize, i++)
    {
		//update index name, append "size" into name, in order to partition index files

		/*   make "size" from int to string  */
		stringstream ssTemp;
		string sizeTemp;
		ssTemp<<size;
		ssTemp>>sizeTemp;

		string indexNameTemp = "";
		indexNameTemp.assign(newIndexName);//"newIndexName" is a dynamic space, must copy it to another string, so as to change file name each while
		indexNameTemp += "_datasize_" + sizeTemp; // append "datasize"
		char * currentIndexName = (char *)indexNameTemp.c_str();//transfer to "char *" type, because the need of below function call


#ifdef __GNUC__
        clock_t beginTime = times(NULL);
#else

        GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalStartTime,&userStartTime);

#endif
        loadDataByType(dataType, rawData, newDataFileName,size,dim,fragmentLength) ;

#ifdef __GNUC__
        clock_t endTime = times(NULL);
        loadDataTimes[i]= (endTime-beginTime)/100.0;
#else

        GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalEndTime,&userEndTime);
        loadDataTimes[i]=((userEndTime.dwLowDateTime + kernalEndTime.dwLowDateTime) - (userStartTime.dwLowDateTime + kernalStartTime.dwLowDateTime)) / 10000000.00;

#endif



#ifdef __GNUC__
        CMemMonitor *memoryMonitor = new CMemMonitor();

        memoryMonitor->getMemorySize();

        output<<"=================Loading Data Used Time And Memory================="<<endl;

        output<<"loadDataTime: "<<loadDataTimes[i]<<" s"<<endl;

        output<<"WorkingSetSize: "<<memoryMonitor->resMemorySize <<" MB / PeakWorkingSetSize: "<<memoryMonitor->peekResMemorySize<<" MB "<<endl; 

        /*output<<"swapOutSize: "<<memoryMonitor->swapMemorySize<<" MB"<<endl;*/

        output<<"MemoryUsage: "<<memoryMonitor->virtMemorySize <<" MB/ PeakMemoryUsage: "<< memoryMonitor->peekVirtMemorySize <<" MB "<<endl;

        output<<"==================================================================="<<endl<<endl;
#else

        GetProcessMemoryInfo(GetCurrentProcess(),&pmc,sizeof(pmc));
        output<<"=================Loading Data Used Time And Memory================="<<endl;
        output<<"loadDataTime  :"<<loadDataTimes[i]<<" s"<<endl;
        output<<"WorkingSetSize:"<<pmc.WorkingSetSize/1048576.0 <<" MB / PeakWorkingSetSize: "<<pmc.PeakWorkingSetSize/1048576.0<<" MB "<<endl; 
        output<<"PagefileUsage :"<<pmc.PagefileUsage/1048576.0<<" MB/ PeakPagefileUsage: "<< pmc.PeakPagefileUsage/1048576.0<<" MB "<<endl;
        output<<"MemoryUsage   :"<<pmc.PagefileUsage/1048576.0 + pmc.WorkingSetSize/1048576.0<<" MB/ PeakMemoryUsage: "<< pmc.PeakPagefileUsage/1048576.0 + pmc.PeakWorkingSetSize/1048576.0 <<" MB "<<endl;
        output<<"==================================================================="<<endl<<endl;

#endif

        getPivotSelectionMethod(pivotSelectionMethod,psm,fftscale,setE,setC) ;
        getPartitionMethod(partitionMethod,pm);

        
		getIndexType(metric,cmetric,rawData,psm,pm,index,dataType,indexType,currentIndexName,numPivot,singlePivotFanout,maxLeafSize,MF_maxRadius,MF_middleProportion,buildMode,bThreadNum,trisectionRadius,selectOptimalPivots,partitionByOnePivot);
        distanceFunctionCounterBeforSearch=dynamic_cast<CountedMetric*>(index->getMetric())->getCounter();

#ifdef __GNUC__
        beginTime = times(NULL);
        index->bulkLoad(*rawData,buildMode,bThreadNum);
        endTime = times(NULL);
        buildTimes[i]= (endTime-beginTime)/100.0;
#else

        GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalStartTime,&userStartTime);
        index->bulkLoad(*rawData,buildMode,bThreadNum);
        GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalEndTime,&userEndTime);
        buildTimes[i]=((userEndTime.dwLowDateTime + kernalEndTime.dwLowDateTime) - (userStartTime.dwLowDateTime + kernalStartTime.dwLowDateTime)) / 10000000.00;

#endif

        distanceFunctionCounterAfterSearch=dynamic_cast<CountedMetric*>(index->getMetric())->getCounter();

        distanceCalculatedTimes[i]=distanceFunctionCounterAfterSearch-distanceFunctionCounterBeforSearch;

        if(putInfo==1)
		    getMVPIndexStructureInfo(currentIndexName,dataType,buildingInformationFile);

#ifdef __GNUC__
        memoryMonitor->getMemorySize();
        output<<"=================Building index Used Time And Memory================="<<endl;
        output<<"datasize: "<<size<<endl;
        output<<"distCalculateTimes: "<<distanceFunctionCounterAfterSearch-distanceFunctionCounterBeforSearch<<endl;
        output<<"buildTime: "<<buildTimes[i]<<" s"<<endl;
        output<<"WorkingSetSize: "<<memoryMonitor->resMemorySize <<" MB / PeakWorkingSetSize: "<<memoryMonitor->peekResMemorySize<<" MB "<<endl; 
        /*output<<"swapOutSize: "<<memoryMonitor->swapMemorySize<<" MB"<<endl;*/
        output<<"MemoryUsage: "<<memoryMonitor->virtMemorySize <<" MB/ PeakMemoryUsage: "<< memoryMonitor->peekVirtMemorySize <<" MB "<<endl;
        output<<"==================================================================="<<endl<<endl;
        delete memoryMonitor;
#else

        GetProcessMemoryInfo(GetCurrentProcess(),&pmc,sizeof(pmc));

        output<<"=================Building index Used Time And Memory================="<<endl;
        output<<"datasize          :"<<size<<endl;
        output<<"distCalculateTimes:"<<distanceFunctionCounterAfterSearch-distanceFunctionCounterBeforSearch<<endl;
        output<<"buildTime         :"<<buildTimes[i]<<" s"<<endl;
        output<<"WorkingSetSize    :"<<pmc.WorkingSetSize/1048576.0 <<" MB / PeakWorkingSetSize: "<<pmc.PeakWorkingSetSize/1048576.0<<" MB "<<endl; 
        output<<"PagefileUsage     :"<<pmc.PagefileUsage/1048576.0<<" MB/ PeakPagefileUsage: "<< pmc.PeakPagefileUsage/1048576.0<<" MB "<<endl;
        output<<"MemoryUsage       :"<<pmc.PagefileUsage/1048576.0 + pmc.WorkingSetSize/1048576.0<<" MB/ PeakMemoryUsage: "<< pmc.PeakPagefileUsage/1048576.0 + pmc.PeakWorkingSetSize/1048576.0 <<" MB "<<endl;
        output<<"==================================================================="<<endl<<endl;

#endif

    }

	//must release these space, because they are dynamic space. see at "joinCharArray" function
    free(resultFileName);
	free(newIndexName);
	free(newDataFileName);

	delete buildTimes;
	delete loadDataTimes;
	delete distanceCalculatedTimes;
}

/**

* @This is a utility class to query a VPIndex. It runs a set of query on the given index and compute the average

* performance.

* 

* main function to evaluate the query performance of an Index The

* eveluation is done by run a set of query on the Index, and compute the

* average performance statistics, such as number of distance calculation,

* number of index node visited I/O, and search time. The command line

* interface to bulkload one Index, or a series of Indexes for

* scalability study.

* 

* @param indexFileName name of the file storing the index structure built before.

* @param queryFileName query file name. 

* @param firstQuery offset of first query to be used in the query file, start from 0, inclusive, default 0. 

* @param lastQuery offset of last query to be used in the query file, exclusive, default 1. 

* @param minRadius maximum search radius, default 10. 

* @param maxRadius minimum search radius, default 0. 

* @param step step size for search radii, default 1. 

* @param verify 1 if search results are to be verified against a linear scan and 0 otherwise, default 0. 

* @param resultsFileName name of the file to store the result information of the search operation. 

* @param dim dimension of vector data to load or number of features to be loaded.

* @param fragmentLength fragment length, only meaningful for sequences.

* 

*/
void searchIndex(int dataBasehSize,char* queryFile,char *dataType,int firstQuery,int lastQuery,int dim,int fragmentLength,double maxRadius,double minRadius,double step,bool verify,char *resultsFile,int buildMode,char *indexName,int searchMode,int cacheHeight,int sThreadNum,bool cacheAll,int numberOfChildrenPathSearchingThreads)
{

#ifdef _WIN32
    char *indexFileName="./SourceFiles/util/database/";
#else
    char *indexFileName="../database/";
#endif
    joinCharArray(indexFileName,indexName);

#ifdef _WIN32
    char *resultFileName="./SourceFiles/util/result/";
#else
    char *resultFileName="../result/";
#endif	
    joinCharArray(resultFileName,resultsFile);

#ifdef _WIN32
    char *queryFileName="./SourceFiles/util/data/";
#else
    char *queryFileName="../data/";
#endif
    joinCharArray(queryFileName,queryFile);

    CMetricDistance *metric=0;

    vector<shared_ptr<CMetricData> > linerDataList;


    vector<shared_ptr<CMetricData> > *queryList = NULL;
    loadDataByType(dataType,queryList, queryFileName,lastQuery,dim,fragmentLength) ;
    getMetricByType(metric,dataType) ;
    CountedMetric *cmetric=new CountedMetric(metric);

    _search(queryList,cmetric,resultFileName,indexFileName,queryFileName,dim,fragmentLength,verify,dataType,maxRadius,minRadius,step,searchMode,firstQuery,lastQuery,dataBasehSize,cacheHeight,cacheAll,sThreadNum,numberOfChildrenPathSearchingThreads);

    free(indexFileName);
	free(resultFileName);
	free(queryFileName);

}

/**@function to build a index first and then do search operation with the given query objects

*

* @param dataFileName raw data file name. 

* @param numPivot number of pivots in an index node.

* @param setA the value of A in the incremental pivot selection.

* @param setN the value of N in the incremental pivot selection.

* @param singlePivotFanout fanout of a pivot.

* @param fftscale use for pcaonfft pivot selection method.

* @param pivotSelectionMethod the pivot selection selection method: random, fft, center, pcaonfft, pca.

* @param partitionMethod data partition method: balanced, clusteringkmeans, clusteringboundary.

* @param maxLeafSize maximum number of children in leaf nodes.

* @param dataType data type, one of "protein", "dna", "vector", "image", "msms","string".

* @param initialSize size of smallest database.

* @param finalSize size of largest database.

* @param stepSize step size of databases.

* @param bucket whether bucketing will be used, 1: use.

* @param fragmentLength fragment length, only meaningful for sequences.

* @param dim dimension of vector data to load or number of features to be loaded.

* @param maxR maximum radius for partition.

* @param indexType [list,mvptree],the type of the index built later.

* @param fftOpt 0:use old version FFT, 1: use new version FFT but don't choose the third point got from FFT as the second pivot, 2:use new version FFT and choose the third point got from FFT as the second pivot. Default 0.

* @param indexFileName name of the file storing the index structure built before.

* @param queryFileName query file name. 

* @param firstQuery offset of first query to be used in the query file, start from 0, inclusive, default 0. 

* @param lastQuery offset of last query to be used in the query file, exclusive, default 1. 

* @param minRadius maximum search radius, default 10. 

* @param maxRadius minimum search radius, default 0. 

* @param step step size for search radii, default 1. 

* @param verify 1 if search results are to be verified against a linear scan and 0 otherwise, default 0. 

* @param resultsFileName name of the file to store the result information of the search operation. 

*

*/
void buildAndSearch(char *&dataFileName,int &numPivot,int setE,int setC,int &singlePivotFanout,int fftscale,char *&pivotSelectionMethod,char *&partitionMethod,int &maxLeafSize,char *&dataType,int initialSize,int finalSize,int stepSize,bool bucket,int &fragmentLength,int dim,double maxR,char *&indexType,int fftOpt,char* queryFileName,int firstQuery,int lastQuery,double maxRadius,double minRadius,double step,bool verify,char *resultsFileName,int buildMode,char *indexName,int searchMode,double MF_maxRadius,double MF_middleProportion,bool putInfo,int cacheHeight,int sThreadNum,bool cacheAll,int bThreadNum,double trisectionRadius,char* selectOptimalPivots,char* partitionByOnePivot,int numberOfChildrenPathSearchingThreads)
{
    vector< shared_ptr<CMetricData> > *rawData=0;

    CMetricDistance *metric=0;

    CPivotSelectionMethod *psm=0;

    CPartitionMethod *pm=0;

    CIndex *index =0;

    int indexNum = (finalSize - initialSize) / stepSize + 1;

    double *loadDataTimes=new double[indexNum];

    double *buildTimes=new double[indexNum];

#ifdef _WIN32
    char *newDataFileName="./SourceFiles/util/data/";
#else
    char *newDataFileName="../data/";
#endif
    joinCharArray(newDataFileName,dataFileName);
	
#ifdef _WIN32
    char *newQueryFile="./SourceFiles/util/data/";
#else
    char *newQueryFile="../data/";
#endif
    joinCharArray(newQueryFile,queryFileName);

#ifdef _WIN32
    char *newIndexName="./SourceFiles/util/database/";
#else
    char *newIndexName="../database/";
#endif
    joinCharArray(newIndexName,indexName);

#ifdef _WIN32
    char *resultFile="./SourceFiles/util/result/";
#else
    char *resultFile="../result/";
#endif
    joinCharArray(resultFile,resultsFileName);


    ofstream output(resultFile,ofstream::app);
    vector<shared_ptr<CMetricData> > *queryList =  NULL;
    stringstream newSize;

    for (int size = initialSize, i = 0; (size <= finalSize) & (i < indexNum); size += stepSize, i++)
    {

		/*********  update index name, append "size" into name, in order to partition index files  ***********/

		/*   make "size" from int to string  */
		stringstream ssTemp;
		string sizeTemp;
		ssTemp<<size;
		ssTemp>>sizeTemp;

		string indexNameTemp = "";
		indexNameTemp.assign(newIndexName);//"newIndexName" is a dynamic space, must copy it to another string, so as to change file name each while
		indexNameTemp += "_datasize_" + sizeTemp;  //append "datasize"
		char * currentIndexName = (char *)indexNameTemp.c_str();//transfer to "char *" type, because the need of below function call

#ifdef __GNUC__
        clock_t beginTime = times(NULL);
#else

        GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalStartTime,&userStartTime);

#endif
        loadDataByType(dataType,rawData,newDataFileName,size,dim,fragmentLength);
        loadDataByType(dataType,queryList,newQueryFile,lastQuery,dim,fragmentLength);
#ifdef __GNUC__
        clock_t endTime = times(NULL);
        loadDataTimes[i]= (endTime-beginTime)/100.0;
#else

        GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalEndTime,&userEndTime);
        loadDataTimes[i]=((userEndTime.dwLowDateTime + kernalEndTime.dwLowDateTime) - (userStartTime.dwLowDateTime + kernalStartTime.dwLowDateTime)) / 10000000.00;

#endif

#ifdef __GNUC__
        CMemMonitor *memoryMonitor = new CMemMonitor();
        memoryMonitor->getMemorySize();
        output<<"=================Loading Data Used Time And Memory================="<<endl;
        output<<"loadDataTime: "<<loadDataTimes[i]<<" s"<<endl;
        output<<"WorkingSetSize: "<<memoryMonitor->resMemorySize <<" MB / PeakWorkingSetSize: "<<memoryMonitor->peekResMemorySize<<" MB "<<endl; 
        /*output<<"swapOutSize: "<<memoryMonitor->swapMemorySize<<" MB"<<endl;*/
        output<<"MemoryUsage: "<<memoryMonitor->virtMemorySize <<" MB/ PeakMemoryUsage: "<< memoryMonitor->peekVirtMemorySize <<" MB "<<endl;
        output<<"==================================================================="<<endl<<endl;
#else

        GetProcessMemoryInfo(GetCurrentProcess(),&pmc,sizeof(pmc));
        output<<"=================Loading Data Used Time And Memory================="<<endl;
        output<<"loadDataTime: "<<loadDataTimes[i]<<" s"<<endl;
        output<<"WorkingSetSize: "<<pmc.WorkingSetSize/1048576.0 <<" MB / PeakWorkingSetSize: "<<pmc.PeakWorkingSetSize/1048576.0<<" MB "<<endl; 
        output<<"PagefileUsage: "<<pmc.PagefileUsage/1048576.0<<" MB/ PeakPagefileUsage: "<< pmc.PeakPagefileUsage/1048576.0<<" MB "<<endl;
        output<<"MemoryUsage: "<<pmc.PagefileUsage/1048576.0 + pmc.WorkingSetSize/1048576.0<<" MB/ PeakMemoryUsage: "<< pmc.PeakPagefileUsage/1048576.0 + pmc.PeakWorkingSetSize/1048576.0 <<" MB "<<endl;
        output<<"==================================================================="<<endl<<endl;

#endif

        getMetricByType(metric,dataType);
        CountedMetric *cmetric=new CountedMetric(metric);

        getPivotSelectionMethod(pivotSelectionMethod,psm,fftscale,setE,setC) ;
        getPartitionMethod(partitionMethod,pm);
        getIndexType(metric,cmetric,rawData,psm,pm,index,dataType,indexType,currentIndexName,numPivot,singlePivotFanout,maxLeafSize,MF_maxRadius,MF_middleProportion,buildMode,bThreadNum,trisectionRadius,selectOptimalPivots,partitionByOnePivot);

        int distanceFunctionCounterBeforSearch=0,distanceFunctionCounterAfterSearch=0;

        distanceFunctionCounterBeforSearch=dynamic_cast<CountedMetric*>(index->getMetric())->getCounter();



#ifdef _WIN32
        GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalStartTime,&userStartTime);
        index->bulkLoad(*rawData,buildMode,bThreadNum);
        GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalEndTime,&userEndTime);
        buildTimes[i] = ((userEndTime.dwLowDateTime + kernalEndTime.dwLowDateTime) - (userStartTime.dwLowDateTime + kernalStartTime.dwLowDateTime))/10000000.00;
        distanceFunctionCounterAfterSearch=dynamic_cast<CountedMetric*>(index->getMetric())->getCounter();

        GetProcessMemoryInfo(GetCurrentProcess(),&pmc,sizeof(pmc));
        output<<"=================Building index Used Time And Memory================="<<endl;
        output<<"datasize: "<<size<<endl;
        output<<"distCalculateTimes: "<<distanceFunctionCounterAfterSearch-distanceFunctionCounterBeforSearch<<endl;
        output<<"buildTime: "<<buildTimes[i]<<" s"<<endl;
        output<<"WorkingSetSize: "<<pmc.WorkingSetSize/1048576.0 <<" MB / PeakWorkingSetSize: "<<pmc.PeakWorkingSetSize/1048576.0<<" MB "<<endl; 
        output<<"PagefileUsage: "<<pmc.PagefileUsage/1048576.0<<" MB/ PeakPagefileUsage: "<< pmc.PeakPagefileUsage/1048576.0<<" MB "<<endl;
        output<<"MemoryUsage: "<<pmc.PagefileUsage/1048576.0 + pmc.WorkingSetSize/1048576.0<<" MB/ PeakMemoryUsage: "<< pmc.PeakPagefileUsage/1048576.0 + pmc.PeakWorkingSetSize/1048576.0 <<" MB "<<endl;
        output<<"==================================================================="<<endl<<endl;
#else

        beginTime = times(NULL);
        index->bulkLoad(*rawData,buildMode,bThreadNum);
        endTime = times(NULL);
        buildTimes[i]= (endTime-beginTime)/100.0;
        distanceFunctionCounterAfterSearch=dynamic_cast<CountedMetric*>(index->getMetric())->getCounter();

        output<<"=================Building index Used Time And Memory================="<<endl;
        output<<"datasize: "<<size<<endl;
        output<<"distCalculateTimes: "<<distanceFunctionCounterAfterSearch-distanceFunctionCounterBeforSearch<<endl;
        output<<"buildTime: "<<buildTimes[i]<<" s"<<endl;
        output<<"WorkingSetSize: "<<memoryMonitor->resMemorySize <<" MB / PeakWorkingSetSize: "<<memoryMonitor->peekResMemorySize<<" MB "<<endl; 
        //output<<"swapOutSize: "<<memoryMonitor->swapMemorySize<<" MB";
        output<<"MemoryUsage: "<<memoryMonitor->virtMemorySize <<" MB/ PeakMemoryUsage: "<< memoryMonitor->peekVirtMemorySize <<" MB "<<endl;
        output<<"==================================================================="<<endl<<endl;
        delete memoryMonitor;

#endif

        if(putInfo==1)
            getMVPIndexStructureInfo(currentIndexName,dataType,resultsFileName);

        _search(queryList,cmetric,resultFile,currentIndexName,newDataFileName,dim,fragmentLength,verify,dataType,maxRadius,minRadius,step,searchMode,firstQuery,lastQuery,size,cacheHeight,cacheAll,sThreadNum,numberOfChildrenPathSearchingThreads);

    }

	//free dynamic space, see at "joinCharArray" function
	free(newDataFileName);   
	free(newIndexName);  
	free(newQueryFile);
    free(resultFile);

}

void _search(vector<shared_ptr<CMetricData>> *queryList,CountedMetric *cmetric,char *resultFileName,char *indexFileName,char *rawDataFile,int dim,int fragmentLength,bool verify,char *dataType,double maxRadius,double minRadius,double step,int searchMode,int firstQuery,int lastQuery,int dataBaseSize,int cacheHeight,bool cacheAll,int maximumThreadsNumForSearching,int totalSearchingThreadForChildPathSearchingTaskInThreadpool)
{
    int distanceFunctionCounterBeforSearch=0,distanceFunctionCounterAfterSearch=0;
    int numRun=0;
    if((maxRadius-minRadius) <= 0.0001)
        numRun=1;
    else
        numRun=ceil((maxRadius-minRadius)/step)+1;

    double *mvpqueryTime=new double[numRun];

    double *maxQueryTime=new double[numRun];

    double *minQueryTime=new double[numRun];

    double *mvpdistanceCalculatedTimes=new double[numRun];

    double *maxDistanceCalculatedTimes=new double[numRun];

    double *minDistanceCalculatedTimes=new double[numRun];

	double *averageMVPSearchCount=new double[numRun];

    double *avgpeakWorkingSetSize=new double[numRun];

    double *peakWorkingSetSize=new double[numRun];


    double *peekmemoryUseSize=new double[numRun];

    double *avgpagefileusage=new double[numRun];

    double *peakpagefileusage=new double[numRun];

    double *avgpeekmemoryUseSize=new double[numRun];

    ofstream output(resultFileName,ofstream::app);

    vector<shared_ptr<CMetricData> > *rawData=0;
    if(verify) 
        loadDataByType(dataType, rawData, rawDataFile,dataBaseSize,dim,fragmentLength) ;

	long* rootAddress=NULL;
    int numTree;
	int* treeHeights=NULL;

    if(searchMode == 2||searchMode == 5)
    {

        cacheForestIndex(&cacheHeight,rootAddress,numTree,treeHeights,indexFileName,dataType,&indexCache,cacheAll);

    }
    else
        cacheIndex(&cacheHeight,indexFileName,dataType,&indexCache,cacheAll);
    delete treeHeights;

    /*********************************************************************************/
    unordered_map<long,std::pair<char*,CIndexNode*> >::iterator ite=indexCache.begin();
    int count=0;
    for(;ite!=indexCache.end();ite++)
    {
        count++;
    }
    cout<<"Number of cached nodes:"<<count<<endl;
    /*********************************************************************************/

    ifstream inFile(resultFileName);
    string str;
    int data=0;
    while(!inFile.eof())
    {
        getline(inFile,str);
        stringstream newStr(str);
        try
        {
            newStr>>data;
        }
        catch(exception e)
        {
            continue;
        }
    }
    if(data != dataBaseSize)
        output<<"datasize   "<<"searchradius   "<<"distcalculation   "<<"searchtime   "<<"searchresults   "<<"avgpeakworkingsetsize   "<<"peakworkingsetsize   "<<"avgpeekmemoryUseSize   "<<"peekmemoryUseSize   "<<endl;

    if(searchMode<=2)
    {

        for(int i=0;i<numRun;i++)
        {

            mvpqueryTime[i]=0;



            maxQueryTime[i]=DBL_MIN;

            minQueryTime[i]=DBL_MAX;

            mvpdistanceCalculatedTimes[i]=0;


            maxDistanceCalculatedTimes[i]=DBL_MIN;

            minDistanceCalculatedTimes[i]=DBL_MAX;

            averageMVPSearchCount[i] = 0.0;



            peakWorkingSetSize[i] = 0.0;

            avgpeakWorkingSetSize[i] = 0.0;



            peekmemoryUseSize[i] = 0.0;

            avgpeekmemoryUseSize[i] = 0.0;



            peakpagefileusage[i] = 0.0;

            avgpagefileusage[i] = 0.0;



            double searchRadius=minRadius+step*i;

			if(searchRadius>maxRadius) break;// immidately break out

            int count=1;

            double t = 0.0;

            int startQuery=(firstQuery<0) ? 0:firstQuery;

            int endQuery=(lastQuery>queryList->size()) ? queryList->size():lastQuery;



            vector<shared_ptr<CMetricData> > newQueryList;

            vector<shared_ptr<CMetricData> >::iterator ite=queryList->begin();

            newQueryList.insert(newQueryList.begin(),ite+startQuery,ite+endQuery);

            ite=newQueryList.begin();



            for(;ite!=newQueryList.end();ite++)
            {

                CMemMonitor::reset();

                CRangeQuery q(minRadius+step*i,*ite);

                list<shared_ptr<CMetricData> >* mvptreeSearchResults = new list<shared_ptr<CMetricData> >();



                distanceFunctionCounterBeforSearch=dynamic_cast<CountedMetric*>(cmetric)->getCounter();

#ifdef _WIN32

                GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalStartTime,&userStartTime);

                if (searchMode == 0)
                {

                    shared_ptr<CMVPIndex>  mvpIndex(new CMVPIndex());

                    mvpIndex->setFileName(indexFileName);

                    mvpIndex->setDataType(dataType);

                    mvpIndex->setMetric(cmetric);

                    delete mvptreeSearchResults;

                    mvptreeSearchResults = mvpIndex->search(&q);

                    CMemMonitor::updateMem();

                    averageMVPSearchCount[i] += mvptreeSearchResults->size();

                }
                else if (searchMode == 1)
                {

                    shared_ptr< CSearchTask > searchTask (new CSearchTask(indexFileName,dataType,*cmetric));

                    list<shared_ptr<CMetricData> >* searchResults = searchTask->search(q);

                    CMemMonitor::updateMem();

                    averageMVPSearchCount[i] += searchResults->size();

                    list<shared_ptr<CMetricData> >::iterator mvp_iter = searchResults->begin();
                    for(;mvp_iter!=searchResults->end();mvp_iter++)
                    {
                        mvptreeSearchResults->push_back(*mvp_iter);
                    }

                }
                else 
                {

                    shared_ptr<CExcludedMiddleForest>  ForestIndex(new CExcludedMiddleForest());

                    ForestIndex->setFileName(indexFileName);

                    ForestIndex->setDataType(dataType);

                    ForestIndex->setMetric(cmetric);

                    list<shared_ptr<CMetricData> >* searchResults = ForestIndex->search(&q);

                    CMemMonitor::updateMem();

                    averageMVPSearchCount[i] += searchResults->size();


                    list<shared_ptr<CMetricData> >::iterator mvp_iter = searchResults->begin();
                    for(;mvp_iter!=searchResults->end();mvp_iter++)
                    {
                        mvptreeSearchResults->push_back(*mvp_iter);
                    }

                }

                GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalEndTime,&userEndTime);

                t=((userEndTime.dwLowDateTime + kernalEndTime.dwLowDateTime) - (userStartTime.dwLowDateTime + kernalStartTime.dwLowDateTime)) / 10000000.00;

#else


                clock_t beginTime = times(NULL);

                if (searchMode == 0)
                {

                    shared_ptr<CMVPIndex>  mvpIndex(new CMVPIndex());

                    mvpIndex->setFileName(indexFileName);

                    mvpIndex->setDataType(dataType);

                    mvpIndex->setMetric(cmetric);

                    delete mvptreeSearchResults;

                    mvptreeSearchResults = mvpIndex->search(&q);

                    CMemMonitor::updateMem();

                    averageMVPSearchCount[i] += mvptreeSearchResults->size();

                }
                else if (searchMode == 1)

                {

                    shared_ptr< CSearchTask > searchTask (new CSearchTask(indexFileName,dataType,*cmetric));

                    list<shared_ptr<CMetricData> >* searchResults = searchTask->search(q);

                    CMemMonitor::updateMem();

                    averageMVPSearchCount[i] += searchResults->size();

                    list<shared_ptr<CMetricData> >::iterator mvp_iter = searchResults->begin();
                    for(;mvp_iter!=searchResults->end();mvp_iter++)
                    {
                        mvptreeSearchResults->push_back(*mvp_iter);
                    }

                }

                else 

                {

                    shared_ptr<CExcludedMiddleForest>  ForestIndex(new CExcludedMiddleForest());

                    ForestIndex->setFileName(indexFileName);

                    ForestIndex->setDataType(dataType);

                    ForestIndex->setMetric(cmetric);

                    list<shared_ptr<CMetricData> >* searchResults = ForestIndex->search(&q);

                    CMemMonitor::updateMem();

                    averageMVPSearchCount[i] += searchResults->size();

                    list<shared_ptr<CMetricData> >::iterator mvp_iter = searchResults->begin();
                    for(;mvp_iter!=searchResults->end();mvp_iter++)
                    {
                        mvptreeSearchResults->push_back(*mvp_iter);
                    }

                }

                clock_t endTime = times(NULL);

                t = (endTime-beginTime)/100.0;

#endif


                distanceFunctionCounterAfterSearch=dynamic_cast<CountedMetric*>(cmetric)->getCounter();

                mvpqueryTime[i] +=t;

                if(maxQueryTime[i]<t)

                    maxQueryTime[i]=t;

                if(minQueryTime[i]>t)

                    minQueryTime[i]=t;

                int d=distanceFunctionCounterAfterSearch-distanceFunctionCounterBeforSearch;



                mvpdistanceCalculatedTimes[i]+=d;

                if(maxDistanceCalculatedTimes[i]<d)

                    maxDistanceCalculatedTimes[i]=d;

                if(minDistanceCalculatedTimes[i]>d)

                    minDistanceCalculatedTimes[i]=d;


#ifdef __GNUC__
                if (peakWorkingSetSize[i] < CMemMonitor::maxWorkingSet)

                    peakWorkingSetSize[i] = CMemMonitor::maxWorkingSet;

                avgpeakWorkingSetSize[i] += CMemMonitor::maxWorkingSet;



                if (peekmemoryUseSize[i] < CMemMonitor::maxMem)

                    peekmemoryUseSize[i] = CMemMonitor::maxMem;

                avgpeekmemoryUseSize[i] += CMemMonitor::maxMem;



                if (peakpagefileusage[i] < CMemMonitor::maxPageUsing)

                    peakpagefileusage[i] = CMemMonitor::maxPageUsing;

                avgpagefileusage[i] += CMemMonitor::maxPageUsing;
#else

                if (peakWorkingSetSize[i] < CMemMonitor::maxWorkingSet)
                    peakWorkingSetSize[i] = CMemMonitor::maxWorkingSet;
                avgpeakWorkingSetSize[i] += CMemMonitor::maxWorkingSet/1048576.0;

                if (peekmemoryUseSize[i] < CMemMonitor::maxMem)
                    peekmemoryUseSize[i] = CMemMonitor::maxMem;
                avgpeekmemoryUseSize[i] += CMemMonitor::maxMem/1048576.0;

                if (peakpagefileusage[i] < CMemMonitor::maxPageUsing)
                    peakpagefileusage[i] = CMemMonitor::maxPageUsing;
                avgpagefileusage[i] += CMemMonitor::maxPageUsing/1048576.0;

#endif

                //for verify
                if(verify)
                {
                    CMetricDistance *metric=0;
                    getMetricByType(metric,dataType);
                    verifySearchResult(rawData,metric,q,mvptreeSearchResults,output);
                }

                count++;
                delete mvptreeSearchResults;
            }

            mvpqueryTime[i]/=count-1;

            mvpdistanceCalculatedTimes[i]/=count-1;

            averageMVPSearchCount[i] /= count-1;

            avgpagefileusage[i] /= count-1;

            avgpeekmemoryUseSize[i] /= count-1;

            avgpeakWorkingSetSize[i] /= count-1;

#ifdef _WIN32
            peakWorkingSetSize[i] = peakWorkingSetSize[i]/1048576.0;
            peekmemoryUseSize[i] = peekmemoryUseSize[i]/1048576.0;
            peakpagefileusage[i] = peakpagefileusage[i]/1048576.0;
#else

            peakWorkingSetSize[i] = peakWorkingSetSize[i];
            peekmemoryUseSize[i] = peekmemoryUseSize[i];
            peakpagefileusage[i] = peakpagefileusage[i];


#endif


            output<<dataBaseSize<<setw(15)<<searchRadius<<setw(15)<< mvpdistanceCalculatedTimes[i] <<setw(20)<<mvpqueryTime[i]<<setw(15)<<averageMVPSearchCount[i]<<setw(20)<<avgpeakWorkingSetSize[i]<<setw(20)<<peakWorkingSetSize[i]<<setw(20)<<avgpeekmemoryUseSize[i]<<setw(20)<<peekmemoryUseSize[i]<<endl;



        }

        delete[] mvpqueryTime;
        delete[] maxQueryTime;
        delete[] minQueryTime;
        delete[] mvpdistanceCalculatedTimes;
        delete[] maxDistanceCalculatedTimes;
        delete[] minDistanceCalculatedTimes;
        delete[] averageMVPSearchCount; 

		delete[] avgpeakWorkingSetSize;

		delete[] peakWorkingSetSize;

		delete[] peekmemoryUseSize;

		delete[] avgpagefileusage;

		delete[] peakpagefileusage;

		delete[] avgpeekmemoryUseSize;

        delete(queryList);

    }
    else if(searchMode==3)
    {
        int queryStart=(firstQuery<0)?0:firstQuery;
        int queryEnd=(lastQuery>queryList->size())?queryList->size():lastQuery;
        vector<shared_ptr<CMetricData> > newQueryList;
        vector<shared_ptr<CMetricData> >::iterator ite=queryList->begin();
        newQueryList.insert(newQueryList.begin(),ite+queryStart,ite+queryEnd);
        globalMultithreadSearch(rawData,&newQueryList,dataType,indexFileName,verify,maxRadius,minRadius,step,cmetric,resultFileName,dataBaseSize,cacheHeight,maximumThreadsNumForSearching);
    }
    else if(searchMode==4)
    {
        int queryStart=(firstQuery<0)?0:firstQuery;
        int queryEnd=(lastQuery>queryList->size())?queryList->size():lastQuery;
        vector<shared_ptr<CMetricData> > newQueryList;
        vector<shared_ptr<CMetricData> >::iterator ite=queryList->begin();
        newQueryList.insert(newQueryList.begin(),ite+queryStart,ite+queryEnd);
        multiThreadsInInterAndInner(&newQueryList,dataType,indexFileName,maxRadius,minRadius,resultFileName,dataBaseSize,cacheHeight,maximumThreadsNumForSearching,totalSearchingThreadForChildPathSearchingTaskInThreadpool);
    }
    else if(searchMode==5)
    {
        int queryStart=(firstQuery<0)?0:firstQuery;
        int queryEnd=(lastQuery>queryList->size())?queryList->size():lastQuery;
        vector<shared_ptr<CMetricData> > newQueryList;
        vector<shared_ptr<CMetricData> >::iterator ite=queryList->begin();
        newQueryList.insert(newQueryList.begin(),ite+queryStart,ite+queryEnd);
        globalForestMultithreadSearch(rawData,&newQueryList,dataType,indexFileName,numTree,rootAddress,verify,maxRadius,minRadius,step,cmetric,resultFileName,dataBaseSize,cacheHeight,maximumThreadsNumForSearching);
        delete rootAddress;
    }
}

ThreadPoolManager tpm;

void* singletf(void* vpb)
{
    singlequerytask *sqt=(singlequerytask*)vpb;

#ifdef _WIN32
    FILETIME kernelStart,kernelEnd;
    FILETIME userStart,userEnd;
    FILETIME dummy;
#else
    clock_t begt,endt;
#endif

#ifdef _WIN32
    GetThreadTimes(GetCurrentThread(),&dummy,&dummy,&kernelStart,&userStart);
#else
    begt=times(NULL);
#endif
    sqt->searchTask->singleSearchWithThreadPool(*(sqt->rq),sqt->rootaddress);
#ifdef _WIN32
    GetThreadTimes(GetCurrentThread(),&dummy,&dummy,&kernelEnd,&userEnd);
    sqt->searchtime=(userEnd.dwLowDateTime+kernelEnd.dwLowDateTime-userStart.dwLowDateTime-kernelEnd.dwLowDateTime)/10000000.00;
#else
    endt=times(NULL);
    sqt->searchtime=(endt-begt)/100.0;
#endif

    sqt->resultnumber=sqt->searchTask->getResultSize();
    sqt->distancecount=sqt->cmetric->getCounter();

    sqt->searchTask.reset();
    sqt->rq.reset();

    return NULL;
}

void multiThreadsInInterAndInner(vector<shared_ptr<CMetricData> >* queryObjectsList,char *&dataType,char *&indexFileName,double maximumSearchRadius,double minimumSearchRadius,char *&resultFileName,int dataSize,int cacheHeight,int totalSearchingThreadInThreadPool,int totalSearchingThreadForChildPathSearchingTaskInThreadpool)
{	
    fstream output(resultFileName,ios::out|ios::app);

    tpm.setMaxThreadNumber(totalSearchingThreadInThreadPool);
    tpm.setMaxThreadNumberForChildren(totalSearchingThreadForChildPathSearchingTaskInThreadpool);

    long rootaddress=0;
    ifstream in(indexFileName,ios::binary);
    in.seekg(-(long)sizeof(long),ios::end);
    in.read((char*)&rootaddress,sizeof(long));
    in.close();

    CMetricDistance *metric=0;
    if(strcmp(dataType,"vector")==0)
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

    int totalqueryobjectnumber=queryObjectsList->size();
    int variablequeryobjectnumber=0;

    mutex condition_mutex;
    condition_variable condition;

    list<tp_task*> tasklist;
    for(int i=0;i<queryObjectsList->size();i++)
    {
        shared_ptr<CountedMetric> cm(new CountedMetric(metric));
        singlequerytask *task=new singlequerytask;
        shared_ptr<CSearchTask> search(new CSearchTask(indexFileName,dataType,*cm));
        shared_ptr<CRangeQuery> rq(new CRangeQuery(maximumSearchRadius,queryObjectsList->at(i)));

        task->searchTask=search;
        task->rq=rq;
        task->cmetric=cm;
        task->rootaddress=rootaddress;
        task->searchtime=0;
        task->distancecount=0;
        task->resultnumber=0;

        tp_task *tpt=new tp_task;
        tpt->ifinner=0;
        tpt->totalroute=totalqueryobjectnumber;
        tpt->variableroutenumber=&variablequeryobjectnumber;
        tpt->cond=&condition;
        tpt->route_mutex=&condition_mutex;
        tpt->task=(void*)task;
        tpt->func=singletf;

        tasklist.push_back(tpt);
    }

    long resultCounter=0;
    int distanceCounter=0;
    double searchTime=0;

#ifdef _WIN32
    FILETIME kernelStart,kernelEnd;
    FILETIME userStart,userEnd;
    FILETIME dummy;
#else
    clock_t begt,endt,ibegt,iendt;
#endif

#ifdef _WIN32
    GetThreadTimes(GetCurrentThread(),&dummy,&dummy,&kernelStart,&userStart);
#else
    begt=times(NULL);
#endif

    tpm.run();
    tpm.addCompleteTask(&tasklist);

    {
        std::unique_lock<std::mutex> lk(condition_mutex);
        while(variablequeryobjectnumber<totalqueryobjectnumber)
            condition.wait(lk);
    }		
#ifdef _WIN32
    GetThreadTimes(GetCurrentThread(),&dummy,&dummy,&kernelEnd,&userEnd);
#else
    endt=times(NULL);
#endif

    for(int i=0;i<totalqueryobjectnumber;i++)
    {
        tp_task *sqt=tasklist.back();
        tasklist.pop_back();

        resultCounter+=((singlequerytask*)(sqt->task))->resultnumber;
        distanceCounter+=((singlequerytask*)(sqt->task))->distancecount;
        searchTime+=((singlequerytask*)(sqt->task))->searchtime;

        delete (singlequerytask*)(sqt->task);
        delete sqt;
    }

    tpm.stop();

    output<<dataSize<<setiosflags(ios::right)<<setiosflags(ios::fixed)<<setw(20)<<maximumSearchRadius<<setiosflags(ios::right)<<setiosflags(ios::fixed)<<setw(15)<<(distanceCounter/(queryObjectsList->size()))<<setiosflags(ios::right)<<setiosflags(ios::fixed)<<setw(15)<<(searchTime)<<setiosflags(ios::right)<<setiosflags(ios::fixed)<<setw(15)<<(((double)resultCounter)/(queryObjectsList->size()))<<endl;
#ifdef _WIN32
    output<<" total queryObject number:"<<queryObjectsList->size()<<"total thread number is:"<<totalSearchingThreadInThreadPool<<" child thread number is:"<<totalSearchingThreadForChildPathSearchingTaskInThreadpool<<" total search time:"<<(kernelEnd.dwLowDateTime+userEnd.dwLowDateTime-kernelStart.dwLowDateTime-userStart.dwLowDateTime)/10000000.00<<" cache height:"<<cacheHeight<<endl;
    output<<"======================================================================================================"<<endl;
#else
    output<<" total queryObject number:"<<queryObjectsList->size()<<"total thread number is:"<<totalSearchingThreadInThreadPool<<" child thread number is:"<<totalSearchingThreadForChildPathSearchingTaskInThreadpool<<" total search time:"<<(endt-begt)/100.0<<" cache height:"<<cacheHeight<<endl;
    output<<"======================================================================================================"<<endl;
#endif

    output.close();
}

/**@function to verify the search result is or not right

* @brief to verify the search result is or not right


* @param rawData raw data set. 

* @param metric to calculate the distance between two objects.

* @param q a query object.

* @param queryResultList mvp search result.

* @param output out stream.


* @post when there is an error, message will export to result file; if there is no error, nothing will export to result file.

*/
void verifySearchResult(vector<shared_ptr<CMetricData> > *&rawData,CMetricDistance *metric,CRangeQuery q, list<shared_ptr<CMetricData> > *queryResultList,ofstream &output)
{
    vector<shared_ptr<CMetricData> > linerSearchResults ;

    //linear traversal to get a result which must be true
    for (vector<CMetricData*>::size_type ii=0;ii!=rawData->size();ii++)
    {

        double objectDistance = metric->getDistance(q.getQueryObject().get(),rawData->at(ii).get());

        if(objectDistance<=q.getRadius())
            linerSearchResults.push_back(rawData->at(ii));

    }

    //check two results'length. if length is not same, it must be an error.
    if(queryResultList->size()!=linerSearchResults.size())
    {
        output<<"radius="<<q.getRadius()<<"  has an error:  "<<"mvptreeSearchResults->size()!=linerSearchResults.size() : "<<queryResultList->size()<<"   "<<linerSearchResults.size()<<endl;
    }

    list<shared_ptr<CMetricData> >::iterator mvp_iter = queryResultList->begin();


    for(;mvp_iter!=queryResultList->end();mvp_iter++)
    {
        vector<shared_ptr<CMetricData> >::size_type jj=0 ;
        for(;jj!=linerSearchResults.size();jj++)//"jj=linerSearchResults.size()" has only one situation which this loop has caught out, but failed to find the result
        {
            if(metric->getDistance((*mvp_iter).get(),linerSearchResults[jj].get())==0.0) break;//get out of loop right now after finding out the result
        }
        if(jj==linerSearchResults.size()) break;//means failing to find the result
    }
    if(mvp_iter!=queryResultList->end()) //get out of loop in the halfway because a result hasn't found.
        output<<"when radius is "<<q.getRadius()<<"  has an error!"<<endl;
}

