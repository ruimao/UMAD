using namespace std;


#include"../../HeaderFiles/classifier/Train.h"
#include"../../HeaderFiles/classifier/util/MetricDataFormat.h"
#include"../../HeaderFiles/classifier/util/svm.h"
//#include "../../HeaderFiles/metricdata/MetricData.h"
#include"../../HeaderFiles/index/PivotSelectionMethod.h"

#include <memory>
#include <iostream>
#include<math.h>
#include <vector>
#include <list>
#include <set>
#include <map>



struct point {                           
	double *feature; 
	int value;
};


class CTrain_SVM : public CTrain{
public:
	CTrain_SVM(GetMetricData inputSet);
	~CTrain_SVM();
	list<point> LoadTrianData();
	vector<int> BuidLabelMap(vector<string>);
	void InitClassifier();
	void CallSVM(list<point> pointList,char *pivotsAndTrainModelFileName);
	void TrainModel(char *pivotsAndTrainModelFileName);
private:
	GetMetricData svm_trainSet;
	int sampleNum;
	int featureNum;	
	svm_parameter param;
	svm_problem prob;
};