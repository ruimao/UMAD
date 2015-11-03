#ifndef DATASETINMETRICSPACE_H
#define DATASETINMETRICSPACE_H

#include"../../../HeaderFiles/index/PivotSelectionMethod.h"
#include"../../../HeaderFiles/index/FFTPivotSelectionMethod.h"
#include"../../../HeaderFiles/metricdistance/MetricDistance.h"
#include "../../../HeaderFiles/metricdata/DoubleVector.h"
#include"../../classifier/util/MetricDataFormat.h"

#include "../../metricdistance/DNAMetric.h"
#include "../../metricdistance/EditDistance.h"
#include "../../metricdistance/EuclideanDistance.h"
#include "../../metricdistance/ImageMetric.h"
#include "../../metricdistance/PeptideMetric.h"
#include "../../metricdistance/CountedMetric.h"
#include "../../metricdistance/LInfinityDistance.h"

#include <fstream>
#include <string>
#include<memory>
#include<vector>
#include<sstream>
/** 
 *@ file  DatasetInMetricSpace.h
 *@ describe the training dataset in metric space
 *@ author  Ping Li
 *@ version  2014-06-13
 */

/**
 *@ class  CDatasetInMetricSpace
 *@ brief  calculate the distance between samples and pivots 
 *@ author  Ping Li
*/

class CDatasetInMetricSpace
{
public:

	/**@brief none parameter constructor*/
	CDatasetInMetricSpace();

	/**@brief destructor*/
	~CDatasetInMetricSpace();

	/**
	 *@ brief calculate the distance between training samples and pivots, return the train data in metric space.
	 *@ param classifyMethod  classifymethod: "knn" , "naviebayes" , "c4.5".
	 *@ param traindata: traindata is the primary dataset
	 *@ param trainDataLabel: the class label of each training data
	 *@ param pivotSelectionMethod: the pivot selection method: "random", "fft", "center", "pcaonfft", "pca","incremental".
	 *@ param metric: the distance function: "EuclideanDistance" , "EditDistance" , "LInfinityDistance".
     *@ param pivotNum: the number of pivots
	 *@ param pivotsFileName: the file to store the selected pivots information.
	 *@ param dim  dimension of vector data to load or number of features to be loaded.
	 *@ return: return an vector contains the distance between each training sample and pivots 
	 */
	 GetMetricData getMetricTrainData(char *classifyMethod,vector<shared_ptr<CMetricData> > *traindata,vector<string> trainDataLabel,CMetricDistance *metric,CPivotSelectionMethod *pivotSelectionMethod,int pivotNum,char *pivotsFileName,int dim);

	 /**
	 *@ select 1/10 data from train data to be the test data,return the test data in metric space
	 *@ param classifyMethod  classifymethod: "knn" , "naviebayes" , "c4.5".
	 *@ param testdata: testdata is the primary dataset
	 *@ param testDataLabel: the class label of each test data
	 *@ param metric: the distance function: "EuclideanDistance" , "EditDistance" , "LInfinityDistance".
	 *@ param pivotsFileName: the file to store the selected pivots information.
	 *@ param splitRatio: decide how much data from training set as a test set.
	 *@ return: return an vector contains the distance between each test sample and pivots 
	 */
	 GetMetricData getMetricTestData_fromTrainData(char *classifyMethod,vector<shared_ptr<CMetricData> > *testdata,vector<string> testDataLabel,CMetricDistance *metric,char *pivotsFileName,int splitRatio);

	/**
	 *@ brief calculate the distance between test samples and pivots, return the test data in metric space.
	 *@ param classifyMethod  classifymethod: "knn" , "naviebayes" , "c4.5".
	 *@ param testdata: testdata is the primary dataset
	 *@ param testDataLabel: the class label of each test data
	 *@ param metric: the distance function: "EuclideanDistance" , "EditDistance" , "LInfinityDistance".
	 *@ param pivotsFileName: the file to store the selected pivots information.
	 *@ return: return an vector contains the distance between each test sample and pivots 
	 */
	 GetMetricData getMetricTestData_fromTestData(char *classifyMethod,vector<shared_ptr<CMetricData> > *testdata,vector<string> testDataLabel,CMetricDistance *metric,char *pivotsFileName); 

	
	int pivotsNum;
	int attNum;
};

#endif