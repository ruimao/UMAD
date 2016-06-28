#include"../../HeaderFiles/classifier/Train_SVM.h"



CTrain_SVM::CTrain_SVM(GetMetricData inputSet){
	
	this->svm_trainSet = inputSet;
	this->sampleNum = svm_trainSet.metricData.capacity();
	this->featureNum = svm_trainSet.metricData.front().capacity();
	InitClassifier();

}

CTrain_SVM::~CTrain_SVM() {}

void CTrain_SVM::InitClassifier(){

	param.svm_type = C_SVC;
	param.kernel_type = RBF;
	param.degree = 3;
	param.gamma = 0;
	param.coef0 = 0;
	param.nu = 0.5;
	param.cache_size = 100;
	param.C = 1;
	param.eps = 1e-3;
	param.p = 0.1;
	param.shrinking = 1;
	param.probability = 0;
	param.nr_weight = 0;
	param.weight_label = NULL;
	param.weight = NULL;

}
vector<int>CTrain_SVM::BuidLabelMap(vector<string> vector_Str){
	vector<int> labelVector;
	int label = 1;
	int sampleNum = vector_Str.capacity();

	set<string> label_str;
	map<string, int> label_int;

	for (int i = 0; i < sampleNum; i++)
	{
		label_str.insert(vector_Str[i]);
	}

	ofstream outmap;
	outmap.open("LabelMap.txt");
	set<string>::iterator iteratorSet;
	for (iteratorSet = label_str.begin(); iteratorSet != label_str.end(); iteratorSet++){
		label_int.insert(pair<string, int>(*iteratorSet, label));
		outmap << *iteratorSet << " " << label << endl;
		label++;
	}
	outmap.close();

	for (int i = 0; i < sampleNum; i++)
	{
		labelVector.push_back(label_int[vector_Str[i]]);
	}

	return labelVector;
}

list<point> CTrain_SVM::LoadTrianData(){

	
	list<point> point_list_triandata;


	for (int i = 0; i < sampleNum; i++)
	{
		double *line = new double[featureNum];
		cout << svm_trainSet.metricDataLabel[i] << " ";
		vector<int> label_int = BuidLabelMap(svm_trainSet.metricDataLabel);
		for (int j = 0; j < featureNum; j++)
		{

			line[j] = svm_trainSet.metricData[i][j];
		}
		point p;
		p.feature = line;
		p.value = label_int[i];
		point_list_triandata.push_back(p);
	}
	return point_list_triandata;
}

void CTrain_SVM::CallSVM(list<point> pointList, char *pivotsAndTrainModelFileName){
	
	if (pointList.empty()) return;

		// build problem
		//svm_problem prob;

		prob.l = pointList.size();
		prob.y = new double[prob.l];

		if (param.kernel_type == PRECOMPUTED)
		{
		}
		else if (param.svm_type == EPSILON_SVR ||
			param.svm_type == NU_SVR)
		{
			if (param.gamma == 0) param.gamma = 1;
		}
		else
		{
			if (param.gamma == 0) param.gamma = 0.5;

			cout << "load data begin" << endl;
			prob.x = new svm_node *[prob.l];
			int k = 0;
			for (list<point>::iterator q = pointList.begin(); q != pointList.end(); q++, k++)
			{
				svm_node *x_space = new svm_node[featureNum + 1];
				for (int i = 0; i<featureNum; i++)
				{

					x_space[i].index = i + 1;
					x_space[i].value = q->feature[i];

				}
				x_space[featureNum].index = -1;
				prob.x[k] = x_space;
				prob.y[k] = q->value;

			}
	

			// build model & classify

			cout << "train model begin" << endl;
			svm_model *model = svm_train(&prob, &param);
			cout << "train model end;" << endl;


			cout << "save model begin" << endl;
			svm_save_model(pivotsAndTrainModelFileName, model);
			cout << "save model end" << endl;
		
			svm_free_and_destroy_model(&model);
			
			for (int i = 0; i < int(pointList.size()); i++)
			{
				delete[] prob.x[i];
			}
			delete[] prob.x;
			delete[] prob.y;
		}
		free(param.weight_label);
		free(param.weight);	
}

void CTrain_SVM::TrainModel(char *pivotsAndTrainModelFileName){

	list<point> trainData = LoadTrianData();
	CallSVM(trainData,pivotsAndTrainModelFileName);

}