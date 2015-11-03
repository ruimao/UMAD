#include "../../../HeaderFiles/cluster/util/utilFunction.h"



#include <memory>


#define INT_MAX ((int)(~0U>>1))
#define INT_MIN (-INT_MAX - 1)

#include<cstdlib>
#include <algorithm>
using namespace std;

/** @file	utilFunction.h
* @classes	Contains common function 
* @author	He Zhang
* @version	2014-07-23
*/

/**
* @class	utilFunction
* @brief	Contains common function,For other function calls
* @author	He Zhang
*/

/**@brief	For the first big k value in the array
*@param	task_distance	Distance array
*@param	low		the low bound of the array
*@param	high	the high bound of the array
*@param	k	The first big K value
*@return	the first big k value in the task_distance
*/
double utilFunction::select_max_k(shared_ptr<vector<double> >& task_distance, int low, int high, int k)  
{  
	/** Randomly choose a piovt*/
	int pivot = low + rand()%(high - low + 1);     
	swap(task_distance->at(low), task_distance->at(pivot));  
	int m = low;  
	int count = 1;  

	/** A trip to traverse, the larger the number on the left of the array*/
	for(int i = low + 1; i <= high; ++i)  
	{  
		if(task_distance->at(i) > task_distance->at(low))   
		{  
			swap(task_distance->at(++m), task_distance->at(i));  
			/** The number of number bigger than a fulcrum for the count - 1*/
			count++;  
		}  
	} 
	/** The fulcrum in the parting of the left and right two parts*/
	swap(task_distance->at(m), task_distance->at(low));
	if(count > k)  
	{  
		return select_max_k(task_distance, low, m - 1, k);  
	}  
	else if( count < k)  
	{  
		return select_max_k(task_distance, m + 1, high, k - count);  
	}  
	else  
	{
		return task_distance->at(m);  
	}  
}  

/**@brief	For the max value in the array
*@param	task_distance	Distance array
*@return	the max value in the task_distance
*/
double utilFunction::find_max(vector<double>& task_distance)

{
	double distance_max = INT_MIN;
	for (int i=0; i <task_distance.size(); ++i )
	{
		if (distance_max < task_distance[i])
			distance_max = task_distance[i];
	}
	return distance_max;
}

/**@brief	For the min value in the array
*@param	task_distance	Distance array
*@return	the min value in the task_distance
*/
double utilFunction::find_min(vector<double>& task_distance)
{
	double distance_min = INT_MAX;
	for (int i=0; i <task_distance.size(); ++i )
	{
		if (distance_min > task_distance[i])
			distance_min = task_distance[i];
	}
	return distance_min;
}

/**@brief	For the first small k value in the array
*@param	task_distance	Distance array
*@param	low		the low bound of the array
*@param	high	the high bound of the array
*@param	k	The first small K value
*@return	the first small k value in the task_distance
*/
double utilFunction::select_min_k(shared_ptr<vector<double> > task_distance, int low, int high, int k)  
{  
	if(k <= 0)  
		return -1;  
	if(k > high - low + 1)  
		return -1;  
	/** Randomly choose a piovt*/
	int pivot = low + rand()%(high - low + 1);
	swap(task_distance->at(low), task_distance->at(pivot));  
	int m = low;  
	int count = 1;  

	/** A trip to traverse, put the smaller number to the left of the array*/
	for(int i = low + 1; i <= high; ++i)  
	{  
		if(task_distance->at(i)<task_distance->at(low))   
		{  
			swap(task_distance->at(++m), task_distance->at(i));  
			/** The number of several smaller than fulcrum for the count - 1*/
			count++;
		}  
	} 
	/** The pivot in the parting of the left and right two parts*/
	swap(task_distance->at(m), task_distance->at(low));
	if(k < count)  
	{  
		return select_min_k(task_distance, low, m - 1, k);  
	}  
	else if( k > count)  
	{  
		return select_min_k(task_distance, m + 1, high, k - count);  
	}  
	else  
	{  
		return task_distance->at(m);  
	}  
}

/**@brief	For the first big k value in the array With the method of iteration
*@param	task_distance	Distance array
*@param	task_distance_size		the size of the task_distance
*@param	k	The first big K value
*@return	the first big k value in the task_distance
*/
double utilFunction::select_max_k_iteration(shared_ptr<vector<double> >& task_distance, int task_distance_size,int k)  
{
	int l = 0;  
	int r = task_distance_size - 1;  
	while (l < r)   
	{  
		int i = l,      //From left to right the cursor
			j = r + 1;      //From right to left of the cursor
		double pivot = task_distance->at(l);  
		//The left > = the pivot element and right < = the pivot element
		while (true)  
		{  
			do  
			{  
				i = i + 1; //On the left side looking for > = the pivot element
			} while (i < task_distance_size && task_distance->at(i) > pivot);  

			do   
			{  
				j = j - 1; //On the right side looking for < = the pivot element
			} while (j < task_distance_size && task_distance->at(j) < pivot);  

			if (i >= j)   
				break;   

			swap(task_distance->at(i), task_distance->at(j));  
		}  
		if (j - l + 1 == k)   
			return pivot;  

		task_distance->at(l) = task_distance->at(j);  
		task_distance->at(j) = pivot;  
		if (j - l + 1 < k)   
		{  
			k -= j - l + 1;   
			l = j + 1;  
		}  
		else  
			r = j - 1;  
	}  

	return task_distance->at(l);  
}  

/**@brief	For the first small k value in the array With the method of iteration
*@param	task_distance	Distance array
*@param	task_distance_size		the size of the task_distance
*@param	k	The first small K value
*@return	the first small k value in the task_distance
*/
double utilFunction::select_min_k_iteration(shared_ptr<vector<double> >& task_distance, int task_distance_size,int k)  
{
	int l = 0;  
	int r = task_distance_size - 1;  
	while (l < r)   
	{  
		int i = l,      //From left to right the cursor
			j = r + 1;      //From right to left of the cursor
		double pivot = task_distance->at(l);  
		//The left > = the pivot element and right < = the pivot element
		while (true)  
		{  
			do  
			{  
				i = i + 1; //On the left side looking for > = the pivot element
			} while (i < task_distance_size && task_distance->at(i) < pivot);  

			do   
			{  
				j = j - 1; //On the right side looking for < = the pivot element
			} while (j < task_distance_size && task_distance->at(j) > pivot);  

			if (i >= j)   
				break;   

			swap(task_distance->at(i), task_distance->at(j));  
		}  
		if (j - l + 1 == k)   
			return pivot;  

		task_distance->at(l) = task_distance->at(j);  
		task_distance->at(j) = pivot;  
		if (j - l + 1 < k)   
		{  
			k -= j - l + 1;   
			l = j + 1;  
		}  
		else  
			r = j - 1;  
	}  

	return task_distance->at(l);  
}

int partitionBilaterally(shared_ptr<vector<double> > task_distance, int low, int high, int stride){
	int pivot = low;
	low += stride;
	double temp = task_distance->at(pivot);
	while(low <= high){
		while(task_distance->at(high) >= temp && low <= high)
			high -= stride;
		if(!(low > high)){
			task_distance->at(pivot) = task_distance->at(high);
			pivot = high;
			high -= stride;
		}
		while(task_distance->at(low) <= temp && low <= high)
			low += stride;
		if(!(low > high)){
			task_distance->at(pivot) = task_distance->at(low);
			pivot = low;
			low += stride;
		}
	}
	task_distance->at(pivot) = temp;
	return pivot;
}

int orderStatisticsItera(shared_ptr<vector<double> > task_distance, int low, int high, int order, int stride){
	int base = low;
	int p = partitionBilaterally(task_distance, low, high, stride), k;
	while((k = (p-base)/stride+1) != order){
		if(k < order)
			low = p + stride;
		else high = p - stride;
		p = partitionBilaterally(task_distance, low, high, stride);
	}
	return p;
}

/**@brief	Looking for an array of k value
*@param	task_distance	Distance array
*@param	low		the low bound of the array
*@param	high	the high bound of the array
*@param	k	The array is divided into several parts
*@param	kth_quantiles	A value k points in the array
*/
void utilFunction::kthQuantiles(shared_ptr<vector<double> > task_distance, int low, int high, int k,shared_ptr<vector<double> > kth_quantiles)
{
	if(k == 1)
		return;
	int size = high-low+1;
	int split = k/2;
	int lowerSize = (size/k)*split + (size%k<split ? size%k : split);
	orderStatisticsItera(task_distance, low, high, lowerSize, 1);
	kthQuantiles(task_distance, low, low+lowerSize-1, split,kth_quantiles);
	kth_quantiles->push_back(task_distance->at(low+lowerSize-1));
	kthQuantiles(task_distance, low+lowerSize, high, k-split,kth_quantiles);
}
