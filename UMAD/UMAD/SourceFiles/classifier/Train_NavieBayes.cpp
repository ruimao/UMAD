#include"../../HeaderFiles/classifier/Train_NavieBayes.h"
#include"../../HeaderFiles/classifier/util/GetValue.h"
#define space(s) (s == ' ' || s == '\n' || s == '\t' || s == '\r')
#define  SkipComment	while ( ( c = getc(f) ) != '\n' )
//#define _SCL_SECURE_NO_WARNINGS  
#pragma warning(disable:4996)   //Disable call any one of potentially unsafe in the standard C ++ library compiler warning£¨level 3£©C4996

CTrain_NavieBayes::CTrain_NavieBayes()
{
	MaxClassNo=-1;
	MaxDiscrValNo=2;
	Inc=1024;
	MaxAttNo=-1;
	Epsilon=1E-3;
	MINOBJS=2;
	None=-1;
	MaxContAttNo=-1;
}

CTrain_NavieBayes::~CTrain_NavieBayes()
{

}


void CTrain_NavieBayes::getname(string names)
{   
	/** remove the filename suffix */
	int n=names.find_last_of('.');
	names=names.substr(0,n);

	Names=names;
	char Buffer[100];
	char Fn[100];
	int AttCeiling=100;
	int ClassCeiling=10;
	int	ValCeiling;
	auto i=Names.begin();
auto j=Names.end();
	copy(i,j,Fn);
	Fn[Names.length()]=NULL;
    strcat(Fn,".names");
	if ( ! ( Nf = fopen(Fn, "r") ) ) 
		Error(0, Fn, "");

	/* read class name*/
#ifdef _WIN32
	ClassName = (string *) calloc(ClassCeiling, sizeof(string));
#endif	

#ifdef __GNUC__
	ClassName=new string[ClassCeiling];
#endif

	do
    	{
	    ReadName(Nf, Buffer);
	    if ( ++MaxClassNo >= ClassCeiling)
	    {
	        ClassCeiling += 100;
#ifdef _WIN32
		ClassName = (string *) realloc(ClassName, ClassCeiling*sizeof(string));
#endif

#ifdef __GNUC__
		 delete[]ClassName;
		ClassName=new string[ClassCeiling];
#endif
	    }

	   ClassName[MaxClassNo]=string(Buffer);
	  // cout <<"ClassName: "<< ClassName[MaxClassNo] << endl;

    }while ( Delimiter == ',' );

	/* Get attribute and attribute value names from names file  */
#ifdef _WIN32   
    AttName = (string *) calloc(AttCeiling, sizeof(string));
    MaxAttValNo = (short *) calloc(AttCeiling, sizeof(short));
    AttValName = (string **) calloc(AttCeiling, sizeof(string *));
    SpecialStatus = (char *) malloc(AttCeiling);
#endif

#ifdef __GNUC__
    AttName = new string[AttCeiling];
    MaxAttValNo = new short[AttCeiling];
    AttValName = new string *[AttCeiling];
    SpecialStatus = new char[AttCeiling];
#endif

    while ( ReadName(Nf, Buffer) )
    {
	    if ( Delimiter != ':' ) 
			Error(1, Buffer, "");
        if ( ++MaxAttNo >= AttCeiling )//expand space
	    {
	        AttCeiling += 100;
#ifdef _WIN32	       
		AttName = (string *) realloc(AttName, AttCeiling*sizeof(string));
	        MaxAttValNo = (short *) realloc(MaxAttValNo, AttCeiling*sizeof(short));
	        AttValName = (string **) realloc(AttValName, AttCeiling*sizeof(string *));
	        SpecialStatus = (char *) realloc(SpecialStatus, AttCeiling);
#endif 

#ifdef __GNUC__
		AttName = new string[AttCeiling];
	        MaxAttValNo = new short[AttCeiling];
		AttValName = new string *[AttCeiling];
		SpecialStatus = new char[AttCeiling];
#endif
	    }
	    AttName[MaxAttNo] = string(Buffer);
	    SpecialStatus[MaxAttNo] = 0;
	    MaxAttValNo[MaxAttNo] = 0;
	    ValCeiling = 100;
	   // cout <<"AttName: "<< AttName[MaxAttNo] << endl;
#ifdef _WIN32
	    AttValName[MaxAttNo] = (string *) calloc(ValCeiling, sizeof(string));
#endif

#ifdef __GNUC__
	    AttValName[MaxAttNo] = new string[ValCeiling];
#endif
	    do
	    {
	        if ( ! ( ReadName(Nf, Buffer) ) ) 
				Error(2, AttName[MaxAttNo], "");
	        if ( ++MaxAttValNo[MaxAttNo] >= ValCeiling )               	            {
		        ValCeiling += 100;
#ifdef _WIN32
		        AttValName[MaxAttNo] =(string *) realloc(AttValName[MaxAttNo], ValCeiling*sizeof(string));
#endif

#ifdef __GNUC__
			AttValName[MaxAttNo] = new string[ValCeiling];
#endif
	        }
            AttValName[MaxAttNo][MaxAttValNo[MaxAttNo]] = string(Buffer);
      
           // cout <<"AttValName: "<< AttValName[MaxAttNo][MaxAttValNo[MaxAttNo]] << endl;       

	    }while ( Delimiter == ',' );
	    if ( MaxAttValNo[MaxAttNo] == 1 )
	    {
	        /*  Check for special treatment  */
	        if (!strcmp(Buffer, "continuous") )
	        {	
				MaxContAttNo++;
			}
	         else
	        {
		     /*  Cannot have only one discrete value for an attribute  */
		         Error(3, AttName[MaxAttNo], "");
	        }
	        MaxAttValNo[MaxAttNo] = 0;
	    }
	    else if ( MaxAttValNo[MaxAttNo] > MaxDiscrValNo ) 
			MaxDiscrValNo = MaxAttValNo[MaxAttNo];
		//cout << "MaxContAttNo: " <<  MaxContAttNo << endl;
    }
    fclose(Nf);
}
void CTrain_NavieBayes::Error(int n, string s1, string s2)
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

	case 4:cout<<"case "<< MaxItemNo+1<<"'s value of '"<<s2<<"' for attribute "<<s1<<"is illegal"<<endl;//Error(4, AttName[AttNo], name);		   
		break;

	case 5:cout<<"case "<<MaxItemNo+1<<"'s class of '"<<s2<<"' is illegal"<<endl;
		break;
    } 
	cout<<"process stop!"<<endl;
    exit(1);
}
bool CTrain_NavieBayes::ReadName(FILE *f,char *Buffer)
{
	register char *Sp=Buffer;
	register int c;
	//Skip the comment Begin with the '|'
	 while ( ( c = getc(f) ) == '|' || space(c) )
    {
	    if ( c == '|' ) 
			SkipComment;
    }
	//Returns false if there is no data in addition to the comments
    if ( c == EOF )
    {
	    Delimiter = EOF;
	    return false;
    }
	//read data
	 while ( c != ':' && c != ',' && c != '\n' && c != '|' && c != EOF )
    {
	    if ( c == '.' )
	    {
	        if ( ( c = getc(f) ) == '|' || space(c) ) //In '. 'if it is behind the carriage returns annotation quit
		{
	#ifdef __GNUC__
			c = getc(f);
	#endif
			break;
                }
	        *Sp++ = '.';
	    }

	    if ( c == '\\' )//skip if it is '\\'
	    {
	        c = getc(f);
	    }
	    *Sp++ = c;
	    if ( c == ' ' )//skip spaces
	    {
	        while ( ( c = getc(f) ) == ' ' );
	    }
	    else
	    {
	        c = getc(f);
	    }
    }

    if ( c == '|' ) 
	    SkipComment;
    Delimiter = c;
    /*  Strip trailing spaces  */
    while ( space(*(Sp-1)) ) Sp--;

    *Sp++ = '\0';
    return true;
}
void CTrain_NavieBayes::getdata(GetMetricData data)
{
	TrainData=data;
	int ItemNo;
	int	ItemSpace=0;
	MaxItemNo=TrainData.metricData.size()-1;
	for(ItemNo=0;ItemNo<=MaxItemNo;++ItemNo)
	{
	    /*  Make sure there is room for another item  */
	    if ( ItemNo >= ItemSpace )
	    {
	        if ( ItemSpace )
	        {
		        ItemSpace += Inc;
		        Item = (Description *)realloc(Item, ItemSpace*sizeof(Description));
	        }
	        else
	        {
		        Item = (Description *)malloc((ItemSpace=Inc)*sizeof(Description));
	         }
	    }
	    Item[ItemNo] = GetDescription(ItemNo,TrainData);      //Item[ItemNo] store the training data
    }
	InitialiseData();
	InitialiseWeights();
	//SplitContinuousAtt(1);
	AnalyzeData();
}
Description CTrain_NavieBayes::GetDescription(int itemNo,GetMetricData Df)
{
	int AttNo;/* attribute number, 0..MaxAttNo */
	int Dv;
    Description Dvec;
	Dvec = (Description)calloc((MaxAttNo+1)*2, sizeof(AttValue));
    for(AttNo=0;AttNo<=MaxAttNo;AttNo++)
    {
		/*  Continuous value  */
		Dvec[AttNo].continuousVal=Df.metricData[itemNo][AttNo];      
    }
	for(int i=0;i<MaxClassNo+1;++i)
	{
		if(Df.metricDataLabel[itemNo]==ClassName[i])
			Dv=i;
	}

	Dvec[MaxAttNo+1].labelPosition=Dv;//the position of classlabel in ClassName
	return Dvec;
}
/**********************************************************************************************/
/*									                                                          */
/*	Locate value Val in List[First] to List[Last]			                                  */
/**********************************************************************************************/

void CTrain_NavieBayes::AnalyzeData()
{   
	for(int ClassNo=0;ClassNo<=MaxClassNo;ClassNo++)//initialize class value frequency
    {
	    ClassFreq[ClassNo] = 0;
    }
	for(int ItemNo=0;ItemNo<=MaxItemNo;ItemNo++)//statistics class value frequency
    { 
		ClassFreq[Item[ItemNo][MaxAttNo+1].labelPosition] += Weight[ItemNo];//MaxAttNo+1 is class attribute£¬0~MaxAttNo is none class attribute
    } 
}
void CTrain_NavieBayes::InitialiseData()
{
	ClassFreq = (double *) calloc(MaxClassNo+1, sizeof(double));
	Weight = (double *) calloc(MaxItemNo+1, sizeof(double));
	Freq  = (double **) calloc(MaxDiscrValNo+1, sizeof(double *));
    for(int DiscrValueNo=0;DiscrValueNo<=MaxDiscrValNo;DiscrValueNo++)
    {
	    Freq[DiscrValueNo]=(double *) calloc(MaxClassNo+1, sizeof(double));
    }
	ValFreq= (double *) calloc(MaxDiscrValNo+1, sizeof(double));
	SplitGain = (double *) calloc(MaxItemNo+1, sizeof(double));
    SplitInfo = (double *) calloc(MaxItemNo+1, sizeof(double));
	Gain	= (double *) calloc(MaxContAttNo+1, sizeof(double));
    //Info	= (float *) calloc(MaxContAttNo+1, sizeof(float));
    Bar		= (double *) calloc(MaxContAttNo+1, sizeof(double));
}

/*************************************************************************/
/*								 	 */
/*		Initialise the weight of each item		 	 */
/*								 	 */
/*************************************************************************/
void CTrain_NavieBayes::InitialiseWeights()
/*  -----------------  */
{
    int ItemNo;
    for(ItemNo=0;ItemNo<=MaxItemNo;ItemNo++)
    {
        Weight[ItemNo] = 1.0;
    }
}
double CTrain_NavieBayes::SplitContinuousAtt(int AttNo)
{   
	double BaseInfo;
	double LowItems = 0;
	int c;
	double MinSplit;
	double AvGain=0; 
	int Tries=0;
	ResetFreq(2);
	Quicksort(0, MaxItemNo,AttNo);
	for(int i=0;i<=MaxItemNo;i++) //#define  Class(Case)		Case[MaxAtt+1]._discr_val
    {
		Freq[ 2 ][ Item[i][MaxAttNo+1].labelPosition ] += Weight[i];
	    SplitGain[i] = -(float)Epsilon;
	    SplitInfo[i] = 0;
     }
     BaseInfo = TotalInfo(Freq[2], 0, MaxClassNo) /(MaxItemNo+1);

	 MinSplit = (double)0.10 * (MaxItemNo+1) / (MaxClassNo + 1);
     if ( MinSplit <= MINOBJS ) 
		MinSplit =(double) MINOBJS;
     else if ( MinSplit > 25 ) 
		MinSplit = 25;     
     for(int i=0;i<=MaxItemNo-1;i++)
     {
		 c = Item[i][MaxAttNo+1].labelPosition;//#define  Class(Case)		Case[MaxAtt+1]._discr_val
	    LowItems   += Weight[i];
	    Freq[1][c] += Weight[i];
	    Freq[2][c] -= Weight[i];
	    if ( LowItems < MinSplit ) 
			continue;
	    else if ( LowItems > MaxItemNo+1 - MinSplit ) 
			break;
		if (Item[i][AttNo].continuousVal<Item[i+1][AttNo].continuousVal- 1E-5 )
	    {
	        ValFreq[1] = LowItems;
	        ValFreq[2] = MaxItemNo+1 - LowItems;
	        SplitGain[i] = ComputeGain(BaseInfo,2,MaxItemNo+1);
	        //SplitInfo[i] = TotalInfo(ValFreq, 0, 2) / Items;
	     }
	 }
	 double BestVal = 0;
     int BestI   = None;
	 double Val;
     for(int i=0;i<=MaxItemNo-1;i++)
     {
	     if((Val = SplitGain[i])> BestVal )
	     {
	         BestI   = i;
	         BestVal = Val;
	     }
		 if(BestI<0)   //make the middle value to the splitpoint when all split point's value < 0
		 {
			 BestI = MaxItemNo/2;
		 }
     }
	 Bar[AttNo]  = (Item[BestI][AttNo].continuousVal +Item[BestI+1][AttNo].continuousVal) / 2;
	 Gain[AttNo] = BestVal;
	 return Bar[AttNo];
	
}
double CTrain_NavieBayes::ComputeGain(double BaseInfo,int MaxValNo,double TotalItems)
/*    -----------  */
{
    int v;
    double ThisInfo=0.0, ThisGain;
    short ReasonableSubsets=0;

    /*  Check whether all values are unknown or the same  */

    if ( ! TotalItems ) return -(float)Epsilon;

    /*  There must be at least two subsets with MINOBJS items  */

    for(v=1;v<=MaxValNo;v++)
    {
	    if ( ValFreq[v] >= MINOBJS ) 
			ReasonableSubsets++;
    }
    if ( ReasonableSubsets < 2 ) 
		return -(float)Epsilon;

    /*  Compute total info after split, by summing the
	info of each of the subsets formed by the test  */

    for(v=1;v<=MaxValNo;v++)
    {
	    ThisInfo += TotalInfo(Freq[v], 0, MaxClassNo);
    }

    /*  Set the gain in information for all items, adjusted for unknowns  */

    ThisGain=BaseInfo - ThisInfo / TotalItems;
    return ThisGain;
}
/*************************************************************************/
/*									 */
/*  Zero the frequency tables Freq[][] and ValFreq[] up to MaxVal	 */
/*									 */
/*************************************************************************/
void CTrain_NavieBayes::ResetFreq(double MaxDiscrValNo)
{
    int DiscrValue;
    int ClassNo;

    for(DiscrValue=0;DiscrValue<=MaxDiscrValNo;DiscrValue++)
    { 
	    for(ClassNo=0;ClassNo<=MaxClassNo;ClassNo++)
	    {
	        Freq[DiscrValue][ClassNo] = 0;
	    }
	    ValFreq[DiscrValue] = 0;
    } 
}
void  CTrain_NavieBayes::SwapItem(int ItemA,int ItemB)
/*   ----  */
{
    register Description Hold;
    register double HoldW;
    Hold = Item[ItemA];
    Item[ItemA] = Item[ItemB];
    Item[ItemB] = Hold;
    HoldW = Weight[ItemA];
    Weight[ItemA] = Weight[ItemB];
    Weight[ItemB] = HoldW;
}
void CTrain_NavieBayes::Quicksort(int ItemNoFp,int ItemNoLp ,int AttNo)
{
	register int Lower, Middle;
    register double Thresh;
    register int i;
    if ( ItemNoFp < ItemNoLp )
    {
		Thresh =Item[ItemNoLp][AttNo].continuousVal;		
		/*  Isolate all items with values <= threshold  */		
		Middle = ItemNoFp;		
		for ( i = ItemNoFp ; i < ItemNoLp ; i++ )
		{ 
			if ( Item[i][AttNo].continuousVal<= Thresh )
			{ 
				if ( i != Middle ) 
					SwapItem(Middle, i);
				Middle++; 
			} 
		} 		
		/*  Extract all values equal to the threshold  */		
		Lower = Middle - 1;		
		for ( i = Lower ; i >= ItemNoFp ; i-- )
		{
			if (Item[i][AttNo].continuousVal == Thresh )
			{ 
				if ( i != Lower ) 
					SwapItem(Lower, i);
				Lower--;
			} 
		} 
		
		/*  Sort the lower values  */
		
		Quicksort( ItemNoFp, Lower, AttNo);
		
		/*  Position the middle element  */
		
		SwapItem(Middle,ItemNoLp);
		
		/*  Sort the higher values  */
		
		Quicksort(Middle+1,  ItemNoLp, AttNo);
    }
}
double CTrain_NavieBayes::TotalInfo(double V[],int MinVal,int MaxVal)
/*--------- */
{
    int v;
    double Sum=0.0;
    double N, TotalItems=0;
    for(v=MinVal;v<=MaxVal;v++)
    {
	    N = V[v];
	    Sum += N * (double) Log(N);
	    TotalItems += N;
    }
    return TotalItems *(double)Log(TotalItems) - Sum;
}

void CTrain_NavieBayes::trainmodel(const char *showClassificationModel)
{   
	ofstream outfile(showClassificationModel,ofstream::app);
	outfile<<"@trainmodel"<<endl;
	outfile<<MaxClassNo+1<<" "<<MaxAttNo+1<<" "<<MaxItemNo+1<<endl;
	for(int i=0; i<MaxClassNo+1; ++i)
	{
		outfile<<ClassFreq[i]<<" ";
	}
	outfile<<endl;
	for(int j=0; j<MaxClassNo+1; ++j)
	{
		outfile<<ClassName[j]<<" ";
	}
	outfile<<endl;

	int SplitVal;
	Cache();//allocate memory for model
	for(int i=0;i<=MaxAttNo;i++)
	{
		if(!MaxAttValNo[i])
			TrainingModelSplitPoint[i]=SplitContinuousAtt(i);
		outfile<<TrainingModelSplitPoint[i]<<" ";
	}
	outfile<<endl;
	for(int i=0;i<=MaxItemNo;i++)
	{
		for(int j=0;j<=MaxAttNo;j++)
		{   
			if(MaxAttValNo[j])
				TrainingModel[Item[i][MaxAttNo+1].labelPosition][j][Item[i][MaxAttNo+1].labelPosition]++;
			else
			{   
				if(Item[i][j].continuousVal<=TrainingModelSplitPoint[j])
					SplitVal=1;
				else
					SplitVal=2;
				TrainingModel[Item[i][MaxAttNo+1].labelPosition][j][SplitVal]++;
			}
			outfile<<Item[i][MaxAttNo+1].labelPosition<<" ";      //class label
			outfile<<SplitVal<<" ";                          //SplitVal 1:<splitpoint   2:>splitpoint
			outfile<<TrainingModel[Item[i][MaxAttNo+1].labelPosition][j][SplitVal]<<" ";   //TrainingModel[classlabel][attributeNum][SplitVal]
		}
		outfile<<endl;
	}
	outfile.close();
}
void CTrain_NavieBayes::Cache()
{   
	TrainingModel=(double ***) calloc(MaxClassNo+1,sizeof(double **));
	for(int i=0;i<=MaxClassNo;i++)
	{
        TrainingModel[i]=(double **) calloc(MaxAttNo+1,sizeof(double *));
		for(int j=0;j<=MaxAttNo;j++)
		{   
			if(MaxAttValNo[j])
				TrainingModel[i][j]=(double *)calloc(MaxAttValNo[j]+1,sizeof(double));
			else
                TrainingModel[i][j]=(double *)calloc(3,sizeof(double));
		}
	}
	TrainingModelSplitPoint=(double *)calloc(MaxAttNo+1,sizeof(double));
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
void CTrain_NavieBayes::TrainModel(char *classifyMethod,vector<shared_ptr<CMetricData> > *traindata,vector<string> trainDataLabel, CMetricDistance *metric,CPivotSelectionMethod *pivotselectionmethod, int pivotNum,char *pivotsAndTrainModelFileName,char *trainDataFileName,int dim)
{
	CDatasetInMetricSpace getTrainData;
	GetMetricData M_traindata;
	M_traindata=getTrainData.getMetricTrainData(classifyMethod,traindata,trainDataLabel,metric,pivotselectionmethod,pivotNum,pivotsAndTrainModelFileName,dim);
	/*ofstream outfile("SourceFiles/util/result/M_traindata.txt",ofstream::out);
	for(int i=0; i<traindata->size();++i)
	{
		for(int j=0; j<pivotNum; ++j)
		{
			outfile<<M_traindata.metricData[i][j]<<" ";
		}                                                          
		outfile<<M_traindata.metricDataLabel[i]<<endl;
	}
	system("pause");*/

	getname(trainDataFileName);
	if(MaxAttNo+1 != pivotNum)
	{
		cout<<"The number of attributes in .names file is not matching the number of pivots! Please note the check!"<<endl;
		exit(1);
	}
	getdata(M_traindata);
	trainmodel(pivotsAndTrainModelFileName);
}
