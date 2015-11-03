#include "../../HeaderFiles/cluster/PartitionByOnePivot_Trisection.h"
#include "../../HeaderFiles/cluster/util/utilFunction.h"

/** @file	partitionByOnePivot_Trisection.h
* @classes	Inherited partitionByOnePivot
* @author	He Zhang
* @version	2014-07-23
*/

/**
* @class	partitionByOnePivot_BalanceTwo
* @brief	Inherited partitionByOnePivot,According to the user's parameters r ,the data points is divided into three parts
* @author	He Zhang
*/

/**@brief	According to the user's parameters r ,the data points is divided into three parts.meanwhile Repeat points also need to consider
*@param	task	To divide the task
*@param	metric	The data types of distance calculation function
*@param	numpartition	The task is divided into several parts
*@param	pivot	The pivot is going to be used
*@param	indictor	Instructions which the pivot is used, is also used as a labeled data class label
*@param	cluster_Start	Points out that the starting value of class labels
*@param	cluster_end	Points out that the end value of the class label
*/

/**@brief Have a parameter constructor
*@param trisectionRadius	The radius of the designated by the user
*/
PartitionByOnePivot_Trisection::PartitionByOnePivot_Trisection(double trisectionRadius)
{
	this->trisectionRadius = trisectionRadius;
}

void PartitionByOnePivot_Trisection::partitionByOnePivot(shared_ptr<ClusteringTask>& task,CMetricDistance* metric,int& numpartition,shared_ptr<CMetricData>& pivot,int indictor,int& cluster_Start,int& cluster_end)
{
	int pivotSize = task->getNewPivots()->size();
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
	vector<double>* kth_quantiles = new vector<double>();
	vector<vector<int> > equal_k_label(numpartition-1);
	vector<int>clusteringSize(numpartition,0);
	double partitionRadius = this->trisectionRadius;
	double mediaValue = 0;
	for(int i=1;i < numpartition; i++)
	{
		kth_quantiles->push_back(utilFunction::select_min_k_iteration(metricData_distance_temp,taskSize,i*taskSize/numpartition + 1));
	}
	if(kth_quantiles->size() == 2 && (kth_quantiles->at(1) - kth_quantiles->at(0)) < 2 * partitionRadius)
	{
		mediaValue =  utilFunction::select_min_k_iteration(metricData_distance_temp,taskSize,1.0 * taskSize/2 + 1);
		kth_quantiles->at(0) = mediaValue-partitionRadius;
		kth_quantiles->at(1) = mediaValue+partitionRadius;
	}
	//double minValue = utilFunction::find_min(*metricData_distance_temp.get());
	//double maxValue = utilFunction::find_max(*metricData_distance_temp.get());

	//if((maxValue-minValue) > (kth_quantiles->at(1)-kth_quantiles->at(0))) 
	if(kth_quantiles->at(0) > 0) 
	{
		for(int i=0; i < taskSize;++i)
		{
			double distanceValue = task->getMetricData_Distance()->at(i);

			for(int j=0; j< kth_quantiles->size(); ++j)
			{
				if(distanceValue < kth_quantiles->at(j) && task->getClusterId()->at(i) == -1)
				{
					task->getClusterId()->at(i) = j + indictor*numpartition + 1;
					clusteringSize[j]++;
					break;
				}
				if(j== kth_quantiles->size()-1 && distanceValue > kth_quantiles->at(j))
				{
					task->getClusterId()->at(i) = j + indictor*numpartition + 2;
					clusteringSize[j+1]++;
				}
				if (distanceValue == kth_quantiles->at(j))
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
	}
	else
	{
		for(int i=0; i < taskSize; ++i)
		{
			task->getClusterId()->at(i) = indictor* numpartition + 1;
		}
	}
	cluster_Start = indictor* numpartition + 1;
	cluster_end = (indictor+1) * numpartition;
}