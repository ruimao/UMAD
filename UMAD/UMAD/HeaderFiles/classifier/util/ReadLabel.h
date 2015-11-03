#ifndef READLABEL_H
#define READLABEL_H

#include "../../../HeaderFiles/metricdata/MetricData.h"
#include <fstream>
#include <string>
#include <sstream>
#include <map>
using namespace std;

/** 
 *@ file	readlabel.h
 *@ classes	store data number and the class label of train data
 *@ author	Ping Li
 *@ version	2014-08-01
 */

/**
 *@ class	CReadLabel
 *@ brief	store data number and the class label of train data
 *@ author	Ping Li
 */

class CReadLabel:public CMetricData
{
public:

/**
 *@ store the class label of train data
 *@ param fileName: fileName is the primary data file that store attribute and class label
 *@ param maxDataNum: the total number of the data list
 */
	static vector<string> loadLabel(string fileName,int maxDataNum);
};

#endif