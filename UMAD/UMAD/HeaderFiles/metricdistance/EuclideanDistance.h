#ifndef EUCLIDEANDISTANCE_H

#define EUCLIDEANDISTANCE_H

#include "MetricDistance.h"



/** @file EuclideanDistance.h

* @classes about metirc methods

* @author Fuli Lei

* @version 2012-12-09

*/



/**

* @class CEuclideanDistance

* @brief abstract class about metric methods

* @author Fuli Lei

*

* this class will generate the distance of two object

*/



class CEuclideanDistance :

	public CMetricDistance

{

public:

	/** no parameter constructor function*/

	CEuclideanDistance();

	/** destructor function*/

	~CEuclideanDistance();



	/**compute the distance of tow objects

	*@return distance of two CMetricData object

	*/

	virtual double getDistance(CMetricData*,CMetricData*);

	/**compute the distance of two array of dimention d

	*@return distance of two double array

	*/

	virtual double getDistance(double *array1,double *array2,int d);

	

};

/**@}*/

#endif





