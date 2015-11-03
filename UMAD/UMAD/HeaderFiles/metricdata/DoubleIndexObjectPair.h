

#ifndef DOUBLEINDEXOBJECTPAIR_H

#define DOUBLEINDEXOBJECTPAIR_H



#include "../metricdata/MetricData.h"

#include <vector>


#include <memory>


/** @file DoubleIndexObjectPair.h

 * @a class used to map a object to a double value.

 * @author Fuli Lei

 * @version 2012-12-09

 *

 * a class used to map a object to a double value, which is the distance between the object to another object

 **/



/**

* @class CDoubleIndexObjectPair

* @brief a class used to map a object to a double value.

* @author Fuli Lei

*

* a class used to map a object to a double value, which is the distance between the object to another object

*/

class CDoubleIndexObjectPair

{

public:

    /**constructor

     *@param d a double value represents the distance between two objects

     *@param obj the object related to the double distance value

     */

	CDoubleIndexObjectPair(double d, shared_ptr<CMetricData> obj);

    /**none parameter constructor*/

	CDoubleIndexObjectPair();

    /**destructor*/

	~CDoubleIndexObjectPair();



    /**get the double distance value

     *@return a double distance value

     */

	double getDouble();

    /**get the object related to the distance value

     *@param return a object related to the distance value

     */

	shared_ptr<CMetricData> getObject();



    /**set the double value related to the object*/

	void setDouble(double value);

    /**set the object related to the distanc evalue*/

	void setObject(shared_ptr<CMetricData> obj);



private:

    /**a double value represents the distance between two objects*/

	double dist;

    /**the object related to the double distance value*/

	shared_ptr<CMetricData> object;

};

#endif