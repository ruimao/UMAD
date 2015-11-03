/** @file EuclideanDistance.cpp

 * @classes about metirc methods

 * @author Fuli Lei

 * @version 2012-12-09

*/

//#define CHECKPOINTER

#include "../../HeaderFiles/metricdistance/EuclideanDistance.h"

#include "../../HeaderFiles/metricdata/DoubleVector.h"



#include <cmath>







/** no parameter constructor function*/

CEuclideanDistance::CEuclideanDistance()

{

}

/** destructor function*/

CEuclideanDistance::~CEuclideanDistance()

{

}



/** compute the distance of two double array. 

* @param[in] v1 receives a pointer which points to a double array

* @param[in] v2 receives a pointer which points to a double array

* @param[in] _length receives a int value which representst he dimension of the array

* @return returns a double which represents the distance of the two given arrays

*/



double CEuclideanDistance::getDistance(double *v1,double *v2,int _length)

{

#ifdef CHECKPOINTER

	if(v1==NULL || v2==NULL)

		cout<<"pointer v1 or v2 is null"<<endl;

#endif

	double _distance=0;



	for(int i=0;i<_length;i++)

    {

       

		_distance+=pow((v1[i]-v2[i]),2);

    }

	return sqrt(_distance);

}



/** compute the distance of two objects of class CMetricData, or its subclass. 

* @param[in] _obj1 receives a pointer of object

* @param[in] _obj2 receives a pointer of object

* @return return a double value which represents the distance of the two given objects

*/

double CEuclideanDistance::getDistance(CMetricData *_obj1,CMetricData *_obj2)

{

#ifdef CHECKPOINTER

	if(_obj1==NULL || _obj2==NULL)

		cout<<"null pointer parameters"<<endl;

#endif



	return getDistance(((CDoubleVector*)_obj1)->getData(),((CDoubleVector*)_obj2)->getData(),((CDoubleVector*)_obj1)->getLen());

}











