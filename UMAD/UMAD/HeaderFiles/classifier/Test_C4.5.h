#ifndef _TEST_C45_
#define _TEST_C45_

#include <iostream>
#include <string>
#include <memory>
#include <cmath>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <malloc.h>
#include <cstdlib>

#include"Train_C4.5.h"
#include"../../HeaderFiles/classifier/Test.h"
#include"../../HeaderFiles/classifier/util/MetricDataFormat.h"
#include"../../HeaderFiles/classifier/util/C4.5.h"
#include <algorithm>

using namespace std;

class CTest_C4_5: public CTest
{
public:
	CTest_C4_5();
	~CTest_C4_5();

	/*************************************************************************/
	/*									 */
	/*	Count the nodes in a tree					 */
	/*									 */
	/*************************************************************************/	
	int TreeSize(Tree Node);

	/*************************************************************************/
	/*                                                              	 */
	/*  Classify a case description using the given subtree by adjusting	 */
	/*  the value ClassSum for each class					 */
	/*                                                              	 */
	/*************************************************************************/
	void Classify(Descript CaseDesc,Tree T,float Weight);

	/*************************************************************************/
	/*                                                              	 */
	/*  Categorize a case description using the given decision tree		 */
	/*                                                              	 */
	/*************************************************************************/
	short Category(Descript CaseDesc,Tree DecisionTree,short VERBOSITY);

	void PrintConfusionMatrix(ItemNo *ConfusionMat);

	/*************************************************************************/
	/*									 */
	/*	Print report of errors for each of the trials			 */
	/*									 */
	/*************************************************************************/
	void Evaluate(Boolean CMInfo,short Saved,short TRIALS,short VERBOSITY,GetMetricData data,char *pivotsAndTrainModelFileName);


	/*************************************************************************/
	/*									 */
	/*	Allocate space then copy string into it				 */
	/*									 */
	/*************************************************************************/
	String CopyString(String x);


	/*************************************************************************/
	/*									 */
	/*  Read a raw case description from file Df.				 */
	/*									 */
	/*  For each attribute, read the attribute value from the file.		 */
	/*  If it is a discrete valued attribute, find the associated no.	 */
	/*  of this attribute value (if the value is unknown this is 0).	 */
	/*									 */
	/*  Returns the Description of the case (i.e. the array of		 */
	/*  attribute values).							 */
	/*									 */
	/*************************************************************************/
	Descript GetDescription(int itemNo,GetMetricData Df);

	/*************************************************************************/
	/*									 */
	/*  Read raw case descriptions from M_traindata.		 */
	/*									 */
	/*  On completion, cases are stored in array Item in the form		 */
	/*  of Descriptions (i.e. arrays of attribute values), and		 */
	/*  MaxItem is set to the number of data items.				 */
	/*									 */
	/*************************************************************************/
	void GetData(GetMetricData data);

	Tree LoadTree(ifstream &infile);

	void GetTree(char *pivotsAndTrainModelFileName);

	/**
	 *@ test training model of classification algorithm.
	 *@ param classifyMethod  classifymethod: "knn" , "naviebayes".
	 *@ param testdata: the primary trainging data.
	 *@ param testDataLabel: the class label of primary training data.
	 *@ param metric: distance function.
	 *@ param pivotsAndTrainModelFileName: the file to store the selected pivots information and training model.
	 *@ param testModelFileName: the file to store the training model.
	 *@ param status: determine the source of the test data. 0: TestDataFromTrainData; 1: TestDataFromTestData.
	*/
	 void TestModel(char *classifyMethod,vector<shared_ptr<CMetricData> > *testdata,vector<string> testDataLabel, CMetricDistance *metric,char *pivotsAndTrainModelFileName,char *testModelFileName,int status,int splitRatio);

	 ItemNo	MaxItem;	/* max data item number */

	 ItemNo	MaxTestItem;


private:
	GetMetricData TestData;
	short	MaxAtt,		/* max att number */
			MaxClass;	/* max class number */
	Descript	*Item;		/* data items */

	String		*ClassName,	/* class names */
		  		*AttName,	/* att names */
		  		**AttValName;	/* att value names */

	float	*Weight;	/* Weight[i]  = current fraction of item i */
	float
			**Freq,		/* Freq[x][c] = no. items of class c with outcome x */
			*ValFreq,	/* ValFreq[x] = no. items with att value v */
			*ClassFreq;	/* ClassFreq[c] = no. items of class c */

	float
			*Gain,		/* Gain[a] = info gain by split on att a */
			*Info,		/* Info[a] = potential info from split on att a */
			*Bar,		/* Bar[a]  = best threshold for contin att a */
			*UnknownRate;	/* UnknownRate[a] = current unknown rate for att a */

	float
			*SplitGain,	/* SplitGain[i] = gain with att value of item i as threshold */
			*SplitInfo;	/* SplitInfo[i] = potential info ditto */

	float
			*Slice1,	/* Slice1[c]    = saved values of Freq[x][c] in subset.c */
			*Slice2;	/* Slice2[c]    = saved values of Freq[y][c] */

	Set
			**Subset;	/* Subset[a][s] = subset s for att a */

	short
			*Subsets;	/* Subsets[a] = no. subsets for att a */
	short   Best;
	Tree    *Raw;
	Tree    *Pruned;
	float	*ClassSum;		/* ClassSum[c] = total weight of class c */
	short	VERBOSITY,	/*v: verbosity level (0 = none) */
			TRIALS;		/*t: number of trees to be grown */
	DiscrValue	*MaxAttVal;	/* number of values for each att */
};

#endif
