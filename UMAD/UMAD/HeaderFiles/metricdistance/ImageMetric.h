/**@file    

 * @brief   A metric to calculate the distance of two index objects of image,

 * @author  Arthur Fu

 * @date    2013/05/29

 * @version Bate_0.9

 *

 * detailed description

 * This class defines a metric to calculate the distance of two image.

 * Designed for the MoBIoS image dataset and may not work for other datasets. 

*/



#ifndef IMAGEMETRIC_H

#define IMAGEMETRIC_H



#include "MetricDistance.h"

#include "../metricdata/Image.h"

#include "../metricdata/MetricData.h"

#include <cmath>



using namespace std;



/**

 * @class

 * @brief   This class defines a metric to calculate the distance of two image.

 * @author  Arthur Fu

 * @note    

 * detailed description

 * The Image class define a CImageMetric type to contain the information of the metric of image.

 *  - The method getDistance(CMetricData*, CMetricData*) will return the distance of the two objects of CMetricData.

 *  - The method getDistance(CImage*, CImage*) will return the distance of the two objects of CImage.

 *  - The method getDistance_Fea(CImage *, CImage *, int) is the key to compute the distance and return the distance.

*/

class CImageMetric :

	public CMetricDistance

{

public:

	/** 

	 * @brief A constructor.

	 */

	CImageMetric();



	/** 

	 * @brief A destructor, do nothing.

	 */

	~CImageMetric();



	/** 

	 * @brief The method will return the distance of the two objects of CMetricData.

	 */

    double getDistance(CMetricData *one, CMetricData *two);



	/** 

	 * @brief The method will return the distance of the two objects of CImage.

	 */

    double getDistance(CImage *one, CImage *two);



	/** 

	 * @brief The method is the key to compute the distance and return the distance.

	 */

    double getDistance_Fea(CImage *one, CImage *two, int FeaIndex);



private:

	int static const feaNum = 3;///<Feature number for each image. We have three features for each image: structure, texture,histogram.



	int feaLength[3];///<Number of floats that can represent each feature.



    bool minBool[3];///<Distance function selection for each feature.



    double weights[3];///<The weights of each feature in the computation of distance.



    double maxDist[3];///<The max distance for each feature.

};

#endif