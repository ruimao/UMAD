#ifndef CLUSTERING_KMEANS_H
#define CLUSTERING_KMEANS_H

#include "ClusteringTask.h"
#include "../../index/PivotSelectionMethod.h"
#include "../../index/FFTPivotSelectionMethod.h"
#include "../../metricdistance/EuclideanDistance.h"

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

class Clustering_Kmeans
{
public:
	/**@brief None parameter constructor */
	Clustering_Kmeans();
	/**@brief destructor*/
	~Clustering_Kmeans();
	/**@brief	this is the function to clustering dataset 
	*@param	task	The data points belonging to the same class in the same task,partition this task into some subtask.
	*@param	numpartition	singlePivotFanout 
	*@param	indictor	Indicates the current use of the pivot 
	*/
	static void K_Means_Algorithm(shared_ptr<ClusteringTask> &task,int numpartition,int indictor);

};



































#endif