#include "../../HeaderFiles/metricdistance/DNAMetric.h"



/**

 * This class define a metric to compute distance of two DNA sequence.

 */



/** A edit distance matrix of each two DNA symbols. */

double CDNAMetric::EditDistanceMatrix[DNASYMBOLNUMBER][DNASYMBOLNUMBER] = 

{

    {  0,  1,  1,  1,0.5,  1,0.5,  1,0.5,  1,  1,0.5,0.5,0.5,0.5}, // A Adenine 

    {  1,  0,  1,  1,  1,0.5,0.5,  1,  1,0.5,0.5,  1,0.5,0.5,0.5}, // C Cytosine

    {  1,  1,  0,  1,0.5,  1,  1,0.5,  1,0.5,0.5,0.5,  1,0.5,0.5}, // G Guanine  

    {  1,  1,  1,  0,  1,0.5,  1,0.5,0.5,  1,0.5,0.5,0.5,  1,0.5}, // T Thymine   

    {0.5,  1,0.5,  1,  0,  1,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5}, // R Purine (A or G)  

    {  1,0.5,  1,0.5,  1,  0,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5}, // Y Pyrimidine (C, T, or U)      

    {0.5,0.5,  1,  1,0.5,0.5,  0,  1,0.5,0.5,0.5,0.5,0.5,0.5,0.5}, // M C or A  

    {  1,  1,0.5,0.5,0.5,0.5,  1,  0,0.5,0.5,0.5,0.5,0.5,0.5,0.5}, // K T, U, or G  

    {0.5,  1,  1,0.5,0.5,0.5,0.5,0.5,  0,  1,0.5,0.5,0.5,0.5,0.5}, // W T, U, or A   

    {  1,0.5,0.5,  1,0.5,0.5,0.5,0.5,  1,  0,0.5,0.5,0.5,0.5,0.5}, // S C or G 

    {  1,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,  0,0.5,0.5,0.5,0.5}, // B C, T, U, or G (not A)

    {0.5,  1,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,  0,0.5,0.5,0.5}, // D A, T, U, or G (not C)

    {0.5,0.5,  1,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,  0,0.5,0.5}, // H A, T, U, or C (not G) 

    {0.5,0.5,0.5,  1,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,  0,0.5}, // V A, C, or G (not T, not U)

    {0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,  0}  // N Anybase (A,C,G,T,or U) 

};



/** A no parameter constructor, do nothing */

CDNAMetric::CDNAMetric()

{



}



/** A destructor, do nothing */

CDNAMetric::~CDNAMetric()

{



}



/**

 * @brief This method return two IndexObjects' distance.

 * @return  Return a double type distance of two objects.

 */

double CDNAMetric::getDistance(CMetricData* one, CMetricData* two)

{

	CDNA* new_one = static_cast<CDNA*>(one);

	CDNA* new_two = static_cast<CDNA*>(two);

	return getDistance(new_one, new_two);

}



/**

 * @brief   This method return two DNA' distance.

 *          Sum up edit distance of two DNA.

 * @return  Return a double type distance of two objects.

 */

double CDNAMetric::getDistance(CDNA* one, CDNA* two)

{

	int i;

	int one_size = one->getSize();

	int two_size = two->getSize();

	if(one_size != two_size){

		cerr << "Stop! Two DNA must have the same length.";

		return -1.0;

	}

	double dist = 0.0;

	vector<int> one_byte = one->getSymbolIDs();

	vector<int> two_byte = two->getSymbolIDs();

	for(i=0;i<one_size;i++){

		dist = dist + CDNAMetric::EditDistanceMatrix[one_byte[i]][two_byte[i]];

	}

	return dist;

}