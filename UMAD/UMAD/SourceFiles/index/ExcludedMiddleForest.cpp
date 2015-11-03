#include "../../HeaderFiles/index/ExcludedMiddleForest.h"



class ForestTask 

{

public:

    /**give class CMVPIndex a friend right to access the data information of a ForestTask instance in the process of building a mvp tree*/

    friend class CExcludedMiddleForest;



    /**move the pivot to the end of a object list partition

    * @param pivotIndex a vector contains all the indices of pivots in the whole objects list

    */

    void groupPivotsAtEnd(vector<int> &pivotIndex);



    /**get the index of current node in the parent's children list*/

    int getMyIndex();



    /**get the height of current node. the height of root is 0*/

    int getMyHeight();



    /**get the parent node's address*/

    shared_ptr<CIndexNode > getParentIndex();



    /**get all the pivots that belongs to current node

    * @param pivots a vector to store the address of pivot.This vector is initialized with none elements, all the elements is generated through calling thing function. This parameter is used to return the address of all pivot to the caller of this funciton.

    */

    void getPivots(vector<shared_ptr<CMetricData> >&pivots);



    /**get all the objects that belong to the tree rooted at this node*/

    void getDataPoints(vector<shared_ptr<CMetricData> >&dataList);



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

    ForestTask():dataList(*(new vector<shared_ptr<CMetricData> >))

    {

        this->parentIndex=shared_ptr<CIndexNode >();

        this->myIndex=0;

        this->myHeight=0;



        this->size=0;

        this->start=0;





        this->numPivots=0;





    };



    /**constructor with six parameters

    *	@param dataList a vector contains the address of all the obejct over which this mvp tree is built

    *	@param parentIndex address of parent node

    *	@param start the start index of current object partition in the whole data list

    *	@param end the last object index of current object partition

    *	@param myIndex current node index in parent node's children list

    *	@param myHeight height of current node

    */

    ForestTask(vector<shared_ptr<CMetricData> > &dataList,shared_ptr<CIndexNode> parentIndex,int start,int end,int myIndex,int myHeight):dataList(dataList)

    {

        this->dataList=dataList;

        this->parentIndex=parentIndex;

        this->myIndex=myIndex;



        this->myHeight=myHeight;





        this->start=start;

        this->size=end-start;



        numPivots=0;



    };



    /**a vector contains the address of all the obejct over which this mvp tree is built*/

    vector<shared_ptr<CMetricData> > &dataList;



    /**address of parent node*/

    shared_ptr<CIndexNode > parentIndex;



    /**current node index in parent node's children list*/

    int myIndex;



    /**height of current node*/

    int myHeight;



    /**the start index of current object partition in the whole objects list*/

    int start;



    /**number of objects in current object partition*/

    int size;



    /**number of pivots in a node of mvp tree*/

    int numPivots;



    shared_ptr<CIndexNode> node;

    long myIndexAdress;

};



/**move the pivot to the end of a object list partition

* @param pivotIndex a vector contains all the indices of pivots in the whole objects list

*/

void ForestTask::groupPivotsAtEnd(vector<int> &pivotIndex)

{

    /**initialize numPivots*/

    this->numPivots=pivotIndex.size();





    if(this->numPivots<this->size)

    {

        /**new address of pivots, the first pivots will be moved to the end of current object partition, start+this->size-1 is the end index of current object partition*/

        int newPivotAddressInCompressedData=start+this->size-1;



        /**move every pivot to the end of partiton*/

        for(int i=0;i<this->numPivots;i++)

        {

            shared_ptr<CMetricData> temp;



            /**exchange the address content of pivot element unit and the target element unit*/

            temp=this->dataList[pivotIndex[i]];

            dataList[pivotIndex[i]]=dataList[newPivotAddressInCompressedData];

            dataList[newPivotAddressInCompressedData]=temp;



            /**judge if the content of target elment unit is the address of another pivot whose index is also in the pivot index list, exchange the index the result of judge is true*/

            for(int j=i+1;j<this->numPivots;j++)

                if(pivotIndex[j]==newPivotAddressInCompressedData)

                    pivotIndex[j]=pivotIndex[i];



            /**the first pivot move the end of partition, the next pivot is always moved to one element unit before the previous moved pivots*/

            newPivotAddressInCompressedData--;

        }

    }



}



/**get the index of current node in the parent's children list*/

shared_ptr<CIndexNode> ForestTask::getParentIndex()

{

    return this->parentIndex;

}



/**get the index of current node in the parent's children list*/

int ForestTask::getMyIndex()

{

    return this->myIndex;

}



/**get the height of current node. the height of root is 0*/

int ForestTask::getMyHeight()

{

    return this->myHeight;

}



/**get all the pivots that belongs to current node

* @param pivots a vector to store the address of pivot.This vector is initialized with none elements, all the elements is generated through calling thing function. This parameter is used to return the address of all pivot to the caller of this funciton.

*/

void ForestTask::getPivots(vector<shared_ptr<CMetricData> > &pivots)

{



    /**start should be the first pivot index in current partition*/ 

    int start=this->start+this->size-this->numPivots;





    for(int i=0;i<this->numPivots;i++)

    {

        pivots.push_back(this->dataList[start]);



        start++;

    }



}



/**get all the objects that belong to the tree rooted at this node*/

void ForestTask::getDataPoints(vector<shared_ptr<CMetricData> > &objectList)

{

    int mySize=0;



    /**if the number of pivot is less 0,return all the objects in current partitin*/

    if(numPivots==-1)

        mySize=this->size;

    else

        mySize=this->size-this->numPivots;



    for(int i=start;i<start+mySize;i++)

    {

        objectList.push_back(this->dataList[i]);

    }

}



/***********************************************************************/



shared_ptr<CIndexNode> CExcludedMiddleForest::createRoot(shared_ptr<ForestTask> forestTask,vector<shared_ptr<CMetricData> >& ecludedData)

{

    this->numInternalNodes ++;



    /**get pivots of current node*/

    vector<shared_ptr<CMetricData> > pivots;







    forestTask->getPivots(pivots);



    /**partition current partition into several smaller child partitons*/





    CPartitionResults pr=this->pm->partition(this->metric,pivots,forestTask->dataList,0,forestTask->size - forestTask->numPivots,this->maxRadius,this->singlePivotFanout,this->maxLeafSize,this->middlePorprotion,this->trisectionRadius,this->selectOptimalPivots,this->partitionByOnePivot);



    int childrenNumber=pr.partitionSize();



    /**create a internal node as root node and assign the address of this node to the root pointer of mvp tree*/

    vector<shared_ptr<CIndexNode> > subTreeNode(childrenNumber-2);



    vector<vector<double> >  lowerBounds=pr.getLowerBounds();

    vector<vector<double> >  upperBounds=pr.getUpperBounds();





    for(int i =0;i<numPivots;i++)

    {

        lowerBounds.at(i).pop_back();

        upperBounds.at(i).pop_back();

    }







    shared_ptr<CMVPInternalNode> root( new CMVPInternalNode(pivots,lowerBounds,upperBounds,subTreeNode,forestTask->getMyHeight()) );



    /**create several ForestTasks base on each of the child partitions created before and then push these ForestTasks to the global variable queue for the iteration of building child trees*/

    for(int i=childrenNumber-1;i>0;i--)

    {

        shared_ptr<ForestTask> newForestTask = shared_ptr<ForestTask>();



        /**for the last child partition of current partition, the end index to create a ForestTask is the end index of current partiton, otherwise the end index is the start of next child partiton*/

        if(i==childrenNumber-1)

        {



            //newForestTask=new ForestTask(ForestTask->dataList,root,pr.getPartition(i-1),ForestTask->start + ForestTask->size - ForestTask->numPivots,i,ForestTask->getMyHeight()+1);



            for(int j=pr.getPartition(i-1);j<pr.getPartition(i) - forestTask->numPivots+1;j++)

            {

                ecludedData.push_back(forestTask->dataList.at(j));

            }



            //newForestTask=new ForestTask(forestTask->dataList,root,pr.getPartition(i-1),pr.getPartition(i),i,forestTask->getMyHeight()+1);//!!!!!!!!!



        }

        else

        {



            newForestTask.reset(new ForestTask(forestTask->dataList,root,pr.getPartition(i-1),pr.getPartition(i),i,forestTask->getMyHeight()+1));





            this->forestTaskList.push_back(newForestTask);

            /**push ForestTask into the queue*/



        }

    }



    return root;





}



void CExcludedMiddleForest::createInternalNode(shared_ptr<ForestTask> forestTask,vector< shared_ptr<CMetricData> >& ecludedData)

{

    this->numInternalNodes ++;











    /**get pivots of current node*/

    vector<shared_ptr<CMetricData> > pivots;

    forestTask->getPivots(pivots);









    /**partition current partition into several smaller child partitons*/

    CPartitionResults pr=this->pm->partition(this->metric,pivots,forestTask->dataList,forestTask->start,forestTask->size-forestTask->numPivots,this->maxRadius,this->singlePivotFanout,this->maxLeafSize,this->middlePorprotion,this->trisectionRadius,this->selectOptimalPivots,this->partitionByOnePivot);









    int childrenNumber=pr.partitionSize();



    vector<vector<double> >  lowerBounds=pr.getLowerBounds();

    vector<vector<double> >  upperBounds=pr.getUpperBounds();



    for(int i =0;i<numPivots;i++)

    {

        lowerBounds.at(i).pop_back();

        upperBounds.at(i).pop_back();

    }







    /**create an internal node and assign its addres to the children list of parent node*/

    vector<shared_ptr<CIndexNode> > *subTreeNode=new vector<shared_ptr<CIndexNode> >(childrenNumber-2);

    shared_ptr<CMVPInternalNode> child( new CMVPInternalNode(pivots,lowerBounds,upperBounds,*subTreeNode,forestTask->getMyHeight()+1) );    

    ((dynamic_pointer_cast<CMVPInternalNode>( (forestTask->getParentIndex()))->getSubTree())[forestTask->myIndex-1] )=child;



    /**create several forestTasks base on each of the child partitions created before and then push these forestTasks to the global variable queue for the iteration of building child trees*/





    for(int i=childrenNumber-1;i>0;i--)

    {

        shared_ptr<ForestTask> newforestTask;



        /**for the last child partition of current partition, the end index to create a ForestTask is the end index of current partiton, otherwise the end index is the start of next child partiton*/

        if(i==childrenNumber-1)

        {



            for(int j=pr.getPartition(i-1);j < pr.getPartition(i) - forestTask->numPivots+1;j++)

            {

                ecludedData.push_back(forestTask->dataList.at(j));

            }





        }

        else

        {



            newforestTask.reset( new ForestTask(forestTask->dataList,child,pr.getPartition(i-1),pr.getPartition(i),i,forestTask->getMyHeight()+1));



            this->forestTaskList.push_back(newforestTask);

        }







    }

}



void CExcludedMiddleForest::createLeafNode(shared_ptr< ForestTask> forestTask,vector<shared_ptr<CMetricData> >& ecludedData)

{

    this->numLeaf++;



    /**get all the objects of current partition*/

    vector<shared_ptr<CMetricData> > children;

    forestTask->getDataPoints(children);



    /**get all the pivots of current node*/

    vector<shared_ptr<CMetricData> > pivots;

    forestTask->getPivots(pivots);



    vector<vector<double> > distance;



    /**calcualte the distance from each of the objects of current parition to every pivots*/

    for(vector<CMetricData*>::size_type i=0;i<pivots.size();i++)

    {

        vector<double> ve;



        for(vector<CMetricData*>::size_type j=0;j<children.size();j++)

        {

            ve.push_back(this->metric->getDistance(children[j].get(),pivots[i].get()));

        }



        distance.push_back(ve);

    }



    /**create a leaf node and assign its memory address to the child list of parent node*/

    shared_ptr< CMVPLeafNode> mvpLeafNode(new CMVPLeafNode(pivots,children,distance,forestTask->getMyHeight()+1));

    ((dynamic_pointer_cast<CMVPInternalNode>(forestTask->getParentIndex()))->getSubTree())[forestTask->myIndex-1]=mvpLeafNode;



}





vector<shared_ptr<CMetricData> > CExcludedMiddleForest::createTree(vector<shared_ptr< CMetricData> > &dataObjects,int treeIndex)

{

    vector<shared_ptr< CMetricData> > ecludedData;

    ofstream out;



    loadRootToLeaf(dataObjects,ecludedData,treeIndex,out);



    return ecludedData;

}



void CExcludedMiddleForest::bulkLoad(vector<std::shared_ptr<CMetricData> > &dataList,int buildMode,int threadNum)
{





    //roots.push_back(newRoot);

    filePointer = 0;



    ofstream out;

    out.open(fileName.c_str());

    out.close();



    rootAddress.push_back(0);
    treeHeight.push_back(0);

    vector<shared_ptr<CMetricData> > formerEcludedData = createTree(dataList,0);

    vector<shared_ptr< CMetricData> >* ecludedData;



    for(int i=1;;i++)

    {

        if(formerEcludedData.size()<=maxLeafSize)

        {

            break;

        }

        rootAddress.push_back(0);
        treeHeight.push_back(0);

        formerEcludedData = createTree(formerEcludedData,i);



    }





    vector<shared_ptr <CMetricData> > pivots;

    vector<shared_ptr< CMetricData> > children;



    vector<int> pivotsIndex = this->psm->selectPivots(metric,formerEcludedData,numPivots);

    ForestTask forestTask(formerEcludedData,shared_ptr<CIndexNode>(),0,formerEcludedData.size(),0,0);



    forestTask.groupPivotsAtEnd(pivotsIndex);



    forestTask.getPivots(pivots);

    forestTask.getDataPoints(children);



    vector<vector<double> > distance;



    /**calcualte the distance from each of the objects of current parition to every pivots*/

    for(vector<CMetricData*>::size_type i=0;i<pivots.size();i++)

    {

        vector<double> ve;



        for(vector<CMetricData*>::size_type j=0;j<children.size();j++)

        {

            ve.push_back(this->metric->getDistance(children[j].get(),pivots[i].get()));

        }

        distance.push_back(ve);

    }



    out.open(fileName.c_str(),ios::binary|ios::in|ios::out);



    out.seekp(filePointer);



    shared_ptr<CIndexNode> newRoot(new CMVPLeafNode(pivots,children,distance,0));  

    out.write("LNODE",6*sizeof(char));

    newRoot->writeExternal(out);



    rootAddress.push_back(filePointer);
    treeHeight.push_back(1);



    for(int i=0;i<rootAddress.size();i++)

    {

        long temp = rootAddress.at(i);
        out.write((char*)&temp,sizeof(long));

    }


    for(int i=0;i<treeHeight.size();i++)

    {
        int tempHeight = treeHeight.at(i);
        out.write((char*)&tempHeight,sizeof(int));

    }



    numTree = rootAddress.size();

    out.write((char*)&numTree,sizeof(int));


    out.close();



}


list<shared_ptr< CMetricData> >* CExcludedMiddleForest:: search(CQuery* q)
{
    CRangeQuery *rq=(CRangeQuery*)q;

    ifstream in(fileName.c_str(),ios::binary);
	if(in.fail()){
		cout<<"ERROR : can't find index file!"<<endl;
		exit(1);
	}
	
    in.seekg(-(long)sizeof(int),ios::end);

    in.read((char*)(&numTree),sizeof(int));



    long* tempAddress = new long[numTree];



    in.seekg(-(long)(numTree*sizeof(long)+sizeof(int)),ios::end);

    in.read((char*)tempAddress,numTree*sizeof(long));



    long temp = tempAddress[1];



    for (int i=0;i<numTree;i++)
    {



        in.seekg(tempAddress[i],ios::beg);

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

        tempAddress[i] += 6*sizeof(char);

        delete(type);	

        root->searchIndex(*rq,tempAddress[i],in,*metric,&re,dataType);



        // CMemMonitor::updateMem();



        // CMemMonitor::updateMem();

    }



    in.close();

    return &re;



}











CExcludedMiddleForest::CExcludedMiddleForest()

{





    metric=0;



    psm=0;



    pm=0;



    singlePivotFanout=0;



    maxLeafSize=0;


    numPivots=0;





}



CExcludedMiddleForest::CExcludedMiddleForest(vector<shared_ptr<CMetricData> > &dataObjects,CMetricDistance *metric,CPivotSelectionMethod* psm,CPartitionMethod* pm,int numPivot,int singlePivotFanout,int maxLeafSize, int maxPathLength,double maxRadius,double middlePorprotion,double trisectionRadius,char* selectOptimalPivots,char* partitionByOnePivot)

{



    this->metric=metric;

    this->numPivots=1;

    this->psm=psm;

    this->pm=pm;

    this->singlePivotFanout=2;

    this->total_size=dataObjects.size();

    this->maxLeafSize=maxLeafSize;

    this->maxPathLength = maxPathLength;

    this->middlePorprotion = middlePorprotion;

    this->maxRadius = maxRadius;

    this->trisectionRadius = trisectionRadius;
    this->selectOptimalPivots = selectOptimalPivots;
    this->partitionByOnePivot = partitionByOnePivot;



}





CExcludedMiddleForest::~CExcludedMiddleForest(void)

{

}



void CExcludedMiddleForest::setMetric(CMetricDistance* metric )

{

    this->metric=metric;

}



void CExcludedMiddleForest::writeRoot(shared_ptr <ForestTask> task,ofstream& out,vector<shared_ptr<CMetricData> >& ecludedData, int treeIndex)

{ 

    out.open(fileName.c_str(),ios::binary|ios::in|ios::out);



    out.seekp(filePointer);



    char *type1="INODE";

    char *type2="LNODE";



    if(rootAddress.at(treeIndex) == -2)

        out.write(type2,6*sizeof(char));

    else

        out.write(type1,6*sizeof(char));



    /* rootAddress.at(treeIndex) = filePointer;*/

    filePointer+=task->getNode()->writeExternal(out)+6;



    out.close();

    //delete(type1);

    //delete(type2);



}



void CExcludedMiddleForest::createAndWriteInternalNode(shared_ptr <ForestTask> task,ofstream& out,vector<shared_ptr<ForestTask> >::iterator &shouldeBeCreate,vector<shared_ptr<CMetricData> >& ecludedData)

{

    this->numInternalNodes ++;

    /*cout<<"internalNode:"<<numInternalNodes<<endl;*/

    /*get pivots of current node*/

    vector<shared_ptr<CMetricData> > pivots;

    task->getPivots(pivots);



    /*partition current partition into several smaller child partitons,the selected pivots are not in the section that will be partitioned in child partitions*/

    CPartitionResults pr=this->pm->partition(this->metric,pivots,task->dataList,task->start,task->size - task->numPivots,this->singlePivotFanout,this->maxLeafSize,this->trisectionRadius,this->selectOptimalPivots,this->partitionByOnePivot);

    int childrenNumber=pr.partitionSize();



    vector<vector<double> >  lowerBounds=pr.getLowerBounds();

    vector<vector<double> >  upperBounds=pr.getUpperBounds();



    for(int i =0;i<numPivots;i++)

    {

        lowerBounds.at(i).pop_back();

        upperBounds.at(i).pop_back();

    }



    /*create an internal node and assign its addres to the children list of parent node*/

    vector<shared_ptr<CIndexNode> > *subTreeNode = new vector<shared_ptr<CIndexNode> >(childrenNumber-1);

    shared_ptr< CMVPInternalNode> node( new CMVPInternalNode(pivots,lowerBounds,upperBounds,*subTreeNode,task->getMyHeight()+1) );

    node->setChildSize(childrenNumber-2);







    if(task->getParentIndex()!=NULL)

    {

        (((CMVPInternalNode*)(task->getParentIndex().get()))->getSubTreeAddress())[task->myIndex-1]=-1;

    }    





    out.open(fileName.c_str(),ios::binary|ios::in|ios::out);



    out.seekp(filePointer);



    /* CMVPInternalNode* parentNode = (CMVPInternalNode*)task->getParentIndex();*/

    if(task->myIndexAdress!=-1)

    {

        out.seekp(task->myIndexAdress);

        out.write((char*)(&filePointer),sizeof(long));

        out.seekp(filePointer);

    }



    char *type = "INODE";

    out.write(type,6*sizeof(char));

    long tempDelta = node->writeExternal(out)+6*sizeof(char);

    /* delete(type);*/



    out.close();



    forestTaskList.erase(shouldeBeCreate);



    /*create several tasks base on each of the child partitions created before and then push these tasks to the global variable queue for the iteration of building child trees*/

    for(int i=childrenNumber-1;i>0;i--)

    {

        if(i==childrenNumber-1)

        {



            for(int j=pr.getPartition(i-1);j < pr.getPartition(i) - task->numPivots+1;j++)

            {

                ecludedData.push_back(task->dataList.at(j));

            }



        }

        else

        {

            shared_ptr <ForestTask> newTask ( new ForestTask(task->dataList,node,pr.getPartition(i-1),pr.getPartition(i),i,task->getMyHeight()+1));



            newTask->myIndexAdress = (long) (filePointer+(newTask->myIndex-1)*sizeof(long)+sizeof(int))+6*sizeof(char);



            this->forestTaskList.push_back(newTask);

        }



    }



    filePointer += tempDelta;

}



void CExcludedMiddleForest::createAndWriteLeafNodeLast(shared_ptr <ForestTask> task,ofstream &out,vector<shared_ptr<CMetricData> >& ecludedData,int treeIndex)

{



    out.open(fileName.c_str(),ios::binary|ios::in|ios::out);

    out.seekp(filePointer);



    this->numLeaf++;

    /*cout<<"leafNode:"<<numLeaf<<endl;*/

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

            ve.push_back(this->metric->getDistance(children[j].get(),pivots[i].get()));

        }



        distance.push_back(ve);

    }



    /*create a leaf node and assign its memory address to the child list of parent node*/

    shared_ptr< CMVPLeafNode> mvpLeafNode(new CMVPLeafNode(pivots,children,distance,task->getMyHeight()+1));



    if(this->rootAddress.at(treeIndex)==-2)

    {        



        task->setNode((shared_ptr<CIndexNode>)mvpLeafNode);



        out.close();

        return;

    }

    else

    {

        char type[6] = "LNODE";





        out.seekp(task->myIndexAdress);

        out.write((char*)(&filePointer),sizeof(long));

        out.seekp(filePointer);



        out.write(type,6*sizeof(char));

        filePointer+=(long)mvpLeafNode->writeExternal(out)+6*sizeof(char);

        /*delete(type);*/

    }



    out.close();

}



void CExcludedMiddleForest::loadRootToLeaf(vector<shared_ptr<CMetricData> > &dataObjects,vector<shared_ptr<CMetricData> >& ecludedData,int treeIndex,ofstream &out)

{      

    long tempAddress = filePointer;
    treeHeight.at(treeIndex) = 0;


    if(dataObjects.size()<=maxLeafSize)

    {

        rootAddress.at(treeIndex) = -2;

    }

    else

    {

        rootAddress.at(treeIndex) = -1;

    }



    vector<shared_ptr <ForestTask> >::iterator taskToGet;



    /*put the task that create the root node to the task queue*/

    shared_ptr<ForestTask> task(new ForestTask(dataObjects,shared_ptr<CIndexNode>(),0,dataObjects.size(),0,0));

    task->myIndexAdress = -1;



    this->forestTaskList.push_back(task);







    /*use every task in the queue to create a internal node or leaf node until the the queue is null*/

    while(this->forestTaskList.size()>0)

    {



        taskToGet=this->forestTaskList.end();

        int remainNodeSize;

        taskToGet--;

        task = *taskToGet;

        treeHeight.at(treeIndex)=(treeHeight.at(treeIndex)>=task->getMyHeight())?treeHeight.at(treeIndex):task->getMyHeight();



        /*get the number of pivot*/

        numPivots = (this->numPivots>=task->size) ? task->size : this->numPivots;



        /*selecte several piovt from current partition based on the given parameter*/

        vector<int> pivotsIndex;

        pivotsIndex=this->psm->selectPivots(this->metric,task->dataList,task->start,task->size,numPivots);



        /*move the pivot to the end of current partition*/

        task->groupPivotsAtEnd(pivotsIndex);



        remainNodeSize=task->size - task->numPivots;//task->size-task->numPivots



        /*if the size of current partition is greater than the max size of a leaf node then create a internal node, otherwise create a leaf node.the max size of leaf node is given by the builder of the mvp-tree*/

        if(remainNodeSize>maxLeafSize)

        {

            createAndWriteInternalNode(task,out,taskToGet,ecludedData);

            task.reset();

        }

        else

        {

            createAndWriteLeafNodeLast(task,out,ecludedData,treeIndex);

            if(rootAddress.at(treeIndex)!=-2)

            {

                forestTaskList.erase(taskToGet);

                task.reset();

            }

            else 

            {

                writeRoot(forestTaskList.at(0),out,ecludedData,treeIndex);

                task.reset();

                break;

            }

        }

    }



    rootAddress.at(treeIndex) = tempAddress;

    // rootAddress.at(treeIndex) = 0;



    // out.open(fileName,ios::binary|ios::_Nocreate);

    // out.seekp(-(long)sizeof(long),ios::end);

    // out.write((char*)&rootAddress,sizeof(long));

    // out.close();





    /*   out.close();*/





}



void CExcludedMiddleForest::setDataType(string dataType)

{

    this->dataType = dataType;

}



string CExcludedMiddleForest::getDataType()

{

    return dataType;

}



void CExcludedMiddleForest::setFileName(string fileName)

{

    this->fileName = fileName;

}



string CExcludedMiddleForest::getFileName()

{

    return fileName;

}