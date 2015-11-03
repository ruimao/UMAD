/**@file BalancePartitionMethod.cpp

* @brief A partition method. This partition method will divide

* data into some well-distributed partitions.

* @author Yaoda Liu(2011150337@email.szu.edu.cn)

* @date 2013/5/16

*/



#include <cmath>

#include <algorithm>

#include <iostream>



#include "../../HeaderFiles/index/BalancePartitionMethod.h"

#include "../../HeaderFiles/metricdata/DoubleIndexObjectPair.h"

#include "../../HeaderFiles/index/MVPInternalNode.h"

#include "../../HeaderFiles/metricdata/DoubleVector.h"



using namespace std;





/**

* @class CBalancePartitionMethod

* @brief This class implements a kind of partition method.

* @author Yaoda Liu.

*

* This class define a parition method. This method partition input data into

* several sections averagely.

*/



/**

* @brief This is a non parameters constructor.

* @author Yaoda Liu.

*/

CBalancePartitionMethod::CBalancePartitionMethod()

{



}



/**

* @brief This is a destructor.

* @author Yaoda Liu.

*/

CBalancePartitionMethod::~CBalancePartitionMethod()

{



}



/** 

* @breif     Sort DoubleIndexObjectPair array where element's index in [fromIndex, toIndex)

*            This is method sorts a list of DoubleIndexObjectPair.

* CDoubleIndexObjectPair     The list waiting to be sorted.

* int                        The minimum index of the list.

* int                        The maximun index of the list.

*/



/**

* @struct

* @brief    A STL comparer to compare two pivots' distance to another pivot.

*/

struct IndexObjectComparer

{

	/** 

	* The pivot waiting to be compared.

	*/

	shared_ptr<CMetricData> pivot;



	/**

	* This is a metric, help calculate the distance of two pivots.

	*/

	CMetricDistance* metric;



	/**

	* The method to compare two IndexObject's distance to the third pivot.

	*/

	bool operator() (shared_ptr<CMetricData> first, shared_ptr<CMetricData> second)

	{

		return metric->getDistance(pivot.get(), first.get()) < metric->getDistance(pivot.get(), second.get());

	}

};



/**

* @brief    Sort vector of CMetricData using std::sort method.

* @param vectorToBeSorted       A vector of CMetricData waiting to be sorted.

* @param pivot                  The third pivot which two pivots in vector calculate the distance with.

* @param metric                 A metric help calculate the distance of two pivots.

* @param begin                  The begin index of vector.

* @param size                   The size of vector choose to sort.

*/

void sortVector(vector<shared_ptr<CMetricData> > &vectorToBeSorted, shared_ptr<CMetricData> pivot, CMetricDistance* metric, int begin, int size)

{

	IndexObjectComparer comparer;

	comparer.metric = metric;

	comparer.pivot = pivot;



	vector<shared_ptr<CMetricData> >::iterator startIndex = vectorToBeSorted.begin() + begin;

	vector<shared_ptr<CMetricData> >::iterator endIndex = vectorToBeSorted.begin() + size;

	std::sort(startIndex, endIndex, comparer);

}



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

CPartitionResults CBalancePartitionMethod::partition(CMetricDistance *metric,

													 const vector<shared_ptr<CMetricData> > &pivots,

													 vector<shared_ptr<CMetricData> > &data, int first,

													 int size, double maxRadius, int maxPartitions,

													 int maxLeafSize, double middleProportion,double trisectionRadius,char* selectOptimalPivots,char* partitionByOnePivot)

{

	return partition(metric,  pivots , data, first,  size,  maxPartitions, maxLeafSize,trisectionRadius,selectOptimalPivots,partitionByOnePivot);

}





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

CPartitionResults CBalancePartitionMethod::partition(CMetricDistance *metric, const vector<shared_ptr<CMetricData> > &pivots, 

													 vector<shared_ptr<CMetricData> > &data, int first, int size,

													 int maxPartitions, int maxLeafSize,double trisectionRadius,char* selectOptimalPivots,char* partitionByOnePivot)

{



	const int numPivots = pivots.size();

	const int fanout = (int) pow(maxPartitions, numPivots);



	/*

	*  The lower and upper bound of distances from each child to each vantage point

	*/



	vector<vector<double> > lower(numPivots);

	vector<vector<double> > upper(numPivots);



	for(int vectorIndex=0;vectorIndex<numPivots;++vectorIndex)

	{

		lower[vectorIndex].resize(fanout);

		upper[vectorIndex].resize(fanout);

	}



	vector<CDoubleIndexObjectPair> wrapper;



	/* split data  */



	/* total cluster number when partition based on each vp, SVF ^ i.  */

	int clusterNumber = 1; 

	/* number of final cluster in each of current cluster.  */

	int clusterCardinality = fanout;



	/*

	*  offset of the first point in current cluster in wrapper, the

	*  whole data list

	*  this array has an additional element at the end of value size,

	*  serving as a loop guard

	*  the first element of this array is always 0

	*/

	vector<int> clusterOffset(2);

	clusterOffset[0] = first;

	clusterOffset[1] = first + size;



	/* compute distance to the current VP */

	for(int i=0;i<numPivots;i++)

	{



		/* sort each part.  */

		for(int j=0;j<clusterNumber;j++)

		{

			sortVector(data, pivots[i], metric, clusterOffset[j], clusterOffset[j+1]);

		}



		//wrapper.clear();

		vector<CDoubleIndexObjectPair>().swap(wrapper);



		for(int p = first; p < first + size; ++p)

		{

			CDoubleIndexObjectPair temp(metric->getDistance(pivots[i].get(), data.at(p).get()), data.at(p));

			wrapper.push_back(temp);

		}



		const int nextClusterNumber = clusterNumber * maxPartitions;

		vector<int> nextClusterOffset(nextClusterNumber + 1);

		nextClusterOffset[0] = first;

		nextClusterOffset[nextClusterNumber] = first + size;



		int nextClusterCardinality = clusterCardinality / maxPartitions;



		/* split each current cluster into SVF sub-clusters based on the distance to current VP.  */

		for(int j=0;j<clusterNumber; j++)

		{

			/* size of current cluster (number of points).  */

			const int clusterSize = clusterOffset[j + 1] - clusterOffset[j];



			/* if this cluster is empty, set all its sub-cluster to be empty.  */

			if(clusterSize == 0)

			{

				for(int k=0;k<maxPartitions;k++)

				{

					nextClusterOffset[j * maxPartitions + k + 1] = clusterOffset[j + 1];

				}



				/* jump to next cluster.  */

				continue;

			}



			/* find the last indices of each distinct distance value in wrapper, which is already sorted.  */

			vector<int> tempIndex;

			vector<double> tempValue;



			/* the distinct disatnce value in check, and the number of points with this distance.  */

			double currentDistance = wrapper.at(clusterOffset[j] - first).getDouble();



			int sum = first;



			for(int k = clusterOffset[j] - first; k < clusterOffset[j + 1] - first;k++)

			{

				const double nextDistance = wrapper[k].getDouble();

				if(nextDistance != currentDistance)

				{

					/* find next  */

					tempIndex.push_back(sum);

					tempValue.push_back(currentDistance);

					currentDistance = nextDistance;

				}

				sum++;

			}

			/* put the last distinct value into the list.  */

			tempIndex.push_back(sum);

			tempValue.push_back(currentDistance);



			const int distinctSize = tempIndex.size();



			/* index of first point with current distinct distance value, this is the offset in current cluster, not the index in wrapper.

			*  distinct distance values

			*/



			int* firstPointWithDistinctDistance = new int[distinctSize + 1];

			double* distinctDistance = new double[distinctSize];

			firstPointWithDistinctDistance[0] = first;

			firstPointWithDistinctDistance[distinctSize] = clusterSize + first;

			distinctDistance[0] = wrapper[clusterOffset[j] - first].getDouble();



			for(int k=1; k<distinctSize;k++)

			{

				firstPointWithDistinctDistance[k] = (int) tempIndex.at(k - 1);

				distinctDistance[k] = (double) tempValue.at(k);

			}



			/* assign the total distinctSize set of points with

			* identical distance value

			* to at most SVF sub-clusters, which is actually split

			* current cluster

			*/



			/* number of distinct set that are already been assigned  */

			int startingDistinctSet = 0;                                                 



			/* if distince set number is greater than SVF, assign them,

			* otherwise,

			* just assign one set to each sub-cluster, remain

			* sub-clusters are all empty

			*/

			int k = 0;

			/*  k is the current sub-cluster to assign distinct set to . */

			

			while((k < maxPartitions - 1) && (distinctSize - startingDistinctSet > maxPartitions - k))

			{

				/* assign sets based on their cardinality, prefer balance sub-cluster  */

				const int median = (clusterSize+first - firstPointWithDistinctDistance[startingDistinctSet]) / (maxPartitions - k);



				/* find the distince set that contains the median point.  */

				int t = startingDistinctSet;

				while( firstPointWithDistinctDistance[t + 1]< median + firstPointWithDistinctDistance[startingDistinctSet])

				{

					t++;

				}



				/* if median falls in the first distinct set, assign this set to current cluster. */



				if(t != startingDistinctSet){

					t = (firstPointWithDistinctDistance[t + 1] - median - firstPointWithDistinctDistance[startingDistinctSet] >= median + firstPointWithDistinctDistance[startingDistinctSet] - firstPointWithDistinctDistance[t]) ? t - 1 : t;

				}



				/* now startingDistinctSet is the index of the first distinct set, and t is the index of the last distinct set, to be assinged to current sub-cluster set the sub-cluster offset, lower, upper bound  */

				nextClusterOffset[j * maxPartitions + k + 1] = clusterOffset[j] + firstPointWithDistinctDistance[t + 1]-first;



				const int firstChild = j * clusterCardinality + k * nextClusterCardinality;



				for(int temp=firstChild;temp<firstChild + nextClusterCardinality;temp++)

				{

					lower[i][temp] = distinctDistance[startingDistinctSet];

					upper[i][temp] = distinctDistance[t];

				}



				startingDistinctSet = t + 1;

				k++;

			}

			/* if reaches the last sub-cluster, assign all remain set to it.  */

			if(k == maxPartitions - 1)

			{

				/* set the sub-cluster offset, lower, upper bound.  */

				nextClusterOffset[j * maxPartitions + k + 1] = clusterOffset[j + 1];



				const int firstChild = j * clusterCardinality + k * nextClusterCardinality;

				for(int temp = firstChild;temp<firstChild+nextClusterCardinality;temp++)

				{

					lower[i][temp] = distinctDistance[startingDistinctSet];

					upper[i][temp] = distinctDistance[distinctSize - 1];

				}

			}

			else

			{

				/* remain set number is not greater than remain sub-cluster number, assign one set to each sub-cluster  */

				for(int t=startingDistinctSet; t<distinctSize;t++)

				{

					nextClusterOffset[j * maxPartitions + k + 1] = clusterOffset[j] + firstPointWithDistinctDistance[t + 1]-first;

					const int firstChild = j * clusterCardinality + k * nextClusterCardinality;

					for(int temp=firstChild; temp<firstChild + nextClusterCardinality; temp++)

					{

						lower[i][temp] = distinctDistance[t];

						upper[i][temp] = distinctDistance[t];

					}

					k++;

				}



				if (k<maxPartitions)

				{

					/* if there are still sub-cluster, set them to be null  */

					for(;k<maxPartitions;k++)

					{

						nextClusterOffset[j * maxPartitions + k + 1] = clusterOffset[j + 1];

					}

				}

			}

			delete[] firstPointWithDistinctDistance;

			delete[] distinctDistance;

		} /*  end of a loop for each cluster  */



		clusterOffset.resize(nextClusterOffset.size());



		for(int i=0; i<nextClusterNumber + 1;i++)

		{

			clusterOffset[i] = nextClusterOffset[i];

		}



		clusterCardinality = nextClusterCardinality;

		clusterNumber = nextClusterNumber;



	} /* end of loop for each vantage point  */



	/*  compute non-empty cluster number.  */



	int childrenNumber = 0;

	for(int i=0;i<fanout;i++)

	{

		if(clusterOffset[i] < clusterOffset[i + 1])

		{

			childrenNumber++;

		}

	}



	/* if there are some empty cluster, delete them.  */

	if(childrenNumber < fanout)

	{

		vector<vector<double> > newLower(numPivots);

		vector<vector<double> > newUpper(numPivots);



		vector<int> newOffset(childrenNumber + 1);

		newOffset[childrenNumber] = size+first;



		for(int vectorIndex=0;vectorIndex<numPivots;++vectorIndex)

		{

			int j = 0;

			newLower[vectorIndex].resize(childrenNumber);

			newUpper[vectorIndex].resize(childrenNumber);



			for(int i=0;i<fanout; i++)

			{

				if(clusterOffset[i] < clusterOffset[i + 1])

				{

					newLower[vectorIndex][j] = lower[vectorIndex][i];

					newUpper[vectorIndex][j] = upper[vectorIndex][i];

					newOffset[j] = clusterOffset[i];

					j++;

				}

			}

			/*lower[vectorIndex] = newLower[vectorIndex];

			upper[vectorIndex] = newUpper[vectorIndex];

			clusterOffset = newOffset;*/

		}

		

		lower = newLower;

		upper = newUpper;

		clusterOffset = newOffset;

		

		//clusterOffset = newOffset;

	}

	int i=0,j=0;

	vector<int> nOffSet;

	/*for(i=0;i<clusterOffset.size()-1;i++)

	{		

		if(clusterOffset[i]<clusterOffset[i+1])

			nOffSet.push_back(clusterOffset.at(i));

		else

		{   

			break;

		}

	}

	nOffSet.push_back(clusterOffset.at(i));*/

	for(int i=0;i<=childrenNumber;i++)

	{

		nOffSet.push_back(clusterOffset.at(i));

	}

	



	CPartitionResults *result = new CPartitionResults(nOffSet, lower, upper);

	return *result;



}

