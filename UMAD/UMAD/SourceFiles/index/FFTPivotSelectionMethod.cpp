/**@file FFTPivotSelectionMethod.cpp

 * @brief  select pivots with FFT

 * @author Yuxuan Qiu

 * @date 2013-4-18

*/



#include "../../HeaderFiles/index/FFTPivotSelectionMethod.h"



#include <cfloat>





/**

 * @brief check the array of pivots, remove the duplicate.

 * @param metric

 * @param data

 * @param pivots

 * @return 

 */

vector<int> removeDuplicate(CMetricDistance *metric,vector<shared_ptr<CMetricData> > data,vector<int> &pivots,int first,int dataSize);



CFFTPivotSelectionMethod::CFFTPivotSelectionMethod()

{



}



CFFTPivotSelectionMethod::~CFFTPivotSelectionMethod()

{



}



vector<int> CFFTPivotSelectionMethod::selectPivots(CMetricDistance *metric, vector<shared_ptr<CMetricData> > &data,int numPivots)

{

	 return selectPivots(metric, data,0,data.size(),numPivots);

}



vector<int> CFFTPivotSelectionMethod::selectPivots(CMetricDistance *metric, vector<shared_ptr<CMetricData> >& data, int first, int dataSize, int numPivots)

{

	        int firstPivot = first; // Math.floor(first + Math.random() *

                                    // dataSize);

            return selectPivots(metric,data,first, dataSize, numPivots,firstPivot);

}



vector<int> CFFTPivotSelectionMethod::selectPivots(CMetricDistance *metric, vector<shared_ptr<CMetricData> > &data,int first, int dataSize, int numPivots, int firstPivot)

{

	if (numPivots >= dataSize) //if the number of pivots required is bigger than dataSize then return all the points in the data(duplicate removed).

	{

		vector<int>* pivots=new vector<int>;

		for (int i = first; i < first+dataSize + 0; i++)

			pivots->push_back(i);

		

		return removeDuplicate(metric, data, *pivots,first,dataSize);

	}



	bool* isCenter = new bool[dataSize];

	double* minDist = new double[dataSize];

	for (int i = 0; i < dataSize; i++)

	{

		isCenter[i] = false;

		minDist[i] = DBL_MAX;

	}



	 isCenter[firstPivot-first] = true;



	 int* indices = new int[numPivots]; // indices is used to record the offsets

                                       // of the pivots in the original data list

	 indices[0] = firstPivot;



	 for (int i = 1; i < numPivots; i++)

                indices[i] = -1;

	 // transparently firstPivot is found already 





	 for (int centerSize = 1; centerSize < numPivots; centerSize++)

	 {

		        double currMaxDist = 0;

                shared_ptr<CMetricData> const lastCenter = data[indices[centerSize - 1]];

				

                for (int i = 0; i < dataSize; i++)

                {

                    if (isCenter[i] == false) // if the point is not a center, we should calculate the distance

											// between this point and the set of Centers, for each centerSize we

											// grasp one Center form the set of Centers.

                    {

                        double tempDist = metric->getDistance(data[i + first].get(), lastCenter.get());



                        minDist[i] = (tempDist < minDist[i]) ? tempDist : minDist[i];



                        // TODO

                        if (minDist[i] > currMaxDist)

                        {

                            indices[centerSize] = i+first; // save the index the

                                                     // current farthest

                                                     // point

                            currMaxDist = minDist[i];

                        }



                    }

                }



                if (indices[centerSize] == -1)

                    break;

                else

                    isCenter[indices[centerSize]-first] = true;

	 }



	int returnSize = 0;

	while ((returnSize < numPivots) && (indices[returnSize] >= 0))

		returnSize++;



	// to decide the size of the result vector.

	if (returnSize > numPivots)

		returnSize = numPivots;

	vector<int> result;

	for(int i=0; i<returnSize; i++)

		result.push_back(indices[i]);



	delete [] isCenter;

	delete [] minDist;

	delete [] indices;





	return result;





}



/**

*to check the array of pivots, remove the duplicate.

*/

vector<int> removeDuplicate(CMetricDistance *metric,vector<shared_ptr<CMetricData> > data,vector<int> &pivots,int first,int dataSize)

{

	const int size = dataSize;

    bool* isDuplicate = new bool[size];

    for (int i=0; i<size; i++)

        isDuplicate[i] = false;

    for (int i=0; i<size-1; i++)

    {

        if (isDuplicate[i])//if the point has been removed, don't need to check the duplication.

            continue;

        for (int j=i+1; j<size; j++)

        {

            if (isDuplicate[j])

                continue;

            if (metric->getDistance(data[i+first].get(), data[j+first].get()) == 0) //to check if two points are the same point.

                isDuplicate[j] = true;

        }

    }

        

    int counter = 0;

    for (int i=0; i< size; i++) //to get the number of points which are not duplicate.

	{

        if (!isDuplicate[i]) 

            counter ++;

	}

        

    if (counter == size)

	{

		delete isDuplicate;

		return pivots;

	}

    else

    {

        vector<int> temp;

        counter = 0;

        for (int i=0; i<size; i++)

            if (!isDuplicate[i])

				temp.push_back(pivots[i]);

		delete isDuplicate;

        return temp;

    }

}

