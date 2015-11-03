#include<cstring>
#include<sstream>
#include<direct.h>
#include<fstream>
#include<iostream>

using namespace std;

extern void joinCharArray(char*&dest,const char *sor);

void resultfilename_1(char *resultsFileName,char *dataType,char *method){
	strcpy(resultsFileName,method);
	strcat(resultsFileName,dataType);
	strcat(resultsFileName,"_");
}

void resultfilename_3(char *resultsFileName,int num,char *str){
	strcat(resultsFileName,str);
	stringstream ssTemp;
	string strTemp;
	ssTemp<<num;
	ssTemp>>strTemp;
	strcat(resultsFileName,strTemp.c_str());
}

void resultfilename_2(char *resultsFileName,char *dataType,int finalSize,int dim,int fragment,int numPivot,int singlePivotFanout,char *which){
	char intTemp[32]="";
	stringstream ssTemp;
	string strTemp;

	//when parameter which="index", don't need "datasize", because we will add "datasise" into file name in function buildAndSearch and batchBulkLoad
	if(strcmp(which,"result")==0)
	{ 
		resultfilename_3(resultsFileName,finalSize,"datasize_") ;
		strcat(resultsFileName,"_");
	}

	//dna and peptide need "fragment", vector and image need "dim", string neither need "fragment" nor "dim"
	if(strcmp(dataType,"dna")==0 || strcmp(dataType,"peptide")==0)
	{
		resultfilename_3(resultsFileName,fragment,"fragment_") ;
		strcat(resultsFileName,"_");
	}
	else if(strcmp(dataType,"vector")==0 || strcmp(dataType,"image")==0)
	{
		resultfilename_3(resultsFileName,dim,"dim_") ;
		strcat(resultsFileName,"_");
	}
	
	resultfilename_3(resultsFileName,numPivot,"numPivot_") ;
	strcat(resultsFileName,"_");
	resultfilename_3(resultsFileName,singlePivotFanout,"singlePivotFanout_") ;

}


/**
*brief : product file name, include result file name and index file name. if "which"="result", means to product a result file name.
		if "which"="index", means to product an index file name.


*param which : "which=index" means to make an index file name, "which=result" means to make a result file name
*param resultsFileName : the file name will be producted, may be result file name, or index file name, see the call in BuildORQueryIndex.cpp
*param indexFile : this parameter is only used when "runOption"="search", because we need the file name to read index file, so as to read the size of whole index

*/
void resultfilename(int buildMode,int numPivot,int singlePivotFanout,char *dataType,int initialSize,int &finalSize,int dim, int fragment, char *resultsFileName, char *indexFile,char *runOption,int searchMode,char * which)
{
	
	if(strcmp(runOption,"build")==0)
	{
		if(initialSize==finalSize)
		{
			resultfilename_1(resultsFileName,dataType,"build_") ;
			resultfilename_2(resultsFileName,dataType,finalSize,dim,fragment,numPivot,singlePivotFanout,which) ;
		}
		else
		{
			resultfilename_1(resultsFileName,dataType,"build_") ;

			//if which="index", here don't need "datasize", because we will add "datasize" into file name in function "buildAndSearch" and "batchBulkLoad"
			if(strcmp(which,"result")==0)
			{
				resultfilename_3(resultsFileName,initialSize,"datasize_") ;
				resultfilename_3(resultsFileName,finalSize,"-") ;
				strcat(resultsFileName,"_");
			}	

			//dna and peptide need "fragment", vector and image need "dim", string neither need "fragment" nor "dim"
			if(strcmp(dataType,"dna")==0 || strcmp(dataType,"peptide")==0)
				resultfilename_3(resultsFileName,fragment,"fragment_") ;
			else if(strcmp(dataType,"vector")==0 || strcmp(dataType,"image")==0) resultfilename_3(resultsFileName,dim,"dim_") ;

			strcat(resultsFileName,"_");
			resultfilename_3(resultsFileName,numPivot,"numPivot_") ;
			strcat(resultsFileName,"_");
			resultfilename_3(resultsFileName,singlePivotFanout,"singlePivotFanout_") ;
		}
		resultfilename_3(resultsFileName,buildMode,"_buildMode_");
	}
	else if(strcmp(runOption,"search")==0)
	{
		/************    modify result file name **************/
		#ifdef _WIN32
			char *indexFileName="./SourceFiles/util/database/";
		#else
			char *indexFileName="../database/";
		#endif
			joinCharArray(indexFileName,indexFile);

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

		resultfilename_1(resultsFileName,dataType,"search_") ;
		resultfilename_2(resultsFileName,dataType,finalSize,dim,fragment,numPivot,singlePivotFanout,which) ;
		strcat(resultsFileName,"_");
		resultfilename_3(resultsFileName,searchMode,"searchMode_");
		
	}
	else
	{
		resultfilename_1(resultsFileName,dataType,"buildAndSearch_") ;
		resultfilename_2(resultsFileName,dataType,finalSize,dim,fragment,numPivot,singlePivotFanout,which) ;
		strcat(resultsFileName,"_");
		resultfilename_3(resultsFileName,buildMode,"buildMode_");
		strcat(resultsFileName,"_");
		resultfilename_3(resultsFileName,searchMode,"searchMode_") ;
	}

	//result file name need ".txt", but index file name don't need
	if(strcmp(which,"result")==0) strcat(resultsFileName,".txt");
}



