#ifndef QUERY_H

#define QUERY_H

#include "../../metricdata/MetricData.h"

#include <memory>


/** @file Query.h

 * @brief base class of all kinds of querys.

 * @author Fuli Lei

 * @version 2012-12-09

 *

 * base class of all kinds of querys.There are several kinds of querys like range query ,knn query and some others. 

 **/



/**

* @class CQuery

* @brief base class of all kinds of querys.

* @author Fuli Lei

*

* This class represents space vectors, where each element is a double.

*/

class CQuery

{

public:

    /**@brief constructor used to create and initialize a object

     *@param _q the center object to search with.

     */

	CQuery(std::shared_ptr<CMetricData> _q);

    /**@brief destructor*/

	virtual ~CQuery(void);



    /**@brief set the center object of a query

     *@param _q the center object to search with.

     */

	void setQueryObject(std::shared_ptr<CMetricData> _q);



    /**@brief get the center object of a query

     *@return return the center object of a query

     */

	std::shared_ptr<CMetricData> getQueryObject() const;



private:

    /**center object of a query*/

	std::shared_ptr<CMetricData> q;

};

#endif

