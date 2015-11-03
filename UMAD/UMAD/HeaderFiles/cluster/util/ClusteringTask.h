#ifndef CLUSTERINGTASK_H
#define CLUSTERINGTASK_H

#include "../../metricdata/MetricData.h"
#include "../../metricdistance/MetricDistance.h"

/** @file	ClusteringTash.h
* @classes	Each task contains attributes and related operations
* @author	He Zhang
* @version	2014-07-21
*/

/**
* @class	ClusteringTask
* @brief	Each task contains attributes and related operations
* @author	He Zhang
*/

class ClusteringTask
{
public:
	/**@brief	None parameter constructor */
	ClusteringTask();
	/**@brief	some parameter constructor
	*@param	data	cmetricData set
	*@param	clusterId	those data should belong to which clusering
	*@param	isdeal	Whether the task is processed
	*@param	newPivots	each task to obtain the optimization of the pivot
	*@param	pivotUse	Whether the pivot is used
	*@param	number	the number of the task
	*@param	fatherNum	the parent task number
	*@param	isLeafNode	Whether the task is leaf node
	*/
	ClusteringTask(vector<shared_ptr<CMetricData> >&data,vector<int>&clusterId,int isdeal,vector<shared_ptr<CMetricData> >&newPivots,vector<int>&pivotUse,int number,int fatherNum,int isLeafNode);
	/**@brief	To get the task status */
	int getIsDeal();
	/**@brief	To get whether the task is leaf node or not */
	int getIsLeafNode();
	/**@brief	Get the parent task number */
	int getFatherNum();
	/**@brief	Get the current task number */
	int getNumber();
	/**@brief	set the status of the task
	*@param	deal	the status of the task
	*/
	void setIsDeal(int deal);
	/**@brief	set whether the task is leaf node or not
	*@param	leafNode	vaule include 0 or 1
	*/
	void setIsLeafNode(int leafNode);
	/**@brief	set the number of the task
	*@param	number	the number of the task
	*/
	void setNumber(int number);
	/**@brief	set the parent thsk number
	*@param	fatherNum	the parent thsk number
	*/
	void setFatherNum(int fatherNum);
	/**@brief	get dataset from the task
	*@return	Type of array pointer to cMetricData
	*/
	vector<shared_ptr<CMetricData> >*getData();
	/**@brief	get new pivots from the task
	*@return	Type of array pointer to cMetricData
	*/
	vector<shared_ptr<CMetricData> >*getNewPivots();
	/**@brief	set new pivots
	*@param newPivots	Type of array pointer to cMetricData
	*/
	void setNewPivots(vector<shared_ptr<CMetricData> >newPivots);
	/**@brief	get the status of the pivot use
	*@return	Pointer to an array of type int
	*/
	vector<int>* getPivotUse();
	/**@brief	set the status of the pivot use
	*@param indictor	Indicate which one pivot will be used
	*/
	void setPivotUse(int indictor);
	/**@brief	get the status of the pivot use
	*@return	Pointer to an array of type int
	*/
	vector<int>* getClusterId();
	/**@brief	get Get distance of data points of the task to a pivot 
	*@return	Pointer to an array of type double
	*/
	vector<double>* getMetricData_Distance();
private:
	/**	Store cMetricData data pointer array */
	vector<shared_ptr<CMetricData> >data;
	/**	give data point of this task a clustering label */
	vector<int>clusterId;
	/**	indictor whether this task is used or not */
	int isdeal;
	/**	the number of the task */
	int number;
	/**	the parent node number of the current task */
	int fatherNum;
	/**	indictor whether this task is leaf node or not,value include 0 and 1  */
	int isLeafNode;
	/**	each task include new pivot data,those pivots is selected by selectOptimalPivots function in selectOptiomalPivot_Sequence or selectOptimalPivot_Variance  */
	vector<shared_ptr<CMetricData> >newPivots;
	/**	indictor the use status of the newPivots in the current task  */
	vector<int>pivotUse;
	/**	Store distance from data point to pivot , this variable associated with data and newPivots*/
	vector<double> metricData_Distance;

};






























#endif