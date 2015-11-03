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



#include "../../HeaderFiles/metricdistance/ImageMetric.h"



/**

 * @brief A constructor.

 * @author Arthur Fu

 * @return none

 * @note

 * detailed description

 *  - The feaLength will be initialized to '3', '48', '15'.

 *  - The minBool will be initialized to 'false'.

 *  - The weights will be initialized to '0.333333'. 

 *  - The maxDist will be initialized to '1.0', '60.0', '1.0'.

*/

CImageMetric::CImageMetric()

{

	int i;



	feaLength[0] = 3;

	feaLength[1] = 48;

	feaLength[2] = 15;



	for(i = 0; i < 3; i++)

		minBool[i] = false;

	for(i = 0; i < 3; i++)

		weights[i] = 0.333333;



	maxDist[0] = 1.0;

	maxDist[1] = 60.0;

	maxDist[2] = 1.0;

}



/**

 * @brief A destructor, do nothing.

 * @author Arthur Fu

*/

CImageMetric::~CImageMetric()

{

}



/**

 * @brief The method will return the distance of the two objects of CMetricData.

 * @author Arthur Fu

 * @param one : the {@link Object} over which the keys are defined.

 * @param two : the other {@link Object} over which the keys are defined.

 * @return The distance of two objects of CIndexBoject.

 * @note

 * detailed description

 *  - The objects of CMetricData are changed into CImage type.

 *  - Then call the methor getDistance(CImage*, CImage*) to compute the distance.

*/

double CImageMetric::getDistance(CMetricData *one, CMetricData *two) 

{

	CImage *new_one = static_cast<CImage *> (one);

	CImage *new_two = static_cast<CImage *> (two);

	return getDistance(new_one, new_two);

}



/**

 * @brief The method will return the distance of the two objects of CImage.

 * @author Arthur Fu

 * @param one : the {@link Image} over which the keys are defined.

 * @param two : the other {@link Image} over which the keys are defined.

 * @return The distance of two objects of CImage.

 * @note

 * detailed description

 *  - Define a variable 'dist' to store the distance.

 *  - The distance is calculated by the methor getDistance_Fea(CImage *, CImage *, int).

 *  - The dist is obtained by a cycle accumulation of (getDistance_Fea(one, two, i) / maxDist[i] * weights[i]).

*/

double CImageMetric::getDistance(CImage *one, CImage *two) 

{

    double dist = 0.0;

    for (int i = 0; i < feaNum; i++)

		dist += (getDistance_Fea(one, two, i) / maxDist[i] * weights[i]);

	return dist;

}



/**

 * @brief The method is the key to compute the distance and return the distance.

 * @author Arthur Fu

 * @param one : the {@link Image} over which the keys are defined.

 * @param two : the other {@link Image} over which the keys are defined.

 * @param FeaIndex : the feature on which distance is to be computed.

 * @return The distance of two objects of CImage.

 * @note

 * detailed description

 *  - Find out start and end index.

 *  - If the value in 'minBool[FeaIndex]' is 'true',cumulative the index from 'StartIndex' to 'EndIndex' all the larger values in the two object at 'cnt'.

 *  - And then execute "dist= abs(1.0 - (dist / tempval))".

 *  - Else, cumulative the index from 'StartIndex' to 'EndIndex' all the values of (one->getFeature(cnt) - two->getFeature(cnt)).

 *  - And then execute "dist= sqrt(dist)".

*/

double CImageMetric::getDistance_Fea(CImage *one, CImage *two, int FeaIndex) 

{

	int StartIndex = 0, EndIndex = 0, cnt;

	double dist = 0.0, tempval = 0.0;



	for (int i = 0; i < FeaIndex; i++)

		StartIndex += feaLength[i];

	EndIndex = StartIndex + feaLength[FeaIndex] - 1;



    // The first method for computing image object distance.

	if (minBool[FeaIndex]) 

	{

		for (cnt = StartIndex; cnt <= EndIndex; cnt++) 

		{

			dist += (one->getFeature(cnt) >= two->getFeature(cnt) ? two->getFeature(cnt) : one->getFeature(cnt));

            tempval += one->getFeature(cnt);

            // to make it a symmetric Metric space, add the following line

            // tempval += two->getFeature(cnt);

		}

		dist= abs(1.0 - (dist / tempval));

	}

		

	else // The second method for computing image object distance.

	{ 

		for (cnt = StartIndex; cnt <= EndIndex; cnt++) 

		{

			tempval = (one->getFeature(cnt) - two->getFeature(cnt));

			dist += (tempval * tempval);

		}

		dist= sqrt(dist);

	}

	return dist;

}

