/** @file BuildORQueryIndex
* @contains the main interface to bulid or query index.
* @author ***
* @date 2013 5 21
* @version 0.x
*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>

using namespace std;

extern void resultfilename(int buildMode,int numPivot,int singlePivotFanout,char *dataType,int initialSize,int &finalSize,int dim, int fragment, char *resultsFileName,char * indexName,char *runOption,int searchMode,char * which);
extern void buildAndSearch(char *&dataFileName,int &numPivot,int setE,int setC,int &singlePivotFanout,int fftscale,char *&pivotSelectionMethod,char *&partitionMethod,int &maxLeafSize,char *&dataType,int initialSize,int finalSize,int stepSize,bool bucket,int &fragmentLength,int dim,double maxR,char *&indexType,int fftOpt,char* queryFileName,int firstQuery,int lastQuery,double maxRadius,double minRadius,double step,bool verify,char *resultsFileName,int buildMode,char *indexName,int searchMode,double MF_maxRadius,double MF_middleProportion,bool putInfo,int cacheHeight,int sThreadNum,bool cacheAll,int bThreadNum,double trisectionRadius,char* selectOptimalPivots,char* partitionByOnePivot,int numberOfChildrenPathSearchingThreads);
extern void batchBulkLoad(char *&dataFileName,int &numPivot,int setE,int setC,int &singlePivotFanout,int fftscale,char *&pivotSelectionMethod,char *&partitionMethod,int &maxLeafSize,char *&dataType,int initialSize,int finalSize,int stepSize,bool bucket,int &fragmentLength,int dim,double maxR,char *&indexType,int fftOpt,char *buildingInformationFile,int buildMode,char *indexName,double MF_maxRadius,double MF_middleProportion,bool putinfo,int bThreadNum,double trisectionRadius,char* selectOptimalPivots,char* partitionByOnePivot);
extern void searchIndex(int dataBasehSize,char* queryFileName,char *dataType,int firstQuery,int lastQuery,int dim,int fragmentLength,double maxRadius,double minRadius,double step,bool verify,char *resultsFileName,int buildMode,char *indexName,int searchMode,int cacheHeight,int sThreadNum,bool cacheAll,int numberOfChildrenPathSearchingThreads);
extern void joinCharArray(char*&dest,const char *sor);

void IndexFun(char *runOption,char *dataFileName,int numPivot,int setE,int setC,int singlePivotFanout,int fftscale,char *pivotSelectionMethod,char *partitionMethod,int maxLeafSize,char *dataType,int initialSize,int finalSize,int stepSize,bool bucket,int fragmentLength,int dim,double maxR,char *indexType,int fftopt,char *queryFileName,int firstQuery,int lastQuery,double maxRadius,double minRadius,double step,bool verify,char *resultsFileName,int buildMode,char *indexName,int searchMode,double MF_maxRadius,double MF_middleProportion,bool putinfo,int cacheHeight,int tsThreadNum,bool cacheAll,int bThreadNum,double trisectionRadius,char *selectOptimalPivots,char *partitionByOnePivot,int csThreadNum)		
{
	if(strlen(resultsFileName)==0)//if "resultsFileName" is null, means user don't input result file name, so system will product a file name
		resultfilename(buildMode,numPivot,singlePivotFanout,dataType,initialSize,finalSize,dim,fragmentLength,resultsFileName,indexName,runOption,searchMode,"result");
	else if(strcmp(runOption,"search")==0)
	{
		/************    modify result file name **************/
		#ifdef _WIN32
			char *indexFileName="./SourceFiles/util/database/";
		#else
			char *indexFileName="../database/";
		#endif
			joinCharArray(indexFileName,indexName);

		int allDataObjectsInIndex = 0;//the size of database(include all data objects)
		ifstream in(indexFileName,ios::binary);
		if(in.fail())
		{//if read fail, may the index file is not exist.
			cout<<"ERROR : can't find index file!"<<endl;
			exit(1);
		}

		in.seekg(-(long)(sizeof(int)+sizeof(long)),ios::end);//find the position of the database size stored in index file
		in.read((char*)(&allDataObjectsInIndex),sizeof(int));//read size, give to "allDataObjectsInIndex"
		free(indexFileName);//release dynamic space, see in function joinCharArray
		in.close();

		finalSize = allDataObjectsInIndex;// modify the value of "finalSize", then below process of making file name is not changed.
	}

	if(strlen(indexName)==0)//if "indexName" is null, means user don't input index file name, so system will product a file name
		resultfilename(buildMode,numPivot,singlePivotFanout,dataType,initialSize,finalSize,dim,fragmentLength,indexName,NULL,runOption,searchMode,"index");

	if(strcmp(runOption,"buildandsearch")==0)
	{
		buildAndSearch(dataFileName,numPivot,setE,setC,singlePivotFanout,fftscale,pivotSelectionMethod,partitionMethod,maxLeafSize,dataType,initialSize,finalSize,stepSize,bucket,fragmentLength,dim,maxR,indexType,fftopt,queryFileName,firstQuery,lastQuery,maxRadius,minRadius,step,verify,resultsFileName,buildMode,indexName,searchMode,MF_maxRadius,MF_middleProportion,putinfo,cacheHeight,tsThreadNum,cacheAll,bThreadNum,trisectionRadius,selectOptimalPivots,partitionByOnePivot,csThreadNum);
	}
	else if(strcmp(runOption,"build")==0)
	{
		batchBulkLoad(dataFileName,numPivot,setE,setC,singlePivotFanout,fftscale,pivotSelectionMethod,partitionMethod,maxLeafSize,dataType,initialSize,finalSize,stepSize,bucket,fragmentLength,dim,maxR,indexType,fftopt,resultsFileName,buildMode,indexName,MF_maxRadius,MF_middleProportion,putinfo,bThreadNum,trisectionRadius,selectOptimalPivots,partitionByOnePivot);
	}
	else if(strcmp(runOption,"search")==0)
	{
		searchIndex(finalSize,queryFileName,dataType,firstQuery,lastQuery,dim,fragmentLength,maxRadius,minRadius,step,verify,resultsFileName,buildMode,indexName,searchMode,cacheHeight,tsThreadNum,cacheAll,csThreadNum);
	}
}

