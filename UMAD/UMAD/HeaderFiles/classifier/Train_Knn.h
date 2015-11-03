#ifndef TRAIN_KNN_H
#define TRAIN_KNN_H

#include"../../HeaderFiles/classifier/Train.h"
#include"../../HeaderFiles/classifier/util/MetricDataFormat.h"
#include"../../HeaderFiles/classifier/util/DatasetInMetricSpace.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <memory>

using namespace std;

/**
 *@ file  Train_Knn.h
 *@ classification algorithm
 *@ author  from network
 *@ version  2014-08-01
 */

/**
 *@ class CTrain_Knn
 *@ classification algorithm
 *@ author from network
 */

class CTrain_Knn:public CTrain
{
private:
struct dataVector 
{
	int ID;                      //ID number
	string classLabel;             //class label
	vector<double> attributes; //attribute
};
struct distanceStruct 
{
	int ID;                      //ID number
	double distance;             //distance
	string classLabel;             //class label
};                         

public:
	CTrain_Knn();
	~CTrain_Knn();

/**
 *@show classification result
 *@infilename: the training set under metric space that after pivot selection and distance calculation
 *@outfilename: outfilename is going to store the classify result
 *@ param pivotNum: the number of pivots
 */
	 void showClassifierModel(GetMetricData M_traindata,const char* outfilename, int pivotNum);

/**
 *@ get training model of classification algorithm.
 *@ param classifyMethod  classifymethod: "knn" , "naviebayes".
 *@ param traindata: the primary trainging data.
 *@ param trainDataLabel: the class label of primary training data.
 *@ param metric: distance function.
 *@ param pivotSelectionMethod: the pivot selection method: "random", "fft", "center", "pcaonfft", "pca","incremental".
 *@ param pivotNum: the number of pivots
 *@ param pivotsAndTrainModelFileName: the file to store the selected pivots information and training model.
 *@ param dim  dimension of vector data to load or number of features to be loaded.
*/
	 void TrainModel(char *classifyMethod,vector<shared_ptr<CMetricData> > *traindata,vector<string> trainDataLabel, CMetricDistance *metric,CPivotSelectionMethod *pivotselectionmethod, int pivotNum,char *pivotsAndTrainModelFileName,int dim);
};

#endif