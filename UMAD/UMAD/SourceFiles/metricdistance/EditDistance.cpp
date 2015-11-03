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

#include "../../HeaderFiles/metricdistance/EditDistance.h"



CEditDistance::CEditDistance(void)

{

	gapCost=1;

	substitutionCost=1;

}



CEditDistance::~CEditDistance(void)

{

}



/*

double CEditDistance::getDistance (Object one, Object two)

{

if ( one == null)

throw new IllegalArgumentException("the first object to compute distance is null!");



if ( two == null)

throw new IllegalArgumentException("the second object to compute distance is null!");



return getDistance ( (String)one, (String) two );

}

*/



/**

* @param first the first String to compute edit distance

* @param second the second String to compute edit distance

* @return the edit distance between the two arguments

*/

double CEditDistance::getDistance (CMetricData *one, CMetricData *two)

{

	/* if ( one == null)

	throw new IllegalArgumentException("the first object to compute distance is null!");



	if ( two == null)

	throw new IllegalArgumentException("the second object to compute distance is null!");

	*/

	return getDistance ( (CStringObject*)one, (CStringObject*) two );

}



/**

* @param first the first String to compute edit distance

* @param second the second String to compute edit distance

* @return the edit distance between the two arguments

*/

double CEditDistance::getDistance (CStringObject *first, CStringObject *second)

{

	return getDistance(first->getData(), second->getData());

}



CEditDistance::CEditDistance(int gapCost, int substitutionCost){

	if ( ( gapCost > substitutionCost *2) || (substitutionCost > gapCost *2) )

		//throw new IllegalArgumentException("the two arguments should satisfy gapCost/2 <= substitutionCost <= gapCost *2!");

			this->gapCost = gapCost;

	this->substitutionCost = substitutionCost;

}



int CEditDistance::minimum (int a, int b, int c) 

{

	int min = ( a <= b)? a :b ;



	return (min <= c)? min : c ;

}



/**

* @param first the first String to compute edit distance

* @param second the second String to compute edit distance

* @return the edit distance between the two arguments

*/

double CEditDistance:: getDistance(string first, string second)

{

	const int firstSize = first.length(); // length of first

	const int secondSize = second.length() ; // length of second



	// Step 1

	if (firstSize == 0) 

		return secondSize*gapCost;



	if (secondSize == 0) 

		return firstSize*gapCost;



	int * * matrix=new int * [firstSize+1]; // the dynamic programming matrix

	for(int i=0;i<=firstSize;i++)

		matrix[i]=new int [secondSize+1];



	// Step 2, initialize the matrix( first row and first column)

	for (int i = 0; i <= firstSize; i++) 

		matrix[i][0] = i*gapCost;

	for (int j = 0; j <= secondSize; j++) 

		matrix[0][j] = j*gapCost;



	// Step 3, fill the matrix

	char firstChar;

	int j;

	int cost;

	for (int i = 1; i <= firstSize; i++) 

	{

		firstChar = first.at (i - 1);

		for (j = 1; j <= secondSize; j++) 

		{

			cost = ( firstChar == second.at (j - 1) )? 0 :substitutionCost ;

			matrix [i] [j] = minimum (matrix[i-1][j]+gapCost, matrix[i][j-1]+gapCost, matrix[i-1][j-1] + cost);

		}

	}



	double matrixNum = matrix[firstSize][secondSize];

	for(int i=0;i<=firstSize;i++)

	{

		delete []matrix[i];

		matrix[i]=NULL;

	}

	delete []matrix;

	matrix=NULL;



	return matrixNum;



}



static const long serialVersionUID = -4658067077491099474L;