#ifndef EDITDISTANCE_H

#define EDITDISTANCE_H



/** @file

* @brief This class computes edit distance between two strings.  Insert and delete cost are 

* considered to be the same, called gap cost.

* @author Qihang Li,Zhiyong Xu

* @date 2013.3.24

* @version <version number>

* @note

* Copyright Information:

* mobios.dist.EditDistance, 2011.07.21

* Change Log:

* 2011.07.21: Modified from http://www.merriampark.com/ld.htm#FLAVORS, by Rui Mao.

*/



#include <string>

#include "MetricDistance.h"



#include "../../HeaderFiles/metricdata/MetricData.h"

#include "../../HeaderFiles/metricdata/StringObject.h"



using namespace std;



/**

* @class CEditDistance

* @brief This class computes edit distance between two strings.  Insert and delete cost are 

* considered to be the same, called gap cost.

* @author Qihang Li,Zhiyong Xu

* @note

* detailed description

*/

class CEditDistance:public CMetricDistance

{

public:

	/** 

	* @brief no parameter constructor function

	*/

	CEditDistance(void);



	CEditDistance( int gapCost, int substitutionCost);

	/** 

	* @brief destructor function

	*/

	~CEditDistance(void);





	//double getDistance (Object *one, Object *two)

	/** 

	* @brief Computes the distance, the two arguments should be of type {@link String}

	*/

	double getDistance (CMetricData *one, CMetricData *two);



	/**

	* @brief Computes the distance between two {@link String}s

	*/

	double getDistance (CStringObject *first, CStringObject *second);



	/**

	* @brief Computes the distance between two {@link String}s

	*/

	double getDistance(string one, string two);

	int gapCost;



	int substitutionCost;

private:

	static const long serialVersionUID ; 



	/**

	* @brief Computes the minimum of three {@link String}s

	*/

	int minimum(int a,int b,int c);



};

#endif

