#ifndef PIVOTWISEPARTITION_H
#define PIVOTWISEPARTITION_H

#include "../index/PartitionMethod.h"
#include "../metricdata/MetricData.h"
#include "../metricdistance/MetricDistance.h"
#include "../metricdistance/DNAMetric.h"
#include "../metricdistance/EditDistance.h"
#include "../metricdistance/EuclideanDistance.h"
#include "../metricdistance/ImageMetric.h"
#include "../metricdistance/MSMSMetric.h"
#include "../metricdistance/PeptideMetric.h"
#include "../metricdistance/RNAMetric.h"
#include "../metricdistance/CountedMetric.h"
#include "util/ClusteringTask.h"

/** @file	PivotWisePartitionMethod.h
* @classes	Inherited CPartitionMethod
* @author	He Zhang
* @version	2014-07-23
*/

/**
* @class	PivotWisePartitionMethod
* @brief	Inherited CPartitionMethod,To achieve data partition
* @author	He Zhang
*/

class PivotWisePartitionMethod : public CPartitionMethod
{
public:

	/**@brief do the partition according to the given parameters
	@param metric	a function used to calculate the distance of two data objects
	@param pivots	a vector container consists of some vantage data points
	@param data		this is the whole data set, which include all the data objects in the index strcture
	@param first	the first object address of a fragment that will be partitioned into many small partitions. the fragment, which will be partitioned, is a small section of the whole data set
	@param size		the size of the fragment that will be partition into many small partitions
	@param numPartitions	the maximal number of partitions into which the given data set will be partitioned
	@param maxLeafSize	the maximal number of objects included in the leaf node of mvp-tree
	@return		return a CPartitionResults object which pack the partition results together
	*/
	virtual CPartitionResults partition(CMetricDistance *metric, const vector<shared_ptr<CMetricData> > &pivots,vector<shared_ptr<CMetricData> > &data, int first, int size,int maxParition, int maxLeafSize,double trisectionRadius,char* selectOptimalPivots,char* partitionByOnePivot);
	/**@brief	do the partition according to the given parameters
	@param metric	a function used to calculate the distance of two data objects
	@param pivots	a vector container consists of some vantage data points
	@param data		this is the whole data set, which include all the data objects in the index strcture
	@param first	the first object address of a fragment that will be partitioned into many small partitions. the fragment, which will be partitioned, is a small section of the whole data set
	@param size		the size of the fragment that will be partition into many small partitions
	@param maxRadius	the maximum partition radius value
	@param numPartition		the maximal number of partitions into which the given data set will be partitioned
	@param maxLeafSize		the maximal number of objects included in the leaf node of mvp-tree
	@param middleProportion
	@return		return a CPartitionResults object which pack the partition results together
	*/
	virtual CPartitionResults partition(CMetricDistance *metric, const vector<shared_ptr<CMetricData> > &pivots,vector<shared_ptr<CMetricData> > &data, int first, int size, double maxRadius,int maxParition, int maxLeafSize, double middleProportion,double trisectionRadius,char* selectOptimalPivots,char* partitionByOnePivot);

};

#endif