#ifndef UTILFUNCTION_H
#define UTILFUNCTION_H
#include <vector>
#include <memory>

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


class utilFunction
{
public:
	/**@brief	For the first big k value in the array With the method of recursive
	*@param	task_distance	Distance array
	*@param	low		the low bound of the array
	*@param	high	the high bound of the array
	*@param	k	The first big K value
	*@return	the first big k value in the task_distance
	*/
	static double select_max_k(shared_ptr<vector<double> >& task_distance, int low, int high, int k);  
	/**@brief	For the max value in the array
	*@param	task_distance	Distance array
	*@return	the max value in the task_distance
	*/
	static double find_max(vector<double>& task_distance);
	/**@brief	For the min value in the array
	*@param	task_distance	Distance array
	*@return	the min value in the task_distance
	*/
	static double find_min(vector<double>& task_distance);
	/**@brief	For the first small k value in the array
	*@param	task_distance	Distance array
	*@param	low		the low bound of the array
	*@param	high	the high bound of the array
	*@param	k	The first small K value
	*@return	the first small k value in the task_distance
	*/
	static double select_min_k(shared_ptr<vector<double> > task_distance, int low, int high, int k);
	/**@brief	For the first big k value in the array With the method of iteration
	*@param	task_distance	Distance array
	*@param	task_distance_size		the size of the task_distance
	*@param	k	The first big K value
	*@return	the first big k value in the task_distance
	*/
	static double select_max_k_iteration(shared_ptr<vector<double> >& task_distance, int task_distance_size,int k);  
	/**@brief	For the first small k value in the array With the method of iteration
	*@param	task_distance	Distance array
	*@param	task_distance_size		the size of the task_distance
	*@param	k	The first small K value
	*@return	the first small k value in the task_distance
	*/
	static double select_min_k_iteration(shared_ptr<vector<double> >& task_distance, int task_distance_size,int k);  

	/**@brief	Looking for an array of k value
	*@param	task_distance	Distance array
	*@param	low		the low bound of the array
	*@param	high	the high bound of the array
	*@param	k	The array is divided into several parts
	*@param	kth_quantiles	A value k points in the array
	*/
	static void kthQuantiles(shared_ptr<vector<double> > task_distance, int low, int high, int k,shared_ptr<vector<double> > kth_quantiles);
};



#endif