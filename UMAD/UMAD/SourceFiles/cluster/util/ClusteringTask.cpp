#include "../../../HeaderFiles/cluster/util/ClusteringTask.h"

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


/**@brief	None parameter constructor */
ClusteringTask::ClusteringTask()
{

}

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
ClusteringTask::ClusteringTask(vector<shared_ptr<CMetricData> >&data,vector<int>&clusterId,int isdeal,vector<shared_ptr<CMetricData> >&newPivots,vector<int>&pivotUse,int number,int fatherNum,int isLeafNode)
{
	this->data = data;
	this->clusterId = clusterId;
	this->isdeal = isdeal;
	this->newPivots = newPivots;
	this->pivotUse = pivotUse;
	this->number = number;
	this->fatherNum = fatherNum;
	this->isLeafNode = isLeafNode;
}
/**@brief	To get the task status */
int ClusteringTask::getIsDeal()
{
	return isdeal;
}

/**@brief	get dataset from the task
*@return	Type of array pointer to cMetricData
*/
vector<shared_ptr<CMetricData> >* ClusteringTask::getData()
{
	return &data;
}

/**@brief	get new pivots from the task
*@return	Type of array pointer to cMetricData
*/
vector<shared_ptr<CMetricData> >* ClusteringTask::getNewPivots()
{
	return &newPivots;
}

/**@brief	set new pivots
*@param newPivots	Type of array pointer to cMetricData
*/
void ClusteringTask::setNewPivots(vector<shared_ptr<CMetricData> > newPivots)
{
	this->newPivots = newPivots;
}

/**@brief	get the status of the pivot use
*@return	Pointer to an array of type int
*/
vector<int>* ClusteringTask::getClusterId()
{
	return &clusterId;
}

/**@brief	get the status of the pivot use
*@return	Pointer to an array of type int
*/
vector<int>* ClusteringTask::getPivotUse()
{
	return &pivotUse;
}

/**@brief	set the status of the pivot use
*@param indictor	Indicate which one pivot will be used
*/
void ClusteringTask::setPivotUse(int indictor)
{
	for (int i=0;i <= indictor; i++)
	{
		this->pivotUse[i] = 1;
	}
}

/**@brief	get Get distance of data points of the task to a pivot 
*@return	Pointer to an array of type double
*/
vector<double>* ClusteringTask::getMetricData_Distance()
{
	return &metricData_Distance;
}

/**@brief	set the status of the task
*@param	deal	the status of the task
*/
void ClusteringTask::setIsDeal(int deal)
{
	this->isdeal = deal;
}

/**@brief	set whether the task is leaf node or not
*@param	leafNode	vaule include 0 or 1
*/
void ClusteringTask::setIsLeafNode(int leafNode)
{
	this->isLeafNode = leafNode;
}

/**@brief	To get whether the task is leaf node or not */
int ClusteringTask::getIsLeafNode()
{
	return isLeafNode;
}

/**@brief	Get the current task number */
int ClusteringTask::getNumber()
{
	return number;
}

/**@brief	Get the parent task number */
int ClusteringTask::getFatherNum()
{
	return fatherNum;
}

/**@brief	set the number of the task
*@param	number	the number of the task
*/
void ClusteringTask::setNumber(int number)
{
	this->number = number;
}

/**@brief	set the parent thsk number
*@param	fatherNum	the parent thsk number
*/
void ClusteringTask::setFatherNum(int fatherNum)
{
	this->fatherNum = fatherNum;
}