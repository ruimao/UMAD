#ifndef SELECTOPTIMALPIVOTS_H
#define SELECTOPTIMALPIVOTS_H

#include "../metricdata/MetricData.h"
#include "../metricdistance/MetricDistance.h"
#include "util/ClusteringTask.h"

/** @file	SelectOptimalPivots.h
* @classes	select optimal pivots from the current povit set
* @author	He Zhang
* @version	2014-07-23
*/

/**
* @class	PivotWisePartitionMethod
* @brief	select optimal pivots from the current povit set
* @author	He Zhang
*/

class SelectOptimalPivots 
{
public:
	/**@brief	select optimal pivots from the current povit set
	*@param	metric	The data types of distance calculation function
	*@param	pivots	Through choosing the pivot method has been chosen to the pivot
	*@param	data	dataset of the current task
	*@param	numpartition	the number of the data should be divided
	*@param	maxLeafSize		the number of the leaf node
	*/
	virtual vector<shared_ptr<CMetricData> > selectOptimalPivots(CMetricDistance *metric, const vector<shared_ptr<CMetricData> > &pivots,vector<shared_ptr<CMetricData> > &data,int numpartition,int maxLeafSize)=0;

};

#endif