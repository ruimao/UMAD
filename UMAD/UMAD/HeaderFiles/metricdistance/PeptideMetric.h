#ifndef SQAI_METRIC_PeptideMETRIC_H

#define SQAI_METRIC_PeptideMETRIC_H



/**@file PeptideMetric.h

 * @brief A metric to calculate the distance of two index object,

 * and this metric is especially for Peptide sequence objects.

 * @author Yadi Cai

 * @date 2013/3/18

 *

 * This class defines a metric to calculate the distance of

 * two Peptide sequence, and then, for the final goal, build the index.

*/



#include <iostream>

#include "../../HeaderFiles/metricdistance/MetricDistance.h"

#include "../../HeaderFiles/metricdata/Peptide.h"



using namespace std;

 

/**

 * @class

 * @brief This class defines a metric to compute distance of two Peptide sequence.

*/



class CPeptideMetric:

	public CMetricDistance

{

public:



	/*

	 *@brief A edit distance matrix of each two Peptide symbols. 

	 */

	static double EditDistanceMatrix[PeptideSYMBOLNUMBER-3][PeptideSYMBOLNUMBER-3];

   

	/*

	 *@brief A no parameter constructor, do nothing 

	 *@return void

	 */

	CPeptideMetric();



    /*

	 *@brief A destructor, do nothing 

	 *@return void

	 */

	~CPeptideMetric();



    /**

     * @brief This method return two IndexObjects' distance.

     * @return  Return a double type distance of two objects.

     */

	virtual double getDistance(CMetricData* one, CMetricData* two);



    /**

     * @brief  This method return two peptides' distance.

     *          Sum up edit distance of two peptides.

     * @return   Return a double type of distance of two objects.

     */

	double getDistance(CPeptide* one, CPeptide* two);

};



#endif

// SQAI_METRIC_PeptideMETRIC_H