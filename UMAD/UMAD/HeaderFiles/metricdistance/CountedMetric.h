#pragma once

#include "../metricdata/MetricData.h"

#include "MetricDistance.h"

#include <mutex>
extern mutex tdm;
extern int gTotalDistanceNum;


class CountedMetric : public CMetricDistance
{

private:

	CMetricDistance *baseMetric;

	int  counter;

	

public:

	CountedMetric(void);

	~CountedMetric(void);

	CountedMetric(CMetricDistance *baseMetric);

	double getDistance(CMetricData *one, CMetricData *two);

	int getCounter();

	void clear();

	void addCount(int);

	CMetricDistance* getBaseMetric();

};



