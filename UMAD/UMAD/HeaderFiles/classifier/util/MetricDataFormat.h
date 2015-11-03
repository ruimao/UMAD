#ifndef METRICDATAFORMAT_H
#define METRICDATAFORMAT_H

#include<string>
#include<vector>

using namespace std;

/**
 * @file MetricDataFormat.h

 * @defines a data structure for storing metric space under the training data set 

 * @author Ping Li

 * @version 2014-08-26
 
 * this is a data structure for storing metric space under the trainint data set.
*/

/**
 * @structure MetricDataFormat

 * @a data structure for storing metric space under the trainint data set.

 * @param metricData: store the data set that after pivots selection and distance calculating
 
 * @param metricDataLaebl: stroe the class label of train data set

 * @author Ping Li

 * this is a data structure for storing metric space under the trainint data set.
*/

typedef struct GetMetricData
{
	vector<vector<double> > metricData;
	vector<string> metricDataLabel;
};

#endif