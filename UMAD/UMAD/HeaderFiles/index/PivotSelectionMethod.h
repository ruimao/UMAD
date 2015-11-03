#ifndef PIVOTSELECTIONMETHOD_H

#define PIVOTSELECTIONMETHOD_H



#include "../metricdistance/MetricDistance.h"

#include <vector>



#include <memory>


/** @file PivotSelectionMethod.h

 * @brief classes about index algrithm used in the process of building a index structure

 * @author Fuli Lei,Yuxuan Qiu

 * @version 2012-12-09

*/



/**

* @class CPivotSelectionMethod

* @brief abstract class about pivot selection methods

* @author Yuxuan Qiu,Fuli Lei

*

* select some data objects from the given data set as the pivots, which will be used in the process of search to exclude some part of the data objects so that we can

* what we want with fewer distance calculations

*/



class CPivotSelectionMethod

{

public:

	/**none parameter constructor*/

	CPivotSelectionMethod(){}

	virtual ~CPivotSelectionMethod(){}

	/**@brief select the pivot according to the given parameters

		@param metric this is a function used to calculate the distance of two data objects

		@param data this is a data set, from which vantage points are selected

		@param first the first object address of a small object section, from which this function will find all the vantage points address

		@param dataSize the size of a small section, from which this function will find all the vantage points address

		@param numPivots this is the maxmum number of pivots selected from the data set



		@return return a container consists of the indexes of the selected pivots in the given data set 

	*/

	virtual vector<int> selectPivots(CMetricDistance *metric,vector<shared_ptr<CMetricData> > &data,int first,int size,int numPivots)=0;

	

	/**@brief select the pivot according to the given parameters

		@param metric this is a function used to calculate the distance of two data objects

		@param data this is a data set, from which vantage points are selected

		@param numPivots this is the maxmum number of pivots selected from the data set



		@return return a container consists of the indexes of the selected pivots in the given data set	*/

	virtual vector<int> selectPivots(CMetricDistance *metric, vector<shared_ptr<CMetricData> > &data,int numPivots) = 0;



};



#endif