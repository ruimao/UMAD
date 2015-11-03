/** @file IndexObject.cpp

* @base object of all type of data object

* @author Lei Fuli

* @version 2012

* this is a base object encapsulats the basic information shared by all types of data objects.

*/

#include "../../HeaderFiles/metricdata/MetricData.h"





/**none parameter constructor*/

CMetricData::CMetricData(void)

{



}





/**destructor*/

CMetricData::~CMetricData(void)

{



}



int CMetricData::writeExternal(ofstream &out)

{

    return 0;

}



int CMetricData::readExternal(ifstream &in)

{

    return 0;

}