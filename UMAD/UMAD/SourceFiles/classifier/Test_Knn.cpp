#include"../../HeaderFiles/classifier/Test_Knn.h"

CTest_Knn::CTest_Knn()
{
	curTrainingSetSize=0;                                 //size of training set
    curTestSetSize=0; 
}

CTest_Knn::~CTest_Knn()
{

}

/**
 *@ Euclidean distance between vector1=(x1,x2,...,xn) and vector2=(y1,y2,...,yn)
 */
double  CTest_Knn::euclideanDistance(struct dataVector vector1,struct dataVector vector2)
{
	double dist,sum=0.0;
	for(int i=0;i<vector1.attributes.size();i++)
	{
		sum+=(vector1.attributes[i]-vector2.attributes[i])*(vector1.attributes[i]-vector2.attributes[i]);
	}
	dist=sqrt(sum);
	return dist;
}

/**
 *@ L infinity distance between vector1=(x1,x2,...,xn) and vector2=(y1,y2,...,yn)
 */
double CTest_Knn::LDistance(struct dataVector vect1,struct dataVector vect2)
{
	double dist_max=0.0;
	vector<double> values;
	for(int i=0;i<vect1.attributes.size();++i)
	{
		double temp;
		temp=fabs(vect1.attributes[i]-vect2.attributes[i]);
		values[i]=temp;
	}
	for(int j=0;j<vect1.attributes.size();++j)
	{
		if(values[j]>dist_max)
			dist_max=values[j];
	}
	return dist_max;
}

/**
 *@ get the maxdistance in gNearestDistance and return the subscript
 */
int CTest_Knn::GetMaxDistance(int k)
{
	int maxNo=0;
	for(int i=1;i<k;i++)
	{
		if(gNearestDistance[i].distance>gNearestDistance[maxNo].distance)	maxNo = i;
	}
    return maxNo;
}

/**
 *@ classify unclassification sample
 */
string CTest_Knn::Classify(struct dataVector Sample,int k)
{
	double dist=0;
	int maxid=0,i,tmpfreq=1; //freq[k] store the number of each class apperaed in k-nearest nighbour
	vector<int> freq;
	distanceStruct gND;
	string curClassLable = " ";
	//memset(&freq,1,sizeof(freq));  //initialized freq[k] to 1
	for(i=0; i<k; ++i)
	{
		freq.push_back(1);
	}
	//step.1---initalize the distance to MAX_VALUE
	for(i=0;i<k;i++)
	{
		gND.distance = MAX_VALUE;
		gNearestDistance.push_back(gND);
	}
	//step.2---calculate K-nearest neighbour distance
	for(i=0;i<curTrainingSetSize;i++)
	{
		//step.2.1---calculate the distance between unclassification sample and each training sample
		dist=euclideanDistance(gTrainingSet[i],Sample);
		//step.2.2---get the max distance in gNearestDistance
		maxid=GetMaxDistance(k);
		//step.2.3---if the dist less than the maxdistance in gNearestDistance£¬it will be one of k-nearest neighbour 
		if(dist<gNearestDistance[maxid].distance) 
		{
			gNearestDistance[maxid].ID=gTrainingSet[i].ID;
			gNearestDistance[maxid].distance=dist;
			gNearestDistance[maxid].classLabel=gTrainingSet[i].classLabel;
		}
	}
	//step.3---statistics the number of each class appeared
	for(i=0;i<k;i++)  
	{
		for(int j=0;j<k;j++)
		{
			if((i!=j)&&(gNearestDistance[i].classLabel==gNearestDistance[j].classLabel))
			{
				freq[i]+=1;
			}
		}
	}
	//step.4---chose the class label with maximum frequency
	for(i=0;i<k;i++)
	{
		if(freq[i]>tmpfreq)  
		{
			tmpfreq=freq[i];
 			curClassLable=gNearestDistance[i].classLabel;
		}
	}
	return curClassLable;
}

/**
 *@show classification result
 *@trainingModelFileName: the file name of training model
 *@M_testdata: the test set under metric space that after distance calculation
 *@outfilename: outfilename is going to stroe the classify result
 *@pivotNumber: the number of selected pivots
 *@ param k: the number of nearest neighbors
 */
void CTest_Knn::showClassiciationResult(char *trainingModelFileName,GetMetricData M_testdata,const char *outfilename,int pivotNumber,int k)
{
	int rowNo=0,modelRowNo=0;
	dataVector getTS,getTT;
	double att,att2;
	ifstream infile(trainingModelFileName);
	if(!infile)
	{
		cout<<"Open training model file "<<trainingModelFileName<<"failed!"<<endl;
		exit(0);
	}
	string str;
	string classLabel="";
	int TruePositive=0,FalsePositive=0;
	infile>>str;
	while(str!="@trainmodel")
	{
		infile>>str;
	}
	while(!infile.eof())
	{
		modelRowNo++;
		getTS.ID = modelRowNo;
		for(int i = 0; i < pivotNumber; ++i)
		{
			infile >> att;
			getTS.attributes.push_back(att);
		}
		infile >> getTS.classLabel;
		gTrainingSet.push_back(getTS);
		getTS.attributes.clear();
		curTrainingSetSize++;
	}
	infile.close();

	for(auto m=0; m<M_testdata.metricData.size(); ++m) 
	{
		rowNo++;//first data:rowNo=1
		
		getTT.ID = rowNo;
		for(int n = 0;n < pivotNumber;++n) 
		{				
			att2 = M_testdata.metricData[m][n];
			getTT.attributes.push_back(att2);
		}
		getTT.classLabel = M_testdata.metricDataLabel[m];
		gTestSet.push_back(getTT);
		getTT.attributes.clear();
		curTestSetSize++;
	}

	ofstream outfile(outfilename,ofstream::out);
	outfile<<"************************************Program description***************************************"<<endl;
	outfile<<"*********************** Use Knn algorithm method to classification ***********************"<<endl;
	outfile<<"***********************************************************************************"<<endl<<endl;
	outfile<<"************************************ Test Result ***************************************"<<endl<<endl;
	for(int i=0;i<curTestSetSize;i++)
	{
		outfile<<"************************************ No:"<<i+1<<" test data**************************************"<<endl;
		classLabel =Classify(gTestSet[i],k);
                string cx="";
                string cxx;
	    if(Classify(gTestSet[i],k).compare(gTestSet[i].classLabel)==0)
		{
			TruePositive++;
		}
		
		outfile<<"rowNo:  "<<gTestSet[i].ID<<"   \t  KNN·test result:  "<<classLabel<<"  ( true class label:  "<<gTestSet[i].classLabel<<" )"<<endl;
		if(Classify(gTestSet[i],k).compare(gTestSet[i].classLabel)!=0)
		{
			outfile<<"                                                                      *** Wrong ***"<<endl;
		}
		outfile<<k<<"-nearest data:"<<endl;
		
		for(int j=0;j<k;j++)
		{
			outfile<<gNearestDistance[j].ID<<"\t"<<gNearestDistance[j].distance<<"\t"<<gNearestDistance[j].classLabel<<"\t"<<"rowNo: "<<gNearestDistance[j].ID<<"\t"<<"Distance: "<<gNearestDistance[j].distance<<"\t"<<"ClassLable: "<<gNearestDistance[j].classLabel<<endl;
		}
		outfile<<endl;
		gNearestDistance.clear();
	}
    FalsePositive=curTestSetSize-TruePositive;
	outfile<<"*********************************** result analyze **************************************"<<endl;
	outfile<<"TP(True positive): "<<TruePositive<<endl<<"FP(False positive): "<<FalsePositive<<endl<<"accuracy: "<<float(TruePositive)/(curTestSetSize)<<endl;
	outfile.close();
  
}

/**
 *@ test training model of classification algorithm.
 *@ param classifyMethod  classifymethod: "knn" , "naviebayes".
 *@ param testdata: the primary trainging data.
 *@ param testDataLabel: the class label of primary training data.
 *@ param metric: distance function.
 *@ param pivotsAndTrainModelFileName: the file to store the selected pivots information and training model.
 *@ param testModelFileName: the file to store the training model.
 *@ param status: determine the source of the test data. 0: TestDataFromTrainData; 1: TestDataFromTestData.
 *@ param k: the number of nearest neighbors
*/
void CTest_Knn::TestModel(char *classifyMethod,vector<shared_ptr<CMetricData> > *testdata,vector<string> testDataLabel, CMetricDistance *metric,char *pivotsAndTrainModelFileName,char *testModelFileName,int status,int k,int splitRatio)
{
	CDatasetInMetricSpace getTestData;
	GetMetricData M_testdata;
	if(status==0)
	{
		M_testdata=getTestData.getMetricTestData_fromTrainData(classifyMethod,testdata,testDataLabel,metric,pivotsAndTrainModelFileName,splitRatio);
		pivotNumber=getTestData.pivotsNum;
	}
	else if(status==1)
	{
		M_testdata=getTestData.getMetricTestData_fromTestData(classifyMethod,testdata,testDataLabel,metric,pivotsAndTrainModelFileName);
		pivotNumber=getTestData.pivotsNum; 
	}
	showClassiciationResult(pivotsAndTrainModelFileName,M_testdata,testModelFileName,pivotNumber,k);
}
