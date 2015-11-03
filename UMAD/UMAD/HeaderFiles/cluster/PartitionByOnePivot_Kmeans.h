#ifndef PARTITIONBYONEPIVOT_KMEANS_H
#define PARTITIONBYONEPIVOT_KMEANS_H

#include "PartitionByOnePivot.h"
#include "../metricdistance/MetricDistance.h"
#include "util/ClusteringTask.h"
#include "util/Clustering_Kmeans.h"

/** @file	partitionByOnePivot_Kmeans.h
* @classes	Inherited partitionByOnePivot
* @author	He Zhang
* @version	2014-07-23
*/

/**
* @class	partitionByOnePivot_BalanceTwo
* @brief	Inherited partitionByOnePivot,To achieve the K-means algorithm
* @author	He Zhang
*/

class PartitionByOnePivot_Kmeans : public PartitionByOnePivot 
{
public:
	/**@brief	Use K-means algorithm in data mining to clustering,Key point is  the initial point selection and termination conditions
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