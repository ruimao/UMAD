#include "../../HeaderFiles/cluster/PivotWisePartitionMethod.h"
#include "../../HeaderFiles/cluster/util/Clustering_Kmeans.h"
#include "../../HeaderFiles/metricdata/DoubleVector.h"
#include "../../HeaderFiles/cluster/util/utilFunction.h"

#include "../../HeaderFiles/cluster/PartitionByOnePivot_BalanceOne.h"
#include "../../HeaderFiles/cluster/PartitionByOnePivot_Balance.h"
#include "../../HeaderFiles/cluster/PartitionByOnePivot_DBSCAN.h"
#include "../../HeaderFiles/cluster/PartitionByOnePivot_Kmeans.h"
#include "../../HeaderFiles/cluster/PartitionByOnePivot_Trisection.h"
#include "../../HeaderFiles/cluster/SelectOptimalPivots_Sequence.h"
#include "../../HeaderFiles/cluster/SelectOptimalPivots_Variance.h"

#include <cmath>
#include <map>
#include<algorithm>
#include <cstring>

/** @file	PivotWisePartitionMethod.h
* @classes	Inherited CPartitionMethod
* @author	He Zhang
* @version	2014-07-23
*/

/**
* @class	PivotWisePartitionMethod
* @brief	Inherited CPartitionMethod,To achieve data partition
* @author	He Zhang
*/

/**@brief	do the partition according to the given parameters
@param metric	a function used to calculate the distance of two data objects
@param pivots	a vector container consists of some vantage data points
@param data		this is the whole data set, which include all the data objects in the index strcture
@param first	the first object address of a fragment that will be partitioned into many small partitions. the fragment, which will be partitioned, is a small section of the whole data set
@param size		the size of the fragment that will be partition into many small partitions
@param maxRadius	the maximum partition radius value
@param numPartition		the maximal number of partitions into which the given data set will be partitioned
@param maxLeafSize		the maximal number of objects included in the leaf node of mvp-tree
@param middleProportion
@return		return a CPartitionResults object which pack the partition results together
*/
CPartitionResults PivotWisePartitionMethod::partition(CMetricDistance *metric, const vector<shared_ptr<CMetricData> > &pivots,vector<shared_ptr<CMetricData> > &data,int first,int size,double maxRadius, int maxPartitions,int maxLeafSize, double middleProportion,double trisectionRadius,char* selectOptimalPivots,char* partitionByOnePivot)
{
	return partition(metric,  pivots , data, first,  size,  maxPartitions, maxLeafSize,trisectionRadius,selectOptimalPivots,partitionByOnePivot);
}

/**@brief do the partition according to the given parameters
@param metric	a function used to calculate the distance of two data objects
@param pivots	a vector container consists of some vantage data points
@param data		this is the whole data set, which include all the data objects in the index strcture
@param first	the first object address of a fragment that will be partitioned into many small partitions. the fragment, which will be partitioned, is a small section of the whole data set
@param size		the size of the fragment that will be partition into many small partitions
@param numPartitions	the maximal number of partitions into which the given data set will be partitioned
@param maxLeafSize	the maximal number of objects included in the leaf node of mvp-tree
@return		return a CPartitionResults object which pack the partition results together
*/
CPartitionResults PivotWisePartitionMethod::partition(CMetricDistance *metric, const vector<shared_ptr<CMetricData> > &pivots,vector<shared_ptr<CMetricData> > &data,int first,int size,int numPartitions,int maxLeafSize,double trisectionRadius,char* selectOptimalPivots,char* partitionByOnePivot)
{
	vector<shared_ptr<CMetricData> > data_current;
	for(int i=first; i < first + size; ++i)
	{
		data_current.push_back(data[i]);
	}
	vector<int>offset;
	offset.push_back(first);
	vector<vector<double> >upper(pivots.size());
	vector<vector<double> >lower(pivots.size());
	shared_ptr<SelectOptimalPivots> selectPivotCase;
	if(!strcmp(selectOptimalPivots,"Sequence"))
	{
		selectPivotCase.reset(new SelectOptimalPivots_Sequence());
	}
	else 
	{
		selectPivotCase.reset(new SelectOptimalPivots_Variance());
	}
	vector<shared_ptr<CMetricData> > newPivots = selectPivotCase->selectOptimalPivots(metric,pivots,data_current,numPartitions,maxLeafSize);
	vector<int>pivotUse(pivots.size(),0);
	vector<shared_ptr<ClusteringTask> > taskList;
	vector<int> clusterId(data_current.size(),-1); 
	int isdeal = 0;
	shared_ptr<ClusteringTask> task(new ClusteringTask(data_current,clusterId,isdeal,newPivots,pivotUse,0,-1,0));
	vector<shared_ptr<ClusteringTask> > taskClustering_temp;
	int tasklist_indictor = 0;
	shared_ptr<PartitionByOnePivot> partitonCase;
	if(!strcmp(partitionByOnePivot,"Balance"))
	{
		partitonCase.reset(new PartitionByOnePivot_Balance());
	}
	else if(!strcmp(partitionByOnePivot,"Kmeans"))
	{
		partitonCase.reset(new PartitionByOnePivot_Kmeans());
	}
	else if(!strcmp(partitionByOnePivot,"DBSCAN"))
	{
		partitonCase.reset(new PartitionByOnePivot_DBSCAN());
	}
	else if(!strcmp(partitionByOnePivot,"Trisection"))
	{
		partitonCase.reset(new PartitionByOnePivot_Trisection(trisectionRadius));
	}
	else
	{
		partitonCase.reset(new PartitionByOnePivot_BalanceOne());
	}
	do
	{
		shared_ptr<CMetricData> pivot;
		int indictor = 0;
		int pivotSize = task->getNewPivots()->size();
		for (int i=0; i < pivotSize; ++i)
		{
			if (task->getPivotUse()->at(i) ==0)
			{
				pivot = task->getNewPivots()->at(i);
				indictor = i;
				break;
			}
		}
		int cluster_Start = indictor*numPartitions + 1;
		int cluster_end = (indictor+1)*numPartitions;
		int numPartitons_temp = numPartitions;
		partitonCase->partitionByOnePivot(task,metric,numPartitions,pivot,indictor,cluster_Start,cluster_end);
		vector<shared_ptr<ClusteringTask> > task_clustering(numPartitions);
		for(int i=0;i < numPartitions; ++i)
		{
			vector<shared_ptr<CMetricData> > data_temp = vector<shared_ptr<CMetricData> >();
			vector<int> clusterId_temp = vector<int>();
			task_clustering[i].reset(new ClusteringTask(data_temp,clusterId_temp,0,newPivots,pivotUse,0,-1,0));
		}
		for(int i=0; i < task->getData()->size(); ++i)
		{
			for(int j=cluster_Start; j <= cluster_end; ++j)
			{
				if (task->getClusterId()->at(i) == j)
				{
					task_clustering[j-cluster_Start]->getData()->push_back(task->getData()->at(i));
					task_clustering[j-cluster_Start]->getClusterId()->push_back(-1);
					task_clustering[j-cluster_Start]->getMetricData_Distance()->push_back(task->getMetricData_Distance()->at(i));
				}
			}
		}
		for(int i=0;i<task_clustering.size();i++)
		{
			if(task_clustering[i]->getData()->size() ==0)
			{
				task_clustering.erase(task_clustering.begin()+i);
				i--;
			}
		}
		int taskNumber = taskList.size();
		for(int i=0;i< task_clustering.size();++i)
		{
			task_clustering[i]->setNumber(++taskNumber);
			task_clustering[i]->setFatherNum(task->getNumber());
			int levelNum =0;
			for (int j=0; j< task->getPivotUse()->size(); ++j)
				levelNum += task->getPivotUse()->at(j);
			if(levelNum == task->getNewPivots()->size())
				task_clustering[i]->setIsLeafNode(1);
			else
				task_clustering[i]->setIsLeafNode(0);
		}
		for (int i=0;i<task_clustering.size(); ++i)
		{
			task_clustering[i]->setIsDeal(0);
			task_clustering[i]->setNewPivots(*task->getNewPivots());
			task_clustering[i]->setPivotUse(indictor);
		}
		task->setIsDeal(1);
		taskClustering_temp = task_clustering;
		numPartitions = numPartitons_temp;
		for(int i=0;i < taskClustering_temp.size(); ++i)
		{
			taskList.push_back(taskClustering_temp.at(i));
		}
		for(int i=0; i< taskList.size();++i)
		{
			task = shared_ptr<ClusteringTask>();
			tasklist_indictor = i;
			int levelNum =0;
			for (int j=0; j< newPivots.size(); ++j)
				levelNum += taskList[i]->getPivotUse()->at(j);
			if(!taskList[i]->getIsDeal() && taskList[i]->getData()->size()>maxLeafSize && levelNum < newPivots.size())
			{
				task = taskList[i];
				break;
			}
			else if((!taskList[i]->getIsDeal() && taskList[i]->getData()->size() <= maxLeafSize)||levelNum == pivots.size())
			{
				taskList[i]->setIsLeafNode(1);
				taskList[i]->setIsDeal(1);
			}
		}
		vector<shared_ptr<ClusteringTask> >().swap(task_clustering);
	}while((tasklist_indictor < taskList.size()-1) || ((tasklist_indictor == taskList.size()-1) && task != 0 && !task->getIsLeafNode()));
	vector<shared_ptr<CMetricData> >data_temp;
	int count = 0;
	vector<int> fatherNode;
	int fatherNum = -1;
	int computerNum = 0;
	for(int i=0;i<taskList.size();i++)
	{
		fatherNode.clear();
		if (taskList[i]->getIsLeafNode())
			task = taskList[i];
		else
			continue;
		for (int j=0; j < (task->getData())->size();++j)
		{
			data_temp.push_back(task->getData()->at(j));
		}
		offset.push_back(data_temp.size()+first);
		fatherNum = task->getFatherNum();
		fatherNode.push_back(task->getNumber());
		while(fatherNum != 0)
		{
			fatherNode.push_back(fatherNum);
			for (int k=0; k< taskList.size(); ++k)
				if (taskList[k]->getNumber() == fatherNum)
				{
					task = taskList[k];
					break;
				}
				fatherNum = task->getFatherNum();
		}
		while(fatherNode.size() < newPivots.size())
		{
			int fatherNum_temp = fatherNode[0];
			fatherNode.insert(fatherNode.begin(),fatherNum_temp);
		}
		for (int t=fatherNode.size()-1,x=0; t >=0;t--,x++)
		{
			/*if(t!=fatherNode.size()-1 && fatherNode[t] == fatherNode[t+1])
			{*/
			taskList[fatherNode[t]-1]->getMetricData_Distance()->clear();
			for(int q =0; q< taskList[fatherNode[t]-1]->getData()->size();++q)
				taskList[fatherNode[t]-1]->getMetricData_Distance()->push_back(metric->getDistance(newPivots[newPivots.size()-t-1].get(),taskList[fatherNode[t]-1]->getData()->at(q).get()));
			//}
			lower[x].push_back(utilFunction::find_min(*taskList[fatherNode[t]-1]->getMetricData_Distance()));
			upper[x].push_back(utilFunction::find_max(*taskList[fatherNode[t]-1]->getMetricData_Distance()));
		}
	}
	for(int i=0; i < data_temp.size(); ++i)
	{
		data[i + first] = data_temp[i];
	}
	shared_ptr<CPartitionResults>result(new CPartitionResults(offset,lower,upper));
	vector<shared_ptr<CMetricData> >().swap(data_current);
	vector<shared_ptr<ClusteringTask> >().swap(taskList);
	vector<shared_ptr<CMetricData> >().swap(newPivots);
	vector<int>().swap(pivotUse);
	vector<int>().swap(clusterId);
	vector<shared_ptr<ClusteringTask> >().swap(taskClustering_temp);
	return *result;
}