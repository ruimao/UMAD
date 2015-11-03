#ifndef INDEXNODE_H

#define INDEXNODE_H



#include "../metricdata/MetricData.h"

#include "query/RangeQuery.h"

#include "../metricdistance/MetricDistance.h"

#include "../util/ObjectFactory.h"

#include "../util/MemMonitor.h"



#include <list>

#include <deque>

#include <thread>

#include <mutex>

#include <memory>


/** @file IndexNode.h

* @classes about index

* @author Fuli Lei

* @version 2012-12-09

*/



/**

* @class CIndexNode

* @brief abstract class about nodes in index structure

* @author Fuli Lei

*

* this is a abstract base class which include all the base information of a node in the index, all the derived class of this class will inherite the information descriped in the class

*/



class CIndexNode

{

public:

    /**none parameter constructor

    this constructor will be called to create an instance of class CIndexNode

    */

    CIndexNode();



    /**constructor with one parameter pivots. 

    this constructor will be called to create an instance of class CIndexNode



    *@param pivots a vector include all the address of pivots belong to single node in the index structor

    *@param height the height of current node

    */

    CIndexNode(vector<shared_ptr<CMetricData> > pivots, int height);



    /**destructor

    the space of this class's instance will be released if the instance is no longer used in the program

    */

    virtual ~CIndexNode();



    /** get the address of a pivot based on the pivot index in the parameter list

    *@param pivot pivot index of a pivot in the pivot address vector in the node

    *@return a pivot address in the main memory

    */

    shared_ptr<CMetricData> getPivot(int pivotIndex);



    /**one can get all the pivots belongs to a single node, which is internal node or leaf node, through this function.

    *@return a vector consists of all the pivot address in a single node

    */

    vector<shared_ptr<CMetricData> >& getPivots();



    /**To get the number of child node in internal node or objects in leaf node. this function must be implemented in advance of being called.

    *@return the total number of child node of current node is an internal node, otherwise it will return the total number of objects in a leaf node 

    */

    virtual int numChildren()=0;



    /**set the height of this node

    *@param degree the value of height of this node

    */

    void setHeight(int degree);



    /**get the height of this node

    * @return the value of height

    */

    int getHeight();



    /**this is function will be called when traversal from the root to the leaf doing the search operation

    *@param q this object package an some information used to do the search

    *@param metric one can get the distance of two objects through the member function of this parameter

    *@raturn return a vector containing all the proper objects in this node 

    */

    virtual vector<CMetricData*>  search(CRangeQuery &q,CMetricDistance &metric)=0;



    virtual void getAlldata(vector<shared_ptr<CMetricData> > &result)=0;

    virtual void travelSearch(CRangeQuery &q,CMetricDistance &metric,vector<shared_ptr<CMetricData> > &result)=0;



    virtual int writeExternal(ofstream &out);

    virtual int readExternal(ifstream &in,string &objectType);

	virtual void search(CRangeQuery &q,CMetricDistance &metric,deque<long> *serachlist,list<shared_ptr<CMetricData> > &re)=0;
	virtual list<std::shared_ptr<CMetricData> >  search(CRangeQuery &q,CMetricDistance &metric,deque<long> *searchlist)=0;

    virtual void searchIndex(CRangeQuery &q,long filePointer,ifstream &in,CMetricDistance &metric,list<shared_ptr<CMetricData> > *result,string &dataType)=0;

    virtual void searchExternal(CRangeQuery &q,long filePointer,ifstream &in,CMetricDistance &metric,list<shared_ptr<CMetricData> > &result,deque<long> &childrenAddress,string &dataType)=0;

	virtual void SMTSearchExternal(CRangeQuery &q,long filePointer,ifstream &in,CMetricDistance &metric,list<shared_ptr<CMetricData> > *rs,deque<long> &childrenAddress,string &dataType,mutex &mux,mutex &r_mutex)=0;

protected:

    /**this is a vector containing the address of all the pivots belonging to this node*/

    vector<shared_ptr<CMetricData> > pivots;



    /**the height of current node, the height of mvp-tree root is 0 while the height of every child of the root is equal to root.height+1,so every child node's height is equal to its father node's height+1*/

    int height;



};



#endif

