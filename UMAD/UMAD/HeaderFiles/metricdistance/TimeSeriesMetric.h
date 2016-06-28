#ifndef TIME_SERIES_METRIC
#define TIME_SERIES_METRIC

/**@file TimeSeriesMetric.h

 * @brief A metric to calculate the distance of two time series object,

 * @author Ping Li

 * @date 2015-5-10

*/

#include <iostream>

#include <vector>

#include <string>

#include <math.h>

#include "MetricDistance.h"

#include "../metricdata/TimeSeries.h"

using namespace std;

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))
#define INFINITY 100000000.0
#define MAXNUM 2000
#define LCSSorEDR  0.001      //Threshold parameter of  Longest Common SubSequence an Edit Distance on Real Sequence
#define G 0    //Threshold parameter of Edit Distance with Real Penalty

class CTimeSeriesMetric: public CMetricDistance
{
public:

	 /**

     * @brief A no parameter constructor, do nothing.

     * @return void

     */
    CTimeSeriesMetric();


    /** 

     * @brief A destructor, do nothing

     * @return void

     */
    ~CTimeSeriesMetric();



	/**
	 
	 *brief This method return two trajectory segments distance

	*/
	double TraSegmentDistance(vector<TimeSeries> one,vector<TimeSeries> two);



    /**

     * @brief This method return two IndexObjects' distance.

     * @return  Return a double type distance of two objects.

     */
	virtual double getDistance(CMetricData* one, CMetricData* two);


    /**

     * @brief  This method return two time series' distance.

     *          Sum up edit distance of two time series.

     * @return   Return a double type of distance of two objects.

     */
	virtual double getDistance(CTimeSeries* one, CTimeSeries* two);


	/**
	 
	 *brief This method return two time series's DTW distance

	*/
	double DTWDistance(vector<TimeSeries> one, vector<TimeSeries> two);


	/**

	 *brief This method return absolute distance between two point

	*/
	double DTWDist(TimeSeries t1, TimeSeries t2);


	/**
	 
	 *brief This method return two time series's LCSS(Longest Common SubSequence) distance

	*/
	double LCSSDistance(vector<TimeSeries> one, vector<TimeSeries> two);


	/**
	
	 *brief This method return the minmum of three parameter
	
	*/


	/**
	 
	 *brief This method return two time series's EDR(Edit Distance on Real Sequence) distance

	*/
	double EDRDistance(vector<TimeSeries> one, vector<TimeSeries> two);

	/**

	 *brief This method return distance between two point

	*/
	double EDRDist(TimeSeries t1, TimeSeries t2);


	/**
	 
	 *brief This method return two time series's ERP(Edit Distance with Real Penalty) distance

	*/
	double ERPDistance(vector<TimeSeries> one, vector<TimeSeries> two);

	/**

	 *brief This method return distance between two point

	*/
	double ERPDist(TimeSeries t1, TimeSeries t2);


	/**
	 
	 *brief This method return two time series's Euclidean Distance

	*/
	double EuclideanDistance(vector<TimeSeries> one, vector<TimeSeries> two);


	double Minmum(double x, double y, double z);


private:

	vector<TimeSeries> onedata,twodata;

	int onelen,twolen,onecoord,twocoord;    //onelen and twolen represent dim. onecoord and twocoord represent coordinate.
};

#endif