#include "../../HeaderFiles/index/MVPIndex.h"

#include "../../HeaderFiles/index/IndexNode.h"
#include "../../HeaderFiles/index/MVPInternalNode.h"
#include "../../HeaderFiles/index/MVPLeafNode.h"

#include "../../HeaderFiles/metricdata/MetricData.h"
#include "../../HeaderFiles/metricdata/DoubleVector.h"
#include "../../HeaderFiles/metricdata/DNA.h"
#include "../../HeaderFiles/metricdata/RNA.h"
#include "../../HeaderFiles/metricdata/Peptide.h"
#include "../../HeaderFiles/metricdata/StringObject.h"
#include "../../HeaderFiles/metricdata/Image.h"
#include "../../HeaderFiles/metricdata/TandemSpectra.h"
#include "../../HeaderFiles/index/query/SearchTask.h"

#include "../../HeaderFiles/metricdistance/DNAMetric.h"
#include "../../HeaderFiles/metricdistance/CountedMetric.h"
#include "../../HeaderFiles/metricdistance/EditDistance.h"
#include "../../HeaderFiles/metricdistance/ImageMetric.h"
//#include "../../HeaderFiles/metricdistance/MSMSMetric.h"
#include "../../HeaderFiles/metricdistance/RNAMetric.h"
#include "../../HeaderFiles/metricdistance/PeptideMetric.h"
#include "../../HeaderFiles/metricdistance/EuclideanDistance.h"
#include "../../HeaderFiles/util/ThreadPoolManager.h"

#include<iomanip>
#include<unordered_map>

#ifdef __GNUC__
#include "/usr/include/sys/times.h"
#endif


void joinCharArray(const char *&dest,const char *sor);
void verifySearchResult(vector<shared_ptr<CMetricData> > *&rawData,CMetricDistance *metric,CRangeQuery q, list<shared_ptr<CMetricData> > *queryResultList,ofstream &output);

struct traversalTask
{
    string type;
    shared_ptr<CIndexNode> currentNode;
    int nodeHeight;
    int currentNodeLayerNO;
    int parentNodeLayerNO;

};

void analysisCurrentNode(vector<shared_ptr<traversalTask> > &taskList,ifstream &iofilen,shared_ptr<traversalTask> &task,long filePointer,int &totalInternalNumber,int &totalLeafNumber,char* dataType,int &tempLayerNodeNumber,int &layerInternalNodeChildNumber,int &layerLeafNodeChildNumber)
{
    string datat(dataType);
    if(!strcmp(task->type.c_str(),"LNODE"))
    {
        /*shared_ptr<CMVPLeafNode> lnode;
        lnode.reset(&(CMVPLeafNode()));*/
        totalLeafNumber++;

    }
    else
    {
        totalInternalNumber++;

        char type[6]="iiiii";
        shared_ptr<CIndexNode> node;
        shared_ptr<traversalTask> ttask;
        traversalTask *t=0;
        for(int i=0;i<dynamic_cast<CMVPInternalNode*>(task->currentNode.get())->getChildeSize();i++)
        {
            filePointer=(dynamic_cast<CMVPInternalNode*>(task->currentNode.get()))->getSubTreeRootAddress(i);
            //cout<<"child "<<i<<" filePointer:"<<filePointer<<endl;
            iofilen.seekg(filePointer,ios::beg);
            iofilen.read(type,6*sizeof(char));
            //cout<<"  type:"<<type<<endl;
            if(!strcmp(type,"LNODE"))
            {
                layerLeafNodeChildNumber++;
                node=dynamic_pointer_cast<CMVPLeafNode,CIndexNode>(node);
                node.reset(new CMVPLeafNode);
                node.get()->readExternal(iofilen,datat);

                t=new traversalTask();
                t->currentNode=node;
                t->nodeHeight=task->nodeHeight+1;
                t->parentNodeLayerNO=task->currentNodeLayerNO;
                t->type="LNODE";
                t->currentNodeLayerNO=layerInternalNodeChildNumber+layerLeafNodeChildNumber;

                ttask.reset(t);
                taskList.push_back(ttask);
            }
            else if(!strcmp(type,"INODE"))
            {
                layerInternalNodeChildNumber++;

                node=dynamic_pointer_cast<CMVPInternalNode,CIndexNode>(node);
                node.reset(new CMVPInternalNode);
                node.get()->readExternal(iofilen,datat);

                /*if(layerInternalNodeChildNumber+layerLeafNodeChildNumber==247)
                {
                cout<<"parent node height:"<<task->nodeHeight<<" parent node layer id:"<<tempLayerNodeNumber<<" task->currentNodeLayerN0:"<<task->currentNodeLayerNO<<"	 current node height:"<<task->nodeHeight+1<<"	 currrent node layer id"<<layerInternalNodeChildNumber+layerLeafNodeChildNumber<<endl;
                }*/

                t=new traversalTask();
                t->currentNode=node;
                t->nodeHeight=task->nodeHeight+1;
                t->parentNodeLayerNO=task->currentNodeLayerNO;
                t->type="INODE";
                t->currentNodeLayerNO=layerInternalNodeChildNumber+layerLeafNodeChildNumber;

                ttask.reset(t);
                taskList.push_back(ttask);

            }
            else
            {
                cout<<type<<" Note:there is some bad pointers in the tree structure!"<<endl;
                exit(0);
            }
        }
    }

}
void getMVPIndexStructureInfo(char *&indexFileName,char *& dataType,char *& outputFileName)
{
    cout<<"start get mvp-tree structure information!"<<endl;

    string datat(dataType);
    long filePointer=0;
    int treeHeight=0;

    if(strcmp(dataType,"vector")==0)
    {
        CObjectFactory::objectRegister("vector",CDoubleVector::getConstructor());
    }
    else if(!strcmp(dataType,"spectra")) 
    {
        //metric = new CMSMSMetric;
        //queryList=CTandemSpectra::loadData(newQueryFile,lastQuery);
        //CObjectFactory::objectRegister("spectra",CTandemSpectra::getConstructor());
    }
    else if(!strcmp(dataType,"string")) 
    {
        CObjectFactory::objectRegister("string",CStringObject::getConstructor());
    }
    else if(!strcmp(dataType,"rna")) 
    {
        //CObjectFactory::objectRegister("rna",CRNA::getConstructor());
    }
    else if(!strcmp(dataType,"image")) 
    {
        CObjectFactory::objectRegister("image",CImage::getConstructor());
    }
    else if(!strcmp(dataType,"dna")) 
    {
        CObjectFactory::objectRegister("dna",CDNA::getConstructor());
    }
    else if(!strcmp(dataType,"peptide"))
    {
        CObjectFactory::objectRegister("peptide",CPeptide::getConstructor());
    }

    //cout<<__FILE__<<__LINE__<<" newIndexFileName:"<<newIndexFileName<<endl;

    ifstream iofilen(indexFileName,ios::in|ios::binary);

    if(!iofilen)
    {
        //cout<<__FILE__<<__LINE__<<" newIndexFileName:"<<newIndexFileName<<endl;
        //cout<<__FILE__<<__LINE__<<" open file "<<newIndexFileName<<" failed!"<<endl;
        exit(0);
    }

    iofilen.seekg(-(long)(sizeof(long)+sizeof(int)),ios::end);
    iofilen.read((char*)&treeHeight,sizeof(int));
    iofilen.read((char*)&filePointer,sizeof(long));
    iofilen.seekg(filePointer,ios::beg);

    char type[6]="";
    iofilen.read(type,6*sizeof(char));
    filePointer+=6*sizeof(char);

    shared_ptr<CIndexNode> node;
    vector<shared_ptr<traversalTask> > taskList;

    if(!strcmp(type,"INODE"))
    {
        node=dynamic_pointer_cast<CMVPInternalNode,CIndexNode>(node);
        node.reset(new CMVPInternalNode());
        node.get()->readExternal(iofilen,datat);
    }
    else if(!strcmp(type,"LNODE"))
    {
        node=dynamic_pointer_cast<CMVPLeafNode,CIndexNode>(node);
        node.reset(new CMVPLeafNode());
        node.get()->readExternal(iofilen,datat);
    }

    shared_ptr<traversalTask> task;
    task.reset(new traversalTask());
    task->type=string(type);
    task->currentNode=node;
    task->parentNodeLayerNO=0;
    task->nodeHeight=1;
    taskList.push_back(task);

    int tempLayerNodeNumber=0;	         //number of nodes per-layer
    int tempLayerInternalNodeChildNumber=0;	//number of internal node children of current node
    int tempLayerLeafNodeChildNumber=0;    //number of leaf node children of current node
    int tempTreeHeight=1;		      //height of mvp tree

    int totalInternalNumber=0;
    int totalLeafNumber=0;

    while(taskList.size()>0)
    {
        vector<shared_ptr<traversalTask> >::iterator ite=taskList.begin();

        task=*ite;

        taskList.erase(ite);

        if(task->nodeHeight==1)
        {
            tempLayerNodeNumber=1;
            tempLayerInternalNodeChildNumber=0;
            tempLayerLeafNodeChildNumber=0;
            tempTreeHeight=1;

            analysisCurrentNode(taskList,iofilen,task,filePointer,totalInternalNumber,totalLeafNumber,dataType,tempLayerNodeNumber,tempLayerInternalNodeChildNumber,tempLayerLeafNodeChildNumber);
        }
        else if(task->nodeHeight==tempTreeHeight)
        {
            tempLayerNodeNumber++;

            analysisCurrentNode(taskList,iofilen,task,filePointer,totalInternalNumber,totalLeafNumber,dataType,tempLayerNodeNumber,tempLayerInternalNodeChildNumber,tempLayerLeafNodeChildNumber);
        }
        else if(task->nodeHeight!=tempTreeHeight)
        {
            cout<<" height: "<<tempTreeHeight<<" nodeNumber: "<<tempLayerNodeNumber<<" internal node child Number: "<<tempLayerInternalNodeChildNumber<<" leaf node child number:"<<tempLayerLeafNodeChildNumber<<endl;

            tempLayerNodeNumber=1;
            tempLayerInternalNodeChildNumber=0;
            tempLayerLeafNodeChildNumber=0;
            tempTreeHeight=task->nodeHeight;

            analysisCurrentNode(taskList,iofilen,task,filePointer,totalInternalNumber,totalLeafNumber,dataType,tempLayerNodeNumber,tempLayerInternalNodeChildNumber,tempLayerLeafNodeChildNumber);
        }
    }

    iofilen.close();
    cout<<"**************"<<" total internal node:"<<totalInternalNumber<<" total leaf node:"<<totalLeafNumber<<endl;
}

/**********************************************************serach***************************************************************************/
int gThreadNum=0;

std::mutex gthread_num_mutex;
std::mutex threadpool_tasklist_m;
std::mutex forPrintf;

void showPromptInformation(int line)
{
    forPrintf.lock();
    gThreadNum++;
    cerr<<"line:"<<line<<" "<<std::this_thread::get_id()<<" thread with task:"<<gThreadNum<<endl;
    forPrintf.unlock();
}
struct pblock
{
    //pblock():queryObjectList(0),searchRadius(0),index(0),dataType(0),newIndexName(0),metric(0),num(0),resultCounter(0),output(0){}
    pblock(std::mutex &t_mutex,vector<shared_ptr<CMetricData> > *&data):thread_num_mutex(t_mutex),rawData(data){}
    vector<shared_ptr<CMetricData> > *queryObjectList;
    double searchRadius;
    char *dataType;
    char *newIndexName;
    CMetricDistance *metric;
    int num;
    double *resultCounter;
    double *distanceCounter;
    ofstream *output;
    double *searchTime;
    long rootaddress;
    int threadNum;

    mutex &thread_num_mutex;


    //for verify
    vector<shared_ptr<CMetricData> > *rawData;
    bool verify;
};

struct pforestBlock
{
    pforestBlock(std::mutex &t_mutex,vector<shared_ptr<CMetricData> > *&data):thread_num_mutex(t_mutex),rawData(data){}
    vector<shared_ptr<CMetricData> > *queryObjectList;
    double searchRadius;
    char *dataType;
    char *newIndexName;
    CMetricDistance *metric;
    int num;
    double *resultCounter;
    int *distanceCounter;
    ofstream *output;
    double *searchTime;
    int numTree;
    long* rootAddress;
    int threadNum;

    mutex &thread_num_mutex;


    //for verify
    vector<shared_ptr<CMetricData> > *rawData;
    bool verify;
};
/**********************************************************thread pool start***********************************************************************/

struct threadpooltask
{
    list<shared_ptr<singlequerytask> > *tasklist;
    double *resultCounter;
    double *distanceCounter;
    double *searchTime;
    int num;
};

struct forestthreadpooltask
{
    list<shared_ptr<singleforestquerytask> > *tasklist;
    double *resultCounter;
    int *distanceCounter;
    double *searchTime;
    int num;
};


void threadpoolfunc(threadpooltask *vpb)
{
#ifdef _WIN32

    FILETIME kernelStart,kernelEnd;

    FILETIME userStart,userEnd;

    FILETIME dummy;

#else
    clock_t begt,endt;
#endif

    while(1)
    {

        // showPromptInformation(__LINE__);
        shared_ptr<singlequerytask> task;
        threadpool_tasklist_m.lock();
        if(vpb->tasklist->size()>0)
        {
            //showPromptInformation(__LINE__);
            task=vpb->tasklist->back();
            //showPromptInformation(__LINE__);
            vpb->tasklist->pop_back();
            //showPromptInformation(__LINE__);
        }
        else
        {
            threadpool_tasklist_m.unlock();
            //showPromptInformation(__LINE__);
            break;
        }
        threadpool_tasklist_m.unlock();
        //showPromptInformation(__LINE__);

#ifdef _WIN32
        GetThreadTimes(GetCurrentThread(),&dummy,&dummy,&kernelStart,&userStart);
#else
        begt=times(NULL);
#endif 
		//showPromptInformation(__LINE__);
        task->searchTask->searchWithCache(*(task->rq),task->rootaddress);
        //showPromptInformation(__LINE__);
#ifdef _WIN32
        GetThreadTimes(GetCurrentThread(),&dummy,&dummy,&kernelEnd,&userEnd);
#else
        endt=times(NULL);
#endif

        task->distancecount=task->cmetric->getCounter();
		//cout << "task->distancecounter:" << task->distancecount << endl;

#ifdef _WIN32

        task->searchtime +=(userEnd.dwLowDateTime+kernelEnd.dwLowDateTime-userStart.dwLowDateTime-kernelEnd.dwLowDateTime)/10000000.00;
#else
        task->searchtime +=(double)(endt-begt)/100.0;
#endif

        //showPromptInformation(__LINE__);
        threadpool_tasklist_m.lock();
        vpb->resultCounter[vpb->num]+=task->searchTask->getResultSize();
        vpb->distanceCounter[vpb->num]+=task->cmetric->getCounter();
        vpb->searchTime[vpb->num]+=task->searchtime;


        //for verify
        if(task->verify)
        {
            verifySearchResult(task->rawData,task->metric,*(task->rq),task->searchTask->getResult(),*(task->output));
        }


        threadpool_tasklist_m.unlock();

        task.reset();
        //showPromptInformation(__LINE__);
    }
}

void forestthreadpoolfunc(forestthreadpooltask *vpb)
{
#ifdef _WIN32

    FILETIME kernelStart,kernelEnd;

    FILETIME userStart,userEnd;

    FILETIME dummy;

#else
    clock_t begt,endt;
#endif

    while(1)
    {

        // showPromptInformation(__LINE__);
        shared_ptr<singleforestquerytask> task;
        threadpool_tasklist_m.lock();
        if(vpb->tasklist->size()>0)
        {
            //showPromptInformation(__LINE__);
            task=vpb->tasklist->back();
            //showPromptInformation(__LINE__);
            vpb->tasklist->pop_back();
            //showPromptInformation(__LINE__);
        }
        else
        {
            threadpool_tasklist_m.unlock();
            //showPromptInformation(__LINE__);
            break;
        }
        threadpool_tasklist_m.unlock();
        //showPromptInformation(__LINE__);

#ifdef _WIN32
        GetThreadTimes(GetCurrentThread(),&dummy,&dummy,&kernelStart,&userStart);
#else
        begt=times(NULL);
#endif 
        //showPromptInformation(__LINE__);
        task->searchTask->searchForestWithCache(*(task->rq),task->rootAddress,task->numTree);
        //showPromptInformation(__LINE__);
#ifdef _WIN32
        GetThreadTimes(GetCurrentThread(),&dummy,&dummy,&kernelEnd,&userEnd);
#else
        endt=times(NULL);
#endif

        task->distancecount=task->cmetric->getCounter();

#ifdef _WIN32

        task->searchtime += (userEnd.dwLowDateTime+kernelEnd.dwLowDateTime-userStart.dwLowDateTime-kernelEnd.dwLowDateTime)/10000000.00;

#else


        task->searchtime += (double)(endt-begt)/100.0;


#endif
        //showPromptInformation(__LINE__);
        threadpool_tasklist_m.lock();
        vpb->resultCounter[vpb->num]+=task->searchTask->getResultSize();
        vpb->distanceCounter[vpb->num]+=task->cmetric->getCounter();
        vpb->searchTime[vpb->num]+=task->searchtime;


        //for verify
        if(task->verify)
        {
            verifySearchResult(task->rawData,task->metric,*(task->rq),task->searchTask->getResult(),*(task->output));
        }


        threadpool_tasklist_m.unlock();

        task.reset();
        //showPromptInformation(__LINE__);
    }
}

void globalThreadFuncWithThreadPool(pblock *&vpb)
{
    //cerr<<__FILE__<<__LINE__<<" number of query:"<<vpb->queryObjectList->size()<<endl;
    list<shared_ptr<singlequerytask> >taskList;
    int taskNum=0;
    //showPromptInformation(__LINE__);
    for(int i=0;i<vpb->queryObjectList->size();i++)
    {

        shared_ptr<CountedMetric> cm(new CountedMetric(vpb->metric));
        shared_ptr<CRangeQuery> rq(new CRangeQuery(vpb->searchRadius,vpb->queryObjectList->at(i)));
        shared_ptr<CSearchTask> searchTask(new CSearchTask(vpb->newIndexName,vpb->dataType,*cm));

        shared_ptr<singlequerytask> task(new singlequerytask);
        task->searchTask=searchTask;
        task->rq=rq;
        task->cmetric=cm;
        task->rootaddress=vpb->rootaddress;
        task->searchtime=0;
        task->distancecount=0;

        //for verify
        task->rawData = vpb->rawData;
        task->metric = vpb->metric;
        task->output = vpb->output ;
        task->verify = vpb->verify;

        taskList.push_back(task);
        taskNum++;
    }
    //showPromptInformation(__LINE__);
    threadpooltask tptask;
    tptask.tasklist=&taskList;
    tptask.resultCounter=vpb->resultCounter;
    tptask.distanceCounter=vpb->distanceCounter;
    tptask.searchTime=vpb->searchTime;
    tptask.num=vpb->num;

    thread *threadlist=new thread[vpb->threadNum];
    //cerr<<__FILE__<<__LINE__<<" threadNum:"<<vpb->threadNum<<endl;
    for(int i=0;i<vpb->threadNum;i++)
    {
        try
        {
            threadlist[i]=thread(threadpoolfunc,&tptask);
        }
        catch(std::system_error &e)
        {
            //showPromptInformation(472);
            //cerr<<__FILE__<<__LINE__<<" Exception:"<<e.what()<<" arise at "<<e.code()<<" when i:"<<i<<endl;
            i--;
            continue;
        }
        catch(std::bad_alloc &e)
        {
            //cerr<<__LINE__<<" Exception:"<<e.what()<<" arise at i:"<<i<<endl;
            i--;
            continue;
        }
        catch(exception &e)
        {
            //cerr<<__LINE__<<" Exception:"<<e.what()<<" arise at i:"<<i<<endl;
            i--;
            continue;
        }
    }
	
    //showPromptInformation(__LINE__);
    for(int i=0;i<vpb->threadNum;i++)
    {
        //showPromptInformation(__LINE__);
        threadlist[i].join();
        //showPromptInformation(__LINE__);
    }

	//cout << __LINE__ << " total distance:" << vpb->distanceCounter[vpb->num] << " count:" << vpb->queryObjectList->size() << endl;

    //showPromptInformation(__LINE__);
    delete [] threadlist;
}

void forestGlobalThreadFuncWithThreadPool(pforestBlock *&vpb)
{
    //cerr<<__FILE__<<__LINE__<<" number of query:"<<vpb->queryObjectList->size()<<endl;
    list<shared_ptr<singleforestquerytask> >taskList;
    int taskNum=0;
    //showPromptInformation(__LINE__);
    for(int i=0;i<vpb->queryObjectList->size();i++)
    {

        shared_ptr<CountedMetric> cm(new CountedMetric(vpb->metric));
        shared_ptr<CRangeQuery> rq(new CRangeQuery(vpb->searchRadius,vpb->queryObjectList->at(i)));
        shared_ptr<CSearchTask> searchTask(new CSearchTask(vpb->newIndexName,vpb->dataType,*cm));

        shared_ptr<singleforestquerytask> task(new singleforestquerytask);
        task->searchTask=searchTask;
        task->rq=rq;
        task->cmetric=cm;
        task->numTree=vpb->numTree;
        task->rootAddress=vpb->rootAddress;
        task->searchtime=0;
        task->distancecount=0;

        //for verify
        task->rawData = vpb->rawData;
        task->metric = vpb->metric;
        task->output = vpb->output ;
        task->verify = vpb->verify;

        taskList.push_back(task);
        taskNum++;
    }
    //showPromptInformation(__LINE__);
    forestthreadpooltask tptask;
    tptask.tasklist=&taskList;
    tptask.resultCounter=vpb->resultCounter;
    tptask.distanceCounter=vpb->distanceCounter;
    tptask.searchTime=vpb->searchTime;
    tptask.num=vpb->num;

    thread *threadlist=new thread[vpb->threadNum];
    //cerr<<__FILE__<<__LINE__<<" threadNum:"<<vpb->threadNum<<endl;
    for(int i=0;i<vpb->threadNum;i++)
    {
        try
        {
            threadlist[i]=thread(forestthreadpoolfunc,&tptask);
        }
        catch(std::system_error &e)
        {
            showPromptInformation(472);
            cerr<<__FILE__<<__LINE__<<" Exception:"<<e.what()<<" arise at "<<e.code()<<" when i:"<<i<<endl;
            i--;
            continue;
        }
        catch(std::bad_alloc &e)
        {
            //cerr<<__LINE__<<" Exception:"<<e.what()<<" arise at i:"<<i<<endl;
            i--;
            continue;
        }
        catch(exception &e)
        {
            //cerr<<__LINE__<<" Exception:"<<e.what()<<" arise at i:"<<i<<endl;
            i--;
            continue;
        }
    }
    //showPromptInformation(__LINE__);
    for(int i=0;i<vpb->threadNum;i++)
    {
        //showPromptInformation(__LINE__);
        threadlist[i].join();
        //showPromptInformation(__LINE__);
    }
    //showPromptInformation(__LINE__);
    delete [] threadlist;
}
/**********************************************************thread pool end***********************************************************************/

void singlethreadfunc(singlequerytask *vpb)
{
#ifdef _WIN32

    FILETIME kernelStart,kernelEnd;

    FILETIME userStart,userEnd;

    FILETIME dummy;

#else

    struct tmp;


#endif


#ifdef _WIN32

    GetThreadTimes(GetCurrentThread(),&dummy,&dummy,&kernelStart,&userStart);

#else


    clock_t begt=times(NULL);

#endif 

    vpb->searchTask->singleSearchWithCache(*(vpb->rq),vpb->rootaddress);

#ifdef _WIN32

    GetThreadTimes(GetCurrentThread(),&dummy,&dummy,&kernelEnd,&userEnd);

#else


    clock_t endt=times(NULL);

#endif


    vpb->distancecount=vpb->cmetric->getCounter();

#ifdef _WIN32

    vpb->searchtime=(userEnd.dwLowDateTime+kernelEnd.dwLowDateTime-userStart.dwLowDateTime-kernelEnd.dwLowDateTime)/10000000.00;

#else


    vpb->searchtime=(endt-begt)/100.0;


#endif

    //for verify
    if(vpb->verify)
    {
        verifySearchResult(vpb->rawData,vpb->metric,*(vpb->rq),vpb->searchTask->getResult(),*(vpb->output));
    }
}

void globalThreadFuncForSingleSearchObjectUnlimitedThreadNumber(pblock *vpb)
{
    int temp=0;
    vector<shared_ptr<singlequerytask> >taskList;

    int taskNum=vpb->queryObjectList->size();
    for(int i=0;i<taskNum;i++)
    {
        shared_ptr<CountedMetric> cm(new CountedMetric(vpb->metric));
        shared_ptr<CRangeQuery> rq(new CRangeQuery(vpb->searchRadius,vpb->queryObjectList->at(i)));
        shared_ptr<CSearchTask> searchTask(new CSearchTask(vpb->newIndexName,vpb->dataType,*cm));

        shared_ptr<singlequerytask> task(new singlequerytask);
        task->searchTask=searchTask;
        task->rq=rq;
        task->cmetric=cm;
        task->rootaddress=vpb->rootaddress;
        task->searchtime=0;
        task->distancecount=0;

        //for verify
        task->rawData = vpb->rawData;
        task->metric = vpb->metric;
        task->output = vpb->output;
        task->verify = vpb->verify;

        taskList.push_back(task);

    }
    thread *threadlist=new thread[taskNum];

    for(int i=0;i<taskNum;i++)
    {
        try
        {
            threadlist[i]=thread(singlethreadfunc,taskList[i].get());
        }
        catch(std::system_error &e)
        {
            //cerr<<__LINE__<<" Exception:"<<e.what()<<" arise at "<<e.code()<<" when i:"<<i<<endl;
            i--;
            continue;
        }
        catch(std::bad_alloc &e)
        {
            //cerr<<__LINE__<<" Exception:"<<e.what()<<" arise at i:"<<i<<endl;
            i--;
            continue;
        }
        catch(exception &e)
        {
            //cerr<<__LINE__<<" Exception:"<<e.what()<<" arise at i:"<<i<<endl;
            i--;
            continue;
        }
    }

    for(int i=0;i<taskNum;i++)
    {
        threadlist[i].join();
    }

    delete [] threadlist;

    for(int i=0;i<taskNum;i++)
    {
        vpb->resultCounter[vpb->num]+=taskList[i]->searchTask->getResultSize();
        vpb->distanceCounter[vpb->num]+=taskList[i]->distancecount;
        vpb->searchTime[vpb->num]+=taskList[i]->searchtime;
    }

}				

void globalThreadFuncForSingleSearchObject(pblock *vpb)
{
    vector<shared_ptr<singlequerytask> > taskList;
    int taskNum=0;
    for(int i=0;i<vpb->queryObjectList->size();i++)
    {
        shared_ptr<CountedMetric> cm(new CountedMetric(vpb->metric));
        shared_ptr<CRangeQuery> rq(new CRangeQuery(vpb->searchRadius,vpb->queryObjectList->at(i)));
        shared_ptr<CSearchTask> searchTask(new CSearchTask(vpb->newIndexName,vpb->dataType,*cm));

        shared_ptr<singlequerytask> task(new singlequerytask);
        task->searchTask=searchTask;
        task->rq=rq;
        task->cmetric=cm;
        task->rootaddress=vpb->rootaddress;
        task->searchtime=0;
        task->distancecount=0;

        //for verify
        task->rawData = vpb->rawData;
        task->metric = vpb->metric;
        task->output = vpb->output;
        task->verify = vpb->verify;


        taskList.push_back(task);
        taskNum++;
    }

    thread *threadlist=new thread[vpb->threadNum];
    int j=0,k=0;
    for(int i=0;i<taskNum;i++)
    {
        if(j<vpb->threadNum)
        {
            try
            {
                threadlist[j]=thread(singlethreadfunc,taskList[i].get());
            }
            catch(std::system_error &e)
            {
                //cerr<<__LINE__<<" Exception:"<<e.what()<<" arise at "<<e.code()<<" when i:"<<i<<" j:"<<j<<endl;
                i--;
                continue;
            }
            catch(std::bad_alloc &e)
            {
                //cerr<<__LINE__<<" Exception:"<<e.what()<<" arise at i:"<<i<<endl;
                i--;
                continue;
            }
            catch(exception &e)
            {
                //cerr<<__LINE__<<" Exception:"<<e.what()<<" arise at i:"<<i<<endl;
                i--;
                continue;
            }
            j++;
        }
        else
        {
            for(int q=0;q<j;q++)
            {
                threadlist[q].join();
                vpb->resultCounter[vpb->num]+=taskList[k+q]->searchTask->getResultSize();
                vpb->distanceCounter[vpb->num]+=taskList[k+q]->distancecount;
                vpb->searchTime[vpb->num]+=taskList[k+q]->searchtime;

                //for verify
                if(taskList[k+q]->verify)
                {
                    verifySearchResult(taskList[k+q]->rawData,taskList[k+q]->metric,*(taskList[k+q]->rq),taskList[k+q]->searchTask->getResult(),*(taskList[k+q]->output));
                }

                taskList[k+q].reset();

            }
            j=0;
            k=i--;
        }


    }

    for(int i=0;i<j;i++)
    {
        threadlist[i].join();
    }

    for(int i=k;i<taskNum;i++)
    {

        vpb->resultCounter[vpb->num]+=taskList[i]->searchTask->getResultSize();
        vpb->distanceCounter[vpb->num]+=taskList[i]->distancecount;
        vpb->searchTime[vpb->num]+=taskList[i]->searchtime;


        //for verify
        if(taskList[i]->verify)
        {
            verifySearchResult(taskList[i]->rawData,taskList[i]->metric,*(taskList[i]->rq),taskList[i]->searchTask->getResult(),*(taskList[i]->output));
        }

        taskList[i].reset();

    }

    delete [] threadlist;
}

void globalMultithreadSearch(vector<shared_ptr<CMetricData> > *rawData,vector<shared_ptr<CMetricData> > *queryObjectsList,char *dataType,char *indexFileName,bool verify,double maxRadius,double minRadius,double step,CountedMetric *&cmetric, char *resultFileName,int dataBaseSize,int cacheHeight,int threadNum)
{

    ofstream output(resultFileName,ios::out|ios::app);

	int runNumber = 1;
	if((maxRadius-minRadius) <= 0.0001)
        runNumber=1;
    else
        runNumber=ceil((maxRadius-minRadius)/step);

    double *resultCounter=new double[runNumber];
    double *distanceCounter=new double[runNumber];
    double *searchTime=new double[runNumber];
    for(int i=0;i<runNumber;i++)
    {
        distanceCounter[i]=0;
        resultCounter[i]=0;
        searchTime[i]=0;
    }

    CMetricDistance *metric=0;
    CountedMetric *cm=0;

    long rootaddress=0;
    ifstream in(indexFileName,ios::binary);
    in.seekg(-(int)sizeof(long),ios::end);
    in.read((char*)&rootaddress,sizeof(long));
    in.close();

    vector<pblock*> task;
    for(int i=0;i<runNumber;i++)
    {
        if(strcmp(dataType,"vector")==0)
        {
            metric = new CEuclideanDistance;
        }
        else if(!strcmp(dataType,"spectra")) 
        {
            //metric = new CMSMSMetric;
        }
        else if(!strcmp(dataType,"string")) 
        {
            metric = new CEditDistance;
        }
        else if(!strcmp(dataType,"rna")) 
        {
            metric = new CRNAMetric ;
        }
        else if(!strcmp(dataType,"image")) 
        {
            metric = new CImageMetric ;	
        }
        else if(!strcmp(dataType,"dna")) 
        {
            metric = new CDNAMetric ;	
        }
        else if(!strcmp(dataType,"peptide"))
        {
            metric = new CPeptideMetric;
        }

        cm=new CountedMetric(metric);

        pblock *pb=new pblock(gthread_num_mutex,rawData);
        {
            pb->dataType=dataType;
            pb->metric=cm;
            pb->rootaddress=rootaddress;
            pb->newIndexName=indexFileName;
            pb->output=&output;
            pb->queryObjectList=queryObjectsList;
            pb->resultCounter=resultCounter;
            pb->distanceCounter=distanceCounter;
            pb->searchTime=searchTime;
            pb->threadNum=threadNum;

            //for verify
            pb->rawData = rawData;
            pb->verify = verify;
        }

        task.push_back(pb);
    }

    thread *threadList=new thread[runNumber];

#ifdef _WIN32

    FILETIME kernelStart,kernelEnd;

    FILETIME userStart,userEnd;

    FILETIME dummy;

#else

    clock_t begt,endt;

#endif

#ifdef _WIN32

    GetThreadTimes(GetCurrentThread(),&dummy,&dummy,&kernelStart,&userStart);

#else


    begt=times(NULL);


#endif
    for(int i=0;i<runNumber;i++)
    {

        pblock *pb=task.at(i);

        double searchRadius=minRadius+step*i;

        pb->num=i;
        pb->searchRadius=searchRadius;


        globalThreadFuncWithThreadPool(pb);//thread pool
        //globalThreadFuncForSingleSearchObjectUnlimitedThreadNumber(pb);//search thread task for every search radius with unlimited thread number -direact parallel
        //globalThreadFuncForSingleSearchObject(pb);//search thread task for every search radius with limited thread number 


    }

#ifdef _WIN32

    GetThreadTimes(GetCurrentThread(),&dummy,&dummy,&kernelEnd,&userEnd);

#else


    endt=times(NULL);

#endif

    for(int i=0;i<runNumber;i++)
    {
        //threadList[i].join();
    }
    //showPromptInformation(__LINE__);
    delete [] threadList;
    for(int i=0;i<runNumber;i++)
        output<<dataBaseSize
        <<setiosflags(ios::right)<<setiosflags(ios::fixed)<<setw(20)<<(minRadius+step*i)
        <<setiosflags(ios::right)<<setiosflags(ios::fixed)<<setw(15)<<(distanceCounter[i]/(queryObjectsList->size()))
        <<setiosflags(ios::right)<<setiosflags(ios::fixed)<<setw(15)<<(searchTime[i]/(queryObjectsList->size()))
        <<setiosflags(ios::right)<<setiosflags(ios::fixed)<<setw(15)<<(resultCounter[i]/(queryObjectsList->size()))
        <<setiosflags(ios::right)<<setiosflags(ios::fixed)<<setw(20)<<"****"
        <<setiosflags(ios::right)<<setiosflags(ios::fixed)<<setw(20)<<"****"
        <<setiosflags(ios::right)<<setiosflags(ios::fixed)<<setw(20)<<"****"
        <<setiosflags(ios::right)<<setiosflags(ios::fixed)<<setw(20)<<"****"
        <<setiosflags(ios::right)<<setiosflags(ios::fixed)<<setw(25)<<"****"
        <<setiosflags(ios::right)<<setiosflags(ios::fixed)<<setw(20)<<"****"<<endl;
#ifdef _WIN32

    output<<"searchMode:"<<3<<" query number:"<<queryObjectsList->size()<<" total queryObject number:"<<runNumber*queryObjectsList->size()<<" threadNum:"<<threadNum<<" total search time:"<<(userEnd.dwLowDateTime+kernelEnd.dwLowDateTime-userStart.dwLowDateTime-kernelEnd.dwLowDateTime)/10000000.00<<" cache height:"<<cacheHeight<<endl;

#else


    output<<"searchMode:"<<3<<" query number:"<<queryObjectsList->size()<<" total queryObject number:"<<runNumber*queryObjectsList->size()<<" threadNum:"<<threadNum<<" total search time:"<<(endt-begt)/100.0<<" cache height:"<<cacheHeight<<endl;


#endif

    output<<"======================================================================================================"<<endl;
    output.close();
    //showPromptInformation(__LINE__);
    for(int i=0;i<task.size();i++)
        delete task.at(i);
    //showPromptInformation(__LINE__);
}

void globalForestMultithreadSearch(vector<shared_ptr<CMetricData> > *rawData,vector<shared_ptr<CMetricData> > *queryObjectsList,char *dataType,char *indexFileName,int& numTree,long* rootAddress,bool verify,double maxRadius,double minRadius,double step,CountedMetric *&cmetric, char *resultFileName,int dataBaseSize,int cacheHeight,int threadNum)
{
    ofstream output(resultFileName,ios::out|ios::app);

    int runNumber=floor((maxRadius-minRadius)/step)+1;

    double *resultCounter=new double[runNumber];
    int *distanceCounter=new int[runNumber];
    double *searchTime=new double[runNumber];
    for(int i=0;i<runNumber;i++)
    {
        distanceCounter[i]=0;
        resultCounter[i]=0;
        searchTime[i]=0;
    }

    CMetricDistance *metric=0;
    CountedMetric *cm=0;

    vector<pforestBlock*> task;
    for(int i=0;i<runNumber;i++)
    {
        if(strcmp(dataType,"vector")==0)
        {
            metric = new CEuclideanDistance;
        }
        else if(!strcmp(dataType,"spectra")) 
        {
            //metric = new CMSMSMetric;
        }
        else if(!strcmp(dataType,"string")) 
        {
            metric = new CEditDistance;
        }
        else if(!strcmp(dataType,"rna")) 
        {
            metric = new CRNAMetric ;
        }
        else if(!strcmp(dataType,"image")) 
        {
            metric = new CImageMetric ;	
        }
        else if(!strcmp(dataType,"dna")) 
        {
            metric = new CDNAMetric ;	
        }
        else if(!strcmp(dataType,"peptide"))
        {
            metric = new CPeptideMetric;
        }

        cm=new CountedMetric(metric);

        pforestBlock *pb=new pforestBlock(gthread_num_mutex,rawData);
        {
            pb->dataType=dataType;
            pb->metric=cm;
            pb->numTree=numTree;
            pb->rootAddress=rootAddress;
            pb->newIndexName=indexFileName;
            pb->output=&output;
            pb->queryObjectList=queryObjectsList;
            pb->resultCounter=resultCounter;
            pb->distanceCounter=distanceCounter;
            pb->searchTime=searchTime;
            pb->threadNum=threadNum;

            //for verify
            pb->rawData = rawData;
            pb->verify = verify;
        }

        task.push_back(pb);
    }

    thread *threadList=new thread[runNumber];

#ifdef _WIN32

    FILETIME kernelStart,kernelEnd;

    FILETIME userStart,userEnd;

    FILETIME dummy;

#else

    clock_t begt,endt;

#endif

#ifdef _WIN32

    GetThreadTimes(GetCurrentThread(),&dummy,&dummy,&kernelStart,&userStart);

#else


    begt=times(NULL);


#endif
    for(int i=0;i<runNumber;i++)
    {

        pforestBlock *pb=task.at(i);

        double searchRadius=minRadius+step*i;

        pb->num=i;
        pb->searchRadius=searchRadius;


        forestGlobalThreadFuncWithThreadPool(pb);//thread pool
        //globalThreadFuncForSingleSearchObjectUnlimitedThreadNumber(pb);//search thread task for every search radius with unlimited thread number -direact parallel
        //globalThreadFuncForSingleSearchObject(pb);//search thread task for every search radius with limited thread number 


    }

#ifdef _WIN32

    GetThreadTimes(GetCurrentThread(),&dummy,&dummy,&kernelEnd,&userEnd);

#else


    endt=times(NULL);

#endif

    for(int i=0;i<runNumber;i++)
    {
        //threadList[i].join();
    }
    //showPromptInformation(__LINE__);
    delete [] threadList;
    for(int i=0;i<runNumber;i++)
        output<<dataBaseSize
        <<setiosflags(ios::right)<<setiosflags(ios::fixed)<<setw(20)<<(minRadius+step*i)
        <<setiosflags(ios::right)<<setiosflags(ios::fixed)<<setw(15)<<(distanceCounter[i]/(queryObjectsList->size()))
        <<setiosflags(ios::right)<<setiosflags(ios::fixed)<<setw(15)<<(searchTime[i]/(queryObjectsList->size()))
        <<setiosflags(ios::right)<<setiosflags(ios::fixed)<<setw(15)<<(resultCounter[i]/(queryObjectsList->size()))
        <<setiosflags(ios::right)<<setiosflags(ios::fixed)<<setw(20)<<"****"
        <<setiosflags(ios::right)<<setiosflags(ios::fixed)<<setw(20)<<"****"
        <<setiosflags(ios::right)<<setiosflags(ios::fixed)<<setw(20)<<"****"
        <<setiosflags(ios::right)<<setiosflags(ios::fixed)<<setw(20)<<"****"
        <<setiosflags(ios::right)<<setiosflags(ios::fixed)<<setw(25)<<"****"
        <<setiosflags(ios::right)<<setiosflags(ios::fixed)<<setw(20)<<"****"<<endl;
#ifdef _WIN32

    output<<"searchMode:"<<3<<" query number:"<<queryObjectsList->size()<<" total queryObject number:"<<runNumber*queryObjectsList->size()<<" threadNum:"<<threadNum<<" total search time:"<<(userEnd.dwLowDateTime+kernelEnd.dwLowDateTime-userStart.dwLowDateTime-kernelEnd.dwLowDateTime)/10000000.00<<" cache height:"<<cacheHeight<<endl;

#else


    output<<"searchMode:"<<3<<" query number:"<<queryObjectsList->size()<<" total queryObject number:"<<runNumber*queryObjectsList->size()<<" threadNum:"<<threadNum<<" total search time:"<<(endt-begt)/100.0<<" cache height:"<<cacheHeight<<endl;


#endif

    output<<"======================================================================================================"<<endl;
    output.close();
    //showPromptInformation(__LINE__);
    for(int i=0;i<task.size();i++)
        delete task.at(i);
    //showPromptInformation(__LINE__);
}


/**********************************************************************************************************************************************************************************************/
void joinCharArray(char *&dest,const char*sor)
{
    char *temp=dest;

    dest=(char*)malloc((strlen(dest)+strlen(sor)+1)*sizeof(char));
    char *t=dest;
    while(*temp!='\0')
    {
        *dest=*temp;
        dest++;
        temp++;
    }

    while(*sor!='\0')
    {
        *dest=*sor;
        dest++;
        sor++;
    }

    *dest='\0';
    dest=t;

}

void cacheIndex(int *height,char* indexfilename,string dataType,unordered_map<long,std::pair<char*,CIndexNode*> > *cache,bool all)
{
    ifstream infile(indexfilename,ios::binary);
	if(infile.fail()){
		cout<<"ERROR : can't find index file!"<<endl;
		exit(1);
	}
	
    long rootAddress=0;

    int treeHeight=0;

    infile.seekg(-(long)(sizeof(long)+sizeof(int)),ios::end);
    infile.read((char*)(&treeHeight),sizeof(int));
    infile.read((char*)(&rootAddress),sizeof(long));

    //cout<<"filename:"<<indexfilename<<" rootaddress:"<<rootAddress<<" tree height:"<<treeHeight<<" cache height:"<<*height<<endl;

    char *type=new char[6];
    infile.seekg(rootAddress,ios::beg);
    infile.read(type,6*sizeof(char));
    string ttp(type);

    CIndexNode *root;

    list<CMVPInternalNode*> taskList;

    if(strcmp(type,"INODE")==0)
    {
        root=new CMVPInternalNode();
        root->readExternal(infile,dataType);
        if(*height>0 && all)
            cache->insert(std::pair<long,std::pair<char*,CIndexNode*> >(rootAddress,std::pair<char*,CIndexNode*>(type,root)));
        taskList.push_back((CMVPInternalNode*)root);
    }
    else if(strcmp(type,"LNODE")==0)
    {
        root=new CMVPLeafNode();
        root->readExternal(infile,dataType);
        if(*height>0);
        cache->insert(std::pair<long,std::pair<char*,CIndexNode*> >(rootAddress,std::pair<char*,CIndexNode*>(type,root)));

    }
    else
    {
        cerr<<"wrong type: "<<type<<endl;
        exit(1);
    }

    int i=1;

    *height=(*height<=treeHeight)?*height:treeHeight;

    while(i<*height && taskList.size()>0)
    {
        int taskSize=taskList.size();

        for(int t=0;t<taskSize;t++)
        {
            CMVPInternalNode *n=taskList.back();
            taskList.pop_back();

            int childsize=n->getChildeSize();
            for(int j=0;j<childsize;j++)
            {
                long addressp=n->getSubTreeRootAddress(j);

                char *type=new char[6];

                infile.seekg(addressp,ios::beg);
                infile.read(type,sizeof(char)*6);
                string ttp(type);
                if(strcmp(type,"INODE")==0)
                {
                    CMVPInternalNode *inode=new CMVPInternalNode();
                    inode->readExternal(infile,dataType);
                    if(all)
                        cache->insert(std::pair<long,std::pair<char*,CIndexNode*> >(addressp,std::pair<char*,CIndexNode*>(type,inode)));
                    taskList.push_front(inode);
                }
                else if(strcmp(type,"LNODE")==0)
                {
                    CMVPLeafNode *lnode=new CMVPLeafNode();
                    lnode->readExternal(infile,dataType);
                    cache->insert(std::pair<long,std::pair<char*,CIndexNode*> >(addressp,std::pair<char*,CIndexNode*>(type,lnode)));
                }
                else
                {
                    cout<<"error node type"<<endl;
                    exit(1);
                }

            }

        }
        i++;

    }

    infile.close();

}

void cacheForestIndex(int* height,long* &rootAddress,int& numTree,int* treeHeights,char* indexfilename,string dataType,unordered_map<long,std::pair<char*,CIndexNode*> > *cache,bool all)
{

    ifstream infile(indexfilename,ios::binary);
	if(infile.fail()){
		cout<<"ERROR : can't find index file!"<<endl;
		exit(1);
	}
	
    numTree = 0;

    infile.seekg(-(long)sizeof(int),ios::end);
    infile.read((char*)(&numTree),sizeof(int));

    rootAddress = new long[numTree];
    treeHeights = new int[numTree];



    infile.seekg(-(long)(numTree*sizeof(long)+sizeof(int)+numTree*sizeof(int)),ios::end);
    infile.read((char*)rootAddress,numTree*sizeof(long));
    infile.read((char*)treeHeights,numTree*sizeof(int));


    //infile.seekg(-(long)(numTree*sizeof(long)+sizeof(int)),ios::end);
    //infile.read((char*)(&treeHeight),sizeof(int));
    //infile.read((char*)(&rootAddress),sizeof(long));

    //cout<<"filename:"<<indexfilename<<" rootaddress:"<<rootAddress<<" tree height:"<<treeHeight<<" cache height:"<<*height<<endl;

    for (int iNumTree =0;iNumTree < numTree; iNumTree++)
    {
        char *type=new char[6];
        infile.seekg(rootAddress[iNumTree],ios::beg);
        infile.read(type,6*sizeof(char));
        string ttp(type);

        CIndexNode *root;

        list<CMVPInternalNode*> taskList;

        if(strcmp(type,"INODE")==0)
        {
            root=new CMVPInternalNode();
            root->readExternal(infile,dataType);
            if(*height>0 && all)
                cache->insert(std::pair<long,std::pair<char*,CIndexNode*> >(rootAddress[iNumTree],std::pair<char*,CIndexNode*>(type,root)));
            taskList.push_back((CMVPInternalNode*)root);
        }
        else if(strcmp(type,"LNODE")==0)
        {
            root=new CMVPLeafNode();
            root->readExternal(infile,dataType);
            if(*height>0);
            cache->insert(std::pair<long,std::pair<char*,CIndexNode*> >(rootAddress[iNumTree],std::pair<char*,CIndexNode*>(type,root)));

        }
        else
        {
            cerr<<"wrong type: "<<type<<endl;
            exit(1);
        }

        int i=1;

        *height=(*height<=treeHeights[iNumTree])?*height:treeHeights[iNumTree];

        while(i<*height && taskList.size()>0)
        {
            int taskSize=taskList.size();

            for(int t=0;t<taskSize;t++)
            {
                CMVPInternalNode *n=taskList.back();
                taskList.pop_back();

                int childsize=n->getChildeSize();
                for(int j=0;j<childsize;j++)
                {
                    long addressp=n->getSubTreeRootAddress(j);

                    char *type=new char[6];

                    infile.seekg(addressp,ios::beg);
                    infile.read(type,sizeof(char)*6);
                    string ttp(type);
                    if(strcmp(type,"INODE")==0)
                    {
                        CMVPInternalNode *inode=new CMVPInternalNode();
                        inode->readExternal(infile,dataType);
                        if(all)
                            cache->insert(std::pair<long,std::pair<char*,CIndexNode*> >(addressp,std::pair<char*,CIndexNode*>(type,inode)));
                        taskList.push_front(inode);
                    }
                    else if(strcmp(type,"LNODE")==0)
                    {
                        CMVPLeafNode *lnode=new CMVPLeafNode();
                        lnode->readExternal(infile,dataType);
                        cache->insert(std::pair<long,std::pair<char*,CIndexNode*> >(addressp,std::pair<char*,CIndexNode*>(type,lnode)));
                    }
                    else
                    {
                        cout<<"error node type"<<endl;
                        exit(1);
                    }

                }

            }
            i++;

        }
    }



    infile.close();

}

void showtime(char *info)
{
    time_t t;
    struct tm *ptr;

    t=time(NULL);
    ptr=localtime(&t);
    cerr<<info<<" hour:"<<ptr->tm_hour<<" minute:"<<ptr->tm_min<<" sec:"<<ptr->tm_sec<<endl;
}

