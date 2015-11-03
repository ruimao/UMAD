#ifndef TEST_H
#define TEST_H
/**
 *@ Filename:  Test.h
 *@ Get test the training model of classification algorithm
 *@ Author:  Ping Li
 *@ Version: 2014-9-28
*/
#include "../../HeaderFiles/metricdata/MetricData.h"
#include "../../HeaderFiles/metricdistance/MetricDistance.h"
#include <iostream>
#include <string>
using namespace std;

class CTest
{
public:
	/** parameterless constructor */
	CTest();

	/** parameterless destructor */
	~CTest();

	/**
	 *@ test the training model of classification algorithm
	 *@ testdata: the primary test data
	 *@ metric: distance function
	 *@ status: determine the source of the test data. 0: TestDataFromTrainData; 1: TestDataFromTestData.
	 */
	virtual void TestModel(vector<shared_ptr<CMetricData> > *testdata,CMetricDistance *metric,int status);
};

#endif