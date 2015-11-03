#ifndef PARTITIONBYONEPIVOT_BALANCEONE_H
#define PARTITIONBYONEPIVOT_BALANCEONE_H

#include "PartitionByOnePivot.h"
#include "../metricdistance/MetricDistance.h"
#include "util/ClusteringTask.h"

/** @file	partitionByOnePivot_BalanceOne.h
* @classes	Inherited partitionByOnePivot
* @author	He Zhang
* @version	2014-07-23
*/

/**
* @class	partitionByOnePivot_BalanceOne
* @brief	Inherited partitionByOnePivot,To achieve the Balance
* @author	He Zhang
*/

class PartitionByOnePivot_BalanceOne : public PartitionByOnePivot 
{
public:
	/**@brief	Thought of this classification method is: first, the data sorting, and then divided into numpartition a part of the average as far as possible, finally consider repeat points, divide these repeat points according to the variance to the appropriate section
	*@param	task	To divide the task
	*@param	metric	The data types of distance calculation function
	*@param	numpartition	The task is divided into several parts
	*@param	pivot	The pivot is going to be used
	*@param	indictor	Instructions which the pivot is used, is also used as a labeled data class label
	*@param	cluster_Start	Points out that the starting value of class labels
	*@param	cluster_end	Points out that the end value of the class label
	*/
	virtual void partitionByOnePivot(shared_ptr<ClusteringTask>&task,CMetricDistance *metric,int& numpartition,shared_ptr<CMetricData>& pivot,int indictor,int& cluster_Start,int& cluster_end);

};

#endif