#pragma once
#ifndef  _PREDICTCLASS_
#define  _PREDICTCLASS_
#pragma message("PredictClass program") 
#include"../../HeaderFiles/classifier/Train_NavieBayes.h"
#include"../../HeaderFiles/classifier/Test.h"
#include"../../HeaderFiles/classifier/util/GetValue.h"
#include"../../HeaderFiles/classifier/util/MetricDataFormat.h"
#include <algorithm>

#ifndef _ITERATOR_DEBUG_LEVEL
#define _ITERATOR_DEBUG_LEVEL 0
#else
#undef _ITERATOR_DEBUG_LEVEL
#define _ITERATOR_DEBUG_LEVEL 0
#endif


class CTest_NavieBayes:public CTest
{
private:
	int Inc;
	char	Delimiter;
	GetMetricData TestData;
	void GetData(char *trainingModelFileName);
	void  Error(int n, string s1, string s2);
	Description GetDescription(int itemNo,GetMetricData Df);

	/* use to store the attributes and class information of all variables in .names file*/
	string *ClassName;//name of class
	string *AttName;//name of attribute
	string **AttValName;//name of attribute value
	short *MaxAttValNo;//the number of categorical attribute.  It starts at 1 deposit£¬0 position to deal with illegal in attribute values.If it's 0£¬said the property for continuous attributes, need discretization.
	short MaxTestItemNo;//the maximum number of all test data
	short MaxDiscrValNo;//the maximum number of all discrete attributes value
	short MaxClassNo;//the number of classes starting from 0, so the total number is MaxClassNo+1  
	short MaxAttNo;//the number of attributes starting from 0, so the total number is MaxAttNo+1
	int MaxItemNo;//number of training set

    /* use to store all variables in .data file*/
	Description	*TestItem;
	void show(const char *resultFileName);
	double ***PostFreq;		/* PostFreq[class][attribute][attribute_value],Used to store trained model*/
	double *SplitPoint;    //use to store continuous attribute discrete points value
	double *ClassFreq;	/* ClassFreq[c] = no. items of class c */
public:
	int TruePositive;
	int FalsePositive;
	double accuracy;
	CTest_NavieBayes();
	~CTest_NavieBayes();
	void Predict(GetMetricData data,char *trainingModelFileName,const char *showClassificationResult,int pivotsNum);

	/**
	 *@ test training model of classification algorithm.
	 *@ param classifyMethod  classifymethod: "knn" , "naviebayes".
	 *@ param testdata: the primary trainging data.
	 *@ param testDataLabel: the class label of primary training data.
	 *@ param metric: distance function.
	 *@ param pivotsAndTrainModelFileName: the file to store the selected pivots information and training model.
	 *@ param testModelFileName: the file to store the training model.
	 *@ param status: determine the source of the test data. 0: TestDataFromTrainData; 1: TestDataFromTestData.
	*/
	 void TestModel(char *classifyMethod,vector<shared_ptr<CMetricData> > *testdata,vector<string> testDataLabel, CMetricDistance *metric,char *pivotsAndTrainModelFileName,char *testModelFileName,int status,int splitRatio);
};
#endif
