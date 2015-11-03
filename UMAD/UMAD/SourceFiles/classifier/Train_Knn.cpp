#include"../../HeaderFiles/classifier/Train_Knn.h"

CTrain_Knn::CTrain_Knn()
{

}

CTrain_Knn::~CTrain_Knn()
{

}


/**
 *@show classification result
 *@infilename: the training set under metric space that after pivot selection and distance calculation
 *@outfilename: outfilename is going to store the classify result
 */
void CTrain_Knn::showClassifierModel(GetMetricData M_traindata,const char* outfilename, int pivotNum)
{
	ofstream outfile(outfilename,ofstream::app);  //ofstream::out : create a new file; ofstream::app : open the existing file and add content at the end.
	outfile<<"@trainmodel"<<endl;
	for(auto m=0; m<M_traindata.metricData.size(); ++m) 
	{			
		for(int n = 0;n < pivotNum; ++n) 
		{					
			outfile << M_traindata.metricData[m][n] << " ";
		}
		if( m != (M_traindata.metricData.size()-1))
			outfile << M_traindata.metricDataLabel[m] << endl;
		else
			outfile << M_traindata.metricDataLabel[m] ;
	}
	outfile.close();
}

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
void CTrain_Knn::TrainModel(char *classifyMethod,vector<shared_ptr<CMetricData> > *traindata,vector<string> trainDataLabel, CMetricDistance *metric,CPivotSelectionMethod *pivotselectionmethod, int pivotNum,char *pivotsAndTrainModelFileName,int dim)
{
	CDatasetInMetricSpace getTrainData;
	GetMetricData M_traindata;
	M_traindata=getTrainData.getMetricTrainData(classifyMethod,traindata,trainDataLabel,metric,pivotselectionmethod,pivotNum,pivotsAndTrainModelFileName,dim);
	showClassifierModel(M_traindata,pivotsAndTrainModelFileName,pivotNum);
}