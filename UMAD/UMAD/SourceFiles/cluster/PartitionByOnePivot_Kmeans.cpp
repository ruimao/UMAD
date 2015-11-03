#include "../../HeaderFiles/cluster/PartitionByOnePivot_Kmeans.h"

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

/**@brief	Use K-means algorithm in data mining to clustering,Key point is  the initial point selection and termination conditions
*@param	task	To divide the task
*@param	metric	The data types of distance calculation function
*@param	numpartition	The task is divided into several parts
*@param	pivot	The pivot is going to be used
*@param	indictor	Instructions which the pivot is used, is also used as a labeled data class label
*@param	cluster_Start	Points out that the starting value of class labels
*@param	cluster_end	Points out that the end value of the class label
*/

void PartitionByOnePivot_Kmeans::partitionByOnePivot(shared_ptr<ClusteringTask>& task,CMetricDistance *metric,int& numpartition,shared_ptr<CMetricData>& pivot,int indictor,int& cluster_Start,int& cluster_end)
{
	task->getMetricData_Distance()->clear();
	for(int i=0; i < task->getData()->size();++i)
	{
		task->getMetricData_Distance()->push_back(metric->getDistance(pivot.get(),((task->getData())->at(i)).get()));
	}
	numpartition = numpartition > task->getData()->size() ? task->getData()->size() : numpartition;

	Clustering_Kmeans::K_Means_Algorithm(task,numpartition,indictor);
	cluster_Start = indictor* numpartition + 1;
	cluster_end = (indictor+1) * numpartition;
}