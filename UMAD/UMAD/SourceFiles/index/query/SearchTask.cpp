#include "../../../HeaderFiles/index/query/SearchTask.h"
#include "../../../HeaderFiles/util/MemMonitor.h"


extern ThreadPoolManager tpm;
extern unordered_map<long,std::pair<char*,CIndexNode*> > indexCache;
extern mutex stderr_mutex;
extern void showPromptInformation(int line);

CSearchTask::CSearchTask(string indexName,string dataType,CMetricDistance &metric):metric(metric),indexName(indexName),dataType(dataType)
{

    /*searchList.clear();*/

}

CSearchTask::~CSearchTask(void){}

void subSearchTask(subsearchtask *pb, deque<long> *sub_list)
{
    deque<long> searchlist;

    list<shared_ptr<CMetricData> > subresult;

    unordered_map<long,std::pair<char*,CIndexNode*> >::iterator ite=indexCache.find(pb->address);

    if(ite!=indexCache.end())
    {

        if(strcmp(ite->second.first,"INODE")==0)
        {
            CMVPInternalNode *node=(CMVPInternalNode*)(ite->second.second);
            node->search(*(pb->rq),*(pb->metric),&searchlist,subresult);
        }
        else if(strcmp(ite->second.first,"LNODE")==0)
        {
            CMVPLeafNode *node=(CMVPLeafNode*)(ite->second.second);
            node->search(*(pb->rq),*(pb->metric),&searchlist,subresult);
        }
        else
        {
            cerr<<__FILE__<<__LINE__<<" wrong data type:"<<ite->second.first<<endl;
            exit(1);
        }
    }
    else
    {

        char type[6]={'\0'};

        pb->file_mutex->lock();

        pb->ifile->seekg(pb->address,ios::beg);	

        pb->ifile->read(type,sizeof(char)*6);

        pb->file_mutex->unlock();

        shared_ptr<CIndexNode> node;

        if(strcmp(type,"INODE")==0)
        {

            node=dynamic_pointer_cast<CMVPInternalNode>(node);

            node=shared_ptr<CMVPInternalNode>(new CMVPInternalNode());

        }
        else if(strcmp(type,"LNODE")==0)
        {

            node=dynamic_pointer_cast<CMVPLeafNode>(node);

            node=shared_ptr<CMVPLeafNode>(new CMVPLeafNode());

        }
        else
        {

            cerr<<__FILE__<<__LINE__<<" wrong node type! "<<pb->address<<endl;

            exit(0);

        }

        node->SMTSearchExternal(*(pb->rq),pb->address+6*sizeof(char),*(pb->ifile),*(pb->metric),&subresult,searchlist,*(pb->dataType),*(pb->file_mutex),*(pb->result_mutex));
    }



    pb->result_mutex->lock();

    pb->result->insert(pb->result->begin(),subresult.begin(),subresult.end());

    pb->result_mutex->unlock();

    sub_list->insert(sub_list->begin(),searchlist.begin(),searchlist.end());
}

void * searchSubTree(void *vpb)
{

    subsearchtask *pb=(subsearchtask*)vpb;



    list<shared_ptr<CMetricData> > subresult;

    deque<long> searchlist;



    unordered_map<long,std::pair<char*,CIndexNode*> >::iterator ite=indexCache.find(pb->address);

    if(ite!=indexCache.end())
    {

        if(strcmp(ite->second.first,"INODE")==0)
        {

            CMVPInternalNode *node=(CMVPInternalNode*)(ite->second.second);

            node->search(*(pb->rq),*(pb->metric),&searchlist,subresult);

        }
        else if(strcmp(ite->second.first,"LNODE")==0)
        {

            CMVPLeafNode *node=(CMVPLeafNode*)(ite->second.second);

            node->search(*(pb->rq),*(pb->metric),&searchlist,subresult);

        }
        else
        {

            cerr<<__FILE__<<__LINE__<<" wrong data type!"<<endl;

            exit(1);

        }

    }
    else
    {

        char type[6]={'\0'};

        pb->file_mutex->lock();

        pb->ifile->seekg(pb->address,ios::beg);	

        pb->ifile->read(type,sizeof(char)*6);

        pb->file_mutex->lock();



        shared_ptr<CIndexNode> node;

        if(strcmp(type,"INODE")==0)
        {

            node=dynamic_pointer_cast<CMVPInternalNode>(node);

            node=shared_ptr<CMVPInternalNode>(new CMVPInternalNode());

        }
        else if(strcmp(type,"LNODE")==0)
        {

            node=dynamic_pointer_cast<CMVPLeafNode>(node);

            node=shared_ptr<CMVPLeafNode>(new CMVPLeafNode());

        }
        else
        {

            cerr<<__FILE__<<__LINE__<<" wrong node type! "<<pb->address<<endl;

            exit(0);

        }



        node->SMTSearchExternal(*(pb->rq),pb->address+6*sizeof(char),*(pb->ifile),*(pb->metric),&subresult,searchlist,*(pb->dataType),*(pb->file_mutex),*(pb->result_mutex));

        //node->searchExternal(*(pb->rq),pb->address+6*sizeof(char),*(pb->ifile),*(pb->metric),subresult,searchlist,*(pb->dataType));



    }

    pb->result_mutex->lock();

    pb->result->insert(pb->result->begin(),subresult.begin(),subresult.end());

    pb->result_mutex->lock();



    while(searchlist.size()>0)
    {



        long address=searchlist.back();

        searchlist.pop_back();

        subsearchtask *task=new subsearchtask();

        task->address=address;

        task->rq=pb->rq;

        task->ifile=pb->ifile;

        task->result_mutex=pb->result_mutex;

        task->file_mutex=pb->file_mutex;

        task->metric=pb->metric;

        task->result=pb->result;

        task->dataType=pb->dataType;

        searchSubTree(task);



    }



}

void CSearchTask::searchWithCache(CRangeQuery &rq,long rootAddress)
{
    filePointer=0;

    ifstream infile(indexName.c_str(),ios::binary);

    std::unordered_map<long,std::pair<char*,CIndexNode*> >::iterator ite=indexCache.find(rootAddress);

    if(ite!=indexCache.end())
    {
        if(strcmp(ite->second.first,"INODE")==0)
        {

            CMVPInternalNode *inode=(CMVPInternalNode*)(ite->second.second);
            inode->search(rq,metric,&searchList,result);
        }
        else
        {
            CMVPLeafNode *lnode=(CMVPLeafNode*)(ite->second.second);
            lnode->search(rq,metric,&searchList,result);

        }
    }
    else
    {
        infile.seekg(rootAddress,ios::beg);

        char type[6]={'\0'};

        infile.read(type,6*sizeof(char));

        shared_ptr<CIndexNode> root=shared_ptr<CIndexNode>();

        if(strcmp(type,"INODE")==0)
        {
            root=dynamic_pointer_cast<CMVPInternalNode>(root);
            root.reset(new CMVPInternalNode());
        }
        else if(strcmp(type,"LNODE")==0)
        {
            root=dynamic_pointer_cast<CMVPLeafNode>(root);
            root.reset(new CMVPLeafNode());
        }
        else
        {
            cerr<<__FILE__<<__LINE__<<" wrong type! type:"<<type<<endl;
            exit(1);
        }

        rootAddress+=6*sizeof(char);

        root->searchExternal(rq,rootAddress,infile,metric,result,searchList,dataType);

    }

    while(searchList.size()>0)
    {

        long nodeAddress=searchList.back();

        searchList.pop_back();

        ite=indexCache.find(nodeAddress);

        if(ite!=indexCache.end())
        {
            if(strcmp(ite->second.first,"INODE")==0)
            {

                CMVPInternalNode *inode=(CMVPInternalNode*)(ite->second.second);
                inode->search(rq,metric,&searchList,result);

            }
            else
            {
                CMVPLeafNode *lnode=(CMVPLeafNode*)(ite->second.second);
                lnode->search(rq,metric,&searchList,result);
            }

        }
        else
        {
            searchEachNode(nodeAddress,rq,infile);

        }



    }

    infile.close();

}

void CSearchTask::searchForestWithCache(CRangeQuery &rq,long* rootAddress,int numTree)
{

    for (int iNumTree=0; iNumTree<numTree; iNumTree++)
    {

        filePointer=0;

        ifstream infile(indexName.c_str(),ios::binary);

        std::unordered_map<long,std::pair<char*,CIndexNode*> >::iterator ite=indexCache.find(rootAddress[iNumTree]);

        if(ite!=indexCache.end())
        {
            if(strcmp(ite->second.first,"INODE")==0)
            {

                CMVPInternalNode *inode=(CMVPInternalNode*)(ite->second.second);
                inode->search(rq,metric,&searchList,result);
            }
            else
            {
                CMVPLeafNode *lnode=(CMVPLeafNode*)(ite->second.second);
                lnode->search(rq,metric,&searchList,result);

            }
        }
        else
        {
            infile.seekg(rootAddress[iNumTree],ios::beg);

            char type[6]={'\0'};

            infile.read(type,6*sizeof(char));

            shared_ptr<CIndexNode> root=shared_ptr<CIndexNode>();

            if(strcmp(type,"INODE")==0)
            {
                root=dynamic_pointer_cast<CMVPInternalNode>(root);
                root.reset(new CMVPInternalNode());
            }
            else if(strcmp(type,"LNODE")==0)
            {
                root=dynamic_pointer_cast<CMVPLeafNode>(root);
                root.reset(new CMVPLeafNode());
            }
            else
            {
                cerr<<__FILE__<<__LINE__<<" wrong type! type:"<<type<<endl;
                exit(1);
            }

            rootAddress+=6*sizeof(char);

            root->searchExternal(rq,rootAddress[iNumTree],infile,metric,result,searchList,dataType);

        }

        while(searchList.size()>0)
        {

            long nodeAddress=searchList.back();

            searchList.pop_back();

            ite=indexCache.find(nodeAddress);

            if(ite!=indexCache.end())
            {
                if(strcmp(ite->second.first,"INODE")==0)
                {

                    CMVPInternalNode *inode=(CMVPInternalNode*)(ite->second.second);
                    inode->search(rq,metric,&searchList,result);

                }
                else
                {
                    CMVPLeafNode *lnode=(CMVPLeafNode*)(ite->second.second);
                    lnode->search(rq,metric,&searchList,result);
                }

            }
            else
            {
                searchEachNode(nodeAddress,rq,infile);

            }



        }

        infile.close();
    }

}

list<shared_ptr<CMetricData> >* CSearchTask::singleSearchWithMultipleThread(CRangeQuery &rq,long rootAddress)
{
    filePointer=0;
    ifstream infile(indexName.c_str(),ios::binary);
    std::unordered_map<long,std::pair<char*,CIndexNode*> >::iterator ite=indexCache.find(rootAddress);

    if(ite!=indexCache.end())
    {

        if(strcmp(ite->second.first,"INODE")==0)
        {
            CMVPInternalNode *inode=(CMVPInternalNode*)(ite->second.second);
            inode->search(rq,metric,&searchList,result);

        }
        else
        {
            CMVPLeafNode *lnode=(CMVPLeafNode*)(ite->second.second);
            lnode->search(rq,metric,&searchList,result);
        }

    }
    else
    {

        infile.seekg(rootAddress,ios::beg);

        char type[6]={'\0'};

        infile.read(type,6*sizeof(char));

        shared_ptr<CIndexNode> root=shared_ptr<CIndexNode>();

        if(strcmp(type,"INODE")==0)
        {
            root=dynamic_pointer_cast<CMVPInternalNode>(root);
            root.reset(new CMVPInternalNode());
        }
        else if(strcmp(type,"LNODE")==0)
        {
            root=dynamic_pointer_cast<CMVPLeafNode>(root);
            root.reset(new CMVPLeafNode());
        }
        else
        {
            cerr<<__FILE__<<__LINE__<<" wrong type! type:"<<type<<endl;
            exit(1);
        }

        rootAddress+=6*sizeof(char);

        root->searchExternal(rq,rootAddress,infile,metric,result,searchList,dataType);

    }

    mutex mutex_result;
    mutex mutex_file;
    std::thread *threadList=new thread[searchList.size()];
    int threadnum=searchList.size();
    vector<subsearchtask*> tasklist;
    for(int i=0;i<threadnum;i++)
    {
        subsearchtask *task=new subsearchtask();
        task->address=searchList.back();
        searchList.pop_back();
        task->rq=&rq;
        task->ifile=&infile;
        task->result_mutex=&mutex_result;
        task->file_mutex=&mutex_file;
        task->metric=&metric;
        task->result=&result;
        task->dataType=&dataType;
        tasklist.push_back(task);
    }
    for(int i=0;i<threadnum;i++)
    {
        try
        {
            threadList[i]=thread(searchSubTree,tasklist[i]);
        }
        catch(std::system_error &e)
        {
            cerr<<__FILE__<<__LINE__<<" Exception:"<<" arise at "<<e.code()<<" "<<e.what()<<" when i:"<<i<<endl;
            i--;
            continue;
        }
    }

    for(int i=0;i<threadnum;i++)
    {
        threadList[i].join();
        delete tasklist[i];
    }
    infile.close();
    return 0;
}

list<shared_ptr<CMetricData> >* CSearchTask::singleSearchWithThreadPool(CRangeQuery &rq,long rootAddress)
{
    filePointer=0;

    ifstream infile(indexName.c_str(),ios::binary);

    std::unordered_map<long,std::pair<char*,CIndexNode*> >::iterator ite=indexCache.find(rootAddress);

    if(ite!=indexCache.end())
    {


        if(strcmp(ite->second.first,"INODE")==0)
        {

            CMVPInternalNode *inode=(CMVPInternalNode*)(ite->second.second);
            inode->search(rq,metric,&searchList,result);

        }
        else
        {

            CMVPLeafNode *lnode=(CMVPLeafNode*)(ite->second.second);
            lnode->search(rq,metric,&searchList,result);
        }

    }
    else
    {
        infile.seekg(rootAddress,ios::beg);

        char type[6]={'\0'};

        infile.read(type,6*sizeof(char));

        shared_ptr<CIndexNode> root=shared_ptr<CIndexNode>();

        if(strcmp(type,"INODE")==0)
        {
            root=dynamic_pointer_cast<CMVPInternalNode>(root);
            root.reset(new CMVPInternalNode());
        }
        else if(strcmp(type,"LNODE")==0)
        {
            root=dynamic_pointer_cast<CMVPLeafNode>(root);
            root.reset(new CMVPLeafNode());
        }
        else
        {
            cerr<<__FILE__<<__LINE__<<" wrong type! type:"<<type<<endl;
            exit(1);
        }

        rootAddress+=6*sizeof(char);
        root->searchExternal(rq,rootAddress,infile,metric,result,searchList,dataType);

    }


    mutex mutex_result;
    mutex mutex_file;

    list<tp_task*> tasklist;
    int childnum=searchList.size();
	CountedMetric *bcm = dynamic_cast<CountedMetric*>(&metric);

    mutex route_mutex;
    condition_variable condition;

    int variableroutenumber=0;
    for(int i=0;i<childnum;i++)
    {
		CountedMetric *cm = new CountedMetric(bcm->getBaseMetric());

        subsearchtask *task=new subsearchtask();
        task->address=searchList.back();
        searchList.pop_back();
        task->rq=&rq;
        task->ifile=&infile;
        task->result_mutex=&mutex_result;
        task->file_mutex=&mutex_file;
        task->metric=cm;
        task->result=&result;
        task->dataType=&dataType;

        tp_task *tpt=new tp_task();
        tpt->ifinner=1;
        tpt->cond=&condition;
        tpt->task=(void*)task;
        tpt->func=searchSubTree;
        tpt->totalroute=childnum;
        tpt->variableroutenumber=&variableroutenumber;
        tpt->route_mutex=&route_mutex;
        tasklist.push_back(tpt);
    }


    tpm.addChildTask(&tasklist);

    {
        std::unique_lock<std::mutex> lk(route_mutex);
        while(variableroutenumber<childnum)
            condition.wait(lk);
    }

    for(int i=0;i<childnum;i++)
    {
        tp_task *ptt=tasklist.back();
		bcm->addCount((dynamic_cast<CountedMetric*>(((subsearchtask*)(ptt->task))->metric))->getCounter());
        tasklist.pop_back();
        delete (subsearchtask*)(ptt->task);
        delete ptt;
    }

    infile.close();
    return 0;
}

list<shared_ptr<CMetricData> >* CSearchTask::singleSearchWithCache(CRangeQuery &rq,long rootAddress)
{

    /**********************************************/

    //char *longtid=(char*)malloc(sizeof(long)*3);

    //sprintf(longtid,"%ld",pthread_self());

    //strcat(longtid,"cs33");

    //showtime(longtid);

    /**********************************************/

    //cerr<<"singleSearchWithCache"<<endl;

    filePointer=0;

    ifstream infile(indexName.c_str(),ios::binary);

    std::unordered_map<long,std::pair<char*,CIndexNode*> >::iterator ite=indexCache.find(rootAddress);

    if(ite!=indexCache.end())
    {

        //list<shared_ptr<CIndexObject> >tempers;
        if(strcmp(ite->second.first,"INODE")==0)
        {

            //cerr<<"root in cache"<<endl;

            CMVPInternalNode *inode=(CMVPInternalNode*)(ite->second.second);

            inode->search(rq,metric,&searchList,result);

            //tempers=inode->search(rq,metric,&searchList);

            //result.insert(result.begin(),tempers.begin(),tempers.end());

        }
        else
        {
            //cerr<<"leaf root in cache"<<endl;

            CMVPLeafNode *lnode=(CMVPLeafNode*)(ite->second.second);

            lnode->search(rq,metric,&searchList,result);

            //tempers=lnode->search(rq,metric,&searchList);

            //result.insert(result.begin(),tempers.begin(),tempers.end());

        }

    }
    else
    {

        //infile.seekg(-(long)sizeof(long),ios::end);

        //infile.read((char*)(&rootAddress),sizeof(long));

        ///**********************************************/

        //memset(longtid,'\0',3*sizeof(long));

        //sprintf(longtid,"%ld",pthread_self());

        //strcat(longtid,"###cs137");

        //showtime(longtid);

        ///**********************************************/

        infile.seekg(rootAddress,ios::beg);

        char type[6]={'\0'};

        infile.read(type,6*sizeof(char));

        ///**********************************************/

        //memset(longtid,'\0',3*sizeof(long));

        //sprintf(longtid,"%ld",pthread_self());

        //strcat(longtid,"cs137###");

        //showtime(longtid);

        ///**********************************************/

        shared_ptr<CIndexNode> root=shared_ptr<CIndexNode>();

        if(strcmp(type,"INODE")==0)
        {

            root=dynamic_pointer_cast<CMVPInternalNode>(root);

            root.reset(new CMVPInternalNode());



        }
        else if(strcmp(type,"LNODE")==0)
        {

            root=dynamic_pointer_cast<CMVPLeafNode>(root);

            root.reset(new CMVPLeafNode());



        }
        else
        {

            cerr<<__FILE__<<__LINE__<<" wrong type! type:"<<type<<endl;

            exit(1);

        }

        rootAddress+=6*sizeof(char);

        root->searchExternal(rq,rootAddress,infile,metric,result,searchList,dataType);

    }



    /**********************************************************************************************SEARCH IN NEW THREAD*********************************************************************/
    //	pthread_mutex_t mutex_result=PTHREAD_MUTEX_INITIALIZER;
    //	pthread_mutex_t mutex_file=PTHREAD_MUTEX_INITIALIZER;
    //    pthread_t *threadList=new pthread_t[searchList.size()];
    //	int threadnum=searchList.size();
    //	vector<subsearchtask*> tasklist;
    //    for(int i=0;i<threadnum;i++)
    //    {
    //	    subsearchtask *task=new subsearchtask();
    //	    task->address=searchList.back();
    //	    searchList.pop_back();
    //	    task->rq=&rq;
    //	    task->ifile=&infile;
    //	    task->result_mutex=&mutex_result;
    //	    task->file_mutex=&mutex_file;
    //		task->metric=&metric;
    //		task->result=&result;
    //		task->dataType=&dataType;
    //		tasklist.push_back(task);
    //    }
    //	for(int i=0;i<threadnum;i++)
    //	{
    //		if(pthread_create(&threadList[i],NULL,searchSubTree,(void*)tasklist[i])!=0)
    //		{
    //			cerr<<__FILE__<<__LINE__<<"create thread failed!"<<endl;
    //			exit(1);
    //		}
    //		//void *threadreturn=0;
    //		//pthread_join(threadList[i],&threadreturn);
    //		//cerr<<"child "<<i <<" completed successifully"<<endl;
    //	}
    //
    //	for(int i=0;i<threadnum;i++)
    //	{
    //		void *threadreturn=0;
    //		pthread_join(threadList[i],&threadreturn);
    //	}
    //
    //	for(int i=0;i<threadnum;i++)
    //	{
    //		delete tasklist[i];
    //	}
    //
    //	pthread_mutex_destroy(&mutex_result);
    //	pthread_mutex_destroy(&mutex_file);
    //
    //	infile.close();
    //	return 0;
    /**********************************************************************************************SEARCH IN NEW THREAD*********************************************************************/

    //ofstream ofs("../results/time.txt",ios::out|ios::app);



    while(searchList.size()>0)
    {

        long nodeAddress=searchList.back();

        searchList.pop_back();

        //struct timespec tv1,tv2;

        //memset(&tv1,0,sizeof(tv1));

        //clock_gettime(CLOCK_REALTIME,&tv1);

        ite=indexCache.find(nodeAddress);

        //memset(&tv2,0,sizeof(tv2));

        //clock_gettime(CLOCK_REALTIME,&tv2);

        //ofs<<"address:"<<nodeAddress<<" cache size:"<<indexCache.size()<<" time elapsed:"<<tv2.tv_sec-tv1.tv_sec<<"s "<<tv2.tv_nsec-tv1.tv_nsec<<"us"<<endl;

        if(ite!=indexCache.end())
        {

            //		/**********************************************/

            //		memset(longtid,'\0',3*sizeof(long));

            //		sprintf(longtid,"%ld",pthread_self());

            //		strcat(longtid,"cs128");

            //		showtime(longtid);

            //		/**********************************************/



            //list<shared_ptr<CIndexObject> >tempers;

            if(strcmp(ite->second.first,"INODE")==0)
            {



                //cerr<<"internal node incache"<<endl;

                CMVPInternalNode *inode=(CMVPInternalNode*)(ite->second.second);

                inode->search(rq,metric,&searchList,result);

                //tempers=inode->search(rq,metric,&searchList);

                //result.insert(result.begin(),tempers.begin(),tempers.end());

            }
            else
            {

                //cerr<<"leaf node in cache"<<endl;

                CMVPLeafNode *lnode=(CMVPLeafNode*)(ite->second.second);

                lnode->search(rq,metric,&searchList,result);

                //tempers=lnode->search(rq,metric,&searchList);

                //result.insert(result.begin(),tempers.begin(),tempers.end());



            }

            //			/**********************************************/

            //			memset(longtid,'\0',3*sizeof(long));

            //			sprintf(longtid,"%ld",pthread_self());

            //			strcat(longtid,"cs152");

            //			showtime(longtid);

            //			/**********************************************/



        }
        else
        {

            //				/**********************************************/

            //				memset(longtid,'\0',3*sizeof(long));

            //				sprintf(longtid,"%ld",pthread_self());

            //				strcat(longtid,"cs162");

            //				showtime(longtid);

            //				/**********************************************/



            //cerr<<"@@@@@@@@@@@@@@@@@@@node not in cache@@@@@@@@@@@@@@@@@@@"<<endl;

            searchEachNode(nodeAddress,rq,infile);

        }



    }



    //ofs<<"************************************************"<<endl;

    //ofs.close();

    infile.close();

    /**********************************************/

    //memset(longtid,'\0',3*sizeof(long));

    //sprintf(longtid,"%ld",pthread_self());

    //strcat(longtid,"cs137");

    //showtime(longtid);

    /**********************************************/



    //pthread_exit(NULL);

    return 0;

}

list<shared_ptr<CMetricData> >* CSearchTask:: search(CRangeQuery &rq)
{

    //showMemoryInfo4("testSearchTask","rootBegin");





    filePointer = 0;

    long rootAddress;



    ifstream in(indexName.c_str(),ios::binary);
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

    }
    else if(!strcmp(type,"LNODE"))
    {

        root = dynamic_pointer_cast<CMVPLeafNode> (root);

        root.reset( new CMVPLeafNode());

    }
    else
    {

        cout<<__FILE__<<__LINE__<<" not inode and lnode:"<<rootAddress<<endl;

        exit(0);

    }

    rootAddress += 6*sizeof(char);

    delete(type);





    root->searchExternal(rq,rootAddress,in,metric,result,searchList,dataType);



    //in.close();

    deque<long>::iterator nodeAddressToGet;

    long nodeAddress;



    while(searchList.size()>0)
    {

        //in.open(indexName,ios::binary);

        nodeAddressToGet=this->searchList.end();

        nodeAddressToGet--;

        nodeAddress= *nodeAddressToGet;

        searchList.pop_back();
        searchEachNode(nodeAddress,rq,in);

        //in.close();

        /*in.clear();*/

    }

    //CMemMonitor::maxMem;

    in.close();

    //CMemMonitor::updateMem();

    //showMemoryInfo4("testSearchTask","allEnd");

    return &result;

}

void CSearchTask::searchEachNode(long &nodeAddress,CRangeQuery &q,ifstream &in)
{

    //showMemoryInfo4("testSearchTask","eachNodeBegin");

    in.seekg(nodeAddress);

    shared_ptr< CIndexNode> node;

    char* type = new char[6];

    in.read(type,6*sizeof(char));

    if(!strcmp(type,"INODE"))
    {
        node = dynamic_pointer_cast<CMVPInternalNode> (node);
        node.reset(new CMVPInternalNode());
    }

    else if(!strcmp(type,"LNODE"))
    {
        node = dynamic_pointer_cast<CMVPLeafNode> (node);

        node.reset( new CMVPLeafNode());
    }
    else
    {

        cout<<__FILE__<<__LINE__<<" not inode and lnode:"<<nodeAddress<<endl;

        exit(0);
    }
    delete(type);

    nodeAddress+=6*sizeof(char);

    node->searchExternal(q,nodeAddress,in,metric,result,searchList,dataType);

    /*node.reset();*/
    //showMemoryInfo4("testSearchTask","eachNodeEnd");
}

/****************************************************************************************************************************************************************************/
struct c_searchTask
{

    long nodeaddress;

    mutex *f_mutex;

    mutex *r_mutex;


    string *datatype;

    CMetricDistance *metric;


    CRangeQuery *rq;

    list<shared_ptr<CMetricData> >*result;

    ifstream *infile;


    deque<long> searchlist;

};


void* searchTask(void *pb)
{

    c_searchTask *vpb=(c_searchTask*)pb;


    //may exist collision here, all thread need the cache to return a cache position

    std::unordered_map<long,std::pair<char*,CIndexNode*> >::const_iterator cite=indexCache.find(vpb->nodeaddress);

    if(cite!=indexCache.end())
    {

        list<shared_ptr<CMetricData> > temprs;

        if(strcmp(cite->second.first,"INODE")==0)
        {

            CMVPInternalNode *inode=(CMVPInternalNode*)(cite->second.second);

            temprs=inode->search(*(vpb->rq),*(vpb->metric),&(vpb->searchlist));

            vpb->r_mutex->lock();

            vpb->result->insert(vpb->result->begin(),temprs.begin(),temprs.end());

            vpb->r_mutex->lock();

        }
        else
        {

            CMVPLeafNode *lnode=(CMVPLeafNode*)(cite->second.second);

            temprs=lnode->search(*(vpb->rq),*(vpb->metric),&(vpb->searchlist));

            vpb->r_mutex->lock();

            vpb->result->insert(vpb->result->begin(),temprs.begin(),temprs.end());

            vpb->r_mutex->lock();

        }		

    }
    else
    {
        char type[6]={'\0'};

        vpb->f_mutex->lock();

        vpb->infile->seekg(vpb->nodeaddress,ios::beg);

        vpb->infile->read(type,sizeof(char)*6);

        vpb->f_mutex->lock();

        if(strcmp(type,"INODE")==0)
        {
            CMVPInternalNode *inode=new CMVPInternalNode;

            inode->SMTSearchExternal(*(vpb->rq),vpb->nodeaddress+6*sizeof(char),*(vpb->infile),*(vpb->metric),vpb->result,vpb->searchlist,*(vpb->datatype),*(vpb->f_mutex),*(vpb->r_mutex));

        }
        else if(strcmp(type,"LNODE")==0)
        {
            CMVPLeafNode *lnode=new CMVPLeafNode;

            lnode->SMTSearchExternal(*(vpb->rq),vpb->nodeaddress+6*sizeof(char),*(vpb->infile),*(vpb->metric),vpb->result,vpb->searchlist,*(vpb->datatype),*(vpb->f_mutex),*(vpb->r_mutex));
        }



    }


    return 0;

}



void CSearchTask::MultiThreadSearchWithCache(CRangeQuery &rq)
{

    mutex f_mutex;

    mutex r_mutex;



    list<c_searchTask*> taskList;



    long rootAddress=0;

    ifstream infile(indexName.c_str(),ios::binary);

    infile.seekg(-(long)(sizeof(long)),ios::end);

    infile.read((char*)&rootAddress,sizeof(long));



    c_searchTask *task=new c_searchTask;

    task->nodeaddress=rootAddress;

    task->r_mutex=&r_mutex;

    task->f_mutex=&f_mutex;

    task->datatype=&dataType;

    task->metric=&metric;

    task->rq=&rq;

    task->result=&result;

    task->infile=&infile;



    taskList.push_back(task);



    while(taskList.size()>0)
    {

        int taskNum=taskList.size();

        thread *threadList=new thread[taskNum];

        list<c_searchTask*> newTaskList;

        int i=0;

        while(i<taskNum)
        {

            c_searchTask *tempTask=taskList.back();

            threadList[i]=thread(searchTask,tempTask);

            newTaskList.push_back(tempTask);

            taskList.pop_back();

            i++;

        }


        for(int i=0;i<taskNum;i++)
        {

            threadList[i].join();

        }	

        for(int i=0;i<taskNum;i++)
        {

            c_searchTask *tempTask=newTaskList.back();

            int searchListSize=tempTask->searchlist.size();

            for(int j=0;j<searchListSize;j++)
            {

                c_searchTask *temp=new c_searchTask;

                temp->nodeaddress=tempTask->searchlist.back();

                temp->r_mutex=&r_mutex;temp->rq=&rq;temp->f_mutex=&f_mutex;temp->datatype=&dataType;temp->metric=&metric;temp->result=&result;temp->infile=&infile;

                taskList.push_back(temp);

                tempTask->searchlist.pop_back();

            }

            delete tempTask;

            newTaskList.pop_back();

        }

    }



}

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
struct pTask
{

    long fileAddress;

    shared_ptr<CMVPInternalNode> inode;

    shared_ptr<CMVPLeafNode> lnode;

    deque<long> searchlist;

    //~pTask(){cout<<fileAddress<<" task destroyed"<<endl;}

};


struct newParallelBlock
{

    shared_ptr<pTask> task;

    string *datatype;

    CMetricDistance *metric;

    mutex *f_mutex;

    mutex *r_mutex;

    CRangeQuery *rq;

    list<shared_ptr<CMetricData> >*result;

    ifstream *infile;

};

void* NMSTSearchTask(void *vpb)
{

    newParallelBlock *pb=(newParallelBlock*)vpb;

    if(pb->task->inode!=0)
    {

        pb->task->inode->SMTSearchExternal(*(pb->rq),pb->task->fileAddress,*(pb->infile),*(pb->metric),pb->result,pb->task->searchlist,*(pb->datatype),*(pb->f_mutex),*(pb->r_mutex));

    }
    else if(pb->task->lnode!=0) 
    {

        pb->task->lnode->SMTSearchExternal(*(pb->rq),pb->task->fileAddress,*(pb->infile),*(pb->metric),pb->result,pb->task->searchlist,*(pb->datatype),*(pb->f_mutex),*(pb->r_mutex));

    }



    return 0;



}

void CSearchTask::MultithreadSearch(CRangeQuery &rq)
{

    filePointer = 0;

    long rootAddress=0;

    mutex f_mutex;

    mutex r_mutex;


    ifstream infile(indexName.c_str(),ios::in|ios::binary);

    infile.seekg(-(long)sizeof(long),ios::end);



    infile.read((char*)(&rootAddress),sizeof(long));



    char type[6];



    infile.seekg(rootAddress,ios::beg);



    infile.read((char*)type,sizeof(char)*6);



    deque<newParallelBlock*> taskList;



    shared_ptr<CIndexNode> root;



    shared_ptr<pTask> task(new pTask);





    if(strcmp(type,"INODE")==0)
    {

        shared_ptr<CMVPInternalNode> newNode=dynamic_pointer_cast<CMVPInternalNode>(root);

        newNode.reset(new CMVPInternalNode());

        task->inode=newNode;

        task->lnode=shared_ptr<CMVPLeafNode>();

    }
    else if(strcmp(type,"LNODE")==0)
    {

        shared_ptr<CMVPLeafNode> newNode=dynamic_pointer_cast<CMVPLeafNode>(root);

        newNode.reset(new CMVPLeafNode);

        task->lnode=newNode;

        task->inode=shared_ptr<CMVPInternalNode>();

    }
    else 
    {

        cout<<__FILE__<<__LINE__<<"read wrong type from file"<<endl;

        exit(1);

    }

    filePointer=rootAddress+sizeof(char)*6;



    newParallelBlock *npb=new newParallelBlock;

    npb->task=task;

    task->fileAddress=filePointer;

    npb->metric=&metric;

    npb->rq=&rq;

    npb->infile=&infile;

    npb->result=&result;

    npb->datatype=&dataType;

    npb->f_mutex=&f_mutex;

    npb->r_mutex=&r_mutex;



    taskList.push_back(npb);





    while(taskList.size()>0)
    {

        shared_ptr<pTask> temptask1;


        newParallelBlock *tempNPB=NULL;

        list<newParallelBlock*> newtasklist;



        thread *threadlistarray=new thread[taskList.size()];

        int arrayNum=taskList.size();



        for(int i=0;i<arrayNum;i++)
        {

            tempNPB=taskList.back();

            threadlistarray[i]=thread(NMSTSearchTask,tempNPB);

            taskList.pop_back();
            newtasklist.push_back(tempNPB);

        }


        for(int i=0;i<arrayNum;i++)
        {

            threadlistarray[i].join();

        }



        delete [] threadlistarray;



        int newtasklistsize=newtasklist.size();

        for(int i=0;i<newtasklistsize;i++)
        {

            tempNPB=newtasklist.back();

            newtasklist.pop_back();



            int searchListsize=tempNPB->task->searchlist.size();

            for(int i=0;i<searchListsize;i++)
            {

                int longNodeAddreass=tempNPB->task->searchlist.back();





                f_mutex.lock();

                infile.seekg(longNodeAddreass,ios::beg);

                infile.read(type,6*sizeof(char));

                f_mutex.lock();



                shared_ptr<CMVPInternalNode> node;

                shared_ptr<pTask> newTask(new pTask);

                newParallelBlock *tempnpb=new newParallelBlock;

                if(strcmp(type,"INODE")==0)
                {

                    shared_ptr<CMVPInternalNode> internode=dynamic_pointer_cast<CMVPInternalNode>(node);

                    internode.reset(new CMVPInternalNode());

                    newTask->inode=internode;

                    newTask->lnode=shared_ptr<CMVPLeafNode>();



                }
                else if(strcmp(type,"LNODE")==0)
                {

                    shared_ptr<CMVPLeafNode> leafnode=dynamic_pointer_cast<CMVPLeafNode>(node);

                    leafnode.reset(new CMVPLeafNode());

                    newTask->lnode=leafnode;

                    newTask->inode=shared_ptr<CMVPInternalNode>();

                }
                else
                {

                    cout<<__FILE__<<__LINE__<<"read wrong type from file"<<endl;

                    exit(1);

                }



                newTask->fileAddress=longNodeAddreass+sizeof(char)*6;

                tempnpb->task=newTask;

                tempnpb->metric=&metric;

                tempnpb->rq=&rq;

                tempnpb->infile=&infile;

                tempnpb->result=&result;

                tempnpb->f_mutex=&f_mutex;

                tempnpb->r_mutex=&r_mutex;

                tempnpb->datatype=&dataType;





                taskList.push_back(tempnpb);



                tempNPB->task->searchlist.pop_back();



            }



            delete tempNPB;

        }

    }


}

/****************************************************************************************************************************************************************************/
/*******************************************************************************deprecated multithread function block*********************************************************************************************/

struct parallelBlock
{

    parallelBlock(mutex &s_mutex, mutex &f_mutex,mutex &r_mutex,mutex &t_mutex):searchList_mutex(s_mutex),fstream_mutex(f_mutex),result_mutex(r_mutex),thread_num_mutex(t_mutex){}

    vector<shared_ptr<pTask> > *taskList;

    CMetricDistance *metric;

    CRangeQuery *rq;

    ifstream *in;

    list<shared_ptr<CMetricData> > *result;

    string *datatype;

    mutex &searchList_mutex;

    mutex &fstream_mutex;

    mutex &result_mutex;

    mutex &thread_num_mutex;

};
#ifdef LOG_FILE
void MSTSearchTaskFile(parallelBlock *pb,int &threadNum,fstream &outfile)
#else
void MSTSearchTask(parallelBlock *pb,int &threadNum)
#endif
{

#ifdef COUT_INFOMATION

    //cout<<__FILE__<<__LINE__<<endl;

    cout<<this_thread::get_id()<<"start"<<endl;

    //cout<<"pass"<<endl;

#endif



#ifdef LOG_FILE

    outfile<<__LINE__<<" "<<"***************MSTSearchTask begin"<<endl;

#endif

    pb->searchList_mutex.lock();

    shared_ptr<pTask> t;

    if(pb->taskList->size()>0)
    {



#ifdef LOG_FILE

        outfile<<__LINE__<<" SMTSearch"<<endl;

#endif

        vector<shared_ptr<pTask> >::iterator ite=pb->taskList->end();

        ite--;

        t=*ite;

        pb->taskList->erase(ite);



#ifdef LOG_FILE

        outfile<<__LINE__<<" SMTSearch"<<endl;

#endif

    }

    pb->searchList_mutex.unlock();



#ifdef COUT_INFOMATION

    cout<<this_thread::get_id()<<"mutex"<<endl;

#endif





    deque<long> searchList;

    //cout<<__LINE__<<"t.usecount:"<<t.use_count()<<endl;

    if(t!=NULL)
    {

        //cout<<__LINE__<<"t.usecount:"<<t.use_count()<<endl;

        if(t->inode!=0)
        {

#ifdef COUT_INFOMATION

            cout<<this_thread::get_id()<<"start search internal"<<" fileaddress:"<<t->fileAddress<<endl;

#endif



#ifdef LOG_FILE

            outfile<<__LINE__<<" SMTSearch"<<endl;

#endif

            t->inode->SMTSearchExternal(*(pb->rq),t->fileAddress,*(pb->in),*(pb->metric),pb->result,searchList,*(pb->datatype),pb->fstream_mutex,pb->result_mutex);



#ifdef LOG_FILE

            outfile<<__LINE__<<" SMTSearch"<<endl;

#endif

#ifdef COUT_INFOMATION

            cout<<this_thread::get_id()<<"end search internal"<<endl;

#endif



            //cout<<"search list:"<<searchList.size()<<endl;

            while(searchList.size()>0)
            {

                long filePointer=0;



                deque<long>::iterator ite=searchList.end();

                ite--;



                filePointer=*ite;



                searchList.pop_back();



                char type[6]={0};

                pb->fstream_mutex.lock();

                pb->in->seekg(filePointer,ios::beg);

                pb->in->read(type,6*sizeof(char));

                pb->fstream_mutex.unlock();



                shared_ptr<pTask> temp(new pTask);



                shared_ptr<CIndexNode> node;





                if(!strcmp(type,"INODE"))
                {

                    shared_ptr<CMVPInternalNode> newNode = dynamic_pointer_cast<CMVPInternalNode>(node);

                    newNode.reset(new CMVPInternalNode());

                    //node=dynamic_pointer_cast<CMVPInternalNode>(node);

                    //node.reset(new CMVPInternalNode());

                    // temp->inode=const_pointer_cast<CMVPInternalNode>(node);

                    temp->inode=newNode;

                    temp->lnode=shared_ptr<CMVPLeafNode>();

                }
                else if(!strcmp(type,"LNODE"))
                {

                    shared_ptr<CMVPLeafNode> newNode = dynamic_pointer_cast<CMVPLeafNode>(node);

                    newNode.reset(new CMVPLeafNode());

                    // node=dynamic_pointer_cast<CMVPLeafNode>(node);

                    // node.reset(new CMVPLeafNode());

                    temp->lnode=newNode;

                    temp->inode=shared_ptr<CMVPInternalNode>();

                }



                filePointer+=6*sizeof(char);



                temp->fileAddress=filePointer;

#ifdef COUT_INFOMATION

                cout<<this_thread::get_id()<<"start push"<<endl;

#endif



                pb->searchList_mutex.lock();

                pb->taskList->push_back(temp);

                pb->searchList_mutex.unlock();



#ifdef LOG_FILE

                outfile<<__LINE__<<" SMTSearch"<<endl;

#endif

#ifdef COUT_INFOMATION

                cout<<this_thread::get_id()<<"end push"<<endl;

#endif

            }



        }
        else
        {

#ifdef COUT_INFOMATION

            cout<<this_thread::get_id()<<"start search leaf node"<<endl;

#endif



#ifdef LOG_FILE

            outfile<<__LINE__<<" SMTSearch"<<endl;

#endif

            t->lnode->SMTSearchExternal(*(pb->rq),t->fileAddress,*(pb->in),*(pb->metric),pb->result,searchList,*(pb->datatype),pb->fstream_mutex,pb->result_mutex);



#ifdef LOG_FILE

            outfile<<__LINE__<<" SMTSearch"<<endl;

#endif

#ifdef COUT_INFOMATION

            cout<<this_thread::get_id()<<"end search leaf node "<<endl;

#endif

        }

    }





    threadNum--;



#ifdef COUT_INFOMATION

    //cout<<__FILE__<<__LINE__<<endl;

    cout<<this_thread::get_id()<<"over"<<endl;

    //cout<<"pass"<<endl;

#endif



#ifdef LOG_FILE

    outfile<<__LINE__<<"***************MSTSearchTask end"<<endl;

#endif

    //cout<<__LINE__<<"t.usecount:"<<t.use_count()<<endl;

    return;

}



void CSearchTask::SMTSearch(CRangeQuery &rq)
{



#ifdef LOG_FILE

    char f_n[100]=".\\log\\";

    std::thread::id perQueryObjectThread=this_thread::get_id();

    stringstream temp;

    string id_str;

    temp<<perQueryObjectThread;

    id_str=temp.str();

    strcat(f_n,id_str.c_str());

    strcat(f_n,".log");



    fstream outfile(f_n,ios::out|ios::app);

    outfile<<__LINE__<<" "<<perQueryObjectThread<<" ************************************SMTSearch begin"<<endl;

#endif



    int threadNum=0;

    mutex searchList_mutex;

    mutex fstream_mutex;

    mutex result_list_mutex;

    mutex thread_num_mutex;



#ifdef COUT_INFOMATION

    cout<<__FILE__<<__LINE__<<"                      radius:"<<rq.getRadius()<<" start"<<endl;

    cout<<this_thread::get_id()<<endl;

    cout<<"pass"<<endl;

#endif



    filePointer = 0;

    long rootAddress=0;



#ifdef COUT_INFOMATION

    cout<<__FILE__<<__LINE__<<endl;

    cout<<this_thread::get_id()<<endl;

    cout<<"pass"<<endl;

#endif



#ifdef LOG_FILE

    outfile<<__LINE__<<" "<<perQueryObjectThread<<endl;

#endif



    ifstream in(indexName,ios::binary);

    in.seekg(-(long)sizeof(long),ios::end);



    in.read((char*)(&rootAddress),sizeof(long));



    in.seekg(rootAddress,ios::beg);



    shared_ptr<CIndexNode> root;



    char type[6]={0};

    in.read(type,6*sizeof(char));



    vector<shared_ptr<pTask> > taskList;

    shared_ptr<pTask> tmp(new pTask);



#ifdef COUT_INFOMATION

    cout<<__FILE__<<__LINE__<<endl;

    cout<<this_thread::get_id()<<endl;

    cout<<"pass"<<endl;

#endif

#ifdef LOG_FILE

    outfile<<__LINE__<<" "<<perQueryObjectThread<<endl;

#endif



    if(!strcmp(type,"INODE"))
    {

        shared_ptr<CMVPInternalNode> newRoot = dynamic_pointer_cast<CMVPInternalNode>(root);

        newRoot.reset(new CMVPInternalNode());



        // root=dynamic_pointer_cast<CMVPInternalNode>(root);

        // root.reset(new CMVPInternalNode());



        // tmp->inode=shared_ptr<CMVPInternalNode>(newRoot.get());



        tmp->inode=newRoot;

        //tmp->inode=const_pointer_cast<CMVPInternalNode>(root);

        tmp->lnode=shared_ptr<CMVPLeafNode>();

    }
    else if(!strcmp(type,"LNODE"))
    {

        shared_ptr<CMVPLeafNode> newRoot = dynamic_pointer_cast<CMVPLeafNode>(root);

        newRoot.reset(new CMVPLeafNode());

        // root=dynamic_pointer_cast<CMVPLeafNode>(root);

        // root.reset(new CMVPLeafNode());

        tmp->lnode=newRoot;

        tmp->inode=shared_ptr<CMVPInternalNode>();

    }



#ifdef LOG_FILE

    outfile<<__LINE__<<" "<<perQueryObjectThread<<endl;

#endif



    rootAddress+=6*sizeof(char);



    tmp->fileAddress=rootAddress;



    taskList.push_back(tmp);



#ifdef LOG_FILE

    outfile<<__LINE__<<" "<<perQueryObjectThread<<endl;

#endif



    parallelBlock pb(searchList_mutex,fstream_mutex,result_list_mutex,thread_num_mutex);
    {

        pb.datatype=&dataType;

        pb.in=&in;

        pb.metric=&metric;

        pb.result=&result;

        pb.rq=&rq;

        pb.taskList=&taskList;

    }

#ifdef COUT_INFOMATION

    cout<<__FILE__<<__LINE__<<endl;

    cout<<this_thread::get_id()<<" "<<threadNum<<endl;

    cout<<"pass"<<endl;

#endif



#ifdef LOG_FILE

    outfile<<__LINE__<<" "<<perQueryObjectThread<<endl;

#endif



    while(taskList.size()>0||threadNum>0)
    {    

        //if(taskList.size()>0||threadNum<4)

        if(taskList.size()>0)

        {



            threadNum++;



#ifdef LOG_FILE

            std::thread childThread(MSTSearchTaskFile,&pb,std::ref(threadNum),std::ref(outfile));

#else

            std::thread childThread(MSTSearchTask,&pb,std::ref(threadNum));

#endif

            childThread.detach();

        }
        else
        {

#ifdef _WIN32

            Sleep(1000);

#else

            //sleep(1);

#endif

        }

    }

#ifdef COUT_INFOMATION

    cout<<__FILE__<<__LINE__<<"                      radius:"<<rq.getRadius()<<" end"<<endl;

    cout<<this_thread::get_id()<<endl;

    cout<<"pass"<<endl;

#endif



#ifdef LOG_FILE

    outfile<<__LINE__<<" "<<perQueryObjectThread<<endl;

#endif

    while(taskList.size()>0||threadNum>0)
    {

#ifdef _WIN32

        Sleep(1000);

#else

        //sleep(1);

#endif

    }





#ifdef LOG_FILE

    outfile<<__LINE__<<" "<<perQueryObjectThread<<" ************************************SMTSearch end"<<endl;

#endif

    //return &result;

}

/********************************************************************************deprecated multithread function block********************************************************************************************/

