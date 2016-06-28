#include"../../HeaderFiles/classifier/Test.h"
#include"../../HeaderFiles/classifier/util/MetricDataFormat.h"
#include"../../HeaderFiles/classifier/util/svm.h"

#include<list>
#include<map>
#include <iostream>

using namespace std;

struct pointTest {
	double *feature;
	int value;
};

class CTest_SVM : public CTest
{
public:
	CTest_SVM(GetMetricData);
	~CTest_SVM();
	list<pointTest> LoadTestData();
	vector<int> BuildLabelMap(char *file,vector<string>);
	void Predict(list<pointTest>,char *pivotsAndTrainModelFileName,char *testModelFileName);
	virtual void TestModel(char *pivotsAndTrainModelFileName,char *testModelFileName);
private:
	GetMetricData svm_testSet;
	int sampleNum;
	int featureNum;


};