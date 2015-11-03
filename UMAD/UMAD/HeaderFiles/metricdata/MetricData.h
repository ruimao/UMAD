#ifndef INDEXOBJECT_H

#define INDEXOBJECT_H



#include <iostream>

#include <fstream>

#include <vector>

#include <memory>



using namespace std;



/** @file MetricData.h

* @base object of all type of data object

* @author Lei Fuli

* @version 2012

* this is a base object encapsulats the basic information shared by all types of data objects.

*/



/**

* @class CMetricData

* @brief base object of all type of data object

* @author Lei Fuli

*

* this is a base object encapsulats the basic information shared by all types of data objects.

*/

class CMetricData

{

public:

    /**none parameter constructor*/

    CMetricData(void);

    /**destructor*/

    virtual ~CMetricData(void);



    virtual int writeExternal(ofstream &out);

    virtual int readExternal(ifstream &in);



};

#endif

