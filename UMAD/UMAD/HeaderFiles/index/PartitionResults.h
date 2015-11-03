#ifndef PARTITIONRESULTS_H

#define PARTITIONRESULTS_H



#include "../metricdata/MetricData.h"

#include "../index/IndexNode.h"



/** @file PartitionResults.h

 * @classes about index algrithm used in the process of building a index structure

 * @author Fuli Lei

 * @version 2012-12-09

*/



/**

* @class CPartitionResults

* @brief abstract class about partition methods

* @author Fuli Lei

*

* this class is used to package the partition results of the function partition in class CPartitionMethod so that we can use all the results packaged to create an internal node

*/

class CPartitionResults

{

public:

	/**null parameter constructor function*/

	CPartitionResults();

	/**@brief this is a constructor, used to create an instance of class CPartitionResults

		@param offsets: this is an address container,each element of this vector represents the first object address of a small partition

		@param lower  In this two dimension vector contains the minimum distance value of data objects in each sub-partition to every given pivots. The total  number of rows of this vector is equal to the number of all pivots in this node while the number of column is equal to the number of child nodes in this node

		@param upper In this two dimension vector contains the minimum distance value of data objects in each sub-partition to every given pivots. The total  number of rows of this vector is equal to the number of all pivots in this node while the number of column is equal to the number of child nodes in this node

	*/

	CPartitionResults(vector<int> &offsets,vector<vector<double> > &lower,vector<vector<double> > &upper);



	/**the destructor*/

	~CPartitionResults();



	/**get the number of partitions

	 *@return return the size of partitions

	*/

	int partitionSize();



	/**return the partition results information to the caller of this funciton

     *@param index the index of sub partitions in the partition list

	 *@return return the start address of an partition base on the given partition index 

	*/

	int getPartition(int index);



	/**return the value of bound distance value of sub partitions

	 *@return return a distance value container

	*/

	vector<vector<double> >& getLowerBounds();



	/**return the value of bound distance value of sub partitions

	 *@return return a distance value container

	*/

	vector<vector<double> >& getUpperBounds();



private:



	/**each element of this vector represents the first object address of a small partition*/

	vector<int> offsets;



	/**In this two dimension vector contains the minimum distance value of data objects in each sub-partition to every given pivots. The total  number of rows of this vector is equal to the number of all pivots in this node while the number of column is equal to the number of child nodes in this node*/

	vector<vector<double> > lowerBounds;

	/**In this two dimension vector contains the maximum distance value of data objects in each sub-partition to every given pivots. The total  number of rows of this vector is equal to the number of all pivots in this node while the number of column is equal to the number of child nodes in this node*/

	vector<vector<double> > upperBounds;



};



#endif