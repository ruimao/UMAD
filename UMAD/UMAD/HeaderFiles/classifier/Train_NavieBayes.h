#pragma once
#ifndef  _TRAIN_NAVIEBAYES_
#define  _TRAIN_NAVIEBAYES_
#pragma message("GetInfo program") 
#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <memory>
#include <sstream>
#include <algorithm>

#ifndef _ITERATOR_DEBUG_LEVEL
#define _ITERATOR_DEBUG_LEVEL 0
#else
#undef _ITERATOR_DEBUG_LEVEL
#define _ITERATOR_DEBUG_LEVEL 0
#endif

using namespace std;
#include<math.h>
#include"../../HeaderFiles/classifier/util/GetValue.h"
#include"../../HeaderFiles/classifier/util/MetricDataFormat.h"
#include"../../HeaderFiles/classifier/Train.h"
#include"../../HeaderFiles/classifier/util/DatasetInMetricSpace.h"
#include "../../HeaderFiles/index/PivotSelectionMethod.h"
#define	 Log(x)	 ((x) <= 0 ? 0.0 : log((float)x) / log(2.0))


class CTrain_NavieBayes:public CTrain
{
public:
	CTrain_NavieBayes();
	~CTrain_NavieBayes();
	void getname(string names);
	void getdata(GetMetricData data);
	double SplitContinuousAtt(int AttNo);//continuous feature discretization
	void  Error(int n, string s1, string s2);

	/*use to store the attributes and class information of all variables in .names file */
	string *ClassName;//name of class
	string *AttName;//name of attribute
	string **AttValName;//name of attribute value
	short *MaxAttValNo;//the number of categorical attribute.  It starts at 1 deposit£¬0 position to deal with illegal in attribute values.If it's 0£¬said the property for continuous attributes, need discretization.
	char  *SpecialStatus;
	short MaxClassNo;//the number of classes starting from 0, so the total number is MaxClassNo+1  
	short MaxAttNo;//the number of attributes starting from 0, so the total number is MaxAttNo+1
	short MaxDiscrValNo;//the maximum number of all discrete attributes value

    /* use to store all variables in .data file*/
    int MaxItemNo;//number of training set
	Description	*Item;
	double
	*Weight,	/* Weight[i]  = current fraction of item i */
	**Freq,		/* Freq[x][c] = no. items of class c with outcome x  x is attribute value£¬c is class value*/
	*ValFreq,	/* ValFreq[x]   = no. items with outcome x */
	*ClassFreq;	/* ClassFreq[c] = no. items of class c */
	int MaxContAttNo;//the number of continuous attributes

	/* Get model*/
	void trainmodel(const char *showClassificationModel);
	double ***TrainingModel;  //use to stroe the training model
	double *TrainingModelSplitPoint;   //use to store continuous attribute discrete points value

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
	 void TrainModel(char *classifyMethod,vector<shared_ptr<CMetricData> > *traindata,vector<string> trainDataLabel, CMetricDistance *metric,CPivotSelectionMethod *pivotselectionmethod, int pivotNum,char *pivotsAndTrainModelFileName,char *trainDataFileName,int dim);

private:
	FILE *Nf;
	GetMetricData TrainData;
	string Names;
	bool ReadName(FILE *f,char *Buffer);
	char	Delimiter;
	int Inc;
	Description GetDescription(int itemNo,GetMetricData Df);
	void AnalyzeData();//Statistics of attribute value or class in number
	void InitialiseData();//allocate space for the data
	void InitialiseWeights();//initialize each Items weight
	void ResetFreq(double MaxDiscrValNo);//initialize frequency£¨0£©
	void Quicksort(int ItemNoFp,int ItemNoLp ,int AttNo);//quicksort for continuous attributes
	void SwapItem(int ItemA,int ItemB);
	float Epsilon ;
	int MINOBJS;
	int None;
	double
	*SplitGain,	/* SplitGain[i] = gain with att value of item i as threshold */
	*SplitInfo;	/* SplitInfo[i] = potential info ditto */
	double TotalInfo(double V[],int MinVal,int MaxVal);
	double ComputeGain(double BaseInfo,int MaxValNo,double TotalItems);
	double
	*Gain,		/* Gain[a] = info gain by split on att a */
	//*Info,		/* Info[a] = potential info of split on att a */
	*Bar;		/* Bar[a]  = best threshold for contin att a */
	void Cache();//Allocate memory for the model
};
#endif
