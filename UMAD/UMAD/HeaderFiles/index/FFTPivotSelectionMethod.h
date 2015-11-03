#ifndef FFTPIVOTSELECTIONMETHODS_H

#define FFTPIVOTSELECTIONMETHODS_H



#include "PivotSelectionMethod.h"
#include <memory>


/**@file PivotSelectionMethod.h

 * @brief  select pivots with FFT

 * @author Yuxuan Qiu

 * @version 2013-4-18

 */



/**@class CFFTPivotSelectionMethod

  *@brief select pivots with FFT



  * select some pivots from the the given data list, the number of pivots should not greater than "numPivots" which is the last parameter of the constructor, and return the selection result through the parameter "pivot"

  */

class CFFTPivotSelectionMethod:public CPivotSelectionMethod

{

public:

	/**none parameter constructor*/

	CFFTPivotSelectionMethod();

	/**destructor*/

	~CFFTPivotSelectionMethod();

	

	 /*

		@param metric this is a function used to calculate the distance of two data objects

		@param data this is a data set, from which vantage points are selected

		@param numPivots this is the maxmum number of pivots selected from the data set



		@return return a container consists of the indexes of the selected pivots in the given data set 

	*/

	vector<int> selectPivots(CMetricDistance *metric,vector<shared_ptr<CMetricData> > &data,int numPivots);

	 

    /*

		@param metric this is a function used to calculate the distance of two data objects

		@param data this is a data set, from which vantage points are selected

		@param first the first object address of a small object section, from which this function will find all the vantage points address

		@param dataSize the size of a small section, from which this function will find all the vantage points address

		@param numPivots this is the maxmum number of pivots selected from the data set 



		@return return a container consists of the indexes of the selected pivots in the given data set 

	*/

	vector<int> selectPivots(CMetricDistance *metric,vector<shared_ptr<CMetricData> > &data,int first,int dataSize,int numPivots);



	/*

		@param metric this is a function used to calculate the distance of two data objects

		@param data this is a data set, from which vantage points are selected

		@param first the first object address of a small object section, from which this function will find all the vantage points address

		@param dataSize the size of a small section, from which this function will find all the vantage points address

		@param numPivots this is the maxmum number of pivots selected from the data set

		@param firstPivot 



		@return return a container consists of the indexes of the selected pivots in the given data set 

	*/

	vector<int> selectPivots(CMetricDistance *metric,vector<shared_ptr<CMetricData> > &data,int first,int dataSize,int numPivots,int firstPivot);





};



#endif