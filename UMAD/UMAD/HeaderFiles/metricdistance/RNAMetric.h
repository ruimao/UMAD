#ifndef SQAI_METRIC_RNAMETRIC_H

#define SQAI_METRIC_RNAMETRIC_H



#include <iostream>

#include "MetricDistance.h"

#include "../metricdata/RNA.h"



using namespace std;

/**

 * @file

 * @brief A metric to calculate the distance of two index objects,

 * and this metric is especial for RNA sequence objects.

 * @author Yaoda Liu(2011150337@email.szu.edu.cn)

 * @date 2013/3/18

*/



/**

 * @class

 * @brief This class defines a metric to compute distance of two RNA sequences.

 */

class CRNAMetric:

	public CMetricDistance

{

public:

    /**

     * @brief A edit distance matrix of each two rna symbols. 

     */

	static double EditDistanceMatrix[RNASYMBOLNUMBER][RNASYMBOLNUMBER];



    /**

     * @brief A no parameter constructor, do nothing.

     */

	CRNAMetric();



    /** 

     * @brief A destructor, do nothing

     */

    ~CRNAMetric();



    /**

     * @brief   This method return two IndexObjects' distance.

     * @return  Return a double type distance of two objects.

     */

	virtual double getDistance(CMetricData* one, CMetricData* two);



    /**

     * @brief   This method return two RNAs' distance.

     *          Sum up edit distance of two RNA.

     * @return  Return a double type of distance of two objects.

     */

	double getDistance(CRNA* one, CRNA* two);



};



#endif

// SQAI_METRIC_RNAMETRIC_H

