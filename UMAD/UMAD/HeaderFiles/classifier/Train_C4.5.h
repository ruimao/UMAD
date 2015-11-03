#ifndef _TRAIN_C45_
#define _TRAIN_C45_

/**
  * file Train_C4.5
  * build decision tree
  * author PingLi
  * version 2014-10-31
  */

#include <iostream>
#include <cstring>
#include <vector>
#include <memory>
#include <cmath>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <malloc.h>
#include <cstdlib>

#include"../../HeaderFiles/classifier/Train.h"
#include"../../HeaderFiles/classifier/util/MetricDataFormat.h"
#include"../../HeaderFiles/classifier/util/DatasetInMetricSpace.h"
#include"../../HeaderFiles/index/PivotSelectionMethod.h"
#include"../../HeaderFiles/classifier/util/C4.5.h"

using namespace std;

class C4_5:public CTrain
{
public:

	C4_5();

	~C4_5();

	/* *************************************************************************** Initialise *********************************************************************** */

	/*************************************************************************/
	/*									 */
	/*  Read a name from file f into string s, setting Delimiter.		 */
	/*									 */
	/*  - Embedded periods are permitted, but periods followed by space	 */
	/*    characters act as delimiters.					 */
	/*  - Embedded spaces are permitted, but multiple spaces are replaced	 */
	/*    by a single space.						 */
	/*  - Any character can be escaped by '\'.				 */
	/*  - The remainder of a line following '|' is ignored.			 */
	/*									 */
	/*************************************************************************/
	Boolean ReadName(FILE *f, String s);

	/*************************************************************************/
	/*									 */
	/*	Locate value Val in List[First] to List[Last]			 */
	/*									 */
	/*************************************************************************/
	int Which(String Val,String List[],short First,short Last);

	/*************************************************************************/
	/*									 */
	/*	Allocate space then copy string into it				 */
	/*									 */
	/*************************************************************************/
	String CopyString(String x);

	/*************************************************************************/
	/*									 */
	/*			Error messages					 */
	/*									 */
	/*************************************************************************/
	 void Error(short n,String s1,String s2);

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
	/*  Read the names of classes, attributes and legal attribute values.	 */
	/*  On completion, these names are stored in:				 */
	/*	ClassName	-  class names					 */
	/*	AttName		-  attribute names				 */
	/*	AttValName	-  attribute value names			 */
	/*  with:								 */
	/*	MaxAttVal	-  number of values for each attribute		 */
	/*									 */
	/*  Other global variables set are:					 */
	/*	MaxAtt		-  maximum attribute number			 */
	/*	MaxClass	-  maximum class number				 */
	/*	MaxDiscrVal	-  maximum discrete values for any attribute	 */
	/*									 */
	/*  Note:  until the number of attributes is known, the name		 */
	/*	   information is assembled in local arrays			 */
	/*									 */
	/*************************************************************************/
	void GetNames(string FileName);

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


	/* **************************************************************************** Build decision trees ***************************************************************** */
	/*************************************************************************/
	/*								 	 */
	/*		Allocate space for tree tables			 	 */
	/*								 	 */
	/*************************************************************************/
	void InitialiseTreeData(char SUBSET);

	/*************************************************************************/
	/*								 	 */
	/*		Initialise the weight of each item		 	 */
	/*								 	 */
	/*************************************************************************/
	void InitialiseWeights();

	/*************************************************************************/
	/*								 	 */
	/*	Return the total weight of items from Fp to Lp		 	 */
	/*								 	 */
	/*************************************************************************/
	ItemCount CountItems(ItemNo Fp,ItemNo Lp);

	/*************************************************************************/
	/*                                                               	 */
	/*		Exchange items at a and b			 	 */
	/*									 */
	/*************************************************************************/
	void Swap(ItemNo a,ItemNo b);

	/*************************************************************************/
	/*									 */
	/*	Construct a leaf in a given node				 */
	/*									 */
	/*************************************************************************/
	Tree Leaf(ItemCount *ClassFreq,ClassNo NodeClass,ItemCount Cases,ItemCount Errors);

	/*************************************************************************/
	/*									 */
	/*  Combine the distribution figures of discrete attribute values	 */
	/*  x and y, putting the combined figures in Freq[x][] and		 */
	/*  ValFreq[x][], and saving old values in Slice1 and Slice2		 */
	/*									 */
	/*************************************************************************/
	void Combine(DiscrValue x,DiscrValue y,DiscrValue Last);
	
	/*************************************************************************/
	/*									 */
	/*  Restore old class distribution figures of discrete attribute	 */
	/*  values x and y from Slice1 and Slice2				 */
	/*									 */
	/*************************************************************************/
	void Uncombine(DiscrValue x,DiscrValue y);

	/*************************************************************************/
	/*									 */
	/*  Print the values of attribute Att which are in the subset Ss	 */
	/*									 */
	/*************************************************************************/
	void PrintSubset(Attribute Att,Set Ss);

	/*************************************************************************/
	/*									 */
	/*	Insert branches in a node 	                 		 */
	/*									 */
	/*************************************************************************/
	void Sprout(Tree Node,DiscrValue Branches);

	/*************************************************************************/
	/*									 */
	/*  Construct and return a node for a test on a subset of values	 */
	/*									 */
	/*************************************************************************/
	void SubsetTest(Tree Node,Attribute Att);

	/*************************************************************************/
	/*									 */
	/*  Return the base info for items with known values of a discrete	 */
	/*  attribute, using the frequency table Freq[][]			 */
	/*	 								 */
	/*************************************************************************/
	float DiscrKnownBaseInfo(DiscrValue KnownItems,ItemCount MaxVal);

	/*************************************************************************/
	/*									 */
	/*  Construct and return a node for a test on a discrete attribute	 */
	/*									 */
	/*************************************************************************/
	void DiscreteTest(Tree Node,Attribute Att);

	/*************************************************************************/
	/*									 */
	/*  Determine the worth of a particular split according to the		 */
	/*  operative criterion							 */
	/*									 */
	/*	    Parameters:							 */
	/*		SplitInfo:	potential info of the split		 */
	/*		SplitGain:	gain in info of the split		 */
	/*		MinGain:	gain above which the Gain Ratio		 */
	/*				may be used				 */
	/*									 */
	/*  If the Gain criterion is being used, the information gain of	 */
	/*  the split is returned, but if the Gain Ratio criterion is		 */
	/*  being used, the ratio of the information gain of the split to	 */
	/*  its potential information is returned.				 */
	/*									 */
	/*************************************************************************/
	float Worth(float ThisInfo,float ThisGain,float MinGain,char GAINRATIO);

	/*************************************************************************/
	/*									 */
	/*  Zero the frequency tables Freq[][] and ValFreq[] up to MaxVal	 */
	/*									 */
	/*************************************************************************/
	void ResetFreq(DiscrValue MaxVal);

	/*************************************************************************/
	/*									 */
	/*  Compute the total information in V[ MinVal..MaxVal ]		 */
	/*									 */
	/*************************************************************************/
	float TotalInfo(ItemCount V[],DiscrValue MinVal,DiscrValue MaxVal);

	/*************************************************************************/
	/*									 */
	/*  Given tables Freq[][] and ValFreq[], compute the information gain.	 */
	/*									 */
	/*	    Parameters:							 */
	/*		BaseInfo:	average information for all items with	 */
	/*				known values of the test attribute	 */
	/*		UnknownRate:	fraction of items with unknown ditto	 */
	/*		MaxVal:		number of forks				 */
	/*		TotalItems:	number of items with known values of	 */
	/*				test att				 */
	/*									 */
	/*  where Freq[x][y] contains the no. of cases with value x for a	 */
	/*  particular attribute that are members of class y,			 */
	/*  and ValFreq[x] contains the no. of cases with value x for a		 */
	/*  particular attribute						 */
	/*									 */
	/*************************************************************************/
	float ComputeGain(float BaseInfo,float UnknFrac,DiscrValue MaxVal,ItemCount TotalItems,short VERBOSITY,int MINOBJS);

	/*************************************************************************/
	/*									 */
	/*	Print distribution table for given attribute			 */
	/*									 */
	/*************************************************************************/
	void PrintDistribution(Attribute Att,DiscrValue MaxVal,Boolean ShowNames);

	/*************************************************************************/
	/*									 */
	/*  Compute frequency tables Freq[][] and ValFreq[] for attribute	 */
	/*  Att from items Fp to Lp, and set the UnknownRate for Att		 */
	/*									 */
	/*************************************************************************/
	void ComputeFrequencies(Attribute Att,ItemNo Fp,ItemNo Lp);

	/*************************************************************************/
	/*									 */
	/*  Set Info[] and Gain[] for discrete partition of items Fp to Lp	 */
	/*									 */
	/*************************************************************************/
	void EvalDiscreteAtt(Attribute Att,ItemNo Fp,ItemNo Lp,ItemCount Items,short VERBOSITY,int MINOBJS);

	/*************************************************************************/
	/*									 */
	/*  Evaluate subsetting a discrete attribute and form the chosen	 */
	/*  subsets Subset[Att][], setting Subsets[Att] to the number of	 */
	/*  subsets, and the Info[] and Gain[] of a test on the attribute	 */
	/*									 */
	/*************************************************************************/
	void EvalSubset(Attribute Att,ItemNo Fp,ItemNo Lp,ItemCount Items,short VERBOSITY,char GAINRATIO,int MINOBJS);

	/*************************************************************************/
	/*									 */
	/*	Sort items from Fp to Lp on attribute a				 */
	/*									 */
	/*************************************************************************/
	void Quicksort(ItemNo Fp,ItemNo Lp,Attribute Att,  void (C4_5::*Exchange)(ItemNo middle,ItemNo n));


	/*************************************************************************/
	/*								  	 */
	/*  Continuous attributes are treated as if they have possible values	 */
	/*	0 (unknown), 1 (less than cut), 2(greater than cut)	  	 */
	/*  This routine finds the best cut for items Fp through Lp and sets	 */
	/*  Info[], Gain[] and Bar[]						 */
	/*								  	 */
	/*************************************************************************/
	void EvalContinuousAtt(Attribute Att,ItemNo Fp,ItemNo Lp,short VERBOSITY,char GAINRATIO,int MINOBJS);

	/*************************************************************************/
	/*                                                                	 */
	/*  Return the greatest value of attribute Att below threshold t  	 */
	/*                                                                	 */
	/*************************************************************************/
	float GreatestValueBelow(Attribute Att,float t);

	/*************************************************************************/
	/*                                                                	 */
	/*  Change a leaf into a test on a continuous attribute           	 */
	/*                                                                	 */
	/*************************************************************************/
	void ContinTest(Tree Node,Attribute Att);

	/*************************************************************************/
	/*								 	 */
	/*  Group together the items corresponding to branch V of a test 	 */
	/*  and return the index of the last such			 	 */
	/*								 	 */
	/*  Note: if V equals zero, group the unknown values		 	 */
	/*								 	 */
	/*************************************************************************/
	ItemNo Group(DiscrValue V,ItemNo Fp,ItemNo Lp,Tree TestNode);


	/*************************************************************************/
	/*								 	 */
	/*  Build a decision tree for the cases Fp through Lp:		 	 */
	/*								 	 */
	/*  - if all cases are of the same class, the tree is a leaf and so	 */
	/*      the leaf is returned labelled with this class		 	 */
	/*								 	 */
	/*  - for each attribute, calculate the potential information provided 	 */
	/*	by a test on the attribute (based on the probabilities of each	 */
	/*	case having a particular value for the attribute), and the gain	 */
	/*	in information that would result from a test on the attribute	 */
	/*	(based on the probabilities of each case with a particular	 */
	/*	value for the attribute being of a particular class)		 */
	/*								 	 */
	/*  - on the basis of these figures, and depending on the current	 */
	/*	selection criterion, find the best attribute to branch on. 	 */
	/*	Note:  this version will not allow a split on an attribute	 */
	/*	unless two or more subsets have at least MINOBJS items. 	 */
	/*								 	 */
	/*  - try branching and test whether better than forming a leaf	 	 */
	/*								 	 */
	/*************************************************************************/
	Tree FormTree(ItemNo Fp,ItemNo Lp,short VERBOSITY,char GAINRATIO,char SUBSET,int MINOBJS);

	/*************************************************************************/
	/*								   	 */
	/*	Indent Sh columns					  	 */
	/*								  	 */
	/*************************************************************************/
	void Indent(short Sh,char *Mark);

	/*************************************************************************/
	/*									 */
	/*	Print a node T with offset Sh, branch value v, and continue	 */
	/*									 */
	/*************************************************************************/
	void ShowBranch(short Sh,Tree T,DiscrValue v);

	/*************************************************************************/
	/*									 */
	/*	Find the maximum single line size for non-leaf subtree St.	 */
	/*	The line format is						 */
	/*			<attribute> <> X.xx:[ <class (<Items>)], or	 */
	/*			<attribute> = <DVal>:[ <class> (<Items>)]	 */
	/*									 */
	/*************************************************************************/
	short MaxLine(Tree St);

	/*************************************************************************/
	/*									 */
	/*	Display the tree T with offset Sh				 */
	/*									 */
	/*************************************************************************/
	void Show(Tree T,short Sh);

	/*************************************************************************/
	/*									 */
	/*	Display entire decision tree T					 */
	/*									 */
	/*************************************************************************/
	void PrintTree(Tree T);

	/*************************************************************************/
	/*									 */
	/*	Return a copy of tree T						 */
	/*									 */
	/*************************************************************************/
	Tree CopyTree(Tree T);

	/*************************************************************************/
	/*									 */
	/*	Remove unnecessary subset tests on missing values		 */
	/*									 */
	/*************************************************************************/
	void CheckPossibleValues(Tree T);

	/*************************************************************************/
	/*									 */
	/*  Compute the additional errors if the error rate increases to the	 */
	/*  upper limit of the confidence level.  The coefficient is the	 */
	/*  square of the number of standard deviations corresponding to the	 */
	/*  selected confidence level.  (Taken from Documenta Geigy Scientific	 */
	/*  Tables (Sixth Edition), p185 (with modifications).)			 */
	/*									 */
	/*************************************************************************/
	float AddErrs(ItemCount N,ItemCount e,float CF);

	/*************************************************************************/
	/*									 */
	/*	Estimate the errors in a given subtree				 */
	/*									 */
	/*************************************************************************/
	float EstimateErrors(Tree T,ItemNo Fp,ItemNo Lp,short Sh,Boolean UpdateTree,short VERBOSITY,float CF);

	/*************************************************************************/
	/*									 */
	/*  Prune tree T, returning true if tree has been modified		 */
	/*									 */
	/*************************************************************************/
	Boolean Prune(Tree T,char SUBSET,short VERBOSITY,float CF);
	
	/*************************************************************************/
	/*									 */
	/*	Grow and prune a single tree from all data			 */
	/*									 */
	/*************************************************************************/
	void OneTree(short VERBOSITY,char GAINRATIO,char SUBSET,int MINOBJS,float CF,char *pivotsAndTrainModelFileName);


	/*************************************************************************/
	/*									 */
	/*  The windowing approach seems to work best when the class		 */
	/*  distribution of the initial window is as close to uniform as	 */
	/*  possible.  FormTarget generates this initial target distribution,	 */
	/*  setting up a TargetClassFreq value for each class.			 */
	/*									 */
	/*************************************************************************/
	void FormTarget(ItemNo Size);

	/*************************************************************************/
	/*									 */
	/*  Form initial window, attempting to obtain the target class profile	 */
	/*  in TargetClassFreq.  This is done by placing the targeted number     */
	/*  of items of each class at the beginning of the set of data items.	 */
	/*									 */
	/*************************************************************************/
	void FormInitialWindow();

	/*************************************************************************/
	/*									 */
	/*		Shuffle the data items randomly				 */
	/*									 */
	/*************************************************************************/
	void Shuffle();

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
	ClassNo Category(Descript CaseDesc,Tree DecisionTree,short VERBOSITY);

	/*************************************************************************/
	/*									 */
	/*	Count the nodes in a tree					 */
	/*									 */
	/*************************************************************************/	
	int TreeSize(Tree Node);

	/*************************************************************************/
	/*									 */
	/*	Free up space taken up by tree Node				 */
	/*									 */
	/*************************************************************************/
	void ReleaseTree(Tree Node);

	/*************************************************************************/
	/*									 */
	/*  Grow a tree iteratively with initial window size Window and		 */
	/*  initial window increment IncExceptions.				 */
	/*									 */
	/*  Construct a classifier tree using the data items in the		 */
	/*  window, then test for the successful classification of other	 */
	/*  data items by this tree.  If there are misclassified items,		 */
	/*  put them immediately after the items in the window, increase	 */
	/*  the size of the window and build another classifier tree, and	 */
	/*  so on until we have a tree which successfully classifies all	 */
	/*  of the test items or no improvement is apparent.			 */
	/*									 */
	/*  On completion, return the tree which produced the least errors.	 */
	/*									 */
	/*************************************************************************/
	Tree Iterate(ItemNo Window,ItemNo IncExceptions,short VERBOSITY,char GAINRATIO,char SUBSET,int MINOBJS);

	/*************************************************************************/
	/*									 */
	/*	Grow and prune TRIALS trees and select the best of them		 */
	/*									 */
	/*************************************************************************/
	short BestTree(short TRIALS,char GAINRATIO,char SUBSET,int MINOBJS,short VERBOSITY,int WINDOW,int INCREMENT,float CF);

	/* ******************************************************************* Soften thresholds in best tree ***************************************************************** */

	/*************************************************************************/
	/*								  	 */
	/*  Calculate upper and lower bounds for each test on a continuous	 */
	/*  attribute in tree T, using data items from Fp to Lp			 */
	/*								  	 */
	/*************************************************************************/
	void ScanTree(Tree T,ItemNo Fp,ItemNo Lp,short VERBOSITY);

	/*************************************************************************/
	/*									 */
	/*  Soften all thresholds for continuous attributes in tree T		 */
	/*									 */
	/*************************************************************************/
	void SoftenThresh(Tree T,short VERBOSITY);


	/* **************************************************************************** Save best tree *********************************************************************** */
	
	/*************************************************************************/
	/*									 */
	/*	Stream characters to/from file TRf from/to an address		 */
	/*									 */
	/*************************************************************************/
	void StreamOut(String s,int n);
	void StreamIn(String s,int n);

	/*************************************************************************/
	/*									 */
	/*	Save tree T as characters					 */
	/*									 */
	/*************************************************************************/
	void OutTree(Tree T,ofstream &outfile);

	/*************************************************************************/
	/*									 */
	/*	Save attribute values read with "discrete N"			 */
	/*									 */
	/*************************************************************************/
	void SaveDiscreteNames();

	/*************************************************************************/
	/*									 */
	/*	Save entire decision tree T in file with extension Extension	 */
	/*									 */
	/*************************************************************************/
	void SaveTree(Tree T,char *FileName);

	
	/* **************************************************************************** TrainModel *********************************************************************** */

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

	ItemNo	MaxItem;	/* max data item number */
	
	Tree    *Pruned;

	int		count;    /* record the times of open pivotsAndTrainModelFileName file */
private:

	GetMetricData TrainData;
	/*************************************************************************/
	/*									 */
	/*		Global data for C4.5					 */
	/*		--------------------					 */
	/*									 */
	/*************************************************************************/
	short		MaxAtt,		/* max att number */
				MaxClass,	/* max class number */
				MaxDiscrVal;	/* max discrete values for any att */
	
	ItemNo		*TargetClassFreq;

	Descript	*Item;		/* data items */

	DiscrValue	*MaxAttVal;	/* number of values for each att */

	char		*SpecialStatus;	/* special att treatment */

	String		*ClassName,	/* class names */
		  		*AttName,	/* att names */
		  		**AttValName;	/* att value names */

	Boolean		AllKnown,	/* true if there have been no splits
							   on atts with missing values above
							   the current position in the tree */
				*Tested,	/* Tested[a] set if att a has already been tested */
				MultiVal;	/* true when all atts have many values */
	char		Delimiter;


	/*************************************************************************/
	/*									 */
	/*	  Global data for C4.5 used for building decision trees		 */
	/*	  -----------------------------------------------------		 */
	/*									 */
	/*************************************************************************/
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

	Tree    *Raw;

	short	Subtree;		/* highest subtree to be printed */
	Tree	Subdef[100];		/* pointers to subtrees */

	FILE	*TRf;	/* file pointer for tree i/o */
	char	Fn[500];		/* file name */

	Set		*PossibleValues;
	Boolean	Changed;

	float	*ClassSum;		/* ClassSum[c] = total weight of class c */

	Boolean *LHSErr,	/*  Does a misclassification occur with this value of an att  */
			*RHSErr;	/*  if the below or above threshold branches are taken  */

	ItemNo	*ThreshErrs;	/*  ThreshErrs[i] is the no. of misclassifications if thresh is i  */

	float	*CVals;		/*  All values of a continuous attribute  */


	short	VERBOSITY,	/*v: verbosity level (0 = none) */
			TRIALS;		/*t: number of trees to be grown */
	char	GAINRATIO,	/*g: true=gain ratio, false=gain */
			SUBSET,		/*s: true if subset tests allowed */
			BATCH,		/*b: true if windowing turned off */
			UNSEENS,	/*u: true if to evaluate on test data */
			PROBTHRESH;	/*p: true if to use soft thresholds */
	int		MINOBJS,	/*m: minimum items each side of a cut */
			WINDOW,		/*w: initial window size */
			INCREMENT;	/*i: max window increment each iteration */
	float	CF;		/*c: confidence limit for tree pruning */

	short Best;
};





#endif
