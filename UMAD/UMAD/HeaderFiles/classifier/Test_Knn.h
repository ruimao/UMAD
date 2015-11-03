#ifndef TEST_KNN_H
#define TEST_KNN_H

#include"../../HeaderFiles/classifier/Test.h"
#include"../../HeaderFiles/classifier/util/MetricDataFormat.h"
#include"../../HeaderFiles/classifier/util/DatasetInMetricSpace.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <memory>

//#define  ATTR_NUM  3                  //the number of attribute
//#define  MAX_SIZE_OF_TRAINING_SET  21000      //maxsize of training set
//#define  MAX_SIZE_OF_TEST_SET      21000       //maxsize of test set
#define  MAX_VALUE  10000.0                  //maxvalue of attribute
//#define  K  7
using namespace std;

/**
 *@ file  Test_Knn.h
 *@ classification algorithm
 *@ author  from network
 *@ version  2014-08-01
 */

/**
 *@ class CTest_Knn
 *@ classification algorithm
 *@ author from network
 */

class CTest_Knn:public CTest
{
private:
struct dataVector 
{
	int ID;                      //ID number
	string classLabel;             //class label
	vector<double> attributes; //attribute
};
struct distanceStruct 
{
	int ID;                      //ID number
	double distance;             //distance
	string classLabel;             //class label
};

 vector<struct dataVector> gTrainingSet; //training set
 vector<struct dataVector> gTestSet;         //test set
 vector<struct distanceStruct> gNearestDistance;                //K nearest neighbour
 int curTrainingSetSize;                                 //size of training set
 int curTestSetSize;                                     //size of test set
 int pivotNumber;

public:
	CTest_Knn();
	~CTest_Knn();
/**
 *@ Euclidean distance between vector1=(x1,x2,...,xn) and vector2=(y1,y2,...,yn)
 */
	 double euclideanDistance(struct dataVector vect1,struct dataVector vect2);

/**
 *@ L infinity distance between vector1=(x1,x2,...,xn) and vector2=(y1,y2,...,yn)
 */
	 double LDistance(struct dataVector vect1,struct dataVector vect2);

/**
 *@ get the maxdistance in gNearestDistance and return the subscript
 */
	 int GetMaxDistance(int k);

/**
 *@ classify unclassification sample
 */
	 string Classify(struct dataVector Sample,int k);

/**
 *@show classification result
 *@trainingModelFileName: the file name of training model
 *@M_testdata: the test set under metric space that after distance calculation
 *@outfilename: outfilename is going to stroe the classify result
 *@pivotNumber: the number of selected pivots
 *@ param k: the number of nearest neighbors
 */
	 void showClassiciationResult(char *trainingModelFileName,GetMetricData M_testdata,const char *outfilename,int pivotNumber,int k);

/**
 *@ test training model of classification algorithm.
 *@ param classifyMethod  classifymethod: "knn" , "naviebayes".
 *@ param testdata: the primary trainging data.
 *@ param testDataLabel: the class label of primary training data.
 *@ param metric: distance function.
 *@ param pivotsAndTrainModelFileName: the file to store the selected pivots information and training model.
 *@ param testModelFileName: the file to store the training model.
 *@ param status: determine the source of the test data. 0: TestDataFromTrainData; 1: TestDataFromTestData.
 *@ param k: the number of nearest neighbors
*/
	 void TestModel(char *classifyMethod,vector<shared_ptr<CMetricData> > *testdata,vector<string> testDataLabel, CMetricDistance *metric,char *pivotsAndTrainModelFileName,char *testModelFileName,int status,int k,int splitRatio);
};

#endif