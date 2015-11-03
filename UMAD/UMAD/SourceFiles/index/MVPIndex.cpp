#include "../../HeaderFiles/index/MVPIndex.h"

#include "../../HeaderFiles/metricdata/DoubleVector.h"



#include<cmath>



/** @file MVPIndex.cpp
* @classes about mvp tree index structure
* @author Fuli Lei,Qiaozhi Li
* @version 2013-6-20
*/



/**
* @class Task
* @brief class about tasks that should be completed during the process of building a mvp-tree
* @author Fuli Lei
*
* In the whole process of building a mvp-tree, some same openration like creating a internal node or leaf node could be encapsulated as a task and then put the task into a queue or stack for the iteration of building child trees
*/

class Task 
{

public:

	/**give class CMVPIndex a friend right to access the data information of a Task instance in the process of building a mvp tree*/

	friend class CMVPIndex;

	/**@brief constructor with six parameters

	*	@param dataList a vector contains the address of all the obejct over which this mvp tree is built

	*	@param parentIndex address of parent node

	*	@param start the start index of current object partition in the whole data list

	*	@param end the last object index of current object partition

	*	@param myIndex current node index in parent node's children list

	*	@param myHeight height of current node

	*/

	Task(vector< shared_ptr<CMetricData> > &dataList,shared_ptr<CIndexNode> parentIndex,int start,int end,int myIndex,int myHeight):dataList(dataList)
	{

		this->parentIndex=parentIndex;

		this->myIndex=myIndex;



		this->myHeight=myHeight;

		



		this->start=start;

		this->size=end-start;



		numPivots=0;



		specialN=0;



		this->node=shared_ptr<CIndexNode>();



	};



	~Task()
	{        

		//node.reset();

	}



	friend int standardCreateInternalNodeToWrite(shared_ptr<Task> task,list<shared_ptr<Task> > *taskList,list<shared_ptr<Task> >*newTaskList,CMetricDistance *metric,CPartitionMethod *pm,int *numInternalNodes,int start,int size,int singlePivotFanout,int maxLeafSize,double trisectionRadius,char* selectOptimalPivots,char* partitionByOnePivot);





	/**@brief move the pivot to the end of a object list partition

	* @param pivotIndex a vector contains all the indices of pivots in the whole objects list

	*/

	void groupPivotsAtEnd(vector<int> &pivotIndex);



	/**@brief get the index of current node in the parent's children list*/

	int getMyIndex();



	/**@brief get the height of current node. the height of root is 0*/

	int getMyHeight();



	void setFlag(int stat){ specialN=stat;}

	int getFlag(){return specialN;}



	/**@brief get the parent node's address*/

	shared_ptr<CIndexNode> getParentIndex();



	/**@brief get all the pivots that belongs to current node

	* @param pivots a vector to store the address of pivot.This vector is initialized with none elements, all the elements is generated through calling thing function. This parameter is used to return the address of all pivot to the caller of this funciton.

	*/

	void getPivots(vector< shared_ptr<CMetricData> >&pivots);



	/**@brief get all the objects that belong to the tree rooted at this node*/

	void getDataPoints(vector< shared_ptr<CMetricData> >&dataList);



	vector<shared_ptr<CMetricData> >& getDataList();

	long getSize();

	int getPivotNumber();

	long getStart();



	void setNode(shared_ptr<CIndexNode> node)
	{

		this->node = node;

	}
	shared_ptr<CIndexNode> getNode()
	{

		return node;

	}

private:

	/**constructor*/

	Task():dataList(*(new vector< shared_ptr<CMetricData> >))
	{



		this->parentIndex=shared_ptr<CIndexNode>();

		this->myIndex=0;

		this->myHeight=0;



		this->size=0;

		this->start=0;



		specialN=0;

		this->numPivots=0;



		this->node=shared_ptr<CIndexNode>();//like this->node=NULL



	};







	int specialN;



	/**a vector contains the address of all the obejct over which this mvp tree is built*/

	vector<shared_ptr<CMetricData> > &dataList;



	/**address of parent node*/

	shared_ptr<CIndexNode> parentIndex;



	shared_ptr<CIndexNode> node;


	/**current node index in parent node's children list*/

	int myIndex;

	
	/*corresponding to the parameter "myIndex", "myIndexAdress" is the true address in index file*/
	long myIndexAdress;


	/**height of current node*/

	int myHeight;



	/**the start index of current object partition in the whole objects list*/

	int start;



	/**number of objects in current object partition*/

	int size;



	/**number of pivots in a node of mvp tree*/

	int numPivots;







};



/**@brief move the pivot to the end of a object list partition
* @param pivotIndex a vector contains all the indices of pivots in the whole objects list
*/

void Task::groupPivotsAtEnd(vector<int> &pivotIndex)
{

	/*initialize numPivots*/

	this->numPivots=pivotIndex.size();





	if(this->numPivots<this->size)
	{

		/*new address of pivots, the first pivots will be moved to the end of current object partition, start+this->size-1 is the end index of current object partition*/

		int newPivotAddressInCompressedData=start+this->size-1;



		/*move every pivot to the end of partiton*/

		for(int i=0;i<this->numPivots;i++)
		{

			shared_ptr<CMetricData> temp;



			/*exchange the address content of pivot element unit and the target element unit*/

			temp=this->dataList[pivotIndex[i]];

			dataList[pivotIndex[i]]=dataList[newPivotAddressInCompressedData];

			dataList[newPivotAddressInCompressedData]=temp;



			/*judge if the content of target elment unit is the address of another pivot whose index is also in the pivot index list, exchange the index the result of judge is true*/

			for(int j=i+1;j<this->numPivots;j++)

				if(pivotIndex[j]==newPivotAddressInCompressedData)

					pivotIndex[j]=pivotIndex[i];



			/*the first pivot move the end of partition, the next pivot is always moved to one element unit before the previous moved pivots*/

			newPivotAddressInCompressedData--;

		}

	}



}



/**@brief get the index of current node in the parent's children list*/

shared_ptr<CIndexNode> Task::getParentIndex()
{

	return this->parentIndex;

}



/**@brief get the index of current node in the parent's children list*/

int Task::getMyIndex()
{

	return this->myIndex;

}



/**@brief get the height of current node. the height of root is 0*/

int Task::getMyHeight()
{

	return this->myHeight;

}



/**@brief get all the pivots that belongs to current node

* @param pivots a vector to store the address of pivot.This vector is initialized with none elements, all the elements is generated through calling thing function. This parameter is used to return the address of all pivot to the caller of this funciton.

*/

void Task::getPivots(vector< shared_ptr<CMetricData> > &pivots)
{

	/*start should be the first pivot index in current partition*/ 

	int start=this->start+this->size-this->numPivots;





	for(int i=0;i<this->numPivots;i++)
	{

		pivots.push_back(this->dataList[start]);



		start++;

	}



}



/**@brief get all the objects that belong to the tree rooted at this node*/

void Task::getDataPoints(vector<shared_ptr<CMetricData> > &objectList)
{

	int mySize=0;



	/**if the number of pivot is less 0,return all the objects in current partition*/

	if(numPivots==-1)

		mySize=this->size;

	else

		mySize=this->size-this->numPivots;



	for(int i=start;i<start+mySize;i++)
	{

		objectList.push_back(this->dataList[i]);

	}

}



vector<shared_ptr<CMetricData> >& Task::getDataList()
{

	return dataList;

}



long Task::getSize()
{

	return size;

}



int Task::getPivotNumber()
{

	return numPivots;

}



long Task::getStart()
{

	return start;

}



/////////////////////////////////////////////////////////////







////////////////////////////////////////////////////////////







/**@brief None parameter constructor */

CMVPIndex::CMVPIndex()
{

	root=shared_ptr<CIndexNode>();//like root=NULL



	metric=0;



	psm=0;

	pm=0;



	singlePivotFanout=0;



	maxLeafSize=0;



	numPivots=0;

	numLeaf=0;

	numInternalNodes=0;

	trisectionRadius = 0.1;
	selectOptimalPivots = "BalanceOne";
	partitionByOnePivot = "Variance";

}



void CMVPIndex::setMetric(CMetricDistance *objMetric)
{

	metric = objMetric;

}



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

CMVPIndex::CMVPIndex(vector<shared_ptr<CMetricData> > &dataObjects,CMetricDistance *metric,CPivotSelectionMethod* psm,CPartitionMethod* pm,int numPivots,int singlePivotFanout,int maxLeafSize,int _buildMode,int _threadNum,double trisectionRadius,char* selectOptimalPivots,char* partitionByOnePivot)
{

	this->root=shared_ptr<CIndexNode>(); //like root=NULL

	this->metric=metric;

	this->numPivots=numPivots;

	this->psm=psm;

	this->pm=pm;

	this->singlePivotFanout=singlePivotFanout;

	this->totalSize=dataObjects.size();

	this->maxLeafSize=maxLeafSize;

	this->buildMode=_buildMode;

	this->threadNum=_threadNum;

	numLeaf=0;

	numInternalNodes=0;

	this->trisectionRadius = trisectionRadius;
	this->selectOptimalPivots = selectOptimalPivots;
	this->partitionByOnePivot = partitionByOnePivot;

}



/**@brief destructor*/

CMVPIndex::~CMVPIndex()
{



}




void CMVPIndex::bulkLoad(vector<std::shared_ptr<CMetricData> > &objectList,int buildMode,int threadNum)
{

	if (buildMode == 0)
		loadLeafToRoot(objectList);
	else if(buildMode == 1)
		loadRootToLeaf(objectList);
	else
		parallelBuildingMVPtree(objectList,threadNum);


}


/**@brief function to create an internal node

* @param task encapsulated the information which will be used to create a internal node of mvp tree

*/

//void CMVPIndex::createInternalNode(Task *task)
//{
//	this->numInternalNodes ++;
//	//cout<<"internalNode:"<<numInternalNodes<<endl;
//	/*get pivots of current node*/
//	vector<shared_ptr<CMetricData> > pivots;
//	task->getPivots(pivots);
//
//	/*partition current partition into several smaller child partitons,the selected pivots are not in the section that will be partitioned in child partitions*/
//	CPartitionResults pr=this->pm->partition(this->metric,pivots,task->dataList,task->start,task->size-task->numPivots,this->singlePivotFanout,this->maxLeafSize);
//
//	int childrenNumber=pr.partitionSize();
//
//	/*create an internal node and assign its addres to the children list of parent node*/
//    vector<shared_ptr<CIndexNode> > *subTreeNode = new vector<shared_ptr<CIndexNode> >(childrenNumber-1);
//	shared_ptr<CMVPInternalNode> node(new CMVPInternalNode(pivots,pr.getLowerBounds(),pr.getUpperBounds(),*subTreeNode,task->getMyHeight()+1));
//
//	if(this->root==0)
//		this->root=node;
//	else
//		(((CMVPInternalNode*)(task->getParentIndex()))->getSubTree())[task->myIndex-1]=node;
//
//	/*create several tasks base on each of the child partitions created before and then push these tasks to the global variable queue for the iteration of building child trees*/
//	for(int i=childrenNumber-1;i>0;i--)
//	{
//		Task *newTask=0;
//
//		newTask = new Task(task->dataList,node,pr.getPartition(i-1),pr.getPartition(i),i,task->getMyHeight()+1);
//
//
//		this->taskList.push_back(newTask);
//
//
//
//		this->taskList.insert(this->taskList.begin(),newTask);
//		//this->taskList.push_back(newTask);
//
//	}
//
//}

/**@brief function to create a leaf node
* @param task encapsulated the information which will be used to create a leaf node of mvp tree
*/
//void CMVPIndex::createLeafNode(Task* task)
//{
//	this->numLeaf++;
//	//cout<<"leafNode:"<<numLeaf<<endl;
//	/*get all the objects of current partition*/
//	vector<CMetricData*> children;
//	task->getDataPoints(children);
//
//	/*get all the pivots of current node*/
//	vector<CMetricData*> pivots;
//	task->getPivots(pivots);
//
//	vector<vector<double>> distance;
//
//	/*calcualte the distance from each of the objects of current parition to every pivots*/
//	for(vector<CMetricData*>::size_type i=0;i<pivots.size();i++)
//	{
//		vector<double> ve;
//
//		for(vector<CMetricData*>::size_type j=0;j<children.size();j++)
//		{
//			ve.push_back(this->metric->getDistance(children[j],pivots[i]));
//		
//
//		distance.push_back(ve);
//	}
//
//	/*create a leaf node and assign its memory address to the child list of parent node*/
//	CMVPLeafNode *mvpLeafNode=new CMVPLeafNode(pivots,children,distance,task->getMyHeight()+1);
//
//	if(this->root==0)
//		this->root=mvpLeafNode;
//	else
//		(((CMVPInternalNode*)(task->getParentIndex()))->getSubTree())[task->myIndex-1]=mvpLeafNode;
//
//}

/**@brief this is the function to build a mvp-tree 
*@param dataObjects the whole object set above which the mvp-tree will be built
*/

vector<shared_ptr<CMetricData> > CMVPIndex::travelSearch(CQuery* q)
{

	vector<shared_ptr<CMetricData> > re;



	CRangeQuery *rq=(CRangeQuery*)q;

	root->travelSearch(*rq,*metric,re);

	return re;

}


/**@brief get the number of data objects that make up the index struct
*@return the number of data objects that make up the index struct
*/
int CMVPIndex::size()
{
	return totalSize;
}



int traversingMVPTree(shared_ptr<CIndexNode> node, vector<shared_ptr<CMetricData> > &data)
{

	if(node==NULL)

		return 0;



	shared_ptr<CMVPInternalNode> interNode(dynamic_cast<CMVPInternalNode*>(node.get()));

	if(interNode!=0)
	{

		for(int i=0;i<interNode->getPivots().size();i++)

		{

			data.push_back(interNode->getPivot(i));

		}

		int childNum=interNode->numChildren();

		for(int i=0;i<childNum;i++)

			traversingMVPTree(interNode->getChildAddress(i),data);

		return 0;

	}


	shared_ptr<CMVPLeafNode> leafNode(dynamic_cast<CMVPLeafNode*>(node.get()));

	if(leafNode!=0)
	{

		for(int i=0;i<leafNode->getPivots().size();i++)
		{

			data.push_back(leafNode->getPivot(i));

		}

		for(int i=0;i<leafNode->numChildren();i++)
		{

			data.push_back(leafNode->getObject(i));

		}

	}

}



/**@brief get the object list contains all the objects that make up the index structure

*@return all data objects that make up the index structure

*/

vector<shared_ptr<CMetricData> > CMVPIndex::getAllDataPoints()
{

	vector<shared_ptr<CMetricData> > data;

	traversingMVPTree(this->root,data);

	return data;

}



/**@brief get the root of a mvptree built before

*@return the root of mvp-tree

*/

shared_ptr<CIndexNode> CMVPIndex::getMVPTreeRoot()
{

	return this->root;

}



/**@brief get the metric object of a mvptree

*@return the member varible metric,which is used to calculate the distance of two data objects

*/

CMetricDistance* CMVPIndex::getMetric()
{

	return metric;

}


void CMVPIndex::setDataType(string dataType)
{

	this->dataType = dataType;

}



string CMVPIndex::getDataType()
{

	return this->dataType;

}



void CMVPIndex::setFileName(string fileName)
{

	this->fileName=fileName;

}



string CMVPIndex::getFileName()
{

	return fileName;

}



/*@brief this is a search method, users can use it to find out all the objects in demand

* @param q: this object is given by users for the search operation, distance from every object in the results to q.centerObject is not greater than r(the search radius)

* @return return a vector contains all the objects that comform to the requirment that distance of each object to q.centerObject is not greater than r

*/

list<shared_ptr<CMetricData> >* CMVPIndex::search(CQuery* q)
{

	//CMemMonitor::updateMem();



	list<shared_ptr<CMetricData> > *re=new list<shared_ptr<CMetricData> >;

	CRangeQuery *rq=(CRangeQuery*)q;


	filePointer = 0;



	ifstream in(fileName.c_str(),ios::binary);
	if(in.fail())
	{
		cout<<"ERROR : can't find index file!"<<endl;
		exit(0);
	}


	in.seekg(-(long)sizeof(long),ios::end);



	in.read((char*)(&rootAddress),sizeof(long));





	in.seekg(rootAddress,ios::beg);





	shared_ptr<CIndexNode> root;

	char* type = new char[6];

	in.read(type,6*sizeof(char));

	if(!strcmp(type,"INODE"))
	{

		root = dynamic_pointer_cast<CMVPInternalNode> (root);

		root.reset(new CMVPInternalNode());

		/*root = new CMVPInternalNode();*/

	}

	else

	{

		root = dynamic_pointer_cast<CMVPLeafNode> (root);

		root.reset( new CMVPLeafNode());

		//root = new CMVPLeafNode();

	}

	rootAddress += 6*sizeof(char);

	delete(type);	

	root->searchIndex(*rq,rootAddress,in,*metric,re,dataType);



	// CMemMonitor::updateMem();

	in.close();

	// CMemMonitor::updateMem();

	return re;
}

#ifdef __GNUC_

/*DATA STRUCT FOR BOTH WINDOWS AND C++11 STANDARD VERSION OF MULTI-THREAD CODING*/

struct parallelBlock
{
	/*constuctor*/
	//parallelBlock(){cout<<"building a parallel block"<<endl;}
	/*desturctor*/
	//~parallelBlock(){cout<<"rootTask.useCount:"<<rootTask.use_count()<<" destroying a parallel block"<<endl;}	
	
	ofstream *fstr;
	int *numPivots;
	CMetricDistance *metric;
	CPivotSelectionMethod *psm;
	CPartitionMethod *pm;

	vector<shared_ptr<Task> > *taskList;
	vector<shared_ptr<Task> > *ntaskList;
	vector<shared_ptr<Task> > *newTaskList;
	list<shared_ptr<Task> > *leafTaskList;

	int maxLeafSize;
	int singlePivotFanout;
	int *numInternalNode;
	int *numLeafNode;

	string fileName;
	long *filePointer;
	long *rootAddress;

	shared_ptr<Task> rootTask;

	int flag;
	int num;
	double t;
	sem_t *semmutex;

	double tr;
	char *sop;
	char *pbop;
};

/***************************************************C++11 STANDARD VERSION MULTI-THREAD CODE BEGIN***************************************************************/
mutex stMutexA;//create internal or leaf for original task list
mutex stMutexB;//create internal for new task list
mutex stMutexO; //create leaf node for leaf task list
mutex stMutexN; mutex tmp;

condition_variable cvg;
condition_variable cvs;

int stdThreadNumA=0;
int stdThreadNumB=0;

int standardCreateInternalNodeToWrite(shared_ptr<Task> task,vector<shared_ptr<Task> > *taskList,vector<shared_ptr<Task> >*newTaskList,CMetricDistance *metric,CPartitionMethod *pm,int *numInternalNodes,int start,int size,int singlePivotFanout,int maxLeafSize, double tr, char *sop, char *pbop)
{
	(*numInternalNodes)++;

	/*get pivots of current node*/
    vector<shared_ptr<CMetricData> > pivots;
    task->getPivots(pivots);


    /*partition current partition into several smaller child partitons,the selected pivots are not in the section that will be partitioned in child partitions*/
	CPartitionResults pr=pm->partition(metric,pivots,task->getDataList(),start,size - task->getPivotNumber(),singlePivotFanout,maxLeafSize, tr, sop, pbop);
    int childrenNumber=pr.partitionSize();

    /*create an internal node and assign its addres to the children list of parent node*/
    vector<shared_ptr<CIndexNode> > *subTreeNode = new vector<shared_ptr<CIndexNode> >(childrenNumber-1);
    shared_ptr< CMVPInternalNode> node(new CMVPInternalNode(pivots,pr.getLowerBounds(),pr.getUpperBounds(),*subTreeNode,task->getMyHeight()+1) );
    node->setChildSize(childrenNumber-1);


    task->setNode((shared_ptr<CIndexNode>) node );

    stMutexB.lock();
    newTaskList->push_back(task);
    stMutexB.unlock();

    /*create several tasks base on each of the child partitions created before and then push these tasks to the global variable queue for the iteration of building child trees*/
    for(int i=childrenNumber-1;i>0;i--)
    {
        int j=0;
	shared_ptr<Task> newTask( new Task(task->getDataList(),node,pr.getPartition(i-1),pr.getPartition(i),i,task->getMyHeight()+1));       
	stMutexA.lock();
        taskList->push_back(newTask);
	stMutexA.unlock();
    }

    return 0;
}
//
int standardCreateAndWriteLeafNode(shared_ptr<Task> task,ofstream *out,vector<shared_ptr<Task> > *taskList,string fileName,long *filePointer,int *numLeaf,CMetricDistance *metric,long *rootAddress,list<shared_ptr<Task> > *leafTaskList)
{

    (*numLeaf)++;

	/*get all the objects of current partition*/
    vector<shared_ptr<CMetricData> > children;
    task->getDataPoints(children);

    /*get all the pivots of current node*/
    vector<shared_ptr<CMetricData> > pivots;
    task->getPivots(pivots);

    vector<vector<double> > distance;

    /*calcualte the distance from each of the objects of current parition to every pivots*/
    for(vector<CMetricData*>::size_type i=0;i<pivots.size();i++)
    {
        vector<double> ve;

        for(vector<CMetricData*>::size_type j=0;j<children.size();j++)
        {
            ve.push_back(metric->getDistance(children[j].get(),pivots[i].get()));
        }

        distance.push_back(ve);
    }

    /*create a leaf node and assign its memory address to the child list of parent node*/
    shared_ptr<CMVPLeafNode> mvpLeafNode(new CMVPLeafNode(pivots,children,distance,task->getMyHeight()+1));

    if(*rootAddress==-2)
    {        

        task->setNode((shared_ptr<CIndexNode>)mvpLeafNode);
		taskList->push_back(task);
        return 0;
    }
    else
    {
	stMutexO.lock();
	//out->open(fileName.c_str(),ios::in|ios::out|ios::binary);
	//out->seekp(*filePointer);
	//char type[6] = "LNODE";
      	//((CMVPInternalNode*)task->getParentIndex().get())->getSubTreeAddress()[task->getMyIndex()-1]=*filePointer;
      	//out->write(type,6*sizeof(char));
	//*filePointer=*filePointer+(long)mvpLeafNode->writeExternal(*out)+6*sizeof(char);
	//out->close();
	
        task->setNode((shared_ptr<CIndexNode>)mvpLeafNode);
	leafTaskList->push_back(task);
	stMutexO.unlock();
	return 0;
    }
  
}

void writeparallelleaftask(list<shared_ptr<Task> > *leafTaskList, ofstream *out, long *filePointer)
{
	int n = leafTaskList->size();
//	cout << "       leaf num:" << n << endl;
	for (int i=0; i<n; i++)
	{
		shared_ptr<Task> task = leafTaskList->back();
		out->seekp(*filePointer);
//		cout << "add[" << task->getMyIndex()-1 <<"]:" << *filePointer << endl;		
		char type[6] = "LNODE";
		((CMVPInternalNode*)task->getParentIndex().get())->getSubTreeAddress()[task->getMyIndex()-1]=*filePointer;
		out->write(type,6*sizeof(char));
		*filePointer=*filePointer+(long)((task->getNode())->writeExternal(*out)+6*sizeof(char));
		leafTaskList->pop_back();
	}
}

void writeparallelleaftaskwithmultipleprocess(list<shared_ptr<Task> > *leafTaskList, ofstream *out, long *filePointer, sem_t *semmutex)
{
	int n = leafTaskList->size();
//	cout << "       leaf num:" << n << endl;
	for (int i=0; i<n; i++)
	{
		shared_ptr<Task> task = leafTaskList->back();
		sem_wait(semmutex);
		out->seekp(*filePointer);
//		cout << "add[" << task->getMyIndex()-1 <<"]:" << *filePointer << endl;		
		char type[6] = "LNODE";
		((CMVPInternalNode*)task->getParentIndex().get())->getSubTreeAddress()[task->getMyIndex()-1]=*filePointer;
		out->write(type,6*sizeof(char));
		*filePointer=*filePointer+(long)((task->getNode())->writeExternal(*out)+6*sizeof(char));
		out->flush();
		sem_post(semmutex);
	//	if (out->tellp() != *filePointer)
	//	{
	//		cout << __LINE__ << " ERROR ERROR" << endl;
	//		exit(1);
	//	}
		leafTaskList->pop_back();
	}
}

void writeInternalNodewithmultipleprocess(shared_ptr<Task>task,ofstream& out, long *filePointer, sem_t *semmutex)
{

    	//cout <<getpid() <<" begin wait, fp:" << *filePointer << endl;
    	sem_wait(semmutex);
    	out.seekp(*filePointer);

    	CMVPInternalNode* parentNode = (CMVPInternalNode*)(task->getParentIndex().get());
	int num = task->getMyIndex();
	/*cout<<(parentNode->getSubTreeAddress())[0]<<endl;*/
	//cout << "--------------------------------------------------------------------------------" << endl;
    	parentNode->getSubTreeAddress()[task->getMyIndex()-1]=*filePointer;
	//cout << "--------------------------------------------------------------------------------" << endl;
    	char *type = "INODE";
    	out.write(type,6*sizeof(char));
    	*filePointer+=(long)task->getNode()->writeExternal(out)+6*sizeof(char);

		
    	//cout <<getpid() << " begin post, fp:" << *filePointer << endl;
	out.flush();
      	sem_post(semmutex);

//	if (out.tellp() != *filePointer)
//	{
//		cout << __LINE__ << " ERROR ERROR" << endl;
//		exit(1);
//	}
}



void standardThreadOfCreateInternalOrLeafNodeRoot(parallelBlock *pb)
{	
	shared_ptr<Task> task;
	if(pb->taskList->size()>0)
	{
		vector<shared_ptr<Task> >::iterator ite=pb->taskList->end();
		ite--;
		task=*ite;
		pb->taskList->erase(ite);
	}

	if(task!=NULL)
	{
		/*get the number of pivot*/
		//(*(pb->numPivots))=((*(pb->numPivots))>=task.get()->getSize())?task.get()->getSize():(*(pb->numPivots));
		int pivotNum=((*(pb->numPivots)>=task.get()->getSize())?task.get()->getSize():(*(pb->numPivots)));

		/*selecte several piovt from current partition based on the given parameter*/
		vector<int> pivotsIndex;
		pivotsIndex=pb->psm->selectPivots(pb->metric,task.get()->getDataList(),task.get()->getStart(),task.get()->getSize(),pivotNum);

		/*move the pivot to the end of current partition*/
		task.get()->groupPivotsAtEnd(pivotsIndex);

		int remainDataSize=task.get()->getSize() - pivotNum;

		if(remainDataSize>pb->maxLeafSize)
		{
			standardCreateInternalNodeToWrite(task,pb->taskList,pb->newTaskList,pb->metric,pb->pm,pb->numInternalNode,task.get()->getStart(),task.get()->getSize(),pb->singlePivotFanout,pb->maxLeafSize, pb->tr, pb->sop, pb->pbop);
		}
		else
		{
			standardCreateAndWriteLeafNode(task,pb->fstr,pb->newTaskList,pb->fileName,pb->filePointer,pb->numLeafNode,pb->metric,pb->rootAddress, pb->leafTaskList);
		}

	}

	return;

}

///***************************************************C++11 STANDARD VERSION MULTI-THREAD CODE END*************************************************************/
int parallelInternalNode(shared_ptr<Task> task,vector<shared_ptr<Task> > *taskList,vector<shared_ptr<Task> >*newTaskList,CMetricDistance *metric,CPartitionMethod *pm,int *numInternalNodes,int start,int size,int singlePivotFanout,int maxLeafSize, double tr, char *sop, char* pbp)
{
	(*numInternalNodes)++;

	/*get pivots of current node*/
    vector<shared_ptr<CMetricData> > pivots;
    task->getPivots(pivots);
    //tmp.lock();
    //cout << "*********************************************************" << endl;
    //tmp.unlock();
    //clock_t bp = times(NULL);
    /*partition current partition into several smaller child partitons,the selected pivots are not in the section that will be partitioned in child partitions*/
	CPartitionResults pr=pm->partition(metric,pivots,task->getDataList(),start,size - task->getPivotNumber(),singlePivotFanout,maxLeafSize,tr,sop,pbp);
    int childrenNumber=pr.partitionSize();

    int flag = 0;
    if (childrenNumber==2)
    {
	flag = 1;
    }

    if (flag == 0)
    {
   	 /*create an internal node and assign its addres to the children list of parent node*/
   	 vector<shared_ptr<CIndexNode> > *subTreeNode = new vector<shared_ptr<CIndexNode> >(childrenNumber-1);
   	 shared_ptr< CMVPInternalNode> node(new CMVPInternalNode(pivots,pr.getLowerBounds(),pr.getUpperBounds(),*subTreeNode,task->getMyHeight()+1) );
   	 node->setChildSize(childrenNumber-1);
   	
 
   	 //clock_t ep = times(NULL);
   	 //tmp.lock();
   	 //cout << "partition:" << (ep-bp)/100.0 << endl;
   	 //cout << "*********************************************************" << endl;
   	 //tmp.unlock();
   	 task->setNode((shared_ptr<CIndexNode>) node );

   	 newTaskList->push_back(task);

   	 /*create several tasks base on each of the child partitions created before and then push these tasks to the global variable queue for the iteration of building child trees*/
   	 for(int i=childrenNumber-1;i>0;i--)
   	 {
   	     int j=0;
   	     shared_ptr<Task> newTask( new Task(task->getDataList(),node,pr.getPartition(i-1),pr.getPartition(i),i,task->getMyHeight()+1));       
   	     taskList->push_back(newTask);
   	 }

    	return 0;
   }
   else
   {
	task->setFlag(1);
	taskList->push_back(task);
        return 0;
   }
}

int parallelLeafNode(shared_ptr<Task> task,ofstream *out,vector<shared_ptr<Task> > *taskList,string fileName,long *filePointer,int *numLeaf,CMetricDistance *metric,long *rootAddress,list<shared_ptr<Task> > *leafTaskList)
{

    (*numLeaf)++;

	/*get all the objects of current partition*/
    vector<shared_ptr<CMetricData> > children;
    task->getDataPoints(children);

    /*get all the pivots of current node*/
    vector<shared_ptr<CMetricData> > pivots;
    task->getPivots(pivots);

    vector<vector<double> > distance;

    /*calcualte the distance from each of the objects of current parition to every pivots*/
    for(vector<CMetricData*>::size_type i=0;i<pivots.size();i++)
    {
        vector<double> ve;

        for(vector<CMetricData*>::size_type j=0;j<children.size();j++)
        {
            ve.push_back(metric->getDistance(children[j].get(),pivots[i].get()));
        }

        distance.push_back(ve);
    }
    
    /*create a leaf node and assign its memory address to the child list of parent node*/
    shared_ptr<CMVPLeafNode> mvpLeafNode(new CMVPLeafNode(pivots,children,distance,task->getMyHeight()+1));

    if(*rootAddress==-2)
    {        

        task->setNode((shared_ptr<CIndexNode>)mvpLeafNode);
		taskList->push_back(task);
        return 0;
    }
    else
    {
	//out->open(fileName.c_str(),ios::in|ios::out|ios::binary);
	//out->seekp(*filePointer);
	//char type[6] = "LNODE";
      	//((CMVPInternalNode*)task->getParentIndex().get())->getSubTreeAddress()[task->getMyIndex()-1]=*filePointer;
      	//out->write(type,6*sizeof(char));
	//*filePointer=*filePointer+(long)mvpLeafNode->writeExternal(*out)+6*sizeof(char);
	//out->close();
	
        task->setNode((shared_ptr<CIndexNode>)mvpLeafNode);
	leafTaskList->push_back(task);
	return 0;
    }
  
}

int parallelBuildingChildrenOfRoot(parallelBlock *pb)
{
	int treeHeight = 0;
	clock_t bps = times(NULL);	
	while (pb->taskList->size()>0)
	{
		shared_ptr<Task> task;
		vector<shared_ptr<Task> >::iterator ite=pb->taskList->end();
		ite--;
		task=*ite;
		pb->taskList->erase(ite);
	
		if(task!=NULL)
		{
			treeHeight = (treeHeight >= task->getMyHeight()) ? treeHeight:task->getMyHeight();
			if (task->getFlag() == 0)
                        {
				/*get the number of pivot*/
				//(*(pb->numPivots))=((*(pb->numPivots))>=task.get()->getSize())?task.get()->getSize():(*(pb->numPivots));
				int pivotNum=((*(pb->numPivots)>=task.get()->getSize())?task.get()->getSize():(*(pb->numPivots)));
	
				/*selecte several piovt from current partition based on the given parameter*/
				vector<int> pivotsIndex;
				pivotsIndex=pb->psm->selectPivots(pb->metric,task.get()->getDataList(),task.get()->getStart(),task.get()->getSize(),pivotNum);
	
				/*move the pivot to the end of current partition*/
				task.get()->groupPivotsAtEnd(pivotsIndex);
				
				int remainDataSize=task.get()->getSize() - pivotNum;
	
				if(remainDataSize>pb->maxLeafSize)
				{
					parallelInternalNode(task,pb->taskList,pb->newTaskList,pb->metric,pb->pm,pb->numInternalNode,task.get()->getStart(),task.get()->getSize(),pb->singlePivotFanout,pb->maxLeafSize, pb->tr, pb->sop, pb->pbop);
				}
				else
				{
					parallelLeafNode(task,pb->fstr,pb->newTaskList,pb->fileName,pb->filePointer,pb->numLeafNode,pb->metric,pb->rootAddress, pb->leafTaskList);
				}
			}
			else
			{
					parallelLeafNode(task,pb->fstr,pb->newTaskList,pb->fileName,pb->filePointer,pb->numLeafNode,pb->metric,pb->rootAddress, pb->leafTaskList);
			}
		}
	}
	clock_t eps = times(NULL);
	pb->t = (eps-bps)/100.0;

	//cout << "time:" << pb->t << " pb[" << pb->num << "]:" << pb->leafTaskList->size() << " &pb:" << pb << " &ltl:" << pb->leafTaskList <<endl;
	
	return treeHeight;

}

void CMVPIndex::parallelBuildingMVPtree(vector<shared_ptr<CMetricData> > &dataObjectList, int maxThreadNum)
{
	clock_t begp, endp, begr, endr, begi, endi, begleaf, endleaf, begwi, endwi;

	//cout<<"***************************************************************************************start parallel building"<<endl;
	begp = times(NULL);
	ofstream outfile(fileName.c_str(),ios::out|ios::binary|ios::trunc);
//	outfile.close();

	cout<<__FILE__<<__LINE__<<" output filename:"<<fileName<<endl;

	filePointer=0;
	int treeHeight = 1;
	if(dataObjectList.size() <= maxLeafSize+numPivots)
		rootAddress=-2;
	else
		rootAddress=-1;

	vector<shared_ptr<Task> > taskList;
	vector<shared_ptr<Task> > ntaskList;
	vector<shared_ptr<Task> > newTaskList;
	list<shared_ptr<Task> > leafTaskList;
	
	shared_ptr<Task> task(new Task(dataObjectList,shared_ptr<CIndexNode>(),0,dataObjectList.size(),0,0));//first task

	taskList.push_back(task);

	parallelBlock pba;
	{
		pba.fileName=fileName;
		pba.filePointer=&filePointer;
		pba.fstr=&outfile;
		pba.maxLeafSize=maxLeafSize;
		pba.metric=metric;
		pba.leafTaskList=&leafTaskList;
		pba.newTaskList=&newTaskList;
		pba.taskList=&taskList;
		pba.ntaskList=&ntaskList;
		pba.numInternalNode=&numInternalNodes;
		pba.numLeafNode=&numLeaf;
		pba.numPivots=&numPivots;
		pba.pm=pm;
		pba.psm=psm;
		pba.rootAddress=&rootAddress;
		pba.singlePivotFanout=singlePivotFanout;
		pba.flag = 0;
		pba.tr = trisectionRadius;
		pba.sop = selectOptimalPivots;
		pba.pbop = partitionByOnePivot;
	}

	begr = times(NULL);
	standardThreadOfCreateInternalOrLeafNodeRoot(&pba);
	endr = times(NULL);
	
	//cout << "root:" << (endr-begr)/100.0 << endl;
	
	begi = times(NULL);
	int num = taskList.size();
	//int num = 1;
	//cout << "  task num:" << num << endl;

	char myshm[] = "myshm.x";
	int fd;
	if ( (fd = open(myshm, O_RDWR | O_CREAT | O_TRUNC, 00700)) == -1)
	{
		perror(strerror(errno));
		cout << "Failed to open " << myshm << endl;
		exit(1);
	} 
	write(fd, &filePointer, sizeof(long));
	
	long *ptr;
	if ( (ptr = (long*)mmap(NULL, sizeof(long), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
	{
		cout << "Failed to map shared memory." << endl;
		exit(1);
	}
	close(fd);	

	char mysem[] = "mysem.x";
	sem_t *semmutex;
	if ( (semmutex = sem_open(mysem, O_CREAT|O_EXCL, 0, 1)) == SEM_FAILED)
	{
		cout << "Failed to create semphore." << endl;
		exit(1);
	}

	if (sem_unlink(mysem) == -1)
	{	
		cout << "Sem_unlink " << mysem << " error." << endl;
		exit(1);
	}

	setbuf(stdout, NULL);	

	int pdf[2];
	
	if (pipe(pdf) < 0)
	{
		cout << "failed to create pipe." << endl;
		exit(1);
	}

	parallelBlock *pblist = new parallelBlock[num];
	for (int i=0; i<num; i++)
	{
		shared_ptr<Task> task = taskList.back();
		if (fork() == 0)
		{	
			close(pdf[0]);
			//cout << "------------------------------begin pblist["<< i << "]" << endl;	
			pblist[i].fileName = fileName;
            pblist[i].filePointer = ptr;
            pblist[i].fstr = &outfile;
            pblist[i].maxLeafSize = maxLeafSize;
            pblist[i].metric = metric;
            pblist[i].leafTaskList = &leafTaskList; 
			pblist[i].newTaskList = &newTaskList; 
            pblist[i].taskList = new vector<shared_ptr<Task> >();
			pblist[i].taskList->push_back(task);
            pblist[i].ntaskList = &ntaskList;
            pblist[i].numInternalNode = &numInternalNodes;
            pblist[i].numLeafNode = &numLeaf;
            pblist[i].numPivots = &numPivots;
            pblist[i].pm = pm;
            pblist[i].psm = psm;
            pblist[i].rootAddress = &rootAddress;
            pblist[i].singlePivotFanout = singlePivotFanout;
            pblist[i].flag = 0;
			pblist[i].num = i;
			pblist[i].t = 0;
			pblist[i].semmutex = semmutex;
			pba.tr = trisectionRadius;
			pba.sop = selectOptimalPivots;
			pba.pbop = partitionByOnePivot;

			
			treeHeight = parallelBuildingChildrenOfRoot(&pblist[i]);
			//cout <<__LINE__ << " &pb[" << i << "]:" << &pblist[i] << " pblist[i].leafTaskList->size:" << pblist[i].leafTaskList->size()  << " leafTaskList.size:" << leafTaskList.size() << " &ltl:" << &leafTaskList << endl;
		
			//cout <<__LINE__ << " leaf num:" <<pblist[i].leafTaskList->size() << endl;	
			writeparallelleaftaskwithmultipleprocess(pblist[i].leafTaskList, &outfile, ptr, semmutex);
			
			//cout <<__LINE__ << " inter num:" << pblist[i].newTaskList->size() << endl;
			vector<shared_ptr<Task>>::iterator ite;	
			while (pblist[i].newTaskList->size() > 1)
			{
				ite = pblist[i].newTaskList->end();
				ite--;
				shared_ptr<Task> temp=*ite;
				
				writeInternalNodewithmultipleprocess(temp, outfile, ptr, semmutex);
			
				pblist[i].newTaskList->erase(ite);
			}
		
			ite = pblist[i].newTaskList->end();
			shared_ptr<Task> task = *ite;			
    		CMVPInternalNode* parentNode = (CMVPInternalNode*)(task->getParentIndex().get());
    		long add = parentNode->getSubTreeAddress()[task->getMyIndex()-1];

			char buffer[20] = {'\0'};
			sprintf(buffer, " %d %ld %d", i,add,treeHeight);
			write (pdf[1], buffer, strlen(buffer));
			//perror(strerror(errno));
			//cout << "------------------------------end pblist["<< i << "]" << endl;	
			outfile.flush();
			exit(0);
		}
		taskList.pop_back();
	}

	for (int i=0; i<num; i++)
	{
		int status = 0;
		int pid = wait(&status);
		//cout << pid << " over!" << endl;
	}
	outfile.flush();
	//cout << "output.pos:" << outfile.tellp() << " filepoinrter:" << *ptr << endl;
	outfile.close();
	endi = times(NULL);
	
	//cout << "internal:" << (endi-begi)/100.0 << endl;
	
	char buffer[500];
	bzero(buffer, sizeof(buffer));
	read(pdf[0], buffer, sizeof(buffer)-1);

	stringstream ss;
	ss << buffer;
	//cout << "buffer:" << buffer << endl;

	shared_ptr<Task> temp = newTaskList.back();
   	CMVPInternalNode* node = (CMVPInternalNode*)(temp->getNode().get());
	for (int i=0; i<num; i++)
	{
		int a = 0;
		long add = 0;
		int temph = 0;
		ss >> a;
		if (a < 0 || a >= num)
		{
			cout << "Child address error" << endl;
			exit(1);
		}
		ss >> add;
		ss >> temph;
    		node->getSubTreeAddress()[a] = add;
		treeHeight = (treeHeight > temph) ? treeHeight:temph;
	}

//	for (int i=0; i<num; i++)
//	{
//		cout << "c[" << i << "]:" << node->getSubTreeAddress()[i] << endl;
//	}
	
	
	filePointer = *ptr;
	writeRoot(temp,outfile,treeHeight);
	
	delete [] pblist;

	endp = times(NULL);
	//cout << "time for parallel building is:" << (endp-begp)/100.0 << endl;
//	cout<<"*************************************build over*******************************************************"<<endl;

}

#else

/***************************************************C++11 STANDARD VERSION MULTI-THREAD CODE BEGIN***************************************************************/

/*DATA STRUCT FOR BOTH WINDOWS AND C++11 STANDARD VERSION OF MULTI-THREAD CODING*/

struct parallelBlock
{

	/*constuctor*/

	//parallelBlock(){cout<<"building a parallel block"<<endl;}

	/*desturctor*/

	//~parallelBlock(){cout<<"rootTask.useCount:"<<rootTask.use_count()<<" destroying a parallel block"<<endl;}	



	ofstream *fstr;

	int *numPivots;

	CMetricDistance *metric;

	CPivotSelectionMethod *psm;

	CPartitionMethod *pm;



	shared_ptr<Task> task;

	list<shared_ptr<Task> > childTaskList;

	list<shared_ptr<Task> > wTaskList;

	bool isleaf;

	int maxLeafSize;

	int singlePivotFanout;

	int *numInternalNode;

	int *numLeafNode;



	string fileName;

	long *filePointer;

	long *rootAddress;



	shared_ptr<Task> rootTask;


	double trisectionRadius;
	char* selectOptimalPivots;
	char* partitionByOnePivot;

};





mutex stMutexA;

mutex stMutexB;

mutex stMutexO;

mutex stMutexN;



int stdThreadNumA=0;

int stdThreadNumB=0;

int standardCreateInternalNodeToWrite(shared_ptr<Task> task,list<shared_ptr<Task> > *taskList,list<shared_ptr<Task> >*newTaskList,CMetricDistance *metric,CPartitionMethod *pm,int *numInternalNodes,int start,int size,int singlePivotFanout,int maxLeafSize,double trisectionRadius,char* selectOptimalPivots,char* partitionByOnePivot)
{

	(*numInternalNodes)++;



	/*get pivots of current node*/

	vector<shared_ptr<CMetricData> > pivots;

	task->getPivots(pivots);





	/*partition current partition into several smaller child partitons,the selected pivots are not in the section that will be partitioned in child partitions*/

	CPartitionResults pr=pm->partition(metric,pivots,task->getDataList(),start,size - task->getPivotNumber(),singlePivotFanout,maxLeafSize,trisectionRadius,selectOptimalPivots,partitionByOnePivot);

	int childrenNumber=pr.partitionSize();

	int flag=0;

	if(childrenNumber==2)
	{
		flag=1;
		/*
		double distance=metric->getDistance(task->dataList[task->start].get(),task->dataList[task->start+1].get());

		for(int i=task->start+1;i<task->start+task->size-task->numPivots;i++)
		{

			if(metric->getDistance(task->dataList[task->start].get(),task->dataList[i].get())==distance)
			{

				flag=1;

			}

			else
			{

				flag=0;

				break;

			}

		}
		*/

	}

	//pthread_mutex_lock(&stMutexA);

	//if(task->getMyHeight()==6)

	//	cerr<<"task.Height:"<<task->getMyHeight()<<" INTER-NODE task->index:"<<task->getMyIndex()<<" pivotsize:"<<pivots.size()<<" task.start:"<<start<<" size:"<<size<<" childpartitionsize:"<<pr.partitionSize()-1<<endl;

	//pthread_mutex_unlock(&stMutexA);

	/*create an internal node and assign its addres to the children list of parent node*/

	if(flag==0)
	{

		vector<shared_ptr<CIndexNode> > *subTreeNode = new vector<shared_ptr<CIndexNode> >(childrenNumber-1);

		shared_ptr< CMVPInternalNode> node(new CMVPInternalNode(pivots,pr.getLowerBounds(),pr.getUpperBounds(),*subTreeNode,task->getMyHeight()+1) );

		node->setChildSize(childrenNumber-1);





		task->setNode((shared_ptr<CIndexNode>)node);



		newTaskList->push_back(task);



		/*create several tasks base on each of the child partitions created before and then push these tasks to the global variable queue for the iteration of building child trees*/

		for(int i=childrenNumber-1;i>0;i--)
		{

			int j=0;

			shared_ptr<Task> newTask( new Task(task->getDataList(),node,pr.getPartition(i-1),pr.getPartition(i),i,task->getMyHeight()+1));       

			taskList->push_back(newTask);

		}

		return 0;

	}
	else
	{

		task->setFlag(1);
		taskList->push_back(task);

	}

}



int standardCreateAndWriteLeafNode(shared_ptr<Task> task,ofstream *out,list<shared_ptr<Task> > *taskList,string fileName,long *filePointer,int *numLeaf,CMetricDistance *metric,long *rootAddress)
{



	(*numLeaf)++;



	/*get all the objects of current partition*/

	vector<shared_ptr<CMetricData> > children;

	task->getDataPoints(children);



	/*get all the pivots of current node*/

	vector<shared_ptr<CMetricData> > pivots;

	task->getPivots(pivots);



	vector<vector<double> > distance;



	/*calcualte the distance from each of the objects of current parition to every pivots*/

	for(vector<CMetricData*>::size_type i=0;i<pivots.size();i++)
	{

		vector<double> ve;



		for(vector<CMetricData*>::size_type j=0;j<children.size();j++)
		{

			ve.push_back(metric->getDistance(children[j].get(),pivots[i].get()));

		}



		distance.push_back(ve);

	}



	/*create a leaf node and assign its memory address to the child list of parent node*/

	shared_ptr<CMVPLeafNode> mvpLeafNode(new CMVPLeafNode(pivots,children,distance,task->getMyHeight()+1));



	if(*rootAddress==-2)
	{        



		task->setNode((shared_ptr<CIndexNode>)mvpLeafNode);

		taskList->push_back(task);

		return 0;

	}
	else
	{

		//pthread_mutex_lock(&stMutexO);

		//out->open(fileName.c_str(),ios::in|ios::out|ios::binary)

		//out->seekp(*filePointer);

		char type[6] = "LNODE";

		((CMVPInternalNode*)task->getParentIndex().get())->getSubTreeAddress()[task->getMyIndex()-1]=*filePointer;

		out->seekp(*filePointer);

		out->write(type,6*sizeof(char));

		*filePointer=*filePointer+(long)mvpLeafNode->writeExternal(*out)+6*sizeof(char);

		//out->close();

		//pthread_mutex_unlock(&stMutexO);

		return 0;

	}



}



void* standardThreadOfCreateInternalOrLeafNode(void *vpb)
{

	parallelBlock *pb=(parallelBlock*)vpb;

	if(pb->task->getFlag()==0)
	{

		int pivotNum=((*(pb->numPivots)>pb->task->getSize())?pb->task->getSize():(*(pb->numPivots)));

		vector<int> pivotsIndex;

		pivotsIndex=pb->psm->selectPivots(pb->metric,pb->task->getDataList(),pb->task->getStart(),pb->task->getSize(),pivotNum);

		//pthread_mutex_lock(&stMutexB);

		//if(pb->task->getMyHeight()==1)

		//	cerr<<"index:"<<pb->task->getMyIndex()<<" task.start:"<<pb->task->getStart()<<" task.size:"<<pb->task->getSize()<<" pivotnum:"<<pivotsIndex.size()<<" pivotNum:"<<pivotNum<<endl;

		//pthread_mutex_unlock(&stMutexB);

		pb->task->groupPivotsAtEnd(pivotsIndex);



		int remainDataSize=pb->task->getSize()-pivotsIndex.size();



		if(remainDataSize>pb->maxLeafSize)
		{

			standardCreateInternalNodeToWrite(pb->task,&(pb->childTaskList),&(pb->wTaskList),pb->metric,pb->pm,pb->numInternalNode,pb->task->getStart(),pb->task->getSize(),pb->singlePivotFanout,pb->maxLeafSize,pb->trisectionRadius,pb->selectOptimalPivots,pb->partitionByOnePivot);

		}
		else
		{	

			pb->isleaf=1;

			//standardCreateAndWriteLeafNode(pb->task,pb->fstr,&(pb->wTaskList),pb->fileName,pb->filePointer,pb->numLeafNode,pb->metric,pb->rootAddress);

		}

	}

	return 0;

}





void CMVPIndex::parallelBuildingMVPtree(vector<shared_ptr<CMetricData> > &dataObjectList,int maxThreadNum)
{

	ofstream outfile(fileName.c_str(),ios::out|ios::binary);

	filePointer=0;

	int maxTreeHeight=0;



	if(dataObjectList.size()<=maxLeafSize+numPivots)

		rootAddress=-2;

	else

		rootAddress=-1;


	list<parallelBlock*> buildTaskList;

	list<shared_ptr<Task> > tobewritetasklist;

	list<parallelBlock*> leafBuildTaskList;

	shared_ptr<Task> task(new Task(dataObjectList,shared_ptr<CIndexNode>(),0,dataObjectList.size(),0,0));//first task



	numPivots=(numPivots>=task->size)?task->size:numPivots;

	vector<int> pivotsIndex;

	pivotsIndex=psm->selectPivots(metric,task->dataList,task->start,task->size,numPivots);

	task->groupPivotsAtEnd(pivotsIndex);



	list<shared_ptr<Task> > childtasklist;

	standardCreateInternalNodeToWrite(task,&childtasklist,&tobewritetasklist,metric,pm,&numInternalNodes,task->start,task->size,singlePivotFanout,maxLeafSize,trisectionRadius,selectOptimalPivots,partitionByOnePivot);





	while(childtasklist.size()>0)
	{

		parallelBlock *pba=new parallelBlock;



		pba->fileName=fileName;

		pba->filePointer=&filePointer;

		pba->fstr=&outfile;

		pba->maxLeafSize=maxLeafSize; 

		pba->metric=metric; 

		pba->task=childtasklist.back();

		pba->numInternalNode=&numInternalNodes;

		pba->numLeafNode=&numLeaf;

		pba->numPivots=&numPivots;

		pba->pm=pm;

		pba->psm=psm;

		pba->rootAddress=&rootAddress;

		pba->singlePivotFanout=singlePivotFanout;

		pba->isleaf=0;

		pba->trisectionRadius = trisectionRadius;
		pba->selectOptimalPivots = selectOptimalPivots;
		pba->partitionByOnePivot = partitionByOnePivot;

		childtasklist.pop_back();

		buildTaskList.push_back(pba);

	}



	/*compute the maximum number of node in the index will be built,use the number as the maximum of thread created in the future */
	//int dataObjectSize=dataObjectList.size();
	//int numThreadNum=pow(pow(singlePivotFanout,numPivots),ceil(log(dataObjectSize)/log(maxLeafSize)));




	cerr<<"Number of thread:"<<maxThreadNum<<endl;
	while(buildTaskList.size()>0)
	{

		int taskNum=buildTaskList.size();



		thread *threadList=new thread[maxThreadNum];

		list<parallelBlock*> newBuildTaskList;

		int j=0;	
		for(int i=0;i<taskNum;i++)
		{

			if(j<maxThreadNum)
			{

				parallelBlock *pb=buildTaskList.back();


				try
				{
					threadList[j]=thread(standardThreadOfCreateInternalOrLeafNode,pb);
				}
				catch(std::system_error &e)
				{
					cerr<<__FILE__<<__LINE__<<" Exception:"<<e.what()<<" arise at "<<e.code()<<" when i:"<<i<<endl;
					i--;
					continue;
				}



				newBuildTaskList.push_back(pb);

				buildTaskList.pop_back();

				j++;

			}
			else
			{

				for(int k=0;k<j;k++)
				{

					threadList[k].join();

				}

				j=0;

			}

		}



		for(int i=0;i<j;i++)
		{

			void *childthread=0;

			threadList[i].join();

		}

		delete [] threadList;


		while(newBuildTaskList.size()>0)
		{

			parallelBlock *pb=newBuildTaskList.back();

			newBuildTaskList.pop_back();



			tobewritetasklist.insert(tobewritetasklist.end(),pb->wTaskList.begin(),pb->wTaskList.end());



			if(!pb->isleaf&&pb->task->getFlag()==0)
			{

				int newTaskNum=pb->childTaskList.size();

				for(int i=0;i<newTaskNum;i++)
				{

					shared_ptr<Task> task=pb->childTaskList.back();

					pb->childTaskList.pop_back();



					parallelBlock *newpb=new parallelBlock;



					newpb->fileName=fileName;

					newpb->filePointer=&filePointer;

					newpb->fstr=&outfile;

					newpb->maxLeafSize=maxLeafSize;

					newpb->metric=metric;

					newpb->task=task;

					newpb->numInternalNode=&numInternalNodes;

					newpb->numLeafNode=&numLeaf;

					newpb->numPivots=&numPivots;

					newpb->pm=pm;

					newpb->psm=psm;

					newpb->rootAddress=&rootAddress;

					newpb->singlePivotFanout=singlePivotFanout;

					newpb->isleaf=0;

					newpb->trisectionRadius = trisectionRadius;
					newpb->selectOptimalPivots = selectOptimalPivots;
					newpb->partitionByOnePivot = partitionByOnePivot;

					buildTaskList.push_back(newpb);



				}

				maxTreeHeight=(maxTreeHeight>=pb->task->getMyHeight())?maxTreeHeight:pb->task->getMyHeight();

				delete pb;	

			}
			else
			{



				leafBuildTaskList.push_back(pb);

				maxTreeHeight=(maxTreeHeight>=pb->task->getMyHeight())?maxTreeHeight:pb->task->getMyHeight();

			}

			//clock_t endt=times(NULL);

			//cout<<__FILE__<<__LINE__<<" time elapsed:"<<(endt-begt)/100.00<<endl;

		}



	}

	int leafTaskNum=leafBuildTaskList.size();
	for(int i=0;i<leafTaskNum;i++)
	{

		parallelBlock *pb=leafBuildTaskList.back();

		if(pb->task->getFlag()==1&&pb->task->getMyHeight()==0)
		{
			this->rootAddress=-2;
			*pb->rootAddress=-2;
			standardCreateAndWriteLeafNode(pb->task,pb->fstr,&(pb->wTaskList),pb->fileName,pb->filePointer,pb->numLeafNode,pb->metric,pb->rootAddress);	
			break;
		}
		else
		{
			leafBuildTaskList.pop_back();

			standardCreateAndWriteLeafNode(pb->task,pb->fstr,&(pb->wTaskList),pb->fileName,pb->filePointer,pb->numLeafNode,pb->metric,pb->rootAddress);			

			delete pb;
		}

	}

	while(tobewritetasklist.size()>1)
	{

		shared_ptr<Task> temp=tobewritetasklist.back();



		writeInternalNode(temp,outfile);



		tobewritetasklist.pop_back();

	}

	outfile.close();
	if(leafBuildTaskList.size()>0)
		writeRoot(leafBuildTaskList.back()->task,outfile,maxTreeHeight);
	else if(tobewritetasklist.size()>0)
		writeRoot(tobewritetasklist.back(),outfile,maxTreeHeight);

}



/***************************************************C++11 STANDARD VERSION MULTI-THREAD CODE END*************************************************************/
#endif

/*
*brief : corresponding to "loadRootToLeaf", "loadLeafToRoot" means that we don't immidately write root node into file, 
		but retain it until all other nodes been writed.

*/

void CMVPIndex::loadLeafToRoot(vector<shared_ptr<CMetricData> > &dataObjects)
{
	/* product index file */
	ofstream out;
	out.open(fileName.c_str(),ios::binary);
	out.close();

	/* file pointer, decide where to write */
	filePointer = 0;

	/* max tree height */
	int maxTreeHeight = 0;

	/*  */
	if(dataObjects.size()<=maxLeafSize+numPivots)
	{// only has one node
		rootAddress = -2;
	}
	else
	{// more than one node
		rootAddress = -1;
	}

	/* an iterator of taskList*/
	vector<shared_ptr<Task> >::iterator taskToGet;

	/* create first task */
	shared_ptr<Task> task(new Task(dataObjects,shared_ptr<CIndexNode>(),0,dataObjects.size(),0,0));

	/* push into taskList */
	this->taskList.push_back(task);

	/******************         use every task in the list to create an internal node or leaf node until the the list is null       ***********/

	while(this->taskList.size()>0)
	{

		taskToGet = this->taskList.end();

		taskToGet--; // point to the last task in taskList

		task = *taskToGet;//get the task

		/* update max tree height */
		maxTreeHeight = maxTreeHeight >= task->getMyHeight() ? maxTreeHeight : task->getMyHeight();

		if(task->getNode()==0)
		{// current task has no node, so need to select pivots and partition data objects, to create node

			/* get the number of pivots */
			int numPivot = (this->numPivots>=task->size) ? task->size : this->numPivots;
			
			/* select pivots */
			vector<int> pivotsIndex = this->psm->selectPivots(this->metric,task->dataList,task->start,task->size,numPivot);

			
			/* move the pivots to the end of current partition */
			task->groupPivotsAtEnd(pivotsIndex);

			int remainNodeSize = task->size - task->numPivots;

			/*if the size of current partition is greater than the max size of a leaf node then create a internal node, otherwise create a leaf node.the max size of leaf node is given by the builder of the mvp-tree*/

			if(task->getFlag()==0)
			{

				if(remainNodeSize>maxLeafSize)
				{
					/* create an internal node, not write */
					createInternalNodeToWrite(task);

				}
				else
				{
					/* create a leaf node, but may not write it into file when rootAddress=-2 */
					createAndWriteLeafNode(task,out);


					if(rootAddress!=-2)
					{/* rootAddress!=-2, means current node has written into file, see above function createAndWriteLeafNode
						so now delete current task from taskList.  */

						taskList.erase(taskToGet); //delete current task from taskList

						task.reset(); // clear task

					}

					/*because this is in the function of "loadLeafToRoot", means the root node is the last node which is writed into file.
						when there is only one task in taskList, it must be root node, and all of other nodes have been writed into file. so 
						now should break while   */
					if(taskList.size()==1) break;

				}

			}
			else
			{
				if(maxTreeHeight <=0)
				{
					rootAddress = -2;
				}
				else
				{
					rootAddress = -1;
				}

				/* create and write a leaf node, */
				createAndWriteLeafNode(task,out);

				if(rootAddress!=-2)
				{
					taskList.erase(taskToGet); // delete current task from taskList

					task.reset(); //  clear current task
				}

				/* when there is only one task in taskList, it must be root node, immidately break while */
				if(taskList.size()==1) break;

			}

		}
		else
		{// current task has already a node, so don't need to create, directly write it into file

			/* open index file */
			out.open(fileName.c_str(),ios::in|ios::out|ios::binary);

			/* write an internal node, don't need to be created, because it was created before and stored in task'node */
			writeInternalNode(task,out);

			out.close();

			taskList.erase(taskToGet); // delete current task from taskList

			task.reset();  // clear current task

			/* when there is only one task in taskList, it must be root node, immidately break while */
			if(taskList.size()==1) break;

		}
	}

	/* through above process, all nodes except root node have been created and written into index file
		so now write root node into file  */
	writeRoot(taskList.at(0),out,maxTreeHeight);
}


/**
*note : loadRootToLeaf, (corresponding to loadLeafToRoot), when create a node, write it into index file immediately, don't wait for its child node,
		when create a child node, its address will insert to the child list of its parent node, by this way, the parent can find it.

**/
void CMVPIndex::loadRootToLeaf(vector<shared_ptr<CMetricData> > &dataObjects)
{
	/* create a index file */
	ofstream out;
	out.open(fileName.c_str(),ios::binary);
	out.close();

	/* initialize file pointer as 0 */
	filePointer = 0;

	/* initialize max tree height as 0 */
	int maxTreeHeight = 0;

	if(dataObjects.size()<=maxLeafSize)
	{//mean to create root node directly
		rootAddress = -2;
	}
	else
	{
		rootAddress = -1;
	}

	int allDataObjectsInIndex = dataObjects.size();//the size of database(include all data objects)

	vector<shared_ptr <Task> >::iterator taskToGet;

	/*put the task that create the root node to the task queue*/
	shared_ptr<Task> task(new Task(dataObjects,shared_ptr<CIndexNode>(),0,dataObjects.size(),0,0));

	/*  set the index address of first task is -1  */
	task->myIndexAdress = -1;

	this->taskList.push_back(task);

	/*use every task in the queue to create a internal node or leaf node until the the queue is null*/
	while(this->taskList.size()>0)
	{
		/*  get end-iterator */
		taskToGet = this->taskList.end();

		/* point to the last element of taskList */
		taskToGet--;

		/* get the last task */
		task = *taskToGet;

		/* update current max tree height */
		maxTreeHeight = (maxTreeHeight >= task->getMyHeight()) ? maxTreeHeight : task->getMyHeight();

		/*get the number of pivot*/
		int numPivot = (this->numPivots>=task->size) ? task->size : this->numPivots;

		/*selecte several piovt from current partition based on the given parameter*/
		vector<int> pivotsIndex = this->psm->selectPivots(this->metric,task->dataList,task->start,task->size,numPivot);

		/*move the pivots to the end of current partition*/
		task->groupPivotsAtEnd(pivotsIndex);

		/** remain data size expect pivots */
		int remainNodeSize = task->size - task->numPivots;

		/*if the size of current partition is greater than the max size of a leaf node then create a internal node, otherwise create a leaf node.the max size of leaf node is given by the builder of the mvp-tree*/

		if(remainNodeSize>maxLeafSize && task->getFlag()==0)
		{//create an internal node

			createAndWriteInternalNode(task,out,taskToGet);

			if(task->getFlag()==0) task.reset();  // clear current task

		}
		else
		{//create leaf node, or create root node

			if(maxTreeHeight <=0)
			{//max tree height <=0, means there is no any node, so should to create root node

				rootAddress = -2;
			}
			else
			{
				rootAddress = -1;
			}
			
			/* create and write leaf node, may not really write node. whern rootAddress=-2, this function will not write node into file */
			createAndWriteLeafNodeLast(task,out);

			if(rootAddress!=-2)
			{
				taskList.erase(taskToGet); // delete current task from taskList

				task.reset();  // clear current task
			}
			else 
			{
				/* when rootAddress=-2, this function is really write node into file */
				writeRoot(taskList.at(0),out,maxTreeHeight);

				task.reset();  // clear current task

				break;
			}

		}

	}

	/******  through above process, all nodes have been written into file. below process will do some final work  *******/

	/* update root address to 0, means root node is at the begin of index file */
	rootAddress = 0;

	maxTreeHeight++;

	/* open index file with binary-in-out form */
	out.open(fileName.c_str(),ios::binary|ios::in|ios::out);


	/* let file pointer point to the end of file */
	out.seekp(0,ios::end);

	/* write max tree height to file */
	out.write((char*)(&maxTreeHeight),sizeof(int));


	/* write the size of all data objects in this index, is also called the size of database 
		this number will be used at search process.
	*/
	out.write((char*)(&allDataObjectsInIndex),sizeof(int));


	/* write root address into index file, and the address of root node is 0, because the buildMode is 
		loadRootToLeaf(root node is the first node to written into index file).
	
		this means the end of index file stores the root address,
	   when we do a search, we firstly read the end of file to get the address of root node. through root node, 
	   we can get all of other nodes, because parent node stores the address of all its child node.
	*/
	out.write((char*)&rootAddress,sizeof(long));

	out.close();
}

/*
*brief : create and write leaf node, however may not write node when rootAddress=-2
*/
void CMVPIndex::createAndWriteLeafNode(shared_ptr<Task> task,ofstream &out)
{
	/* open index file */
	out.open(fileName.c_str(),ios::binary|ios::in|ios::out);

	/* move file pointer to the position of "filePointer" */
	out.seekp(filePointer);

	/* update the number of leaf node */
	this->numLeaf++;

	/* get all the data objects of current task */
	vector<shared_ptr<CMetricData> > dataObjects;
	task->getDataPoints(dataObjects);

	/* get all the pivots of current node */
	vector<shared_ptr<CMetricData> > pivots;
	task->getPivots(pivots);

	/* use to store the distance between each pivot and each data object */
	vector<vector<double> > distance;
	
	/*calcualte the distance from each of the data objects of current parition to every pivots*/
	for(vector<CMetricData*>::size_type i=0;i<pivots.size();i++)
	{
		/* the distance between pivots[i] and all data objects */
		vector<double> ve;

		/* calculate distance */
		for(vector<CMetricData*>::size_type j=0;j<dataObjects.size();j++)
		{
			ve.push_back(this->metric->getDistance(dataObjects[j].get(),pivots[i].get()));
		}

		/* push into "distance" */
		distance.push_back(ve);

	}

	/**************    create a leaf node and assign its memory address to the child list of parent node     *******************/

	/* create a leaf node */
	shared_ptr<CMVPLeafNode> mvpLeafNode(new CMVPLeafNode(pivots,dataObjects,distance,task->getMyHeight()+1));

	if(this->rootAddress==-2)
	{        
		task->setNode((shared_ptr<CIndexNode>)mvpLeafNode);

		//out.close();
		//return;
	}
	else
	{

		char type[6] = "LNODE";

		/* insert this node address to the child list of its parent node */
		((CMVPInternalNode*)task->getParentIndex().get())->getSubTreeAddress()[task->getMyIndex()-1] = filePointer;

		/* move file pointer to the position of "filePointer" */
		out.seekp(filePointer);

		/* write node type */
		out.write(type,6*sizeof(char));

		/* write node information into file, and update "filePointer" */
		filePointer += (long)mvpLeafNode->writeExternal(out) + 6*sizeof(char);

	}

	out.close();
}


/*
*brief : only create internal node and insert its children task into taskList, not to write node into index file

*/
void CMVPIndex::createInternalNodeToWrite(shared_ptr<Task> task)
{
	/* update the number of internal nodes */
	this->numInternalNodes ++;

	/*get pivots of current node*/
	vector<shared_ptr<CMetricData> > pivots;
	task->getPivots(pivots);

	/*
		partition current data objects into several smaller child partitons, 
		the	selected pivots are not in the section that will be partitioned in child partitions, 
		this means the selected pivots is only in this internal node.
	*/
	CPartitionResults pr = this->pm->partition(this->metric,pivots,task->dataList,task->start,task->size - task->numPivots,this->singlePivotFanout,this->maxLeafSize,this->trisectionRadius,this->selectOptimalPivots,this->partitionByOnePivot);

	int childrenNumber = pr.partitionSize();// the number of children is "childrenNumber"-1

	int flag = 0;

	if(childrenNumber==2)
	{/* if there is only one child after partitioning data objects, set flag=1, in order to write this child into file, 
		because it is a leaf node.  */	
		flag = 1;
	}

	/*********     create an internal node and assign its addres to the children list of parent node     ********/
	if(flag==0)
	{
		/* create child list */
		vector<shared_ptr<CIndexNode> > *subTreeNode = new vector<shared_ptr<CIndexNode> >(childrenNumber-1);

		/* create this internal node, and initial it */
		shared_ptr<CMVPInternalNode> node(new CMVPInternalNode(pivots,pr.getLowerBounds(),pr.getUpperBounds(),*subTreeNode,task->getMyHeight()+1) );
		
		/* set child size */
		node->setChildSize(childrenNumber-1);

		/* set the node of task */
		task->setNode((shared_ptr<CIndexNode>) node );

		/*create several tasks base on each of the child partitions created before and then push these tasks to the global variable queue for the iteration of building child trees*/
		for(int i=childrenNumber-1;i>0;i--)
		{
			int j=0;

			/* new task, is also a new node, a child node, later will be written into index file */
			shared_ptr<Task> newTask( new Task(task->dataList,node,pr.getPartition(i-1),pr.getPartition(i),i,task->getMyHeight()+1) );       

			/* push into taskList, later use */
			this->taskList.push_back(newTask);
		}
	}
	else
	{
		task->setFlag(1);
	}

}


/*
*brief : write an internal node into index file
*param task : is current task, include the internal node which will be written into index file.
*/
void CMVPIndex::writeInternalNode(shared_ptr<Task>task, ofstream& out)
{

	//    out.open(fileName.c_str(),ios::binary|ios::in|ios::out);

	/* move file pointer to the position of "filePointer" */
	out.seekp(filePointer);

	/* get parent node */
	CMVPInternalNode* parentNode = (CMVPInternalNode*)(task->getParentIndex().get());

	/* 
		get own index, this index is the index in the child list of parent node, 
		from 0 to n-1, n is the number of children.
	*/
	int num = task->getMyIndex();

	/* write own index address into the child list of parent node */
	parentNode->getSubTreeAddress()[task->getMyIndex()-1] = filePointer;

	/* this is an internal node */
	char *type = "INODE";

	/* write node type */
	out.write(type,6*sizeof(char));

	/* write node information with "writeExternal" function, and update file pointer */
	filePointer += (long)task->getNode()->writeExternal(out) + 6*sizeof(char); // 6*sizeof(char) represents the type of node

	//    out.close();

}



/**
* brief : write root node into index file.

*/
void CMVPIndex::writeRoot(shared_ptr <Task> task,ofstream& out,int treeHeight)
{ 
	/* open index file */
	out.open(fileName.c_str(),ios::binary|ios::in|ios::out);

	/* find the position of filePointer pointing in file stream */
	out.seekp(filePointer);

	/* internal node */
	char *type1="INODE";

	/* leaf node */
	char *type2="LNODE";

	if(rootAddress == -2)
		out.write(type2,6*sizeof(char));//this means the type of node takes up the size of 6 "char".
	else
		out.write(type1,6*sizeof(char));

	/* 
		this writeExternal function is index-node's, not data-type's.
		if the node is a leaf node, it should be MVPLeafNode->writeExternal.
		if the node is an internal node, it should be MVPInternalNode->writeExternal.
	*/
	task->getNode()->writeExternal(out);

	treeHeight++;

	/*
		note : the parameter rootAddress is not only used to distinguish current node is or not a "root node",
			but also be the address of root node at last, see below.
	*/
	rootAddress = filePointer; // the real address of root node

	/* write the height of whole tree */
	out.write((char*)(&treeHeight),sizeof(int));

	/* write the size of all data objects in this index, is also called the size of database 
		this number will be used at search process.
	*/
	int allDataObjectsInIndex = task->dataList.size();//the size of database(include all data objects)

	out.write((char*)(&allDataObjectsInIndex),sizeof(int));

	/* write root address */
	out.write((char*)(&rootAddress),sizeof(long));

	out.close();

	cout<<"Root address:"<<rootAddress<<" height of MVP-tree:"<<treeHeight<<endl;
}


void CMVPIndex::createAndWriteInternalNode(shared_ptr<Task> task,ofstream& out,vector<shared_ptr<Task> >::iterator &shouldeBeCreate)
{
	/* the number of internal node add 1 */
	this->numInternalNodes ++;

	/* get pivots of current node */
	vector<shared_ptr<CMetricData> > pivots;
	task->getPivots(pivots);

	/*partition current data objects into several smaller child partitons, the selected pivots are not in the section that will be partitioned in child partitions*/
	CPartitionResults pr = this->pm->partition(this->metric,pivots,task->dataList,task->start,task->size - task->numPivots,this->singlePivotFanout,this->maxLeafSize,this->trisectionRadius,this->selectOptimalPivots,this->partitionByOnePivot);

	int childrenNumber = pr.partitionSize();

	/* a flag */
	int flag = 0;

	if(childrenNumber==2)
	{//only has 1 child

		flag = 1;
	}

	if(flag==0)
	{//more than 1 child

		/*create an internal node and assign its address to the children list of parent node*/

		/* create children list */
		vector<shared_ptr<CIndexNode> > *subTreeNode = new vector<shared_ptr<CIndexNode> >(childrenNumber-1);

		/* create an internal node */
		shared_ptr< CMVPInternalNode> node( new CMVPInternalNode(pivots,pr.getLowerBounds(),pr.getUpperBounds(),*subTreeNode,task->getMyHeight()+1) );

		/* set child size */
		node->setChildSize(childrenNumber-1);

		if(task->getParentIndex()!=NULL)
		{//has parent node

			(((CMVPInternalNode*)(task->getParentIndex().get()))->getSubTreeAddress())[task->myIndex-1] = -1;  // set the own address in the child list of parent node is -1

		}

		/* open index file */
		out.open(fileName.c_str(),ios::binary|ios::in|ios::out);


		/* find current file pointer(point to somewhere in index file) */
		out.seekp(filePointer);

		/* CMVPInternalNode* parentNode = (CMVPInternalNode*)task->getParentIndex();*/

		if(task->myIndexAdress!=-1)
		{//this node address is exist, 

			/* find this node */
			out.seekp(task->myIndexAdress);

			/* write "filePointer" into file stream */
			out.write((char*)(&filePointer),sizeof(long));

			/* let file pointer return to "filePointer" */
			out.seekp(filePointer);

		}

		/* this is internal node */
		char *type = "INODE";

		/* write node type */
		out.write(type,6*sizeof(char));

		/* write node information, see writeExternal in MVPIternalNode.cpp */
		long tempDelta = node->writeExternal(out)+6*sizeof(char);

		out.close();

		/* delete the last task in taskList, because it has been stored */
		taskList.erase(shouldeBeCreate);

		/*create several tasks base on each of the child partitions created before and then push these tasks to the global variable queue for the iteration of building child trees*/
		for(int i=childrenNumber-1;i>0;i--)
		{
			/* create new task with each partition */
			shared_ptr <Task> newTask ( new Task(task->dataList,node,pr.getPartition(i-1),pr.getPartition(i),i,task->getMyHeight()+1));

			/* each child address */
			/*  
				filePointer : current file pointer
				newTask->myIndex : the index of this child node in parent' child list
				sizeof(long) : each index is a long number
			    sizeof(int) : the number of children
			    6*sizeof(char) : the type of node
			*/
			newTask->myIndexAdress = (long) (filePointer+(newTask->myIndex-1)*sizeof(long)+sizeof(int))+6*sizeof(char);

			this->taskList.push_back(newTask);
		}

		/* update the value of file pointer */
		filePointer += tempDelta;

	}
	else
	{
		task->setFlag(1);
	}
}


/*
*brief : create and write leaf node. 
		however, when rootAddress is -2, this function is not really write leaf node into file
*/
void CMVPIndex::createAndWriteLeafNodeLast(shared_ptr <Task> task,ofstream &out)
{
	/* open index file */
	out.open(fileName.c_str(),ios::binary|ios::in|ios::out);

	/* find the position of "filePointer" pointing in file stream */
	out.seekp(filePointer);

	/* the number of leaf add 1 */
	this->numLeaf++;

	/* get all the data objects of current node */
	vector<shared_ptr<CMetricData> > dataObjects;
	task->getDataPoints(dataObjects);

	/* get all the pivots of current node */
	vector<shared_ptr<CMetricData> > pivots;
	task->getPivots(pivots);

	/* store ths distances between pivots and data objects */
	vector<vector<double> > distance;

	/* calcualte the distance between each objects and each pivots */
	for(vector<CMetricData*>::size_type i=0;i<pivots.size();i++)
	{
		/* store the distance between pivots[i] and all data objects */
		vector<double> ve;

		/* calculate distance */
		for(vector<CMetricData*>::size_type j=0;j<dataObjects.size();j++)
		{
			ve.push_back(this->metric->getDistance(dataObjects[j].get(),pivots[i].get()));
		}

		distance.push_back(ve);
	}

	/* create a leaf node and assign its memory address to the child list of parent node */
	shared_ptr< CMVPLeafNode> mvpLeafNode(new CMVPLeafNode(pivots,dataObjects,distance,task->getMyHeight()+1));

	if(this->rootAddress==-2)
	{// not write leaf node into file
		task->setNode((shared_ptr<CIndexNode>)mvpLeafNode);
		
		//out.close();
		//return;
	}
	else
	{// write leaf node into file

		char type[6] = "LNODE";

		/* 
			firstly, move file pointer to the position in the child list of the parent node, 
			write the address of current node in file to the list, in order to its parent can find it
		*/
		out.seekp(task->myIndexAdress);

		out.write((char*)(&filePointer),sizeof(long));



		/* 
			secondly, move file pointer to the position of "filePointer", is also current position in file should be written,
			is also the end of file. begin to write current node into file, call "writeExternal" function to complete specific work.
		*/
		out.seekp(filePointer);

		out.write(type,6*sizeof(char));

		filePointer += (long)mvpLeafNode->writeExternal(out) + 6*sizeof(char);

	}

	out.close();
}

