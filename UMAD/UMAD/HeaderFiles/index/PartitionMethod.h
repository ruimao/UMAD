#ifndef PARTITIONMETHOD_H

#define PARTITIONMETHOD_H



#include "../metricdistance/MetricDistance.h"



#include "PartitionResults.h"

/** @file PartitionMethod.h

 * @classes about index algrithm used in the process of building a index structure

 * @author Fuli Lei,Yaoda Liu

 * @version 2012-12-09

*/



/**

* @class CPartitionMethod

* @brief abstract class about partition methods

* @author Fuli Lei,Yaoda Liu

*

* partiton the given data set into some small sections based on the distance from each of the objects in the data set to every given pivot

*/



class CPartitionMethod

{

public:

	/**@brief do the partition according to the given parameters

		@param metric a function used to calculate the distance of two data objects

		@param pivots a vector container consists of some vantage data points

		@param data this is the whole data set, which include all the data objects in the index strcture

		@param first the first object address of a fragment that will be partitioned into many small partitions. the fragment, which will be partitioned, is a small section of the whole data set

		@param size the size of the fragment that will be partition into many small partitions

		@param numPartitions the maximal number of partitions into which the given data set will be partitioned

		@param maxLeafSize the maximal number of objects included in the leaf node of mvp-tree



		@return  return a CPartitionResults object which pack the partition results together

	*/

	virtual CPartitionResults partition(CMetricDistance *metric, const vector<shared_ptr<CMetricData> > &pivots,vector<shared_ptr<CMetricData> > &data,int first,int size,int numPartitions,int maxLeafSize,double trisectionRadius,char* selectOptimalPivots,char* partitionByOnePivot)=0;

      /**@brief do the partition according to the given parameters

		@param metric a function used to calculate the distance of two data objects

		@param pivots a vector container consists of some vantage data points

		@param data this is the whole data set, which include all the data objects in the index strcture

		@param first the first object address of a fragment that will be partitioned into many small partitions. the fragment, which will be partitioned, is a small section of the whole data set

		@param size the size of the fragment that will be partition into many small partitions

        @param maxRadius the maximum partition radius value

		@param numPartition the maximal number of partitions into which the given data set will be partitioned

		@param maxLeafSize the maximal number of objects included in the leaf node of mvp-tree

        @param middleProportion



		@return  return a CPartitionResults object which pack the partition results together

	*/

     virtual CPartitionResults partition(CMetricDistance *metric, const vector<shared_ptr<CMetricData> > &pivots,vector<shared_ptr<CMetricData> > &data,int first,int size,double maxRadius,int numPartitions,int maxLeafSize,double middleProportion,double trisectionRadius,char* selectOptimalPivots,char* partitionByOnePivot)=0;

	

};



/**@}*/

#endif