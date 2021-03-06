#ifndef TASKFUN_H
#define TASKFUN_H


//lib from cplusplus
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <string>
#include <vector>
#include <fstream>
#include <memory>
using namespace std;

#include <iomanip>
#include <float.h>



/***********************************************************************************************************************/
#include "../../../HeaderFiles/metricdata/DoubleVector.h"
//#include  "../../../HeaderFiles/metricdata/TandemSpectra.h"
#include "../../../HeaderFiles/metricdistance/MetricDistance.h"
#include "../../../HeaderFiles/metricdistance/EuclideanDistance.h"
#include "../../../HeaderFiles/index/Index.h"
#include "../../../HeaderFiles/index/MVPIndex.h"
#include "../../../HeaderFiles/index/ListIndex.h"
#include "../../../HeaderFiles/index/PartitionMethod.h"
#include "../../../HeaderFiles/index/PivotSelectionMethod.h"
#include "../../../HeaderFiles/index/PCAOnFFT.h"
#include "../../../HeaderFiles/index/FFTPivotSelectionMethod.h"
#include "../../../HeaderFiles/index/IncrementalSelection.h"
#include "../../../HeaderFiles/index/BalancePartitionMethod.h"
#include "../../../HeaderFiles/index/query/Query.h"
#include "../../../HeaderFiles/index/query/RangeQuery.h"
#include "../../../HeaderFiles/metricdistance/DNAMetric.h"
#include "../../../HeaderFiles//metricdistance/CountedMetric.h"
#include "../../../HeaderFiles/metricdistance/EditDistance.h"
#include "../../../HeaderFiles/metricdistance/ImageMetric.h"
//#include "../../../HeaderFiles/metricdistance/MSMSMetric.h"
#include "../../../HeaderFiles/metricdistance/RNAMetric.h"
#include "../../../HeaderFiles/metricdistance/PeptideMetric.h"
#include "../../../HeaderFiles/index/query/SearchTask.h"
#include "../../../HeaderFiles/metricdata/Image.h"
#include "../../../HeaderFiles/metricdata/DNA.h"
#include "../../../HeaderFiles/metricdata/Peptide.h"
#include "../../../HeaderFiles/metricdata/StringObject.h"
#include "../../../HeaderFiles/util/MemMonitor.h"
#include "../../../HeaderFiles/index/ExcludedMiddleForest.h"
#include "../../../HeaderFiles/index/ExcludedMiddlePartitionMethod.h"
#include "../../../HeaderFiles/util/TaskDeclaration.h"

#include <sys/times.h>

#ifdef _WIN32
#include <Windows.h>
#include <psapi.h>
#pragma comment(lib,"psapi.lib")
FILETIME kernalStartTime,kernalEndTime;
FILETIME userStartTime,userEndTime;
FILETIME ftDummy;
PROCESS_MEMORY_COUNTERS pmc;
#endif

//arguments for build and search operation
typedef struct basarg
{
	//server
	int socketfd;
	string *argstr;
	
	//for building index
	char *rawdatafilename;
	int bstart;
	int bend;

	long btime;
	char *indexfilename;
	
	//for query in index
	char *querydatafilename;
	int sstart;
	int send;

	long stotaltime;
	long sresponsetime;
	long sresultnum;	
}basarg;

typedef struct sarg
{
	//server
	int socketfd;
	string *argstr;

	char *querydatafilename;
	int sstart;
	int send;
	
	long stotaltime;
	long sresponsetime;
	long sresultnum;
}sarg;

typedef struct barg
{
	//server
	int socketfd;
	string *argstr;
	
	char *rawdatafilename;
	int bstart;
	int bend;

	long btime;
	char *indexfilename;
}barg;


//extern void bastask(basarg *);
//extern void btask(barg *);
//extern void stask(sarg *);

void btask(string &dataFileName, int &numPivot, int &setE, int &setC, int &singlePivotFanout, int &fftscale, string &pivotSelectionMethod, string &partitionMethod,int &maxLeafSize, string &dataType, int &initialSize, int &finalSize, int &stepSize, bool &bucket, int &fragmentLength, int &dim, double &maxR, string &indexType, int &fftOpt, string &buildingInformationFile, int &buildMode, string &indexName, double &MF_maxRadius, double &MF_middleProportion, bool &putInfo, int &bThreadNum, double &trisectionRadius,string &selectOptimalPivots,string &partitionByOnePivot, string &resultcontainer);
void stask(int &dataBasehSize, string &queryFileName, string &dataType, int &firstQuery, int &lastQuery, int &dim, int &fragmentLength, double &maxRadius, double &minRadius, double &step, bool &verify, string &resultsFileName, int &buildMode, string &indexName, int &searchMode, int &cacheHeight, int &sThreadNum, bool &cacheAll, int &numberOfChildrenPathSearchingThreads, string &resultcontainer);
void bastask(string &dataFileName, int &numPivot, int &setE, int &setC, int &singlePivotFanout, int &fftscale, string &pivotSelectionMethod, string &partitionMethod, int &maxLeafSize, string &dataType, int &initialSize, int &finalSize, int &stepSize, bool &bucket, int &fragmentLength, int &dim, double &maxR, string &indexType, int &fftOpt, string &queryFileName, int &firstQuery, int &lastQuery, double &maxRadius, double &minRadius, double &step, bool &verify, string &resultsFileName, int &buildMode, string &indexName, int &searchMode, double &MF_maxRadius, double &MF_middleProportion, bool &putInfo, int &cacheHeight, int &sThreadNum, bool &cacheAll, int &bThreadNum, double &trisectionRadius, string &selectOptimalPivots, string &partitionByOnePivot,int &numberOfChildrenPathSearchingThreads, string &resultcontainer);


void ncacheIndex(int *height,char* indexfilename,string dataType,unordered_map<long,std::pair<char*,CIndexNode*> > *cache,bool all);
void multiThreadsInInterAndInner(vector<shared_ptr<CMetricData> >* queryObjectsList,char *&dataType,char *&indexFileName,double maximumSearchRadius,double minimumSearchRadius, double step, char *&resultFileName,int dataSize,int cacheHeight,int maxsearchthread,int maxthreadnumberforchildren, string &);
void nglobalMultithreadSearch(vector<shared_ptr<CMetricData> > *rawData,vector<shared_ptr<CMetricData> > *queryObjectsList,char *dataType,char *indexFileName,bool verify,double maxRadius,double minRadius,double step,CountedMetric *&cmetric, char *resultFileName,int dataBaseSize,int cacheHeight,int threadNum, string &);
void _search(vector<shared_ptr<CMetricData>> *queryList,CountedMetric *cmetric,char *resultFileName,char *indexFileName,char *rawDataFile,int dim,int fragmentLength,bool verify,char *dataType,double maxRadius,double minRadius,double step,int searchMode,int firstQuery,int lastQuery,int dataBaseSize,int cacheHeight,bool cacheAll,int maximumThreadsNumForSearching,int totalSearchingThreadForChildPathSearchingTaskInThreadpool, string &);
void verifySearchResult(vector<shared_ptr<CMetricData> > *&rawData,CMetricDistance *metric,CRangeQuery q, list<shared_ptr<CMetricData> > *queryResultList,ofstream &output);

extern void getMVPIndexStructureInfo(char *&indexName,char *& dataType,char *& outputFileName);
extern void joinCharArray(char*&dest,const char *sor);
extern char *produceFileName(char *filename, int kind);
extern void loadDataByType(char * dataType, vector<shared_ptr<CMetricData> > *&dataList, char *dataFileName,int size,int dim,int fragmentLength);
extern void getMetricByType(CMetricDistance *&metric,char *dataType);
extern void getPivotSelectionMethod(char *pivotSelectionMethod,CPivotSelectionMethod *&psm,int fftscale,int setA,int setN);
extern void getPartitionMethod(char *partitionMethod,CPartitionMethod *&pm);
extern void getIndexType(CMetricDistance *metric,CountedMetric *cmetric,vector<shared_ptr<CMetricData> > *rawData,CPivotSelectionMethod *psm,CPartitionMethod *pm,CIndex *&index,char * dataType,char *indexType,char *newIndexName,int &numPivot,int &singlePivotFanout,int &maxLeafSize,double MF_maxRadius,double MF_middleProportion,int buildMode,int bThreadNum,double trisectionRadius,char* selectOptimalPivots,char* partitionByOnePivot);
/***********************************************************************************************************************/

#endif
