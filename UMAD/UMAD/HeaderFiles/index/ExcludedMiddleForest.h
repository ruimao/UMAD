#pragma once



#include "Index.h"

#include "IndexNode.h"



#include "../index/PartitionMethod.h"

#include "../index/PivotSelectionMethod.h"



#include "MVPInternalNode.h"

#include "MVPLeafNode.h"





#include <vector>
#include <memory>


/** @file 

* @brief A Forest built by EcludedMiddlePartitionMethod

* @author Qiaozhi LI

* @date 2013.5.26

* @version 1.0.0

* @note

* Include the em-forest and a task class to build an em-forest

*/





/**

* @class ForestTask

* @brief class about tasks that should completed during the process of building a em-forest

* @author Qiaozhi Li

* @note

* In the whole process of building a em-Forest, some same openration like creating a internal node or leaf node and necessary information could be encapsulated as a task and then put the task into a queue or stack for the iteration of building child trees

*/

class ForestTask;



class CExcludedMiddleForest:public CIndex
{

public:



	friend class ForestTask;



	CExcludedMiddleForest(void);



	/**

	* @brief constructor with 10 parameters.

	* @param data the whole object set above which the mvp-tree will be built

	* @param metric it is a distance function, we can use it to work out the distance of two objects

	* @param psm the name of this parameter means pivot-selection-method, it is a vantage points selection method

	* @param pm pm means partition-method, it is used to partition a given data set into several small partitions, we can get a partition list throught it

	* @param numPivot the maxmum number of pivots in a single mvp-tree node

	* @param singlePivotFanout the maxmum number of partitions related to a single pivots

	* @param maxLeafSize the maxmum number of objects in a leaf node of mvp-tree

	* @param maxPathLength the maxmum number of distance values stored in a single leaf node, each distance value represent the distance from the object in every leaf node to the pivots in the parent node of the mvp-tree

	* @param maxRadius the exclusion width of middle partition 

	* @param middlePorprotion the porprotion of middle partition in each dataset which will be partitioned

	* @note

	*

	*/

	CExcludedMiddleForest(vector<shared_ptr<CMetricData> > &dataObjects,CMetricDistance *metric,CPivotSelectionMethod* psm,CPartitionMethod* pm,int numPivot,int singlePivotFanout,int maxLeafSize, int maxPathLength,double maxRadius,double middlePorprotion,double trisectionRadius,char* selectOptimalPivots,char* partitionByOnePivot);

	~CExcludedMiddleForest(void);





	/**

	* @brief build an em-forest 

	* @auther Qiaozhi Li

	* @param objectList data which will be used to build an em-forest

	* @note

	*

	*/

	void bulkLoad(vector<std::shared_ptr<CMetricData> > &objectList,int buildMode,int threadNum);

	/**

	* @brief Create a tree of em-forest

	* @param objectList data which will be used to build a tree of em-forest

	* @param treeIndex the index of the tree will be created in the em-forest

	* @return The datalist ecluded by partitionmethod

	* @note

	*

	*/

	vector<shared_ptr<CMetricData> > createTree(vector<shared_ptr<CMetricData> > &objectList,int treeIndex);



	/**

	* @brief fucntion to create an root of em-forest

	* @param task a ForestTask encapsulated necessary information to create a node

	* @note

	*

	*/

	shared_ptr<CIndexNode> createRoot(shared_ptr<ForestTask> ,vector<shared_ptr<CMetricData> >& ecludedData);



	/**

	* @brief fucntion to create an internalnode

	* @param task a ForestTask encapsulated necessary information to create a node

	* @note

	*

	*/

	void createInternalNode(shared_ptr<ForestTask> ,vector<shared_ptr<CMetricData> >& ecludedData);



	/**

	* @brief fucntion to create a leafNode

	* @param task a ForestTask encapsulated necessary information to create a node

	* @note

	*

	*/

	void createLeafNode(shared_ptr<ForestTask> task,vector<shared_ptr<CMetricData> >& ecludedData);



	/*

	* @brief this is a search method, users can use it to find out all the objects in demand

	* @param CQuery this object is given by users, all the result of this function are found out based on this object

	* @return search result

	* @note

	*

	*/

	list<shared_ptr<CMetricData> > *search(CQuery* q);

	void setMetric(CMetricDistance *metric);





	void createAndWriteInternalNode(shared_ptr <ForestTask> task,ofstream& out,vector<shared_ptr < ForestTask> >::iterator &shouldeBeCreate, vector<shared_ptr<CMetricData> >& ecludedData);

	void createAndWriteLeafNodeLast(shared_ptr <ForestTask> task,ofstream &out, vector<shared_ptr<CMetricData> >& ecludedData,int treeIndex);

	void writeRoot(shared_ptr <ForestTask> task,ofstream& out,vector<shared_ptr<CMetricData> >& ecludedData,int treeIndex);

	void loadRootToLeaf(vector<shared_ptr<CMetricData> > &dataObjects,vector<shared_ptr<CMetricData> >& ecludedData,int treeIndex,ofstream &out);



	void setDataType(string dataType);

	string getDataType();

	void setFileName(string fileName);

	string getFileName();





	vector<shared_ptr<CMetricData> > getAllDataPoints(){ return vector<shared_ptr<CMetricData> >();}

	CMetricDistance* getMetric(){return metric;}

	int size(){return 0;}

	void setDataType(){}



	//vector<shared_ptr<CMetricData> > getAllDataPoints();

	//CMetricDistance* getMetric();

	//int size();

	//void setDataType();



private:



	vector<shared_ptr<CIndexNode> > roots;





	CPartitionMethod *pm;

	CPivotSelectionMethod *psm;

	CMetricDistance *metric;

	vector<shared_ptr<ForestTask> > forestTaskList;   

	vector<shared_ptr<CMetricData> > rs;



	int numPivots;

	int singlePivotFanout;

	int total_size;

	int numLeaf;

	int numInternalNodes;

	int maxLeafSize;

	int maxPathLength;



	double middlePorprotion;

	double maxRadius;



	string fileName;

	string dataType;



	vector<long> rootAddress;
    vector<int> treeHeight;

	int numTree;

	long filePointer;

	list<shared_ptr< CMetricData> > re;

	double trisectionRadius;
	char* selectOptimalPivots;
	char* partitionByOnePivot;













};



