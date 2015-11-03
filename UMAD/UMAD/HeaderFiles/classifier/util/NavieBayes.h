#ifndef NAVIEBAYES_H
#define NAVIEBAYES_H

#include "../../../HeaderFiles/classifier/Train_NavieBayes.h"
#include "../../../HeaderFiles/classifier/Test_NavieBayes.h"
#include "MetricDataFormat.h"
#include <string>

using namespace std;

/**
* @file NavieBayes.h
* @implementation classification and show classification result
* @author Ping Li
* @version 2014-8-12
*/

/**
* @class CNavieBayes
* @implementation classification and show classification result
* @author Ping Li
* @version 2014-8-12
*/

class CNavieBayes
{
public:
	/**
     *@show classification result
     *@infilename: the training set under metric space that after pivot selection and distance calculation
	 *@testfilename: unclassified samples
	 *@testlabel: the true class label of unclassified samples
     *@outfilename: outfilename is going to store the classify result
     */
	virtual void showClassifierModel(GetMetricData M_traindata,const char* outfilename);

	/**
     *@show classification result
     *@infilename: the training set under metric space that after pivot selection and distance calculation
	 *@testfilename: unclassified samples
	 *@testlabel: the true class label of unclassified samples
     *@outfilename: outfilename is going to store the classify result
	 *@names: names is a describing file about the metric train data
     */
	virtual void showClassifierModel(GetMetricData M_traindata,const char* outfilename,char * names);

	/**
	 *@show classification result
	 *@trainingModelFileName: the file name of training model
	 *@M_testdata: the test data under metric space that after distance calculation
	 *@outfilename: outfilename is going to stroe the classify result
	 */
	virtual void showClassiciationResult(char *trainingModelFileName,GetMetricData M_testdata, const char *outfilename);
};

#endif