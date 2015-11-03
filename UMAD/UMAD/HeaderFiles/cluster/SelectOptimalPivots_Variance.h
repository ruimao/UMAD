#ifndef SELECTOPTIMALPIVOTS_VARIANCE_H
#define SELECTOPTIMALPIVOTS_VARIANCE_H

#include "SelectOptimalPivots.h"

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

class SelectOptimalPivots_Variance : public SelectOptimalPivots
{
public:
	/**@brief	Select the pivot in variance
	*@param	metric	The data types of distance calculation function
	*@param	pivots	Through choosing the pivot method has been chosen to the pivot
	*@param	data	dataset of the current task
	*@param	numpartition	the number of the data should be divided
	*@param	maxLeafSize		the number of the leaf node
	*/
	virtual vector<shared_ptr<CMetricData> > selectOptimalPivots(CMetricDistance *metric, const vector<shared_ptr<CMetricData> > &pivots,vector<shared_ptr<CMetricData> > &data,int numpartition,int maxLeafSize);

};

#endif