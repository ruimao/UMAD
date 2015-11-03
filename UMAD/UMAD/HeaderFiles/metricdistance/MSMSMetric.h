#ifndef MSMSMETRIC_H

#define MSMSMETRIC_H



/** @file

* @brief MSMSMetric is an implementation of a fuzzy cosine distance metric for comparing tandem spectra

* signatures. Elements of the vectors are equal, within a given tolerance.

* @author Qihang Li,Zhiyong Xu

* @date 2013.3.24

* @version <version number>

* @note

* detailed description

*/



#include "../../HeaderFiles/metricdistance/MetricDistance.h"

#include "../../HeaderFiles/metricdata/MetricData.h"

#include "../../HeaderFiles/metricdata/TandemSpectra.h"



/**

* @class CMSMSMetric

* @brief MSMSMetric is an implementation of a fuzzy cosine distance metric for comparing tandem spectra

* signatures. Elements of the vectors are equal, within a given tolerance.

* @author Qihang Li,Zhiyong Xu

* @note

* detailed description

*/

class CMSMSMetric :

	public CMetricDistance

{

public:



	/**

	* @brief default constructor provides default values for min, max, step, tolerance

	*/

	CMSMSMetric(void);



	/**

	* @brief constructor provides optional values for min, max, step, tolerance

	*/

	CMSMSMetric(int _min, int _max, double _step, double _tol);





	/**

	* @param v1

	*        the {@link IndexObject} over which the keys are defined.

	* @param v2

	*        the other {@link IndexObject} over which the keys are defined.

	*/

	double getDistance(CMetricData* one,CMetricData* two);



	/**

	* @brief Returns absolute difference between precursor masses - within a tolerance

	*/

	double	getAbsPrecursorMassDiff(CTandemSpectra* v1,CTandemSpectra* v2);



	/**

	* @brief compute cosine distance between precursor masses

	* @param v1

	*        the {@link SpectraWithPrecursorMass} over which the keys are defined.

	* @param v2

	*        the other {@link SpectraWithPrecursorMass} over which the keys are defined.

	*/

	double getCosine(CTandemSpectra* one , CTandemSpectra* two);



	/**

	* @brief Computes inner product within a certain tolerance-- "fuzzy" inner product.

	*/

	int getInnerProduct(double * ,int ,double * , int   );



	/**

	* @brief Computes the magnitude of the double array by treating it as a *sparse* binary vector (store

	* only 1's)

	*/

	double getMagnitude(int length);



	static  const double MS_PRECURSOR_TOLERANCE;

	static	const double MS_TOLERANCE;



	~CMSMSMetric(void);



private:



	int min ;

	int max ;// min, max are mass ranges

	double step ;



	double tol ;



	double mscosdist;



	double absMassDiff;



	double massDiffTerm;



	const double  COS_THRESHOLD ;

};

#endif