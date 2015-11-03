/** @file DoubleIndexObjectPair.cpp

* @a class used to map a object to a double value.

* @author Fuli Lei

* @version 2012-12-09

*

* a class used to map a object to a double value, which is the distance between the object to another object

**/

#include "../../HeaderFiles/metricdata/DoubleIndexObjectPair.h"





/**none parameter constructor*/

CDoubleIndexObjectPair::CDoubleIndexObjectPair()

{

	object=shared_ptr<CMetricData>();

}





/**constructor

*@param d a double value represents the distance between two objects

*@param obj the object related to the double distance value

*/

CDoubleIndexObjectPair::CDoubleIndexObjectPair(double d,shared_ptr<CMetricData> obj)

{

	dist = d;

	object=obj;

}



/**destructor*/

CDoubleIndexObjectPair::~CDoubleIndexObjectPair()

{



}



/**get the double distance value

*@return a double distance value

*/

double CDoubleIndexObjectPair::getDouble()

{

	return dist;

}



/**get the object related to the distance value

*@param return a object related to the distance value

*/

shared_ptr<CMetricData> CDoubleIndexObjectPair::getObject()

{

	return object;

}



/**set the double value related to the object*/

void CDoubleIndexObjectPair::setDouble(double value)

{

	dist = value;



}



/**set the object related to the distanc evalue*/

void CDoubleIndexObjectPair::setObject(shared_ptr<CMetricData> obj)

{

	object=obj;

}

