#include "../../HeaderFiles/cluster/PartitionByOnePivot_BalanceOne.h"
#include<algorithm>

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


struct metricDataComparer
{
	shared_ptr<CMetricData> pivot;
	CMetricDistance* metric;
	bool operator() (shared_ptr<CMetricData> first, shared_ptr<CMetricData> second)
	{
		return metric->getDistance(pivot.get(), first.get()) < metric->getDistance(pivot.get(), second.get());
	}
};

void sortTaskData(vector<shared_ptr<CMetricData> > &vectorToBeSorted, shared_ptr<CMetricData> pivot, CMetricDistance* metric, int begin, int size)
{
	metricDataComparer comparer;
	comparer.metric = metric;
	comparer.pivot = pivot;
	vector<shared_ptr<CMetricData> >::iterator startIndex = vectorToBeSorted.begin() + begin;
	vector<shared_ptr<CMetricData> >::iterator endIndex = vectorToBeSorted.begin() + size;
	std::sort(startIndex, endIndex, comparer);
}

/**@brief	Thought of this classification method is: first, the data sorting, and then divided into numpartition a part of the average as far as possible, finally consider repeat points, divide these repeat points according to the variance to the appropriate section
*@param	task	To divide the task
*@param	metric	The data types of distance calculation function
*@param	numpartition	The task is divided into several parts
*@param	pivot	The pivot is going to be used
*@param	indictor	Instructions which the pivot is used, is also used as a labeled data class label
*@param	cluster_Start	Points out that the starting value of class labels
*@param	cluster_end	Points out that the end value of the class label
*/
void PartitionByOnePivot_BalanceOne::partitionByOnePivot(shared_ptr<ClusteringTask>& task,CMetricDistance *metric,int& numpartition,shared_ptr<CMetricData>& pivot,int indictor,int& cluster_Start,int& cluster_end)
{
	sortTaskData(*task->getData(), pivot, metric, 0, task->getData()->size());
	task->getMetricData_Distance()->clear();
	for(int i=0; i < task->getData()->size();++i)
	{
		task->getMetricData_Distance()->push_back(metric->getDistance(pivot.get(),((task->getData())->at(i)).get()));
	}
	numpartition = numpartition > task->getData()->size() ? task->getData()->size() : numpartition;

	//char dataFileName[100] = "./SourceFiles/util/data/dataFile_vector_";
	//char taskSize[10];
	//char xiabiao[5] = "_";
	//_itoa_s(task->getNumber(),taskSize,10);
	//strcat_s(dataFileName,taskSize);
	//strcat_s(dataFileName,xiabiao);
	//_itoa_s(task->getData()->size(),taskSize,10);
	//strcat_s(dataFileName,taskSize);

	//ofstream outDataFile(dataFileName,ofstream::app);
	//for(int i=0; i< task->getData()->size(); i++)
	//{
	//	outDataFile << task->getMetricData_Distance()->at(i) << endl;
	//}


	int task_start=0,repetitionLabel=0,varicance_temp=0;
	vector<int>clusterVariance(2);
	vector<int>clusterLabel(2);
	for(int i=0,k=0; i < numpartition; ++i,k++)
	{
		int subTaskSize = (task->getData()->size() - task_start) / (numpartition - k);
		repetitionLabel = task_start+subTaskSize-1;
		if(((repetitionLabel+1) < task->getData()->size())&&repetitionLabel !=0)
		{
			if (task->getMetricData_Distance()->at(repetitionLabel) != task->getMetricData_Distance()->at(repetitionLabel-1)&&task->getMetricData_Distance()->at(repetitionLabel) == task->getMetricData_Distance()->at(repetitionLabel+1))
			{
				subTaskSize--;
			}
			if((task->getMetricData_Distance()->at(repetitionLabel) == task->getMetricData_Distance()->at(repetitionLabel-1)&&task->getMetricData_Distance()->at(repetitionLabel) == task->getMetricData_Distance()->at(repetitionLabel+1)))
			{
				while((repetitionLabel!=0) && (task->getMetricData_Distance()->at(repetitionLabel) == task->getMetricData_Distance()->at(repetitionLabel-1)))
				{
					repetitionLabel--;
				}
				clusterLabel[0]=repetitionLabel;
				varicance_temp = task->getData()->size() + task_start-2*repetitionLabel-1;
				//varicance_temp = repetitionLabel - (task_start+subTaskSize-1);
				clusterVariance[0]= abs(varicance_temp);
				repetitionLabel = task_start+subTaskSize-1;
				while(((repetitionLabel+1) < task->getMetricData_Distance()->size()) && (task->getMetricData_Distance()->at(repetitionLabel) == task->getMetricData_Distance()->at(repetitionLabel+1)))
				{
					repetitionLabel++;
				}
				clusterLabel[1]=repetitionLabel;
				varicance_temp = task->getData()->size() + task_start-2*repetitionLabel-1;
				//varicance_temp = repetitionLabel - (task_start+subTaskSize-1);
				clusterVariance[1]= abs(varicance_temp);
				if(clusterVariance[0] < clusterVariance[1])
					subTaskSize = clusterLabel[0]-task_start;
				else
					subTaskSize = clusterLabel[1]-task_start+1;
			}
		}
		for (int j=task_start; j < task_start+subTaskSize; ++j)
		{
			task->getClusterId()->at(j) = i + indictor*numpartition + 1;
		}
		task_start += subTaskSize;
	}
	cluster_Start = indictor* numpartition + 1;
	cluster_end = (indictor+1) * numpartition;
}