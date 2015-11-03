#include "../../HeaderFiles/cluster/SelectOptimalPivots_Variance.h"
#include "../../HeaderFiles/cluster/PartitionByOnePivot_Kmeans.h"
#include "../../HeaderFiles/cluster/util/utilFunction.h"

/** @file	SelectOptimalPivots_Variance.h
* @classes	Inherited SelectOptimalPivots
* @author	He Zhang
* @version	2014-07-23
*/

/**
* @class	SelectOptimalPivots_Variance
* @brief	Select the pivot in variance
* @author	He Zhang
*/

/**@brief	Select the pivot in variance
*@param	metric	The data types of distance calculation function
*@param	pivots	Through choosing the pivot method has been chosen to the pivot
*@param	data	dataset of the current task
*@param	numpartition	the number of the data should be divided
*@param	maxLeafSize		the number of the leaf node
*/
vector<shared_ptr<CMetricData> > SelectOptimalPivots_Variance::selectOptimalPivots(CMetricDistance* metric,const vector<shared_ptr<CMetricData> > &pivots,vector<shared_ptr<CMetricData> > &data,int numPartition,int maxLeafSize)
{
	vector<int> clusterData(numPartition,0);
	vector<int>pivotUse(pivots.size(),0);
	vector<int> clusterId(data.size(),0); 
	vector<shared_ptr<CMetricData> > newPivots =pivots;
	vector<double> variance(pivots.size(),0);
	shared_ptr<ClusteringTask> task(new ClusteringTask(data,clusterId,0,newPivots,pivotUse,0,-1,0));
	for (int i=0; i < pivots.size(); ++i)
	{
		task->getMetricData_Distance()->clear();
		for (int j=0; j < task->getData()->size(); ++j)
		{
			task->getMetricData_Distance()->push_back(metric->getDistance(pivots[i].get(),((task->getData())->at(j)).get()));
		}
		numPartition = numPartition > task->getData()->size() ? task->getData()->size() : numPartition;
		Clustering_Kmeans::K_Means_Algorithm(task,numPartition,i);
		for (int j=0; j < task->getMetricData_Distance()->size(); ++j)
		{
			for(int k = i*numPartition+1; k <= (i+1)*numPartition; ++k)
			{
				if (task->getClusterId()->at(j) == k)
					clusterData[k-i*numPartition-1] += 1;
			}
		}
		int Expectation = task->getData()->size();
		Expectation /= numPartition;
		for (int j=0; j< numPartition; ++j)
		{
			variance[i] += pow((clusterData[j] - Expectation),2);
		}
		variance[i] /= numPartition;
		variance[i] = sqrt(variance[i]);
	}
	int minValueNum = 1;
	newPivots.clear();
	shared_ptr<vector<double> > variance_temp(new vector<double>());
	for(int i=0; i < variance.size(); ++i)
	{
		variance_temp->push_back(variance[i]);
	}
	for(int i=0;i< pivots.size(); ++i)
	{
		double pivotLabel = utilFunction::select_min_k_iteration(variance_temp,variance.size(),minValueNum);
		for(int j=0; j<variance.size();j++)
		{
			if(pivotLabel == variance[j])
			{
				newPivots.push_back(pivots[j]);
				break;
			}
		}

		minValueNum++;
	}
	return newPivots;
}