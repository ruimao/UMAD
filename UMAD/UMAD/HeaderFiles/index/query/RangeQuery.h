#ifndef RANGEQUERY_H

#define RANGEQUERY_H



#include "Query.h"
#include <memory>




/** @file RangeQuery.h

 * @brief class about a kind of query.

 * @author Fuli Lei

 * @version 2012-12-09

 *

 * range query object include a center object and a search radius, the purpose of this query is to get all data objects whose distance to the center object is not more than the search radius.

 **/



/**

* @class CRangeQuery

* @brief a kind of query has a search radius together with a search object.

* @author Fuli Lei

*

* range query object include a center object and a search radius, the purpose of this query is to get all data objects whose distance to the center object is not more than the search radius.

*/

class CRangeQuery :

	public CQuery

{

public:

    /**@brief constructor

     *@param radius the maximum distance between each target data object to the center object

     *@param q the center object,the distance of the data object in the result list to which is not more than the radius 

     */

	CRangeQuery(double radius, shared_ptr<CMetricData> q);

    /**@brief constructor

     *@param q the center object,the distance of the data object in the result list to which is not more than the radius 

     */

	CRangeQuery(shared_ptr<CMetricData> q);

	

    /**@brief destructor*/

	virtual ~CRangeQuery(void);



    /**@brief set the search radius of a range query object

     *@param radius the maximum distance between each target data object to the center object

     */

	void setRadius(double radius);

    /**@brief get the search radius of a range query object

     *@return a double value represents the maximum distance between each target data object to the center object

     */

	double getRadius() const;

private:

    /**the maximum distance between each target data object to the center object*/

	double r;

};

#endif

