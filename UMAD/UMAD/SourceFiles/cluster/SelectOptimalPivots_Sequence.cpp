#include "../../HeaderFiles/cluster/SelectOptimalPivots_Sequence.h"

/** @file	SelectOptimalPivots_Sequence.h
* @classes	Inherited SelectOptimalPivots
* @author	He Zhang
* @version	2014-07-23
*/

/**
* @class	SelectOptimalPivots_Sequence
* @brief	Select the pivot in sequence
* @author	He Zhang
*/

/**@brief	Select the pivot in sequence
*@param	metric	The data types of distance calculation function
*@param	pivots	Through choosing the pivot method has been chosen to the pivot
*@param	data	dataset of the current task
*@param	numpartition	the number of the data should be divided
*@param	maxLeafSize		the number of the leaf node
*/
vector<shared_ptr<CMetricData> > SelectOptimalPivots_Sequence::selectOptimalPivots(CMetricDistance* metric,const vector<shared_ptr<CMetricData> > &pivots,vector<shared_ptr<CMetricData> > &data,int numPartition,int maxLeafSize)
{
	vector<shared_ptr<CMetricData> > newPivots =pivots;
	return newPivots;
}