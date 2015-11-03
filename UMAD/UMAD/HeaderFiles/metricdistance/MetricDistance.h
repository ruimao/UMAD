#ifndef METRIC_H

#define METRIC_H

#include "../metricdata/MetricData.h"



/** @file MetricDistance.h

 * @classes about metirc methods

 * @author Fuli Lei

 * @version 2012-12-09

*/





/**

* @class CMetricDistance

* @brief abstract class about methic methods

* @author Fuli Lei

* @version 2013526

*

* class name is CMetricDistance, this class descripe the

* basic information of how to get the distance of two object

*/



class CMetricDistance

{

public:

	    /**@brief no parameter constructor function*/

        CMetricDistance();

		/**@brief destructor function of this class type for releasing the internal memory space which allocated dynamically

		 */

		~CMetricDistance();



		/**@brief compute the distance of two given CMetricData(or it's subclass) objects

            @return return the distance of to objects.

        */

        virtual double getDistance(CMetricData*,CMetricData*)=0;



};



#endif