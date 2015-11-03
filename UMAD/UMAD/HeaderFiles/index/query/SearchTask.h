#pragma once

#include "../Index.h"

#include "Query.h"

#include "RangeQuery.h"

#include "../IndexNode.h"

#include "../../util/ObjectFactory.h"

#include "../MVPInternalNode.h"

#include "../MVPLeafNode.h"

#include <deque>

#include <thread>
#include <mutex>
#include <unordered_map>
#include "../../util/ThreadPoolManager.h"

#include <memory>


class CSearchTask
{
public:
    CSearchTask(string indexName,string dataType,CMetricDistance &metric);

    ~CSearchTask(void);

	int getResultSize(){return result.size();}
	list<shared_ptr<CMetricData> >* getResult(){ return &result;}

    list<shared_ptr<CMetricData> >* search(CRangeQuery &q);
	void searchWithCache(CRangeQuery &rq,long rootAddress);

    void searchForestWithCache(CRangeQuery &rq,long* rootAddress,int numTree);

	void SMTSearch(CRangeQuery &rq);//deprecated

	/*parallel search layer by layer(parallel intra-layer)*/
	void MultithreadSearch(CRangeQuery &rq);
	/*parallel search layer by layer with cache(parallel intra-layer)*/
	void MultiThreadSearchWithCache(CRangeQuery &rq);
	/*single search with cache and intra search parallel*/
	list<shared_ptr<CMetricData> >* singleSearchWithCache(CRangeQuery &rq,long rootAddress);
	list<shared_ptr<CMetricData> >* singleSearchWithMultipleThread(CRangeQuery &rq,long rootAddress);
	list<shared_ptr<CMetricData> >* singleSearchWithThreadPool(CRangeQuery &rq,long rootAddress);

	

private:
    void searchEachNode(long &nodeAddress,CRangeQuery &q,ifstream &in);
    string indexName;

    string indexType;

    string dataType;

    long filePointer;

    CMetricDistance &metric;

    deque<long> searchList;

    list<shared_ptr<CMetricData> > result;
};



