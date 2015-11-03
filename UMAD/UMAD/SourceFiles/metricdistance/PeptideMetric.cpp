#include "../../HeaderFiles/metricdistance/PeptideMetric.h"



/**

 * This class define a metric to compute distance of two Peptide sequence.

 */



/** A edit distance matrix of each two Peptide symbols. */

double CPeptideMetric::EditDistanceMatrix[PeptideSYMBOLNUMBER-3][PeptideSYMBOLNUMBER-3] = 

{

            { 0, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 5, 4, 2, 7 },//A

            { 2, 0, 2, 2, 4, 2, 2, 2, 2, 3, 3, 2, 2, 4, 2, 2, 2, 4, 4, 3, 7 },//R

            { 2, 2, 0, 2, 4, 2, 2, 2, 2, 3, 3, 2, 2, 4, 2, 2, 2, 5, 4, 2, 7 },//N

            { 2, 2, 2, 0, 4, 2, 2, 2, 2, 3, 3, 2, 3, 4, 2, 2, 2, 6, 4, 2, 7 },//D

            { 3, 4, 4, 4, 0, 4, 4, 3, 4, 3, 4, 4, 4, 4, 3, 3, 3, 7, 3, 3, 7 },//C

            { 2, 2, 2, 2, 4, 0, 2, 2, 2, 3, 3, 2, 2, 4, 2, 2, 2, 5, 4, 3, 7 },//Q

            { 2, 2, 2, 2, 4, 2, 0, 2, 2, 3, 3, 2, 3, 4, 2, 2, 2, 6, 4, 2, 7 },//E

            { 2, 2, 2, 2, 3, 2, 2, 0, 2, 2, 3, 2, 2, 4, 2, 2, 2, 6, 4, 2, 7 },//G

            { 2, 2, 2, 2, 4, 2, 2, 2, 0, 3, 3, 2, 3, 3, 2, 2, 2, 5, 3, 3, 7 },//H

            { 2, 3, 3, 3, 3, 3, 3, 2, 3, 0, 1, 3, 2, 2, 2, 2, 2, 5, 3, 2, 7 },//I

            { 2, 3, 3, 3, 4, 3, 3, 3, 3, 1, 0, 3, 1, 2, 3, 3, 2, 4, 2, 1, 7 },//L

            { 2, 2, 2, 2, 4, 2, 2, 2, 2, 3, 3, 0, 2, 4, 2, 2, 2, 4, 4, 3, 7 },//K

            { 2, 2, 2, 3, 4, 2, 3, 2, 3, 2, 1, 2, 0, 2, 2, 2, 2, 4, 3, 2, 7 },//M

            { 3, 4, 4, 4, 4, 4, 4, 4, 3, 2, 2, 4, 2, 0, 4, 3, 3, 3, 1, 2, 7 },//F

            { 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 3, 2, 2, 4, 0, 2, 2, 5, 4, 2, 7 },//P

            { 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 3, 2, 2, 3, 2, 0, 2, 5, 4, 2, 7 },//S

            { 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2, 2, 0, 5, 3, 2, 7 },//T

            { 5, 4, 5, 6, 7, 5, 6, 6, 5, 5, 4, 4, 4, 3, 5, 5, 5, 0, 4, 5, 7 },//W

            { 4, 4, 4, 4, 3, 4, 4, 4, 3, 3, 2, 4, 3, 1, 4, 4, 3, 4, 0, 3, 7 },//Y

            { 2, 3, 2, 2, 3, 3, 2, 2, 3, 2, 1, 3, 2, 2, 2, 2, 2, 5, 3, 0, 7 },//V

            { 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 0 } //OTHER

};



/** A no parameter constructor, do nothing */

CPeptideMetric::CPeptideMetric()

{

}



/** A destructor, do nothing */

CPeptideMetric::~CPeptideMetric()

{

}



/**

 * @brief This method return two IndexObjects' distance.

 * @return  Return a double type distance of two objects.

 */

double CPeptideMetric::getDistance(CMetricData* one, CMetricData* two)

{

	CPeptide* new_one = static_cast<CPeptide*>(one);

	CPeptide* new_two = static_cast<CPeptide*>(two);

	return getDistance(new_one, new_two);

}



/**

 * @brief   This method return two Peptide' distance.

 *          Sum up edit distance of two Peptide.

 * @return  Return a double type distance of two objects.

 */

double CPeptideMetric::getDistance(CPeptide* one, CPeptide* two)

{

	int i;

	int one_size = one->getSize();

	int two_size = two->getSize();

	if(one_size != two_size)

	{

		cerr << "Stop! Two peptide must have the same length.";

		return -1.0;

	}

	double dist = 0.0;

	vector<int> one_byte = one->getSymbolIDs();

	vector<int> two_byte = two->getSymbolIDs();

	for(i=0;i<one_size;i++)

	{

		dist = dist + CPeptideMetric::EditDistanceMatrix[one_byte[i]][two_byte[i]];

	}

	/*delete &one_byte;

	delete &two_byte;*/

	return dist;

}