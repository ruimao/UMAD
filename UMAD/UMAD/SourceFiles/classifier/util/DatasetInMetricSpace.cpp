#include"../../../HeaderFiles/classifier/util/DatasetInMetricSpace.h"

/**@brief none parameter constructor*/
CDatasetInMetricSpace::CDatasetInMetricSpace()
{

}

/**@brief destructor*/
CDatasetInMetricSpace::~CDatasetInMetricSpace()
{

}

/**
*@ brief calculate the distance between samples and pivots, return the train data in metric space.
*@ param classifyMethod  classifymethod: "knn" , "naviebayes".
*@ param traindata: traindata is the primary dataset
*@ param trainDataLabel: the class label of each training data
*@ param pivotSelectionMethod: the pivot selection method: "random", "fft", "center", "pcaonfft", "pca","incremental".
*@ param metric: the distance function: "EuclideanDistance" , "EditDistance" , "LInfinityDistance".
*@ param pivotNum: the number of pivots
*@ param pivotsFileName: the file to store the selected pivots information.
*@ param dim  dimension of vector data to load or number of features to be loaded.
*@ return: return an vector contains the distance between each sample and pivots 
*/
GetMetricData CDatasetInMetricSpace::getMetricTrainData(char *classifyMethod,vector<shared_ptr<CMetricData> > *traindata,vector<string> trainDataLabel,CMetricDistance *metric,CPivotSelectionMethod *pivotSelectionMethod,int pivotNum,char *pivotsAndTrainModelFileName,int dim)
{
	GetMetricData M_traindata;
	ofstream outfile(pivotsAndTrainModelFileName,ofstream::out);
	outfile<<classifyMethod<<endl;

	vector<int> pivotsindex;
	pivotsindex=pivotSelectionMethod->selectPivots(metric,*traindata,pivotNum);
	outfile<<pivotsindex.size()<<endl;

	CountedMetric* coutMetric = dynamic_cast<CountedMetric*>(metric);
	CEuclideanDistance* eudlideanDistance = dynamic_cast<CEuclideanDistance*>(coutMetric->getBaseMetric());
	CEditDistance* editDistance = dynamic_cast<CEditDistance*>(coutMetric->getBaseMetric());
	CImageMetric* imageMetric = dynamic_cast<CImageMetric*>(coutMetric->getBaseMetric());
	CDNAMetric* dnaMetric = dynamic_cast<CDNAMetric*>(coutMetric->getBaseMetric());
	CPeptideMetric* peptideMetric = dynamic_cast<CPeptideMetric*>(coutMetric->getBaseMetric());
	CLInfinityDistance* lInfinityMetric = dynamic_cast<CLInfinityDistance*>(coutMetric->getBaseMetric());
	;
	for(auto i=pivotsindex.begin();i<pivotsindex.end();++i)
	{
		double* dataList=NULL;
		if(eudlideanDistance != NULL) {CDoubleVector* vectorData = (CDoubleVector*)(traindata->at(*i).get());dataList = vectorData->getData();outfile<<"eudlideanDistance"<<" ";}
		
		if(editDistance != NULL) {CDoubleVector* vectorData = (CDoubleVector*)(traindata->at(*i).get());dataList = vectorData->getData();outfile<<"editDistance"<<" ";}

		//if(imageMetric != NULL) {CImage* vectorData = (CImage*)(traindata->at(*i).get());dataList = vectorData->getData();outfile<<"imageMetric"<<" ";}

		//if(dnaMetric != NULL){CDNA* vectorData = (CDNA*)(traindata->at(*i).get());dataList = vectorData->getData();outfile<<"dnaMetric"<<" ";}

		//if(peptideMetric != NULL) {CPeptide* vectorData = (CPeptide*)(traindata->at(*i).get());dataList = vectorData->getData();outfile<<"peptideMetric"<<" ";}

		if(lInfinityMetric != NULL) {CDoubleVector* vectorData = (CDoubleVector*)(traindata->at(*i).get());dataList = vectorData->getData();outfile<<"lInfinityMetric"<<" ";}


		outfile<<dim<<endl;
		for(int j=0; j < dim; ++j)
		{
			if(j == dim - 1)
				outfile << dataList[j] << endl;
			else 
				outfile << dataList[j] << " ";
		}
	}
	outfile.close();

	vector<double> distance;

	for(decltype(traindata->size()) i=0;i<traindata->size();++i)
	{
		for(auto j=pivotsindex.begin(); j!=pivotsindex.end(); ++j)
		{
			distance.push_back(metric->getDistance(traindata->at(i).get(),traindata->at(*j).get()));
		}
		M_traindata.metricData.push_back(distance);
		M_traindata.metricDataLabel.push_back(trainDataLabel.at(i));
		distance.clear();
	}
	return M_traindata;
}

/**
*@ select 1/10 data from train data to be the test data,return the test data in metric space
*@ param classifyMethod  classifymethod: "knn" , "naviebayes".
*@ param testdata: testdata is the primary dataset
*@ param testDataLabel: the class label of each test data
*@ param metric: the distance function: "EuclideanDistance" , "EditDistance" , "LInfinityDistance".
*@ param pivotsFileName: the file to store the selected pivots information.
*@ param splitRatio: decide how much data from training set as a test set.
*@ return: return an vector contains the distance between each test sample and pivots 
*/
GetMetricData CDatasetInMetricSpace::getMetricTestData_fromTrainData(char *classifyMethod,vector<shared_ptr<CMetricData> > *traindata,vector<string> trainDataLabel,CMetricDistance *metric,char *pivotsAndTrainModelFileName,int splitRatio)
{
	ifstream infile(pivotsAndTrainModelFileName);
	if(!infile)
	{
		cout<<"Can't open the file "<<pivotsAndTrainModelFileName<<" !"<<endl;
		exit(0);
	}
	string classificationMethod="";
	infile >> classificationMethod;
	if(classificationMethod == classifyMethod)
	{		
		string distanceFun="";
		infile >> pivotsNum;
		infile >> distanceFun;
		infile >> attNum;
		vector<shared_ptr<CMetricData> > *pivots = new vector<shared_ptr<CMetricData> >();

		if(distanceFun == "eudlideanDistance")
		{
			string str="";
			double *data;
			shared_ptr<CDoubleVector> temp = shared_ptr<CDoubleVector>();
			for(int i=0;i<2*pivotsNum-1;++i)
			{
				if(i%2==0)
				{
					data = new double[attNum];
					for(int j=0; j<attNum; ++j)
					{
						infile>>data[j];
					}
					temp.reset(new CDoubleVector(data,attNum));
					pivots->push_back(temp);
				}
				else
				{
					infile >> distanceFun;
					infile >> attNum;
				}
			}
			infile.close();
		}

		else if(distanceFun == "lInfinityMetric")
		{
			string str="";
			double *data;
			shared_ptr<CDoubleVector> temp = shared_ptr<CDoubleVector>();
			for(int i=0;i<2*pivotsNum-1;++i)
			{
				if(i%2==0)
				{
					data = new double[attNum];
					for(int j=0; j<attNum; ++j)
					{
						infile>>data[j];
					}
					temp.reset(new CDoubleVector(data,attNum));
					pivots->push_back(temp);
				}
				else
				{
					infile >> distanceFun;
					infile >> attNum;
				}
			}
			infile.close();
		}

		else if(distanceFun == "editDistance")
		{

		}

		else if(distanceFun == "imageMetric")
		{

		}

		else if(distanceFun == "dnaMetric")
		{

		}

		else if(distanceFun == "peptideMetric")
		{

		}	

		GetMetricData M_testdata;
		vector<double> distance;
		for(decltype(traindata->size()) i=1; i<=traindata->size(); ++i)
		{
			if(i%splitRatio == 0)
			{
				for(int j=0; j<pivotsNum; ++j)
				{
					distance.push_back(metric->getDistance(traindata->at(i-1).get(),pivots->at(j).get()));
				}
				M_testdata.metricData.push_back(distance);
				M_testdata.metricDataLabel.push_back(trainDataLabel.at(i-1));
				distance.clear();
			}
		}
		return M_testdata;
	}
	else
	{
		cout<<"Please check out your pivotsAndTrainModel file ! Make sure that matching the classificaiton algorithm!"<<endl;
		exit(1);
	}		
}

/**
*@ brief calculate the distance between test samples and pivots, return the test data in metric space.
*@ param classifyMethod  classifymethod: "knn" , "naviebayes".
*@ param testdata: testdata is the primary dataset
*@ param testDataLabel: the class label of each test data
*@ param metric: the distance function: "EuclideanDistance" , "EditDistance" , "LInfinityDistance".
*@ param pivotsFileName: the file to store the selected pivots information.
*@ return: return an vector contains the distance between each test sample and pivots 
*/
GetMetricData CDatasetInMetricSpace::getMetricTestData_fromTestData(char *classifyMethod,vector<shared_ptr<CMetricData> > *testdata,vector<string> testDataLabel,CMetricDistance *metric,char *pivotsAndTrainModelFileName)
{
	ifstream infile(pivotsAndTrainModelFileName);
	if(!infile)
	{
		cout<<"Can't open the file "<<pivotsAndTrainModelFileName<<" !"<<endl;
		exit(0);
	}
	string classificationMethod="";
	infile >> classificationMethod;
	if(classificationMethod==classifyMethod)
	{
		string distanceFun="";
		infile >> pivotsNum;
		infile >> distanceFun;
		infile >> attNum;
		vector<shared_ptr<CMetricData> > *pivots = new vector<shared_ptr<CMetricData> >();

		if(distanceFun=="eudlideanDistance")
		{
			string str="";
			double *data;
			shared_ptr<CDoubleVector> temp = shared_ptr<CDoubleVector>();
			for(int i=0;i<2*pivotsNum-1;++i)
			{
				if(i%2==0)
				{
					data = new double[attNum];
					for(int j=0; j<attNum; ++j)
					{
						infile>>data[j];
					}
					temp.reset(new CDoubleVector(data,attNum));
					pivots->push_back(temp);
				}
				else
				{
					infile >> distanceFun;
					infile >> attNum;
				}
			}
		}

		else if(distanceFun=="lInfinityMetric")
		{
			string str="";
			double *data;
			shared_ptr<CDoubleVector> temp = shared_ptr<CDoubleVector>();
			for(int i=0;i<2*pivotsNum-1;++i)
			{
				if(i%2==0)
				{
					data = new double[attNum];
					for(int j=0; j<attNum; ++j)
					{
						infile>>data[j];
					}
					temp.reset(new CDoubleVector(data,attNum));
					pivots->push_back(temp);
				}
				else
				{
					infile >> distanceFun;
					infile >> attNum;
				}
			}
		}

		else if(distanceFun == "editDistance")
		{

		}

		else if(distanceFun == "imageMetric")
		{

		}

		else if(distanceFun == "dnaMetric")
		{

		}

		else if(distanceFun == "peptideMetric")
		{

		}	

		GetMetricData M_testdata;
		vector<double> distance;
		for(decltype(testdata->size()) i=0;i<testdata->size();++i)
		{
			for(decltype(pivots->size()) j=0;j<pivots->size();++j)
			{
				distance.push_back(metric->getDistance(testdata->at(i).get(),pivots->at(j).get()));
			}
			M_testdata.metricData.push_back(distance);
			M_testdata.metricDataLabel.push_back(testDataLabel.at(i));
			distance.clear();
		}
		infile.close();
		return M_testdata;
	}
	else
		cout<<"Please check out your pivotsAndTrainModel file ! Make sure that matching the classificaiton algorithm!"<<endl;


}

