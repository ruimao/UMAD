#ifndef SQAI_INDEXALGORITHM_BALANCE_PARTITIONMETHOD_H

#define SQAI_INDEXALGORITHM_BALANCE_PARTITIONMETHOD_H



/**@file

 * @brief A partition method. This partition method will divide

 * data into some well-distributed partitions.

 * @author Yaoda Liu

 * @author 2011150337 at email.szu.edu.cn

 * @date 2013/5/16

 *

 * This class implements a kind of partition method,

 *  which partition the given data set into serveral samll partitions with

 *  the same cardinality.

*/



#include "PartitionMethod.h"

#include "../metricdata/DoubleIndexObjectPair.h"


#include <memory>




/**

 * @class CBalancePartitionMethod

 * @brief This class implements a kind of partition method.

 * @author Yaoda Liu.

 *

 * This class implements a kind of partition method,

 *  which partition the given data set into serveral samll partitions with

 *  the same cardinality.

 */



class CBalancePartitionMethod:

    public CPartitionMethod

{

public:



    /**

     * @brief This is a non parameters constructor.

     * @author Yaoda Liu.

     */

    CBalancePartitionMethod();



    /**

     * @brief This is a destructor.

     * @author Yaoda Liu.

     */

    ~CBalancePartitionMethod();

    

    /**

     * @brief This method partition input data into several partitions.

     * @author Yaoda Liu.

     * @param metric          A class to calculate the distance of two data objects.

     * @param pivots          A vector container consists of some vantage data points.

     * @param data            This is the whole data set, which include all the data objects in the index strcture.

     * @param first           The first object address of a fragment that will be partitioned into many small partitions. 

     *                        The fragment, which will be partitioned, is a small section of the whole data set.

     * @param size            The size of the fragment that will be partition into many small partitions.

     * @param maxParition     The maxmum number of partitions the given data set will be partitioned.

     * @param maxLeafSize     The maxmum number of objects included in the leaf node of mvp-tree

     * @return      A CPartitionResults object which pack the partition results together.

     */

    virtual CPartitionResults partition(CMetricDistance *metric, const vector<shared_ptr<CMetricData> > &pivots,

                                vector<shared_ptr<CMetricData> > &data, int first, int size,

                                int maxParition, int maxLeafSize,double trisectionRadius,char* selectOptimalPivots,char* partitionByOnePivot);



	/**

     * @brief This method partition input data into several partitions.

     * @author Yaoda Liu.

     * @param metric            A class to calculate the distance of two data objects.

     * @param pivots            A vector container consists of some vantage data points.

     * @param data              This is the whole data set, which include all the data objects in the index strcture.

     * @param first             The first object address of a fragment that will be partitioned into many small partitions. 

     *                          The fragment, which will be partitioned, is a small section of the whole data set.

     * @param size              The size of the fragment that will be partition into many small partitions.

     * @param maxRaduis         The maxmun raduis of each two objects.

     * @param maxParition       The maxmum number of partitions the given data set will be partitioned.

     * @param maxLeafSize       The maxmum number of objects included in the leaf node of mvp-tree

     * @param middleProportion  

     * @return      A CPartitionResults object which pack the partition results together.

     */

	virtual CPartitionResults partition(CMetricDistance *metric, const vector<shared_ptr<CMetricData> > &pivots,

                                        vector<shared_ptr<CMetricData> > &data, int first, int size, double maxRadius,

                                        int maxParition, int maxLeafSize, double middleProportion,double trisectionRadius,char* selectOptimalPivots,char* partitionByOnePivot);



};



#endif

// SQAI_INDEXALGORITHM_BALANCE_PARTITIONMETHOD_H

