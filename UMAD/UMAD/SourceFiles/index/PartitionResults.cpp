#include "../../HeaderFiles/index/PartitionResults.h"

/** @file PartitionResults.cpp

 * @classes about index algrithm used in the process of building a index structure

 * @author Fuli Lei

 * @version 2012-12-09

*/



/**@brief this is a constructor, used to create an instance of class CPartitionResults

		@param offsets: this is an address container,each element of this vector represents the first object address of a small partition

		@param lower  In this two dimension vector contains the minimum distance value of data objects in each sub-partition to every given pivots. The total  number of rows of this vector is equal to the number of all pivots in this node while the number of column is equal to the number of child nodes in this node

		@param upper In this two dimension vector contains the minimum distance value of data objects in each sub-partition to every given pivots. The total  number of rows of this vector is equal to the number of all pivots in this node while the number of column is equal to the number of child nodes in this node

	*/

CPartitionResults::CPartitionResults(vector<int> &offsets,vector<vector<double> > &lowerBounds,vector<vector<double> > &upperBounds)

{

	this->offsets=offsets;



	this->lowerBounds=lowerBounds;

	this->upperBounds=upperBounds;



}



/**null parameter constructor function*/

CPartitionResults::CPartitionResults()

{

}

/**the destructor*/

CPartitionResults::~CPartitionResults()

{



}



/**get the number of partitions

*@return return the size of partitions

*/

int CPartitionResults::partitionSize()

{

	return offsets.size();

}



/**return the partition results information to the caller of this funciton

*@param index the index of sub partitions in the partition list

*@return return the start address of an partition base on the given partition index 

*/

int CPartitionResults::getPartition(int index)

{

	return offsets.at(index);

}





/**return the value of bound distance value of sub partitions

 *@return return a distance value container

*/

vector<vector<double> >& CPartitionResults::getLowerBounds()

{

	return lowerBounds;

}



/**return the value of bound distance value of sub partitions

 *@return return a distance value container

*/

vector<vector<double> >& CPartitionResults::getUpperBounds()

{

	return upperBounds;

}