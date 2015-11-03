#ifndef ICREMENTALSELECTION_H

#define ICREMENTALSELECTION_H



#include "PivotSelectionMethod.h"

#include <cstdlib>

#include <ctime>

#include <limits>

#include <cmath>




#include <memory>
using namespace std;



/**@file PivotSelectionMethod.h

 * @brief select pivots with IncrementalSelection

 * @author WeiPeng Zhang

 * @data 2013-5-30

 * @£û

 */



/**@class CIncrementalSelection

 * @brief select pivots with IncrementalSelection

 * select some pivots from the a given data list, the number of pivots should not greater than the parameter "numPivots", and return the selection result through the parameter "pivots"

 */

class CIncrementalSelection :

	public CPivotSelectionMethod

{

public:

	/**A constructor that have two parameters , the first one is A , and the second is N*/

	CIncrementalSelection(int, int);



	/**destructor*/

	~CIncrementalSelection();



    /* @param    metric: this is a function used to calculate the distance of two data objects

     * @param      data: this is a data set, which is a global array, is used to stored the datas, from which vantage points are selected

     * @param     first: the start position of the data , the default value is 0

     * @param  dataSize: size of the data

     * @param numPivots: the numbers of the pivots that will be selected



     * @return: return a container consists of the indexes of the selected pivots in the given data set 

     */

	vector<int> selectPivots(CMetricDistance *metric, vector<shared_ptr<CMetricData> > &data, int numPivots);

	vector<int> selectPivots(CMetricDistance *metric, vector<shared_ptr<CMetricData> > &data, int first, int size, int numPivots);



	 /**

      * This function is used to remove the repeated data in pivots, usually it's used when a predetermined number of the pivots is greater than the number of remaining datas.

      * @param metric: this is a function used to calculate the distance of two data objects

      * @param data  : this is a data set, which is a global array, is used to stored the datas, from which vantage points are selected

      * @param pivots: pivot's index that are be selected in data set;

      * @return a new array which used to store the pivotIndex( include unreplicated pivots which are the index of the datas )

      */

	static vector<int> removeDuplicate(CMetricDistance *metric, vector<shared_ptr<CMetricData> > &data, vector<int> pivots);





private:

	int constantA;

	int constantN;



};



#endif