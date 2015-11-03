#include"../../HeaderFiles/classifier/Test_NavieBayes.h"
#include"../../HeaderFiles/classifier/util/GetValue.h"
#define space(s) (s == ' ' || s == '\n' || s == '\t')
#define  SkipComment	while ( ( c = getc(f) ) != '\n' )
#define p 1/3
//#define _SCL_SECURE_NO_WARNINGS  

#pragma warning(disable:4996)   

CTest_NavieBayes::CTest_NavieBayes():MaxDiscrValNo(2),MaxTestItemNo(-1)
{
	this->Inc=1024;
	TruePositive=0;
	FalsePositive=0;
	accuracy=0.0;
}
CTest_NavieBayes::~CTest_NavieBayes()
{

}
void CTest_NavieBayes::Predict(GetMetricData data,char *trainingModelFileName,const char *showClassificationResult,int pivotsNum)
{
	TestData=data;
	MaxTestItemNo=data.metricData.size()-1;
	double *prob;  //store posteriori probability of each class
	int BestI=-1;
	double MaxProb=0;
	int j;
	GetData(trainingModelFileName);
    prob=(double *)calloc(MaxClassNo,sizeof(double));
	for(int i=0;i<MaxClassNo;++i)
	    prob[i]=1.0;
	for(int i=0;i<=MaxTestItemNo;++i)
	{
		for(j=0;j<MaxClassNo;++j)
		{   
			for(int k=0;k<MaxAttNo;++k)
			{   
				/*
				 ***Using m estimation to calculate the conditional probability£ºp(x/y)=(nc+mp)/(n+m)
				 */
				if(TestItem[i][k].continuousVal<=SplitPoint[k])
					prob[j]*=((PostFreq[j][k][1]+ClassFreq[j]*p)/(ClassFreq[j]+ClassFreq[j]));  //prob[j]=p(x/y)=p(x1/y=j)*p(x2/y=j)*p(x3/y=j)*p(x4/y=j)
						//prob[j] += (log(PostFreq[j][k][1])-log(ClassFreq[j]));
				else
					prob[j]*=((PostFreq[j][k][2]+ClassFreq[j]*p)/(ClassFreq[j]+ClassFreq[j]));
					//prob[j] += (log(PostFreq[j][k][2])-log(ClassFreq[j]));
			}
			prob[j]*=(ClassFreq[j]/(MaxItemNo));
			//prob[j] += (log(ClassFreq[j])-log(MaxItemNo));
			if(prob[j]>MaxProb)   
		    {
			    MaxProb=prob[j];  //Store the value of maximum a posteriori probability
                BestI=j;   //get the subscript of maximun a posteriori probability
		    }
		}

		if(ClassName[BestI]==ClassName[TestItem[i][MaxAttNo].labelPosition])
			TruePositive++;
		else
			FalsePositive++;

		/*reset*/
		{
            BestI=-1;
	        MaxProb=0;
			for(int l=0;l<MaxClassNo;++l)
				prob[l]=1.0;
		}
	}
	show(showClassificationResult);
}
void CTest_NavieBayes::show(const char *resultFileName)
{
	ofstream outfile(resultFileName,ofstream::out);
	int i,j;
	for( i=0;i<=MaxTestItemNo;i++)
	{   
		outfile<<i+1<<" : ";
		for( j=0;j<MaxAttNo;j++)
		{
			outfile<<TestItem[i][j].continuousVal<<" ";
		}
		outfile<<"Ô¤²â½á¹ûÎª:"<<ClassName[TestItem[i][MaxAttNo].labelPosition]<<endl;
	}

	outfile<<"TruePositive:"<<TruePositive<<endl;
	outfile<<"FalsePositive:"<<FalsePositive<<endl;
	accuracy=double(TruePositive)/(MaxTestItemNo+1);
	outfile<<"Accuracy:"<<accuracy<<endl;
	outfile.close();
}
void CTest_NavieBayes::GetData(char *trainingModelFileName)
{   
	int classLabelPosition;
	int SplitVal;
	int ClassCeiling=100;
	ifstream infile(trainingModelFileName);
	if(!infile)
	{
		cout<<"Open training model file "<<trainingModelFileName<<"failed!"<<endl;
		exit(0);
	}
	string str="";
	infile >> str;
	while(str!="@trainmodel")
	{
		infile >> str;
	}
	
	infile >> MaxClassNo >> MaxAttNo >> MaxItemNo;
	ClassFreq = (double *) calloc(MaxClassNo+1, sizeof(double));
#ifdef _WIN32
	ClassName = (string *) calloc(ClassCeiling, sizeof(string));
#endif

#ifdef __GNUC__
	ClassName = new string[ClassCeiling];
#endif

	int temp;
	string tempClass;
	for(int i=0; i<MaxClassNo; ++i)
	{
		infile >> temp;
		ClassFreq[i] = temp;
	}
	for(int j=0; j<MaxClassNo; ++j)
	{
		infile >> tempClass;
		ClassName[j] = tempClass;
	}
	PostFreq=(double ***) calloc(MaxClassNo,sizeof(double **));
	for(int i=0;i<MaxClassNo;i++)
	{
		PostFreq[i]=(double **) calloc(MaxAttNo,sizeof(double *));
		for(int j=0;j<MaxAttNo;j++)
		{   
			PostFreq[i][j]=(double *)calloc(3,sizeof(double));
		}
	}
	SplitPoint=(double *)calloc(MaxAttNo,sizeof(double));

	double tempS;
	for(int k=0; k<MaxItemNo+1; ++k)
	{
		for(int l=0; l<MaxAttNo; ++l)
		{
			if(k==0)
			{
				infile >> tempS;
				SplitPoint[l] = tempS;
			}
			else
			{
				infile >> classLabelPosition;
				infile >> SplitVal;
				infile >> temp;
				if(SplitVal==1)
					PostFreq[classLabelPosition][l][1]=temp;
				else if(SplitVal==2)
					PostFreq[classLabelPosition][l][2]=temp;
			}
		}
	}

	infile.close();
	int ItemNo;
	int	ItemSpace=0;
	for(ItemNo=0;ItemNo<=MaxTestItemNo;++ItemNo)
	{
	    /*  Make sure there is room for another item  */
	    if ( ItemNo >= ItemSpace )
	    {
	        if ( ItemSpace )
	        {
		        ItemSpace += Inc;
		        TestItem = (Description *)realloc(TestItem, ItemSpace*sizeof(Description));
	        }
	        else
	        {
		        TestItem = (Description *)malloc((ItemSpace=Inc)*sizeof(Description));
	         }
	    }
	    TestItem[ItemNo] = GetDescription(ItemNo,TestData);      //Item[ItemNo] store the training data
    }
}
void CTest_NavieBayes::Error(int n, string s1, string s2)
/*  -----  */
{
    cout<<"ERROR:  ";
    switch(n)
    {
	case 0: cout<<"cannot open file "<<s1<<s2<<endl;
		break;

	case 1:	cout<<"colon expected after attribute name "<<s1<<endl;
		break;

	case 2:	cout<<"unexpected EOF while reading attribute "<< s1<<endl;
		break;

	case 3:cout<<"attribute "<<s1<<" has only one value"<<endl;
		break;

	case 4:cout<<"case "<< MaxItemNo<<"'s value of '"<<s2<<"' for attribute "<<s1<<"is illegal"<<endl;//Error(4, AttName[AttNo], name);		   
		break;

	case 5:cout<<"case "<<MaxTestItemNo+1<<"'s class of '"<<s2<<"' is illegal"<<endl;
		break;
    } 
	cout<<"process stop!"<<endl;
    exit(1);
}
Description CTest_NavieBayes::GetDescription(int itemNo,GetMetricData Df)
{
	int AttNo;/* attribute number, 0..MaxAttNo */
	int Dv=0;
    Description Dvec;
    Dvec = (Description)calloc(MaxTestItemNo*2, sizeof(AttValue));
    for(AttNo=0;AttNo<MaxAttNo;AttNo++)
    {
		/*  Continuous value  */
		Dvec[AttNo].continuousVal=Df.metricData[itemNo][AttNo];      
    }
	for(int i=0;i<MaxClassNo;++i)
	{
		if(Df.metricDataLabel[itemNo]==ClassName[i])
			Dv=i;
	}

	Dvec[MaxAttNo].labelPosition=Dv;//the position of classlabel in ClassName
	return Dvec;
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
*/
void CTest_NavieBayes::TestModel(char *classifyMethod,vector<shared_ptr<CMetricData> > *testdata,vector<string> testDataLabel, CMetricDistance *metric,char *pivotsAndTrainModelFileName,char *testModelFileName,int status,int splitRatio)
{
	CDatasetInMetricSpace getTestData;
	GetMetricData M_testdata;
	if(status==0)
	{
		M_testdata=getTestData.getMetricTestData_fromTrainData(classifyMethod,testdata,testDataLabel,metric,pivotsAndTrainModelFileName,splitRatio);
	}
	else if(status==1)
	{
		M_testdata=getTestData.getMetricTestData_fromTestData(classifyMethod,testdata,testDataLabel,metric,pivotsAndTrainModelFileName);
	}

	int pivotsNum=getTestData.pivotsNum;
	CTest_NavieBayes pt;
	pt.Predict(M_testdata,pivotsAndTrainModelFileName,testModelFileName,pivotsNum);
}
