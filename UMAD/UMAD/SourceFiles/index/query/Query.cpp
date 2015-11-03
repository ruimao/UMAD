/**@file Query.cpp

 * @brief base class of all kinds of querys.

 * @author Fuli Lei

 * @version 2012-12-09

 *

 * base class of all kinds of querys.There are several kinds of querys like range query ,knn query and some others. 

 **/

#include "../../../HeaderFiles/index/query/Query.h"



/**@brief constructor used to create and initialize a object

*@param _q the center object to search with.

*/

CQuery::CQuery(shared_ptr<CMetricData> _q)

{

	q = _q;

}



/**@brief destructor*/

CQuery::~CQuery(void)

{

}



/**@brief set the center object of a query

*@param _q the center object to search with.

*/

void CQuery::setQueryObject(shared_ptr<CMetricData> _q)

{

	q=_q;

}



/**@brief get the center object of a query

*@return return the center object of a query

*/

shared_ptr<CMetricData> CQuery::getQueryObject() const

{

	return q;

}