/**
 *@ Filename: classificaiton.cpp
 *@ Implement training model and the test of classification algorithm
 *@ Author: Ping Li
 *@ Version: 2014-09-28
*/

#include "../../../HeaderFiles/metricdata/MetricData.h"
#include "../../../HeaderFiles/metricdata/DoubleVector.h"
#include "../../../HeaderFiles/metricdata/Image.h"
#include "../../../HeaderFiles/metricdata/StringObject.h"
#include "../../../HeaderFiles/metricdata/DNA.h"
#include "../../../HeaderFiles/metricdata/Peptide.h"
#include "../../../HeaderFiles/metricdata/TimeSeries.h"
#include "../../../HeaderFiles/metricdata/DNA_Classify.h"

#include "../../../HeaderFiles/metricdistance/MetricDistance.h"
#include "../../../HeaderFiles/metricdistance/EuclideanDistance.h"
#include "../../../HeaderFiles/metricdistance/EditDistance.h"
#include "../../../HeaderFiles/metricdistance/ImageMetric.h"
#include "../../../HeaderFiles/metricdistance/DNAMetric.h"
#include "../../../HeaderFiles/metricdistance/PeptideMetric.h"
#include "../../../HeaderFiles/metricdistance/LInfinityDistance.h"
#include "../../../HeaderFiles/metricdistance/TimeSeriesMetric.h"

#include "../../../HeaderFiles/index/PivotSelectionMethod.h"
#include "../../../HeaderFiles/index/FFTPivotSelectionMethod.h"
#include "../../../HeaderFiles/index/PCAOnFFT.h"
#include "../../../HeaderFiles/index/IncrementalSelection.h"

#include "../../../HeaderFiles/classifier/Train_Knn.h"
#include "../../../HeaderFiles/classifier/Test_Knn.h"
#include "../../../HeaderFiles/classifier/util/NavieBayes.h"
#include "../../../HeaderFiles/classifier/Train_NavieBayes.h"
#include "../../../HeaderFiles/classifier/Test_NavieBayes.h"
#include "../../../HeaderFiles/classifier/Train_C4.5.h"
#include "../../../HeaderFiles/classifier/Test_C4.5.h"
#include "../../../HeaderFiles/classifier/Train_SVM.h"
#include "../../../HeaderFiles/classifier/Test_SVM.h"

#include "../../../HeaderFiles/classifier/util/DatasetInMetricSpace.h"
#include "../../../HeaderFiles/classifier/util/MetricDataFormat.h"
#include "../../../HeaderFiles/classifier/util/ReadLabel.h"
#include "../../../HeaderFiles/classifier/Train.h"
#include "../../../HeaderFiles/classifier/Test.h"

#include "../../../HeaderFiles/metricdistance/CountedMetric.h"

#include<memory>
#include<iostream>
#include<string>
#include<vector>

using namespace std;

extern void joinCharArray(char*&dest,const char *sor);

void Train(char *dataType,char *disfun,char *pivotSelectionMethod,int numPivot,char *classifyMethod,char *trainDataFileName,int initialSize,int dim,char *pivotsAndTrainModelFileName,int coordinate)
{
#ifdef __GNUC__
	char *newTrainDataFileName="../../../SourceFiles/util/data/";
#endif

#ifdef _WIN32
	char *newTrainDataFileName="./SourceFiles/util/data/";
#endif
	joinCharArray(newTrainDataFileName,trainDataFileName);

#ifdef __GNUC__
	char *newPivotsAndTrainModelFileName="../../../SourceFiles/util/result/";
#endif

#ifdef _WIN32
	char *newPivotsAndTrainModelFileName="./SourceFiles/util/result/";
#endif
	joinCharArray(newPivotsAndTrainModelFileName,pivotsAndTrainModelFileName);

	vector<shared_ptr<CMetricData> > *traindata=0; //store the traindata
	vector<string> trainDataLabel;

	if(strcmp(dataType,"vector")==0)
	{
		traindata=CDoubleVector::loadData(newTrainDataFileName,initialSize,dim);

		CReadLabel readTrainDataLabel;
		trainDataLabel = readTrainDataLabel.loadLabel(newTrainDataFileName,initialSize,dataType);
	}

	else if(strcmp(dataType,"string")==0)
	{
		traindata=CStringObject::loadData(newTrainDataFileName,initialSize);

		CReadLabel readTrainDataLabel;
		trainDataLabel = readTrainDataLabel.loadLabel(newTrainDataFileName,initialSize,dataType);
	}
	else if(strcmp(dataType,"dna")==0)
	{
		traindata=CDNA_CLASSIFY::loadData(newTrainDataFileName,initialSize,dim);

		CReadLabel readTrainDataLabel;
		trainDataLabel = readTrainDataLabel.loadLabel(newTrainDataFileName,initialSize/57,dataType);
	}

	else if(strcmp(dataType,"time_series")==0)
	{
		traindata=CTimeSeries::loadData(newTrainDataFileName,initialSize,dim);

		CReadLabel readTrainDataLabel;
		trainDataLabel = readTrainDataLabel.loadLabel(newTrainDataFileName,initialSize,dataType);
	}

	else if(strcmp(dataType,"image")==0)
	{
		traindata=CImage::loadData(newTrainDataFileName,initialSize,dim);
	}
	
	else if(strcmp(dataType,"peptide")==0)
	{
		traindata=CPeptide::loadData(newTrainDataFileName,initialSize,dim);
	}

	CMetricDistance *metric=0;
	if(strcmp(disfun,"EuclideanDistance")==0)
	{
		metric = new CEuclideanDistance;
	}
	else if(strcmp(disfun,"EditDistance")==0)
	{
		metric = new CEditDistance;
	}
	else if(strcmp(disfun,"TimeSeriesMetric")==0)
	{
		metric = new CTimeSeriesMetric;
	}
	else if(strcmp(disfun,"ImageMetric")==0)
	{
		metric = new CImageMetric;
	}
	else if(strcmp(disfun,"DNAMetric")==0)
	{
		metric = new CDNAMetric;
	}
	else if(strcmp(disfun,"PeptideMetric")==0)
	{
		metric = new CPeptideMetric;
	}
	else if(strcmp(disfun,"LInfinityDistance")==0)
	{
		metric = new CLInfinityDistance;
	}

	CountedMetric *cmetric=new CountedMetric(metric);
	CPivotSelectionMethod *pivotselectionmethod=0;
	if(strcmp(pivotSelectionMethod,"fft")==0)
	{
		pivotselectionmethod = new CFFTPivotSelectionMethod;
	}
	else if(strcmp(pivotSelectionMethod,"pcaonfft")==0)
	{
		pivotselectionmethod = new CPCAOnFFT(2);
	}
	else if(strcmp(pivotSelectionMethod,"incremental")==0)
	{
		pivotselectionmethod = new CIncrementalSelection(1,2);
	}
	
	if(numPivot > initialSize)
	{
		cout << "The pivot number larger than the traindata size. Please reset the number of pivot !" << endl;
		exit(0);
	}

	if(strcmp(classifyMethod,"knn")==0)
	{
		CTrain_Knn train_knn;
		train_knn.TrainModel(classifyMethod,traindata,trainDataLabel,cmetric,pivotselectionmethod,numPivot,newPivotsAndTrainModelFileName,dim,coordinate);
	}
	else if(strcmp(classifyMethod,"knnCrossValid")==0)
	{
		CTrain_Knn train_knn;
		train_knn.TrainModelUseCrossValidation(classifyMethod,traindata,trainDataLabel,cmetric,pivotselectionmethod,numPivot,newPivotsAndTrainModelFileName,dim,coordinate);
	}
	else if(strcmp(classifyMethod,"naviebayes")==0)
	{
		CTrain_NavieBayes train_naviebayes;
		train_naviebayes.TrainModel(classifyMethod,traindata,trainDataLabel,cmetric,pivotselectionmethod,numPivot,newPivotsAndTrainModelFileName,newTrainDataFileName,dim,coordinate);
	}
	else if(strcmp(classifyMethod,"c4.5")==0)
	{
		C4_5 c45;
		c45.TrainModel(classifyMethod,traindata,trainDataLabel,cmetric,pivotselectionmethod,numPivot,newPivotsAndTrainModelFileName,newTrainDataFileName,dim,coordinate);
	}
	else if(strcmp(classifyMethod,"svm")==0)
	{
		CDatasetInMetricSpace getTrainData;
		GetMetricData M_traindata;
		M_traindata=getTrainData.getMetricTrainData(classifyMethod,traindata,trainDataLabel,cmetric,pivotselectionmethod,numPivot,newPivotsAndTrainModelFileName,dim,coordinate);
		CTrain_SVM svm(M_traindata);
		svm.TrainModel(newPivotsAndTrainModelFileName);
	}
}

void Test(char *dataType,char *disfun,char *classifyMethod,char *testDataFileName,int finalSize,int dim,char *pivotsAndTrainModelFileName,char *testModelFileName,int status,int k,int splitRatio)
{
#ifdef __GNUC__
	char *newTestDataFileName="../../../SourceFiles/util/data/";
#endif

#ifdef  _WIN32
	char *newTestDataFileName="./SourceFiles/util/data/";
#endif
	joinCharArray(newTestDataFileName,testDataFileName);
	
#ifdef __GNUC__
	char *newPivotsAndTrainModelFileName="../../../SourceFiles/util/result/";
#endif

#ifdef _WIN32
	char *newPivotsAndTrainModelFileName="./SourceFiles/util/result/";
#endif
	joinCharArray(newPivotsAndTrainModelFileName,pivotsAndTrainModelFileName);

#ifdef __GNUC__
	char *newTestModelFileName="../../../SourceFiles/util/result/";
#endif

#ifdef _WIN32
	char *newTestModelFileName="./SourceFiles/util/result/";	
#endif
	joinCharArray(newTestModelFileName,testModelFileName);

	vector<shared_ptr<CMetricData> > *testdata=0; //store the testdata
	vector<string> testDataLabel;

	if(strcmp(dataType,"vector")==0)
	{
		testdata=CDoubleVector::loadData(newTestDataFileName,finalSize,dim);

		CReadLabel readTestDataLabel;
		testDataLabel=readTestDataLabel.loadLabel(newTestDataFileName,finalSize,dataType);
	}

	else if(strcmp(dataType,"string")==0)
	{
		testdata=CStringObject::loadData(newTestDataFileName,finalSize);

		CReadLabel readTestDataLabel;
		testDataLabel=readTestDataLabel.loadLabel(newTestDataFileName,finalSize,dataType);
	}
	else if(strcmp(dataType,"dna")==0)
	{
		testdata=CDNA_CLASSIFY::loadData(newTestDataFileName,finalSize,dim);

		CReadLabel readTestDataLabel;
		testDataLabel=readTestDataLabel.loadLabel(newTestDataFileName,finalSize/57,dataType);
	}

	else if(strcmp(dataType,"time_series")==0)
	{
		testdata=CTimeSeries::loadData(newTestDataFileName,finalSize,dim);

		CReadLabel readTestDataLabel;
		testDataLabel=readTestDataLabel.loadLabel(newTestDataFileName,finalSize,dataType);
	}

	else if(strcmp(dataType,"image")==0)
	{
		testdata=CImage::loadData(newTestDataFileName ,finalSize, dim);
	}
	
	else if(strcmp(dataType,"peptide")==0)
	{
		testdata=CPeptide::loadData(newTestDataFileName,finalSize,dim);
	}	

	CMetricDistance *metric=0;
	if(strcmp(disfun,"EuclideanDistance")==0)
	{
		metric = new CEuclideanDistance;
	}
	else if(strcmp(disfun,"EditDistance")==0)
	{
		metric = new CEditDistance;
	}
	else if(strcmp(disfun,"TimeSeriesMetric")==0)
	{
		metric = new CTimeSeriesMetric;
	}
	else if(strcmp(disfun,"ImageMetric")==0)
	{
		metric = new CImageMetric;
	}
	else if(strcmp(disfun,"DNAMetric")==0)
	{
		metric = new CDNAMetric;
	}
	else if(strcmp(disfun,"PeptideMetric")==0)
	{
		metric = new CPeptideMetric;
	}
	else if(strcmp(disfun,"LInfinityDistance")==0)
	{
		metric = new CLInfinityDistance;
	}

	CountedMetric *cmetric=new CountedMetric(metric);
	if(strcmp(classifyMethod,"knn")==0)
	{
		CTest_Knn test_knn;
		test_knn.TestModel(classifyMethod,testdata,testDataLabel,cmetric,newPivotsAndTrainModelFileName,newTestModelFileName,status,k,splitRatio);
	}
	else if(strcmp(classifyMethod,"naviebayes")==0)
	{
		CTest_NavieBayes test_naviebayes;
		test_naviebayes.TestModel(classifyMethod,testdata,testDataLabel,cmetric,newPivotsAndTrainModelFileName,newTestModelFileName,status,splitRatio);
	}
	else if(strcmp(classifyMethod,"c4.5")==0)
	{
		CTest_C4_5 test_c45;
		test_c45.TestModel(classifyMethod,testdata,testDataLabel,cmetric,newPivotsAndTrainModelFileName,newTestModelFileName,status,splitRatio);
	}
	else if(strcmp(classifyMethod,"svm")==0)
	{
		CDatasetInMetricSpace getTestData;
		GetMetricData M_testdata;
		if(status==0)
		{
			M_testdata=getTestData.getMetricTestData_fromTrainData(classifyMethod,testdata,testDataLabel,cmetric,newPivotsAndTrainModelFileName,splitRatio);
			printf("\n\nEvaluation on training data (%d items):\n", M_testdata.metricData.size());
		}
		else if(status==1)
		{
			M_testdata=getTestData.getMetricTestData_fromTestData(classifyMethod,testdata,testDataLabel,cmetric,newPivotsAndTrainModelFileName);
			printf("\nEvaluation on test data (%d items):\n", M_testdata.metricData.size());
		}
		CTest_SVM svm(M_testdata);
		svm.TestModel(newPivotsAndTrainModelFileName,newTestModelFileName);
	}
}
