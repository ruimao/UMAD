#ifndef LINFINITYDISTANCE_H

#define LINFINITYDISTANCE_H

#include "MetricDistance.h"


/** @defgroup LINFINITYDISTANCE SQAI:LINFINITYDISTANCE

 *  @author weipeng zhang

 *  @version 2013-7-18

 *  @{

 */



/** this class will generate the distance of two object*/

class CLInfinityDistance :

        public CMetricDistance

{

public:

		/** no parameter constructor function*/

        CLInfinityDistance();

		/** destructor function*/

        ~CLInfinityDistance();



		/** return distance of two CMetricData object*/

        virtual double getDistance(CMetricData*,CMetricData*);

		/** return distance of two double array*/

		virtual double getDistance(double *,double *,int);
};

/**@}*/

#endif





