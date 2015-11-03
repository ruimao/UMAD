#ifndef MVPINDEX_H

#define MVPINDEX_H



#include "../index/PartitionMethod.h"

#include "../index/PivotSelectionMethod.h"




#include <cstring>
#include <thread>
#include <mutex>
#include <memory>

#ifdef __GNUC__
#include <sys/timeb.h>
#include "/usr/include/sys/times.h"
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>// for open flags
#include <semaphore.h>
#endif


#include "Index.h"
#include "MVPInternalNode.h"
#include "MVPLeafNode.h"

using namespace std;



/** @file MVPIndex.h

* @classes about mvp tree index structure

* @author Fuli Lei

* @version 2012-12-09

*/



/**

* @class Task

* @brief class about tasks that should completed during the process of building a mvp-tree

* @author Fuli Lei

*

* In the whole process of building a mvp-tree, some same openration like creating a internal node or leaf node could be encapsulated as a task and then put the task into a queue or stack for the iteration of building child trees

*/

class Task;

/**

* @class CMVPIndex

* @brief abstract class about index structure

* @author Fuli Lei, Qiaozhi Li, Yuxuan Qiu, Yaoda Liu

*

* this class is to build a mvp-tree and give out a search method, users can use the given search method to find out what they want quickly from the mvp-tree

*/

class CMVPIndex:public CIndex
{

public:

	/**@brief None parameter constructor */

	CMVPIndex();



	friend class Task;



	/**@brief constructor with eight parameters.

	* @param data the whole object set above which the mvp-tree will be built

	* @param metric it is a distance function, we can use it to work out the distance of two objects

	* @param psm the name of this parameter means pivot-selection-method, it is a vantage points selection method

	* @param pm pm means partition-method, it is used to partition a given data set into several small partitions, we can get a partition list throught it

	* @param numPivot the maxmum number of pivots in a single mvp-tree node

	* @param singlePivotFanout the maxmum number of partitions related to a single pivots

	* @param maxLeafSize the maxmum number of objects in a leaf node of mvp-tree

	* @param maxPathLength the maxmum number of distance values stored in a single leaf node, each distance value represent the distance from the object in every leaf node to the pivots in the parent node of the mvp-tree

	*/

	CMVPIndex(vector<shared_ptr<CMetricData> > &dataObjects,CMetricDistance *metric,CPivotSelectionMethod* psm,CPartitionMethod* pm,int numPivot,int singlePivotFanout,int maxLeafSize,int buildMode,int threadNum,double trisectionRadius,char* selectOptimalPivots,char* partitionByOnePivot);



	/**@brief destructor*/

	virtual ~CMVPIndex();



	/**@brief this is the function to build a mvp-tree 

	*@param dataObjects the whole object set above which the mvp-tree will be built

	*/


	void bulkLoad(vector<std::shared_ptr<CMetricData> > &objectList,int buildMode,int threadNum);




	/*@brief this is a search method, users can use it to find out all the objects in demand

	* @param q: this object is given by users for the search operation, distance from every object in the results to q.centerObject is not greater than r(the search radius)

	* @return return a vector contains all the objects that comform to the requirment that distance of each object to q.centerObject is not greater than r

	*/

	list<shared_ptr<CMetricData> >*search(CQuery* q);



	vector<shared_ptr<CMetricData> > travelSearch(CQuery* q);



	/**@brief function to create an internal node

	* @param task encapsulated the information which will be used to create a internal node of mvp tree

	*/

	void createInternalNode(Task* task);



	/**@brief function to create a leaf node

	* @param task encapsulated the information which will be used to create a leaf node of mvp tree

	*/

	void createLeafNode(Task* task);	

	/**@brief get the number of data objects that make up the index struct

	*@return the number of data objects that make up the index struct

	*/

	int size();



	/**@brief get the object list contains all the objects that make up the index structure

	*@return all data objects that make up the index structure

	*/

	vector<shared_ptr<CMetricData> > getAllDataPoints();



	/**@brief get the root of a mvptree built before

	*@return the root of mvp-tree

	*/

	shared_ptr<CIndexNode> getMVPTreeRoot();

	/**@brief get the metric object of a mvptree

	*@return the member varible metric,which is used to calculate the distance of two data objects

	*/

	CMetricDistance* getMetric();

	void setMetric(CMetricDistance *objMetric);

	virtual void setDataType(string dataType);

	virtual string getDataType();

	virtual void setFileName(string fileName);

	virtual string getFileName();

	void loadLeafToRoot(vector<shared_ptr<CMetricData> > &dataObjects);

	void loadRootToLeaf(vector<shared_ptr<CMetricData> > &dataObjects);

	void createAndWriteLeafNode(shared_ptr <Task> task,ofstream& out);

	void createInternalNodeToWrite(shared_ptr <Task> task);

	void writeInternalNode(shared_ptr <Task> task,ofstream& out);

	void createAndWriteInternalNode(shared_ptr <Task> task,ofstream& out,vector<shared_ptr <Task> >::iterator &shouldeBeCreate);

	

	void writeRoot(shared_ptr <Task> task,ofstream& out,int maxTreeHeight);
	void parallelBuildingMVPtree(vector<shared_ptr<CMetricData> > &dataObjectList,int maxThreadNum);


	void createAndWriteLeafNodeLast(shared_ptr <Task> task,ofstream &out);

	
private:




	/** the root of mvp-tree */

	shared_ptr<CIndexNode> root;

	/** the metric over which this index will be built */

	CMetricDistance *metric;



	/** the task list contains all the tasks that creating a internal or leaf node*/

	vector<shared_ptr<Task> > taskList;


	/** the total number of pivots to use */

	int numPivots;


	/** the method,used when building a index structure,to select a pivot(or pivots) */

	CPivotSelectionMethod *psm;


	/** the method,used when building a index structure,to partition the data set */

	CPartitionMethod *pm;


	/** the fanout of a single pivot. the total fanout is singlePivotFanout*numPivot*/

	int singlePivotFanout;


	/** the total number of data objects over which the index is built */

	int totalSize;

	/** the maximum number of data objects in a leaf node */

	int maxLeafSize;


	/** the number of leaf nodes */

	int numLeaf;


	/** the number of internal nodes of the index */

	int numInternalNodes;

	/** file pointer of index file, in read or write process, this parameter is very important */
	long filePointer;



	long myIndexFilePointer;


	long rootAddress;

	/** the mode of build index */
	int buildMode;

	/** the num of thread */
	int threadNum;

	/** the set of result when doing a search in index file*/
	vector<shared_ptr<CMetricData> > re;

	double trisectionRadius;
	char* selectOptimalPivots;
	char* partitionByOnePivot;
};







#endif

