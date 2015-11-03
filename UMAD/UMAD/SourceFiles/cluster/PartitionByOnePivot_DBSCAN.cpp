#include "../../HeaderFiles/cluster/PartitionByOnePivot_DBSCAN.h"
#include "../../HeaderFiles/cluster/util/utilFunction.h"

#include <cmath>
#include <map>
#include<algorithm>
#include <sstream>
#define INT_MAX ((int)(~0U>>1))
#define INT_MIN (-INT_MAX - 1)

using namespace std;

/** @file	partitionByOnePivot_DBSCAN.h
* @classes	Inherited partitionByOnePivot
* @author	He Zhang
* @version	2014-07-23
*/

/**
* @class	partitionByOnePivot_BalanceTwo
* @brief	Inherited partitionByOnePivot,To achieve the DBSCAN algorithm
* @author	He Zhang
*/

/** store radius in the DBSCAN algorithm*/
vector<double>EpsTable;
int Epsi = 0;

/** change the neighbor's class labels by region query*/
double changeClId(shared_ptr<ClusteringTask>& task,map<int,double>& seeds,int ClId)
{
	map<int,double>::iterator it;
	double distance_max = INT_MIN;
	for (it = seeds.begin(); it != seeds.end(); ++it)
	{
		task->getClusterId()->at(it->first) = ClId;
		if (distance_max < it->second)
			distance_max = it->second;
	}
	return distance_max;
}

/** change the neighbor of the data point*/
map<int,double> regionQuery(shared_ptr<ClusteringTask>& task,double Point,int indictor,double Eps,vector<map<int,double> >&hashTable)
{
	double dist;
	map<int,double> searchPoints;
	vector<int>hashIndictor_Array;
	int hashIndictor = Point / Eps;
	if (hashIndictor > 0 && hashIndictor < hashTable.size()-1)
	{
		hashIndictor_Array.push_back(hashIndictor);
		hashIndictor_Array.push_back(hashIndictor-1);
		hashIndictor_Array.push_back(hashIndictor+1);
	}
	else if (hashIndictor == 0 && hashIndictor < hashTable.size()-1)
	{
		hashIndictor_Array.push_back(hashIndictor);
		hashIndictor_Array.push_back(hashIndictor+1);
	}
	else if (hashIndictor > 0 && hashIndictor == hashTable.size()-1)
	{
		hashIndictor_Array.push_back(hashIndictor);
		hashIndictor_Array.push_back(hashIndictor-1);
	}
	else
		hashIndictor_Array.push_back(hashIndictor);

	for(int i=0;i < hashIndictor_Array.size(); ++i)
	{
		int hashIndictor_temp = hashIndictor_Array[i];
		if(hashIndictor == hashIndictor_temp)
		{
			/*for (map<int,double>::iterator it = hashTable[hashIndictor].begin() ; it != hashTable[hashIndictor].end(); ++it)
			searchPoints.insert(map<int,double>::value_type(it->first,it->second));*/
			searchPoints.insert(hashTable[hashIndictor].begin(),hashTable[hashIndictor].end());
			//searchPoints = hashTable[hashIndictor];
		}
		else
		{
			for (map<int,double>::iterator it = hashTable[hashIndictor_temp].begin() ; it != hashTable[hashIndictor_temp].end(); ++it)
			{
				if(abs(Point - it->second) <= Eps)
					searchPoints.insert(map<int,double>::value_type(it->first,it->second));
			}
		}
	}
	return searchPoints;
}

/** expand clustering one data point by one data point*/
bool ExpandCluster(shared_ptr<ClusteringTask>& task,double Point,int ClusterId,double Eps,int Minpts,int pointIndex,vector<map<int,double> >&hashTable,int indictor,double& clusterMaxValue_Single,map<double,int>& outliers)
{
	map<int,double> seeds;
	map<int,double> result;
	seeds = regionQuery(task,Point,indictor,Eps,hashTable);
	if (seeds.size() < Minpts)
	{
		task->getClusterId()->at(pointIndex) = 0;
		outliers.insert(map<double,int>::value_type(Point,pointIndex));
		return false;
	}
	else 
	{
		clusterMaxValue_Single = changeClId(task,seeds,ClusterId);
		seeds.erase(pointIndex);
		while (seeds.size() > 0)
		{
			double currentP = seeds.begin()->second;
			int currentIndictor = seeds.begin()->first;
			if(Point != currentP)
			{
				result = regionQuery(task,currentP,indictor,Eps,hashTable);
				if (result.size() >= Minpts)
				{
					map<int,double>::iterator loopIterator;
					for (loopIterator=result.begin(); loopIterator != result.end(); ++loopIterator)
					{
						double resultp = loopIterator->second;
						int clusterLabel = loopIterator->first;
						if (task->getClusterId()->at(clusterLabel) == -1 || task->getClusterId()->at(clusterLabel) == 0)
						{
							if (task->getClusterId()->at(clusterLabel) ==-1)
								seeds.insert(map<int,double>::value_type(clusterLabel,resultp));
							task->getClusterId()->at(clusterLabel) = ClusterId;
							clusterMaxValue_Single < (task->getMetricData_Distance()->at(clusterLabel)) ? (task->getMetricData_Distance()->at(clusterLabel)) : clusterMaxValue_Single;
						}
					}
				}
			}
			seeds.erase(currentIndictor);
		}
		return true;
	}
}

/** select eps of the DBSCAN algorithm*/
void selectEps(shared_ptr<ClusteringTask>& task,int numpartition,int indictor)
{
	//int taskSize= task->getData()->size();
	//double randomNum=0;
	int sampleSize = task->getData()->size() > 10000 ? 10000 : task->getData()->size();
	//vector<double> taskSample;
	//int Step = 0;
	//while(sampleSize > 0)
	//{
	//	randomNum = rand()%10;
	//	randomNum /= 10;
	//	if(taskSize*randomNum > sampleSize)
	//	{
	//		taskSize--;
	//		Step++;
	//		continue;
	//	}
	//	else
	//	{
	//		taskSample.push_back(task->getMetricData_Distance()->at(Step));
	//		sampleSize--;
	//		taskSize--;
	//	}
	//}
	vector<vector<double> >distanceMatrix(sampleSize);
	for(int i=0; i < sampleSize; ++i)
	{
		for(int j=0; j< sampleSize; ++j)
		{
			if(i!=j)
				distanceMatrix[i].push_back(abs(task->getMetricData_Distance()->at(i) - task->getMetricData_Distance()->at(j)));
		}
	}
	for(int i=0; i < sampleSize; ++i)
	{
		sort(distanceMatrix[i].begin(),distanceMatrix[i].end());
	}

	int selectKDistance = 10;
	double Eps= 0.0;
	for(int i=0; i< distanceMatrix.size(); ++i)
	{
		Eps += distanceMatrix[i][selectKDistance];
	}
	//for(int i=0;i < distanceMatrix.size(); i++)
	//{
	//	shared_ptr<vector<double> >pointMatrix(new vector<double>());
	//	for(int j=0; j< distanceMatrix[i].size(); ++j)
	//		pointMatrix->push_back(distanceMatrix[i][j]);
	//	Eps +=utilFunction::select_min_k_iteration(pointMatrix,distanceMatrix[i].size(),selectKDistance);
	//}
	Eps /= sampleSize;
	EpsTable[indictor] = Eps;
	Epsi = selectKDistance;
}

/**@brief	Use DBSCAN algorithm in data mining to clustering,Key point is the determination of the radius, hash function and the handling of a large number of repeat points
*@param	task	To divide the task
*@param	metric	The data types of distance calculation function
*@param	numpartition	The task is divided into several parts
*@param	pivot	The pivot is going to be used
*@param	indictor	Instructions which the pivot is used, is also used as a labeled data class label
*@param	cluster_Start	Points out that the starting value of class labels
*@param	cluster_end	Points out that the end value of the class label
*/
void PartitionByOnePivot_DBSCAN::partitionByOnePivot(shared_ptr<ClusteringTask>& task,CMetricDistance* metric,int& numpartition,shared_ptr<CMetricData>& pivot,int indictor,int& cluster_Start,int& cluster_end)
{
	int taskSize = task->getData()->size();
	/*sortTaskData(*task->getData(), pivot, metric, 0, task->getData()->size());*/
	task->getMetricData_Distance()->clear();
	for(int i=0; i < taskSize;++i)
	{
		task->getMetricData_Distance()->push_back(metric->getDistance(pivot.get(),((task->getData())->at(i)).get()));
	}
	numpartition = numpartition > taskSize ? taskSize : numpartition;

	//string dataFileName = "./SourceFiles/util/data/dataFile_vector_";
	//strcat(dataFileName,taskSize);
	//ofstream outDataFile(dataFileName,ofstream::app);
	//for(int i=0; i< taskSize; i++)
	//{
	//	outDataFile << task->getMetricData_Distance()->at(i) << endl;
	//}

	CountedMetric* coutMetric = dynamic_cast<CountedMetric*>(metric);
	CEuclideanDistance* eudlideanDistance = dynamic_cast<CEuclideanDistance*>(coutMetric->getBaseMetric());
	CEditDistance* editDistance = dynamic_cast<CEditDistance*>(coutMetric->getBaseMetric());
	CRNAMetric* rnaMetric = dynamic_cast<CRNAMetric*>(coutMetric->getBaseMetric());
	CImageMetric* imageMetric = dynamic_cast<CImageMetric*>(coutMetric->getBaseMetric());
	CDNAMetric* dnaMetric = dynamic_cast<CDNAMetric*>(coutMetric->getBaseMetric());
	CPeptideMetric* peptideMetric = dynamic_cast<CPeptideMetric*>(coutMetric->getBaseMetric());

	string fileName;
	if(eudlideanDistance != NULL) fileName="./SourceFiles/util/data/epsFile_vector.txt";
	if(editDistance != NULL) fileName="./SourceFiles/util/data/epsFile_string.txt";
	if(rnaMetric != NULL) fileName="./SourceFiles/util/data/epsFile_rna.txt";
	if(imageMetric != NULL) fileName="./SourceFiles/util/data/epsFile_image.txt";
	if(dnaMetric != NULL) fileName="./SourceFiles/util/data/epsFile_dna.txt";
	if(peptideMetric != NULL) fileName="./SourceFiles/util/data/epsFile_peptide.txt";

	if(EpsTable.empty())
	{
		EpsTable.resize(task->getNewPivots()->size());
		ifstream inFile(fileName.c_str());
		/*if(!inFile)
		{*/
			for(int i=0; i< task->getNewPivots()->size(); ++i)
			{
				task->getMetricData_Distance()->clear();
				for(int j=0; j < task->getData()->size(); ++j)
				{
					task->getMetricData_Distance()->push_back(metric->getDistance(task->getNewPivots()->at(i).get(),((task->getData())->at(j)).get()));
				}
				selectEps(task,numpartition,i);
				ofstream outFile(fileName);
				for(int k=0; k < EpsTable.size(); k++)
					if(k != EpsTable.size() - 1)
						outFile << EpsTable[k] << " ";
					else
						outFile << EpsTable[k];
			}
		/*}
		else
		{
			string str="";
			getline(inFile,str);
			stringstream newStr(str);
			for(int k=0;k<task->getNewPivots()->size();++k)
				newStr >> EpsTable[k];
		}*/
	}

	double Eps = EpsTable[indictor];
	int clusterId = indictor*numpartition + 1;
	int Minpts=4,clusterNumIndictor = 0;
	vector<map<int,double> >hashTable(utilFunction::find_max(*task->getMetricData_Distance()) / Eps + 1); 
	for(int j=0; j < task->getData()->size(); ++j)
		hashTable[task->getMetricData_Distance()->at(j)/Eps].insert(map<int,double>::value_type(j,task->getMetricData_Distance()->at(j)));

	//for(int i=0;i<hashTable.size();i++)
	//{
	//	if(hashTable[i].size() ==0)
	//	{
	//		hashTable.erase(hashTable.begin()+i);
	//		i--;
	//	}
	//}

	map<double,int> clusterMaxValue;
	double clusterMaxValue_Single = 0;
	map<double,int> outliers;
	for (int i = 0; i < task->getData()->size();i++)
	{
		double Point = task->getMetricData_Distance()->at(i);
		if (task->getClusterId()->at(i) == -1)
			if (ExpandCluster(task,Point,clusterId,Eps,Minpts,i,hashTable,indictor,clusterMaxValue_Single,outliers))
			{
				clusterMaxValue.insert(map<double,int>::value_type(clusterMaxValue_Single,clusterId));
				clusterId += 1;
				clusterNumIndictor += 1;
			}
	}

	for(map<double,int>::iterator mapIt = outliers.begin();mapIt != outliers.end(); ++mapIt)
	{
		for(map<double,int>::iterator it=clusterMaxValue.begin(); it != clusterMaxValue.end(); ++it)
		{
			if(mapIt->first <= it->first || (mapIt->first > clusterMaxValue.rbegin()->first))
			{
				task->getClusterId()->at(mapIt->second) = it->second;
				break;
			}
		}
	}

	ofstream outFile("./SourceFiles/util/data/outFile_string.txt",ios::app);
	outFile<< "Epsi= " << Epsi << endl << "Eps=" << EpsTable[0] << endl <<  "clusterNumber: " << clusterMaxValue.size() << endl << "outliersNumber: " << outliers.size() << endl;

	cluster_Start = indictor*numpartition + 1;
	cluster_end = clusterId - 1;
	numpartition = numpartition > clusterNumIndictor ? numpartition : clusterNumIndictor;
}
