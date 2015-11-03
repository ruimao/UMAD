#include "../../../HeaderFiles/classifier/util/NavieBayes.h"
#include <string.h>


/**
 *@ show classification result
 *@ datafilename: the primary training set
 *@ infilename: the training set under metric space that after pivot selection and distance calculation
 *@ testfilename: unclassified samples
 *@ outfilename: outfilename is going to store the classify result
*/
void CNavieBayes::showClassifierModel(GetMetricData M_traindata,const char* outfilename)
{
	char *names="SourceFiles/util/data/train";
	showClassifierModel(M_traindata,outfilename,names);
}
/**
 *@show classification result
 *@infilename: the training set under metric space that after pivot selection and distance calculation
 *@testfilename: unclassified samples
 *@testlabel: the true class label of unclassified samples
 *@outfilename: outfilename is going to store the classify result
 *@names: names is a describing file about the metric train data
*/
CTrain_NavieBayes gi;
void CNavieBayes::showClassifierModel(GetMetricData M_traindata,const char *outfilename,char * names)
{	
	gi.getname(names);
	gi.getdata(M_traindata);
	
	//gm.TrainModel(&gi,outfilename);
}

/**
 *@show classification result
 *@trainingModelFileName: the file name of training model
 *@M_testdata: the test data under metric space that after distance calculation
 *@outfilename: outfilename is going to stroe the classify result
 */
void CNavieBayes::showClassiciationResult(char *trainingModelFileName,GetMetricData M_testdata, const char *outfilename)
{
	//PredictClass pt(&gi,&gm,M_testdata);
	//pt.Predict(trainingModelFileName,outfilename);
}