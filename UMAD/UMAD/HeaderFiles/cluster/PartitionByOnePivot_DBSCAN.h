#ifndef PARTITIONBYONEPIVOT_DBSCAN_H
#define PARTITIONBYONEPIVOT_DBSCAN_H

#include "PartitionByOnePivot.h"
#include "../metricdistance/MetricDistance.h"
#include "../metricdistance/DNAMetric.h"
#include "../metricdistance/EditDistance.h"
#include "../metricdistance/EuclideanDistance.h"
#include "../metricdistance/ImageMetric.h"
#include "../metricdistance/MSMSMetric.h"
#include "../metricdistance/PeptideMetric.h"
#include "../metricdistance/RNAMetric.h"
#include "../metricdistance/CountedMetric.h"
#include "util/ClusteringTask.h"

/** @file	partitionByOnePivot_DBSCAN.h
* @classes	Inherited partitionByOnePivot
* @author	He Zhang
* @version	2014-07-23
*/

/**
* @class	partitionByOnePivot_BalanceTwo
* @brief	Inherited partitionByOnePivot,To achieve the DBSCAN algorithm
* @author	He Zhang
*/

class PartitionByOnePivot_DBSCAN : public PartitionByOnePivot 
{
public:
	/**@brief	Use DBSCAN algorithm in data mining to clustering,Key point is the determination of the radius, hash function and the handling of a large number of repeat points
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