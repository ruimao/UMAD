#include "../../HeaderFiles/classifier/Test.h"

/** parameterless constructor */
CTest::CTest()
{

}

/** parameterless destructor */
CTest::~CTest()
{

}

/**
 *@ test the training model of classification algorithm
 *@ testdata: the primary test data
 *@ metric: distance function
 *@ status: determine the source of the test data. 0: TestDataFromTrainData; 1: TestDataFromTestData.
 */
void CTest::TestModel(vector<shared_ptr<CMetricData> > *testdata,CMetricDistance *metric,int status)
{

}