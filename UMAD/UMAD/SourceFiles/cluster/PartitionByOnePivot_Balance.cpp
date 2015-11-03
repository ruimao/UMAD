#include "../../HeaderFiles/cluster/PartitionByOnePivot_Balance.h"
#include "../../HeaderFiles/cluster/util/utilFunction.h"

/** @file	PartitionByOnePivot_Balance.h
* @classes	Inherited partitionByOnePivot
* @author	He Zhang
* @version	2014-07-23
*/

/**
* @class	PartitionByOnePivot_Balance
* @brief	Inherited partitionByOnePivot,To achieve the Balance
* @author	He Zhang
*/

/**@brief	Thought of this classification method is: first, choose the data as possible numpartition points of data points, and then to compare the data set data in their labels used to identify these data points belonging to the class, finally consider repeat points, divide these repeat points according to the variance to the appropriate section
*@param	task	To divide the task
*@param	metric	The data types of distance calculation function
*@param	numpartition	The task is divided into several parts
*@param	pivot	The pivot is going to be used
*@param	indictor	Instructions which the pivot is used, is also used as a labeled data class label
*@param	cluster_Start	Points out that the starting value of class labels
*@param	cluster_end	Points out that the end value of the class label
*/
void PartitionByOnePivot_Balance::partitionByOnePivot(shared_ptr<ClusteringTask>& task,CMetricDistance *metric,int& numpartition,shared_ptr<CMetricData>& pivot,int indictor,int& cluster_Start,int& cluster_end)
{
	int taskSize = task->getData()->size();
	task->getMetricData_Distance()->clear();
	for(int i=0; i < taskSize;++i)
	{
		task->getMetricData_Distance()->push_back(metric->getDistance(pivot.get(),((task->getData())->at(i)).get()));
	}
	numpartition = numpartition > taskSize ? taskSize : numpartition;

	shared_ptr<vector<double> > metricData_distance_temp(new vector<double>());
	for(int i=0; i < task->getMetricData_Distance()->size(); ++i)
	{
		metricData_distance_temp->push_back(task->getMetricData_Distance()->at(i));
	}
	int task_start=0,subTaskSize=0,max_k=0;
	vector<double> max_k_value;
	vector<vector<int> > equal_k_label(numpartition-1);
	vector<int>clusteringSize(numpartition,0);
	for(int i=0,k=0; i< numpartition; ++i,k++)
	{
		subTaskSize = (taskSize - task_start) / (numpartition -k);
		max_k = taskSize - (task_start+subTaskSize) + 1;
		max_k_value.push_back(utilFunction::select_max_k_iteration(metricData_distance_temp,taskSize,max_k));
		task_start += subTaskSize;
	}
	for(int i=0; i < taskSize;++i)
	{
		double distanceValue = task->getMetricData_Distance()->at(i);
		for(int j=0; j< max_k_value.size(); ++j)
		{
			if((distanceValue < max_k_value[j]) || (distanceValue == max_k_value[j] && j == max_k_value.size()-1))
			{
				task->getClusterId()->at(i) = j + indictor*numpartition + 1;
				clusteringSize[j]++;
				break;
			}
			if (distanceValue == max_k_value[j] && j < max_k_value.size()-1)
			{
				equal_k_label[j].push_back(i);
				break;
			}
		}
	}
	for(int i=0; i < equal_k_label.size(); ++i)
	{
		int leftTaskSize = clusteringSize[i] + equal_k_label[i].size() - clusteringSize[i+1];
		int rightTaskSize = clusteringSize[i+1] + equal_k_label[i].size() - clusteringSize[i];
		if(abs(leftTaskSize) <= abs(rightTaskSize))
		{
			for(int j=0; j < equal_k_label[i].size(); ++j)
			{
				task->getClusterId()->at(equal_k_label[i][j]) = i + indictor*numpartition + 1;
			}
		}
		else
		{
			for(int j=0; j < equal_k_label[i].size(); ++j)
			{
				task->getClusterId()->at(equal_k_label[i][j]) = i + indictor*numpartition + 2;
			}
		}
	}
	cluster_Start = indictor* numpartition + 1;
	cluster_end = (indictor+1) * numpartition;
}