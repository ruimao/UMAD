/** @file RangeQuery.cpp

 * @brief class about a kind of querys.

 * @author Fuli Lei

 * @version 2012-12-09

 *

 * range query object include a center object and a search radius, the purpose of this query is to get all data objects whose distance to the center object is not more than the search radius.

**/

#include "../../../HeaderFiles/index/query/RangeQuery.h"



/**@brief constructor

*@param radius the maximum distance between each target data object to the center object

*@param q the center object,the distance of the data object in the result list to which is not more than the radius 

*/

CRangeQuery::CRangeQuery(double radius, shared_ptr<CMetricData> q):CQuery(q)

{

	r = radius;

}



/**@brief constructor

*@param q the center object,the distance of the data object in the result list to which is not more than the radius 

*/

CRangeQuery::CRangeQuery(shared_ptr<CMetricData> _q):CQuery(_q)

{

}



/**@brief destructor*/

CRangeQuery::~CRangeQuery(void)

{

}



/**@brief set the search radius of a range query object

*@param radius the maximum distance between each target data object to the center object

*/

void CRangeQuery::setRadius(double radius)

{

	r = radius;

}



/**@brief get the search radius of a range query object

*@return a double value represents the maximum distance between each target data object to the center object

*/

double CRangeQuery::getRadius() const

{

	return r;

}