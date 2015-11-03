#include "../../HeaderFiles/index/ListIndex.h"

/** @file ListIndex.cpp

 * @classes about index

 * @author Fuli Lei

 * @version 2012-12-09

*/





/**constructor*/

CListIndex::CListIndex()

{

	this->metric=0;

}



/**constructor with a parameter

 * @param metric used for calculating the distance of two objects

*/

CListIndex::CListIndex(CMetricDistance *metric)

{

	this->metric=metric;

}

/**destructor*/

CListIndex::~CListIndex()

{

}



/**A index struct can be built through this function

 * @param objectList this is a vector contains all the objects of the index structure built later

*/

void CListIndex::bulkLoad(vector<std::shared_ptr<CMetricData> > &_objectList,int buildMode,int threadNum)
{

    objectList = _objectList;

}



/**get all the objects resides in the index structure built before

 * @return return a vector contains all the object address

*/

vector<shared_ptr<CMetricData> > CListIndex::getAllDataPoints()

{

    return objectList;

}



/**get the number of objects in the index structure

 * @return int the size of objectList

*/

int CListIndex::size()

{

	return objectList.size();

}





/**get the object which is used to calculate the distance of two objects when constructing or search in the index structure

   @return return the address of the distance metric object

*/

CMetricDistance* CListIndex::getMetric()

{

    return metric;

}



/**Find out the proper object from the index structure built before

	   @param q this object is given by users for the search operation, distance from every object in the results to q.centerObject is not greater than r(the search radius)

	   @return return a vector contains all the objects that comform to the requirment that distance of each object to q.centerObject is not greater than r

*/

list<shared_ptr<CMetricData> >* CListIndex::search(CQuery* q)
{

	CRangeQuery *rq=(CRangeQuery*)q;

    list<shared_ptr<CMetricData> > rs;

	double objectDistance = 0;

	double radius = 0;

	for (vector<CMetricData*>::size_type i=0;i!=objectList.size();i++)

    {

        objectDistance = metric->getDistance(objectList.at(i).get(),rq->getQueryObject().get());



        radius = rq->getRadius();

		if(objectDistance<=radius)

			rs.push_back(objectList.at(i));

    }

    return &rs;

}

