#include "../../../HeaderFiles/cluster/util/Clustering_Kmeans.h"
#include "../../../HeaderFiles/cluster/util/utilFunction.h"
#include <cmath>
#define INT_MAX ((int)(~0U>>1))
#define INT_MIN (-INT_MAX - 1)
/** @file	Clustering_Kmeans.h
* @classes	to data partition by k-means algorithm
* @author	He Zhang
* @version	2014-07-21
*/

/**
* @class	Clustering_Kmeans
* @brief	to data partition by k-means algorithm
* @author	He Zhang
*
* In the function of selcectOptimalPivot and paritionbyonePivot,they haved adopted K_Means_Algorithm.
*/

/** Store the clustering center of mass*/
vector<double>means_Pivots;

/**@brief	Find out the minimum values from an array and returns it
*@param	task_distance	distance array
*@return	the label of the minimum values 
*/
int find_min_label(vector<double>& task_distance)
{
	double distance_min = INT_MAX;
	int label;
	for (int i=0; i <task_distance.size(); ++i )
	{
		if (distance_min > task_distance[i])
		{
			distance_min = task_distance[i];
			label = i;
		}
	}
	return label;
}

/**@brief	Find out the minimum values from an array and returns it
*@param	task_distance	distance array
*@return	the minimum values in task_distance
*/
double find_min_distance(vector<double>& task_distance)
{
	double distance_min = INT_MAX;
	for (int i=0; i <task_distance.size(); ++i )
	{
		if (distance_min > task_distance[i])
			distance_min = task_distance[i];
	}
	return distance_min;
}

/**@brief	Find out the maximum values from an array and returns it
*@param	task_distance	distance array
*@return	the label of the maximum values 
*/
int find_max_label(vector<double>& task_distance)
{
	double distance_max = INT_MIN;
	int label;
	for (int i=0; i <task_distance.size(); ++i )
	{
		if (distance_max < task_distance[i])
		{
			distance_max = task_distance[i];
			label = i;
		}
	}
	return label;
}

/**@brief	data clustering using metricData_Distance in the task
*@param	task	A task for data partitioning
*@param	numpartition	The data is divided into several parts
*@param	task_distance	Indicate which one pivot is used,numpartition and indictor determine the class labels
*/
void clusterOfData(shared_ptr<ClusteringTask> &task,int numpartition,int indictor)
{  
	double distance_min = INT_MAX;
	int label = 0;
	for (int i=0; i < task->getMetricData_Distance()->size(); ++i)
	{
		for (int j=0; j < means_Pivots.size(); ++j)
		{
			double distanceValue = task->getMetricData_Distance()->at(i) - means_Pivots[j];
			if (distance_min > abs(distanceValue))
			{
				distance_min = abs(distanceValue);
				label = j;
			}
		}
		distance_min = INT_MAX;
		task->getClusterId()->at(i) = label + indictor* numpartition +1;
	}
}  

/**@brief	Get the square error of a given cluster
*@param	clusterData		Deposit has been completed clustering of data
*@return	return the sum of the square error
*/
double getSSE(vector<vector<double> > &clusterData)
{  
	double SSE = 0;  
	for (int i = 0; i < clusterData.size(); i++)  
	{  
		vector<double> temp = clusterData[i];  
		for (int j = 0; j< temp.size(); j++)  
		{  
			SSE += abs(temp[j]-means_Pivots[i]);  
		}  
	}  
	return SSE;

}  

/**@brief	Get the current cluster average (center)
*@param	clusterData		Deposit has been completed clustering of data
*@return	return the means of this clusterData
*/
double getMeans(vector<double>& clusterData){  

	double sum = 0;
	for (int i = 0; i < clusterData.size(); i++)  
	{  
		sum += clusterData[i];
	}  
	sum /= clusterData.size();
	return sum;  
}  

/**@brief	Get the current cluster means value (center)
*@param	task	A task for data partitioning
*@param	numpartition	The data is divided into several parts
*/
void selectCenterPivots(shared_ptr<ClusteringTask> &task,int numpartition)
{
	//k-median Choose k quantile, this method can also be used
	//K-select method
	int taskSize = task->getMetricData_Distance()->size();
	shared_ptr<vector<double> > metricData_distance_temp(new vector<double>());
	for(int i=0; i < task->getMetricData_Distance()->size(); ++i)
	{
		metricData_distance_temp->push_back(task->getMetricData_Distance()->at(i));
	}
	for(int i=1;i < 2*numpartition; i += 2)
	{
		means_Pivots.push_back(utilFunction::select_min_k_iteration(metricData_distance_temp,taskSize,i*taskSize/(2*numpartition)+1));
	}
	//FFT thought
	//int loopIndictor = 1;
	//means_Pivots.push_back(task->getMetricData_Distance()->at(0));
	//vector<vector<double> >distanceMatrix(task->getData()->size());
	//vector<double> distance_min(task->getData()->size());
	//while(loopIndictor < numpartition)
	//{

	//	for(int i=0; i < task->getData()->size(); ++i)
	//	{
	//		distanceMatrix[i].clear();
	//	}
	//	for(int i=0; i < task->getData()->size(); ++i)
	//	{
	//		for(int j=0;j < means_Pivots.size(); ++j)
	//		{
	//			distanceMatrix[i].push_back(abs(task->getMetricData_Distance()->at(i) - means_Pivots[j]));
	//		}
	//	}
	//	for(int i=0; i< task->getData()->size();++i)
	//	{
	//		distance_min[i] = find_min_distance(distanceMatrix[i]);
	//	}
	//	means_Pivots.push_back(task->getMetricData_Distance()->at(find_max_label(distance_min)));
	//	loopIndictor++;
	//}

	//Order to choose
	//for(int i=0; i< numpartition; i++)
	//{
	//	means_Pivots.push_back(task->getMetricData_Distance()->at(i));
	//}
}

/**@brief	For clusterData assignment
*@param	task	A task for data partitioning
*@param	numpartition	The data is divided into several parts
*@param	indictor	Indicate which one pivot is used
*@param	clusterData	Deposit has been completed clustering of data
*/
void setClusterData(shared_ptr<ClusteringTask> &task,int numpartition,int indictor,vector<vector<double> > &clusterData)
{
	for (int i=0; i < task->getMetricData_Distance()->size(); ++i)
	{
		for(int j = indictor*numpartition+1; j <= (indictor+1)*numpartition; ++j)
		{
			if (task->getClusterId()->at(i) == j)
				clusterData[j-indictor*numpartition-1].push_back(task->getMetricData_Distance()->at(i));
		}
	}
}

/**@brief	K-means clustring algorithm
*@param	task	A task for data partitioning
*@param	numpartition	The data is divided into several parts
*@param	indictor	Indicate which one pivot is used
*/
void Clustering_Kmeans::K_Means_Algorithm(shared_ptr<ClusteringTask> &task,int numpartition,int indictor)
{
	means_Pivots.clear();
	selectCenterPivots(task,numpartition);
	clusterOfData(task,numpartition,indictor);
	vector<vector<double> > clusterData(numpartition);
	setClusterData(task,numpartition,indictor,clusterData);
	double oldSSE=-1;  
	double newSSE=getSSE(clusterData);
	int count = 0;
	/** When the old and new function value less than 1 the criterion function values are not obvious changes, termination algorithm*/
	while(count < 2 || abs(newSSE - oldSSE)/oldSSE > 0.01) 
	{  
		/** Update each cluster center*/
		for (int i = 0; i < numpartition; i++) 
		{  
			means_Pivots[i] = getMeans(clusterData[i]);  
		} 
		/** Empty each cluster*/
		for (int i = 0; i < numpartition; i++) 
		{  
			clusterData[i].clear(); 
		}  
		/** According to the new center of mass for the new cluster*/
		clusterOfData(task,numpartition,indictor); 
		setClusterData(task,numpartition,indictor,clusterData);
		oldSSE = newSSE;  
		newSSE = getSSE(clusterData);
		count ++;
	}
	//for (int i=0; i < clusterData.size(); i++)
	//{
	//	char fileName[100] = "Clustering";
	//	char str[10];
	//	int clusterId = i +1;
	//	_itoa_s(clusterId,str,10);
	//	strcat_s(fileName,str);
	//	ofstream outFile (fileName,ofstream::app);
	//	
	//	for (int j=0; j<clusterData[i].size(); j++)
	//	{
	//		outFile << clusterData[i][j]<<endl;
	//	}
	//}

}