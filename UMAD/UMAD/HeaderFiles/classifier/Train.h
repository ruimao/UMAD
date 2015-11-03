#ifndef TRAIN_H
#define TRAIN_H
/**
 *@ Filename:  Train.h
 *@ Get training model of classification algorithm
 *@ Author:  Ping Li
 *@ Version: 2014-9-28
*/
#include "../../HeaderFiles/metricdata/MetricData.h"
#include "../../HeaderFiles/metricdistance/MetricDistance.h"
#include <iostream>
#include <string>
using namespace std;

class CTrain
{
public:
	/** parameterless constructor */
	CTrain();

	/** parameterless destructor */
	~CTrain();

	/**
	 *@ get training model of classification algorithm
	 *@ traindata: the primary training data
	 *@ metric: distance function
	 */
	virtual void TrainModel(vector<shared_ptr<CMetricData> > *traindata,CMetricDistance *metric);
};

#endif