/**
 *@ Filename: TrainOrTest.cpp
 *@ Determine the runModel and transferred to the corresponding
 *@ Author: Ping Li
 *@ Version: 2014-09-28
 *@ 
*/


#include<iostream>
#include<cstring>
using namespace std;

/**
* @param runM  runModel[train,test,trainandtest]:input "train" to trainging a classification model and input "test" if you want to do test in the classification model trained before.
* @param t  data type, one of "peptide", "dna", "vector", "image", "msms","string".
* @param disFun  the distance function: "EuclideanDistance" , "EditDistance" , "ImageMetric" , "DNAMetric" , "PeptideMetric".
* @param psm  the pivot selection method: "random", "fft", "center", "pcaonfft", "pca","incremental".
* @param v  number of pivots in an index node.
* @param classifyMethod  classifymethod: "knn" , "naviebayes" , "c4.5".
* @param trainDFN  the traindata file name.
* @param init  size of traindata.
* @param testDFN  the testdata file name.
* @param F  size of testdata.
* @param dim  dimension of vector data to load or number of features to be loaded.
* @param ptFN the file to store pivots information and training model.
* @param testMFN  the file to store test classificaiton model result.
* @param status  determine the source of the test data. 0: TestDataFromTrainData; 1: TestDataFromTestData.
*/

extern void Train(char *dataType,char *disfun,char *pivotSelectionMethod,int numPivot,char *classifyMethod,char *trainDataFileName,int initialSize,int dim,char *pivotsAndTrainModelFileName);
extern void Test(char *dataType,char *disfun,char *classifyMethod,char *testDataFileName,int finalSize,int dim,char *pivotsAndTrainModelFileName,char *testModelFileName,int status,int k,int splitRatio);

void ClassificationFun(char *runModel,char *dataType,char *disfun,char *pivotSelectionMethod,int numPivot,char *classifyMethod,char *trainDataFileName,int initialSize,char *testDataFileName,int finalSize,int dim,char *pivotsAndTrainModelFileName,char *testModelFileName,int status,int k,int splitRatio)
{  
	if(strcmp(runModel,"train")==0)
	{

		Train(dataType,disfun,pivotSelectionMethod,numPivot,classifyMethod,trainDataFileName,initialSize,dim,pivotsAndTrainModelFileName);

	}

	else if(strcmp(runModel,"test")==0)
	{

		Test(dataType,disfun,classifyMethod,testDataFileName,finalSize,dim,pivotsAndTrainModelFileName,testModelFileName,status,k,splitRatio);

	}

	else if(strcmp(runModel,"trainandtest")==0)
	{

		Train(dataType,disfun,pivotSelectionMethod,numPivot,classifyMethod,trainDataFileName,initialSize,dim,pivotsAndTrainModelFileName);

		Test(dataType,disfun,classifyMethod,testDataFileName,finalSize,dim,pivotsAndTrainModelFileName,testModelFileName,status,k,splitRatio);

	}

}
