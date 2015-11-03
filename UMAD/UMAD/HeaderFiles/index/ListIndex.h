#ifndef LISTINDEX_H

#define LISTINDEX_H

#include "Index.h"

#include "query/RangeQuery.h"



#include <string>




#include <memory>






/** @file ListIndex.h

 * @classes about index

 * @author Fuli Lei

 * @version 2012-12-09

*/



/**

* @class CListIndex

* @brief class about list index whose data structure likes a liner array

* @author Fuli Lei

*

* A liner index structure can be built through this class and then write structure into the file stored in the external memory

*/



class CListIndex :

    public CIndex

{

public:

	/**constructor*/

	CListIndex();



	/**constructor with a parameter

	   @param metric used for calculating the distance of two objects

	*/

	CListIndex(CMetricDistance *metric);



	/**destructor*/

    ~CListIndex();



	/**A index struct can be built through this function

	   @param objectList this is a vector contains all the objects of the index structure built later

	*/
	void bulkLoad(vector<std::shared_ptr<CMetricData> > &objectList,int buildMode,int threadNum);



	/**Find out the proper object from the index structure built before

	   @param q this object is given by users for the search operation, distance from every object in the results to q.centerObject is not greater than r(the search radius)

	   @return return a vector contains all the objects that comform to the requirment that distance of each object to q.centerObject is not greater than r

	*/

    list<shared_ptr<CMetricData> >* search(CQuery *q);



	/**get all the objects resides in the index structure built before

	   @return return a vector contains all the object address

	*/

    virtual vector<shared_ptr<CMetricData> > getAllDataPoints();



	/**get the object which is used to calculate the distance of two objects when constructing or search in the index structure

	   @return return the address of the distance metric object

	*/

    virtual CMetricDistance* getMetric();



	/**get the number of objects in the index structure

	   @return int the size of objectList

	*/

	virtual int size();





private:



	/**this vector represents the linder struct*/

    vector<shared_ptr<CMetricData> > objectList;

	/**An distance object used to calculat the distance of two objects*/

    CMetricDistance *metric;



};



#endif

