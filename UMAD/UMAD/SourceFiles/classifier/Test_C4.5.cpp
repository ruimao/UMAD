#include "../../HeaderFiles/classifier/Test_C4.5.h"

CTest_C4_5::CTest_C4_5()
{
	VERBOSITY = 0 ;		/*v: verbosity level (0 = none) */
	TRIALS = 1 ;		/*t: number of trees to be grown */
	ClassSum = 0;
}

CTest_C4_5::~CTest_C4_5()
{

}

/*************************************************************************/
/*									 */
/*	Count the nodes in a tree					 */
/*									 */
/*************************************************************************/	
int CTest_C4_5::TreeSize(Tree Node)
/*  --------  */
{
    int Sum=0;
    DiscrValue v;

    if ( Node->NodeType )
    {
		ForEach(v, 1, Node->Forks)
		{
			Sum += TreeSize(Node->Branch[v]);
		}
    }

    return Sum + 1;
}

/*************************************************************************/
/*                                                              	 */
/*  Classify a case description using the given subtree by adjusting	 */
/*  the value ClassSum for each class					 */
/*                                                              	 */
/*************************************************************************/
void CTest_C4_5::Classify(Descript CaseDesc,Tree T,float Weight)
/*  --------  */
{
    DiscrValue v, dv;
    float Cv;
    Attribute a;
    ClassNo c;

    switch ( T->NodeType )
    {
        case 0:  /* leaf */

			if ( T->Items > 0 )
			{
				/*  Update from ALL classes  */

				ForEach(c, 0, MaxClass-1)
				{
					if ( T->ClassDist[c] )
					{
						ClassSum[c] += Weight * T->ClassDist[c] / T->Items;
					}
				}
			}
			else
			{
				ClassSum[T->Leaf] += Weight;
			}

			return;

	case BrDiscr:  /* test of discrete attribute */

			a = T->Tested;
			v = DVal(CaseDesc, a);

			if ( v && v <= T->Forks )	/*  Make sure not new discrete value  */
			{
				Classify(CaseDesc, T->Branch[v], Weight);
			}
			else
			{
				ForEach(v, 1, T->Forks)
				{
					Classify(CaseDesc, T->Branch[v], 
						 (Weight * T->Branch[v]->Items) / T->Items);
				}
			}

			return;

	case ThreshContin:  /* test of continuous attribute */

			a = T->Tested;
			Cv = CVal(CaseDesc, a);

			if ( Cv == Unknown )
			{
				ForEach(v, 1, 2)
				{
					Classify(CaseDesc, T->Branch[v], 
						 (Weight * T->Branch[v]->Items) / T->Items);
				}
			}
			else
			{
				v = ( Cv <= T->Cut ? 1 : 2 );
				Classify(CaseDesc, T->Branch[v], Weight);
			}

			return;

	case BrSubset:  /* subset test on discrete attribute  */

			a = T->Tested;
			dv = DVal(CaseDesc, a);

			if ( dv )
			{
				ForEach(v, 1, T->Forks)
				{
					if ( In(dv, T->Subset[v]) )
					{
						Classify(CaseDesc, T->Branch[v], Weight);

						return;
					}
				}
			}

	    /*  Value unknown or not found in any of the subsets  */

	    ForEach(v, 1, T->Forks)
	    {
			Classify(CaseDesc, T->Branch[v], 
		         (Weight * T->Branch[v]->Items) / T->Items);
	    }

	    return;
    } 
}

/*************************************************************************/
/*                                                              	 */
/*  Categorize a case description using the given decision tree		 */
/*                                                              	 */
/*************************************************************************/
ClassNo CTest_C4_5::Category(Descript CaseDesc,Tree DecisionTree,short VERBOSITY) 
/*       --------  */
{ 
    ClassNo c, BestClass;

    if ( ! ClassSum )
    {
		ClassSum = (float *) malloc((MaxClass+1) * sizeof(float));
    }

    ForEach(c, 0, MaxClass-1)
    {
		ClassSum[c] = 0;
    }

    Classify(CaseDesc, DecisionTree, 1.0);

    BestClass = 0;
    ForEach(c, 0, MaxClass-1)
    {
		Verbosity(5) 
			printf("class %s weight %.2f\n", ClassName[c], ClassSum[c]);

		if ( ClassSum[c] > ClassSum[BestClass] ) 
			BestClass = c;
    }

    return BestClass;
}

void CTest_C4_5::PrintConfusionMatrix(ItemNo *ConfusionMat)
{
    short Row, Col;

    if ( MaxClass > 20 ) return;  /* Don't print nonsensical matrices */

    /*  Print the heading, then each row  */

    printf("\n\n\t");
    ForEach(Col, 0, MaxClass-1)
    {
	printf("  (%c)", 'a' + Col);
    }

    printf("\t<-classified as\n\t");
    ForEach(Col, 0, MaxClass-1)
    {
	printf(" ----");
    }
    printf("\n");

    ForEach(Row, 0, MaxClass-1)
    {
		printf("\t");
		ForEach(Col, 0, MaxClass-1)
		{
			if ( ConfusionMat[Row*(MaxClass) + Col] )
			{
				printf("%5d", ConfusionMat[Row*(MaxClass) + Col]);
			}
			else
			{
				printf("     ");
			}
		}
		printf("\t(%c): class %s\n", 'a' + Row, ClassName[Row]);
	}
	printf("\n");
}


/*************************************************************************/
/*									 */
/*	Print report of errors for each of the trials			 */
/*									 */
/*************************************************************************/
void CTest_C4_5::Evaluate(Boolean CMInfo,short Saved,short TRIALS,short VERBOSITY,GetMetricData data,char *pivotsAndTrainModelFileName)
/*  --------  */
{
	GetTree(pivotsAndTrainModelFileName);
	GetData(data);

    ClassNo RealClass, PrunedClass;
    short t;
    ItemNo *ConfusionMat = 0, i, RawErrors, PrunedErrors;

	ConfusionMat = (ItemNo *) calloc((MaxClass)*(MaxClass), sizeof(ItemNo));

    printf("\n");

    //if ( TRIALS > 1 )
    //{
		//printf("Trial\t Before Pruning           After Pruning\n");
		//printf("-----\t----------------   ---------------------------\n");
    //}
    //else
    //{
		printf("\t Before Pruning           After Pruning\n");
		printf("\t----------------   ---------------------------\n");
	//}
    printf("\tSize      Errors   Size      Errors   Estimate\n\n");

    ForEach(t, 0, TRIALS-1)
    {
		RawErrors = PrunedErrors = 0;

		ForEach(i, 0, MaxTestItem)
		{
			RealClass = Item[i][MaxAtt]._discr_val;

			if ( Category(Item[i], Raw[t],VERBOSITY) != RealClass ) 
				
				RawErrors++;

			PrunedClass = Category(Item[i], Pruned[t],VERBOSITY);

			if ( PrunedClass != RealClass ) 
				
				PrunedErrors++;

			ConfusionMat[RealClass*(MaxClass)+PrunedClass]++;
		}
    
		//printf("\t%4d", t);

		printf("\t%4d  %3d(%4.1f%%)   %4d  %3d(%4.1f%%)    (%4.1f%%)%s\n",
	       TreeSize(Raw[t]), RawErrors, 100.0*RawErrors / ((MaxTestItem-1)+1.0),
	       TreeSize(Pruned[t]), PrunedErrors, 100.0*PrunedErrors / ((MaxTestItem-1)+1.0),
	       100 * Pruned[t]->Errors / Pruned[t]->Items,
	       ( t == Saved ? "   <<" : "" ));
    }

	PrintConfusionMatrix(ConfusionMat);
	free(ConfusionMat);
}


/*************************************************************************/
/*									 */
/*	Allocate space then copy string into it				 */
/*									 */
/*************************************************************************/
String CTest_C4_5::CopyString(String x)
{
    char *s;
    s = (char *) calloc(strlen(x)+1, sizeof(char));
    strcpy(s, x);
    return s;
}


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
Descript CTest_C4_5::GetDescription(int itemNo,GetMetricData Df)
/*          ---------------  */
{
    short Att;
    int Dv;
    float Cv;
    Descript Dvec;
	Dvec = (Descript)calloc(MaxAtt+2, sizeof(AttributeValue));
	for(Att=0;Att<MaxAtt;Att++)
    {
		/*  Continuous value  */
		Dvec[Att]._cont_val=Df.metricData[itemNo][Att];      
    }

	for(int i=0;i<MaxClass;++i)
	{
		if(Df.metricDataLabel[itemNo]==ClassName[i])
			Dv=i;
	}

	Dvec[MaxAtt]._discr_val=Dv;//the position of classlabel in ClassName
	
	return Dvec;
}


/*************************************************************************/
/*									 */
/*  Read raw case descriptions from M_traindata.		 */
/*									 */
/*  On completion, cases are stored in array Item in the form		 */
/*  of Descriptions (i.e. arrays of attribute values), and		 */
/*  MaxItem is set to the number of data items.				 */
/*									 */
/*************************************************************************/
void CTest_C4_5::GetData(GetMetricData data)
{
	TestData = data;
    ItemNo ItemNo=0,ItemSpace=0;
    MaxTestItem=TestData.metricData.size()-1;

	for(ItemNo=0;ItemNo<=MaxTestItem;++ItemNo)
	{
	    /*  Make sure there is room for another item  */
	    if ( ItemNo >= ItemSpace )
	    {
	        if ( ItemSpace )
	        {
		        ItemSpace += Inc;
		        Item = (Descript *)realloc(Item, ItemSpace*sizeof(Descript));
	        }
	        else
	        {
		        Item = (Descript *)malloc((ItemSpace=Inc)*sizeof(Descript));
	         }
	    }
	    Item[ItemNo] = GetDescription(ItemNo,TestData);      //Item[ItemNo] store the training data
    }
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

Tree CTest_C4_5::LoadTree(ifstream &infile)
{
	Tree Node;
	Node = (Tree ) calloc(1, sizeof(TreeRec));	
	Node->ClassDist = (ItemCount *) calloc(MaxClass, sizeof(ItemCount));
	Node->Branch = (Tree *) calloc(1, sizeof(Tree));
	Node->Subset = (Set *) calloc(100,sizeof(Set));

	infile >> Node->NodeType;
	infile >> Node->Leaf;
	infile >> Node->Items;
	infile >> Node->Errors;
	for(int i=0; i<MaxClass; ++i)
	{
		infile >> Node->ClassDist[i];
	}
	
	if(Node->NodeType)
	{
		infile >> Node->Tested;
		infile >> Node->Forks;
		switch(Node->NodeType)
		{
			case BrDiscr: break;

			case ThreshContin:
				infile >> Node->Cut;
				infile >> Node->Lower;
				infile >> Node->Upper;
				break;

			default:
				for(short i=1;i<=Node->Forks;++i)
				{
					infile >> Node->Subset[i];
				}
				break;
		}

		for(int j=1; j<=Node->Forks; ++j)
		{
			Node->Branch[j] = LoadTree(infile);
		}
	}
	return Node;
}

void CTest_C4_5::GetTree(char *pivotsAndTrainModelFileName)
{
	int Best;
	Raw = (Tree *) calloc(1, sizeof(Tree));
    Pruned = (Tree *) calloc(1, sizeof(Tree));

	ifstream infile(pivotsAndTrainModelFileName);
	if(!infile)
	{
		cout<<"Open training model file "<<pivotsAndTrainModelFileName<<"failed!"<<endl;
		exit(0);
	}

	string str="";	
	infile >> str;
	while(str!="@unpruned_tree")
	{
		infile >> str;
	}
	infile >> MaxClass >> MaxAtt >> MaxItem;

	ClassName = (String *) calloc(100, sizeof(String));
	char Buffer[1000];

	for(int i=0; i<MaxClass; ++i)
	{
		infile >> Buffer;
		ClassName[i] = CopyString(Buffer);
	}
	
	Raw[0] = LoadTree(infile);

	while(str!="@pruned_tree")
	{
		infile >> str;
	}
	
	infile >> Best;

	Pruned[0] = LoadTree(infile);

	infile.close();
}

void CTest_C4_5::TestModel(char *classifyMethod,vector<shared_ptr<CMetricData> > *testdata,vector<string> testDataLabel, CMetricDistance *metric,char *pivotsAndTrainModelFileName,char *testModelFileName,int status,int splitRatio)
{
	CDatasetInMetricSpace getTestData;
	GetMetricData M_testdata;
	if(status==0)
	{
		M_testdata=getTestData.getMetricTestData_fromTrainData(classifyMethod,testdata,testDataLabel,metric,pivotsAndTrainModelFileName,splitRatio);
		printf("\n\nEvaluation on training data (%d items):\n", M_testdata.metricData.size());
	}
	else if(status==1)
	{
		M_testdata=getTestData.getMetricTestData_fromTestData(classifyMethod,testdata,testDataLabel,metric,pivotsAndTrainModelFileName);
		printf("\nEvaluation on test data (%d items):\n", M_testdata.metricData.size());
	}

	int pivotsNum=getTestData.pivotsNum;

	Evaluate(status, Best,TRIALS,VERBOSITY,M_testdata,pivotsAndTrainModelFileName);
#ifdef _WIN32
	system("pause");
#endif
}
