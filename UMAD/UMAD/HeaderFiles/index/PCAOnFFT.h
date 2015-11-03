/** @file

* @brief Define the CPCAOnFFT class

* @author Abel, Xavier

* @date 2013/6/8

* @version 1.0

*/



#ifndef SQAI_INDEXALGORITHM_PCAONFFT_H

#define SQAI_INDEXALGORITHM_PCAONFFT_H

#include <vector>

#include "../../HeaderFiles/index/PivotSelectionMethod.h"
#include "../../HeaderFiles/metricdata/DoubleVector.h"

using namespace std;



class CPCAOnFFT :public CPivotSelectionMethod

{

public:

    CPCAOnFFT(int _FFTScale);

    ~CPCAOnFFT();



    /**@brief select the pivot according to the given parameters

		@param metric: this is a function used to calculate the distance of two data objects

		@param data: this is a data set, from which vantage points are selected

		@param first: the first object address of a small object section, from which this function will find all the vantage points address

		@param dataSize: the size of a small section, from which this function will find all the vantage points address

		@param numPivots: this is the maxmum number of pivots selected from the data set



		@return return a container consists of the indexes of the selected pivots in the given data set 

	*/

	vector<int> selectPivots(CMetricDistance *metric, vector<shared_ptr<CMetricData> > &data, 

                                int first, int size, int numPivots);

	

	/**@brief select the pivot according to the given parameters

		@param metric: this is a function used to calculate the distance of two data objects

		@param data: this is a data set, from which vantage points are selected

		@param numPivots: this is the maxmum number of pivots selected from the data set



		@return return a container consists of the indexes of the selected pivots in the given data set	*/

	vector<int> selectPivots(CMetricDistance *metric, vector<shared_ptr<CMetricData> > &data, 

                                                            int numPivots);



private:

    const int FFTScale;

};





#endif