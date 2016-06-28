#include"../../HeaderFiles/classifier/Test_SVM.h"
#include"../../HeaderFiles/classifier/Train_SVM.h"
#include"../../HeaderFiles/classifier/util/svm.h"

CTest_SVM::CTest_SVM(GetMetricData testData){

	this->svm_testSet = testData;
	this->sampleNum = testData.metricData.capacity();
	this->featureNum = testData.metricData.front().capacity();
}

CTest_SVM::~CTest_SVM() {}

vector<int> CTest_SVM::BuildLabelMap(char *file, vector<string> vector_Str){
	
	int testNum = vector_Str.capacity();
	
	vector<int> label_intVector;
	map<string, int> map_str_int;
	ifstream map_lable;
	map_lable.open(file, ios::in);
	while (!map_lable.eof()){
		string lable_str;
		int label_int;
		map_lable >> lable_str >> label_int;
		map_str_int.insert(pair<string, int>(lable_str, label_int));
	}

	for (int i = 0; i < testNum; i++)
	{
		label_intVector.push_back(map_str_int[vector_Str[i]]);
	}
	return label_intVector;
}

list<pointTest> CTest_SVM::LoadTestData(){

	list<pointTest> point_list_testdata;

	vector<int> labelInt = BuildLabelMap("LabelMap.txt", svm_testSet.metricDataLabel);

	for (int i = 0; i < sampleNum; i++)
	{
		double *line = new double[featureNum];
		//cout << svm_testSet.metricDataLabel[i] << " ";
		for (int j = 0; j < featureNum; j++)
		{
			line[j] = svm_testSet.metricData[i][j];
		}
		pointTest p;
		p.feature = line;
		p.value = labelInt[i];
		point_list_testdata.push_back(p);
	}
	return point_list_testdata;
}

void CTest_SVM::Predict(list<pointTest> testData, char *pivotsAndTrainModelFileName,char *testModelFileName){

	cout << "Predict data begin!" << endl;

	svm_model *model = svm_load_model(pivotsAndTrainModelFileName);
	svm_node *x_space = new svm_node[featureNum + 1];
	double label_predict, accuracy;
	int correct = 0;
	int err = 0;
	for (list<pointTest>::iterator q = testData.begin(); q != testData.end(); q++)
	{

		for (int i = 0; i < featureNum; i++)
		{

			x_space[i].index = i + 1;
			x_space[i].value = q->feature[i];

		}
		x_space[featureNum].index = -1;


		label_predict = svm_predict(model, x_space);
		if (label_predict == q->value){
			//cout << "right = " << label_predict << endl;
			correct++;
		}
		else
		{
			cout << "error" << "   " << label_predict << endl;
			err++;
		}
	}
	accuracy = (correct * 100) / (correct + err);
	//cout << "error times = " << err << endl;
	cout << "Predict data end!" << endl;
	cout << "Accuracy=  " << accuracy << "%  " << "(" << correct << "/" << correct + err << ")" ;

	ofstream outfile(testModelFileName,ofstream::out);
	outfile << "Predict data end!" << endl;
	outfile << "Accuracy=  " << accuracy << "%  " << "(" << "correct:" <<correct << "/" << "correct + err:" << correct + err << ")" ;
	outfile.close();

	delete[] x_space;
}

void CTest_SVM::TestModel(char *pivotsAndTrainModelFileName,char *testModelFileName){
	list<pointTest> testData = LoadTestData();
	Predict(testData,pivotsAndTrainModelFileName,testModelFileName);
}
