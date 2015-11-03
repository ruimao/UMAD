#pragma once

#include "PartitionMethod.h"

#include "../metricdata/DoubleIndexObjectPair.h"



#include <algorithm>



/** @file 

* @brief A partition method named EcludedMiddlePartitionMethod

* @author Qiaozhi LI

* @date 2013.5.26

* @version 1.0.0

* @note

* Include partition function to divide dataset into three partitions base on maxRadius and middleProportion

*/





class CExcludedMiddlePartitionMethod :

    public CPartitionMethod

{

public:

    CExcludedMiddlePartitionMethod(void);

    ~CExcludedMiddlePartitionMethod(void);



    /**

    * @brief divide dataset into three partitions base on maxRadius and middleProportion

    * @param metric a function used to calculate the distance of two data objects

    * @param pivots a vector container consists of some vantage data points

    * @param data this is the whole data set, which include all the data objects in the index strcture

    * @param frist the first object address of a fragment that will be partitioned into many small partitions. the fragment, which will be partitioned, is a small section of the whole data set

    * @param size the size of the fragment that will be partition into many small partitions

    * @param numPartition the maxmum number of partitions into which the given data set will be partitioned

    * @param maxLeafSize the maxmum number of objects included in the leaf node of mvp-tree

    * @param maxRadius the exclusion width of middle partition 

    * @param middlePorprotion the porprotion of middle partition in each dataset which will be partitioned

    * @return : return a CPartitionResults object which pack the partition results together

    * @note

    *

    */

    CPartitionResults partition(CMetricDistance *metric, const vector<shared_ptr<CMetricData> > &pivots,vector<shared_ptr<CMetricData> > &data,int first,int size,double maxRadius,int numPartitions,int maxLeafSize,double middleProportion,double trisectionRadius,char* selectOptimalPivots,char* partitionByOnePivot);





    CPartitionResults partition(CMetricDistance *metric, const vector<shared_ptr<CMetricData> > &pivots,vector<shared_ptr<CMetricData> > &data, int first, int size, int numPartitions, int maxLeafSize,double trisectionRadius,char* selectOptimalPivots,char* partitionByOnePivot);    



    int sort(vector<CDoubleIndexObjectPair> &array, int fromIndex, int toIndex);



    double getMid(vector<double> distance);

    int getMin(vector<double> &distance,int begin,int end);

    int getMax(vector<double> &distance,int begin,int end);



    static double _middleProportion;

    static double _maxRadius;



};



