#ifndef TASKDECLARATION_H
#define TASKDECLARATION_H

#include "../index/query/RangeQuery.h"
#include "../metricdata/MetricData.h"
#include "../metricdistance/CountedMetric.h"
#include<string>
#include<fstream>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<memory>

using std::shared_ptr;

class CSearchTask;

struct singlequerytask
{
	shared_ptr<CSearchTask> searchTask;
	shared_ptr<CRangeQuery> rq;
	shared_ptr<CountedMetric> cmetric;
	long rootaddress;
	double searchtime;
	double distancecount;
	int resultnumber;

	//for verify 
	vector<shared_ptr<CMetricData> > *rawData;
	CMetricDistance *metric;
	ofstream *output;
	bool verify;
};

struct singleforestquerytask
{
	shared_ptr<CSearchTask> searchTask;
	shared_ptr<CRangeQuery> rq;
	shared_ptr<CountedMetric> cmetric;
    int numTree;
	long* rootAddress;
	double searchtime;
	int distancecount;
	int resultnumber;

	//for verify 
	vector<shared_ptr<CMetricData> > *rawData;
	CMetricDistance *metric;
	ofstream *output;
	bool verify;
};


struct tp_task
{
	int ifinner;
	int totalroute;
	int *variableroutenumber;
	condition_variable *cond;
	mutex *route_mutex;
	void *task;
	void* (*func)(void*);
	~tp_task(){}
};

struct subsearchtask
{
	long address;
	CRangeQuery *rq;
	ifstream *ifile;
	mutex *result_mutex;
	mutex *file_mutex;
	CMetricDistance *metric;
	list<shared_ptr<CMetricData> > *result;
	string *dataType;

};

#endif