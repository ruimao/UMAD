#include "../../HeaderFiles/util/ThreadPoolManager.h"
#include "../../HeaderFiles/index/query/SearchTask.h"

#ifdef __GNUC__
#include <sys/timeb.h>
#include "/usr/include/sys/times.h"
#else
#include <Windows.h>
#endif

mutex stderr_mutex;

extern void subSearchTask(subsearchtask *pb,deque<long> *sub_list);


ThreadPoolManager::ThreadPoolManager()
{
	maxthreadnumber=32;
	maxthreadnumberforchildren=9;

	//innerthreadlist=(thread *)malloc(maxthreadnumberforchildren*sizeof(thread));
	//completethreadlist=(thread *)malloc((maxthreadnumber-maxthreadnumberforchildren)*sizeof(thread));

	shutdownflag=0;

}

ThreadPoolManager::ThreadPoolManager(int maxthreadnum,int maxthreadnumforinnerquery)
{
	maxthreadnumber=maxthreadnum;
	maxthreadnumberforchildren=maxthreadnumforinnerquery;

	//innerthreadlist=(thread *)malloc(maxthreadnumberforchildren*sizeof(thread));
	//completethreadlist=(thread *)malloc((maxthreadnumber-maxthreadnumberforchildren)*sizeof(thread));

	shutdownflag=0;
}

void innerroutine(ThreadPoolManager *tp)
{
	//ThreadPoolManager *tp=(ThreadPoolManager*)p;
	while(1)
	{			    
		if(tp->shutdownflag)
	    {
	    	break;
	    }
	
	    struct tp_task 	*tpt=NULL;
		tp->innertasklistmutex.lock();
		int tempcounter=tp->innersearchtasklist.size();
		if(tempcounter>0)
		{
			tpt=tp->innersearchtasklist.back();
			tp->innersearchtasklist.pop_back();
		}
		tp->innertasklistmutex.unlock();
		

		if(tpt==NULL)
		{
			{
				std::unique_lock<std::mutex> lk(tp->innertasklistmutex);	
				while(tempcounter<=0)
				{	
					tp->inner_ready_condition.wait(lk);
					if(tp->shutdownflag)
						break;
					tempcounter=tp->innersearchtasklist.size();
				}
			}

			continue;
		}
		else
		{
			subsearchtask *pb=(subsearchtask*)(tpt->task);
			deque<long> searchlist;
			subSearchTask(pb,&searchlist);

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
	      		subSearchTask(task,&searchlist);       
	      		delete task;
			}	
			
			tpt->route_mutex->lock();
		    *(tpt->variableroutenumber)+=1;
		    int temp=*(tpt->variableroutenumber);
			tpt->route_mutex->unlock();


		    if(temp>=tpt->totalroute)
		    {
				tpt->cond->notify_one();
			}		
		}
	}
}

void completeroutine(ThreadPoolManager *tp)
{
	//ThreadPoolManager *tp=(ThreadPoolManager*)p;
	while(1)
	{			    
		if(tp->shutdownflag)
	    {
	      	break;
	    }
	
	    struct tp_task 	*tpt=NULL;		
		
		
		tp->completetasklistmutex.lock();
	   	int tempcounter=tp->completesearchtasklist.size();
		if(tempcounter>0)
	    {
	    	tpt=tp->completesearchtasklist.back();
	    	tp->completesearchtasklist.pop_back();
	    }
		tp->completetasklistmutex.unlock();
		
		
		  if(tpt==NULL)
		  {
		 
		      {
				  std::unique_lock<std::mutex> lk(tp->completetasklistmutex);	
				  while(tempcounter<=0)
				  {	
					  tp->complete_ready_condition.wait(lk);
					  if(tp->shutdownflag)
						  break;
					  tempcounter=tp->completesearchtasklist.size();
				  }
			  }
      
		  	    continue;
		  }
		  else
		  {
#ifdef _WIN32
	FILETIME kernelStart,kernelEnd;
	FILETIME userStart,userEnd;
	FILETIME dummy;
#endif
	      		singlequerytask *sqt=(singlequerytask*)(tpt->task);
#ifdef _WIN32

		GetThreadTimes(GetCurrentThread(),&dummy,&dummy,&kernelStart,&userStart);

#else


		clock_t begt=times(NULL);


#endif 
				sqt->searchTask->singleSearchWithThreadPool(*(sqt->rq),sqt->rootaddress);
				
#ifdef _WIN32

		GetThreadTimes(GetCurrentThread(),&dummy,&dummy,&kernelEnd,&userEnd);
		sqt->searchtime=(userEnd.dwLowDateTime+kernelEnd.dwLowDateTime-userStart.dwLowDateTime-kernelEnd.dwLowDateTime)/10000000.00;

#else


		clock_t endt=times(NULL);

		sqt->searchtime=(endt-begt)/100.0;

#endif
		
				sqt->distancecount=sqt->cmetric->getCounter();
				sqt->resultnumber=sqt->searchTask->getResultSize();

				//gai wei zai searchmode(4) de han shu li mian she fang le, yin wei xu yao jin yi bu verify.
				//sqt->searchTask.reset();
				//sqt->rq.reset();
			
				tpt->route_mutex->lock();
				*(tpt->variableroutenumber)+=1;
				int temp=*(tpt->variableroutenumber);
				tpt->route_mutex->unlock();


				if(temp>=tpt->totalroute)
				{
					tpt->cond->notify_one();
				}		
		}
	}

}

void ThreadPoolManager::run()
{
	for(int i=0;i<maxthreadnumberforchildren;i++)
	{
		try
		{
			innerthreadlist.push_back(thread(innerroutine,this));
		}
		catch(std::system_error &e)
		{
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
	for(int i=0;i<(maxthreadnumber-maxthreadnumberforchildren);i++)
	{
		try
		{
			completethreadlist.push_back(thread(completeroutine,this));
		}
		catch(std::system_error &e)
		{
			cerr<<__FILE__<<__LINE__<<" Exception:"<<e.what()<<" arise at "<<e.code()<<" when i:"<<i<<endl;
			i--;
			continue;
		}
		catch(std::bad_alloc &e)
		{
			cerr<<__LINE__<<" Exception:"<<e.what()<<" arise at i:"<<i<<endl;
			i--;
			continue;
		}
		catch(exception &e)
		{
			cerr<<__LINE__<<" Exception:"<<e.what()<<" arise at i:"<<i<<endl;
			i--;
			continue;
		}

	}
}

void ThreadPoolManager::stop()
{
	shutdownflag=1;
	
	inner_ready_condition.notify_all();
	

	for(int i=0;i<maxthreadnumberforchildren;i++)
	{
		innerthreadlist[i].join();
	}
	
	complete_ready_condition.notify_all();
	for(int i=0;i<(maxthreadnumber-maxthreadnumberforchildren);i++)
	{
		completethreadlist[i].join();
	}
	
	innerthreadlist.clear();
	completethreadlist.clear();
	//free(innerthreadlist);
	//free(completethreadlist);
}

void ThreadPoolManager::addChildTask(tp_task *task)
{
	innertasklistmutex.lock();
	innersearchtasklist.push_back(task);
	innertasklistmutex.unlock();
	inner_ready_condition.notify_one();
}

void ThreadPoolManager::addChildTask(list<tp_task*> *cont)
{
	innertasklistmutex.lock();
	innersearchtasklist.insert(innersearchtasklist.begin(),cont->begin(),cont->end());
	innertasklistmutex.unlock();
	inner_ready_condition.notify_all();
	
}

void ThreadPoolManager::addCompleteTask(tp_task *task)
{
	completetasklistmutex.lock();
	completesearchtasklist.push_back(task);
	completetasklistmutex.unlock();
	complete_ready_condition.notify_one();
}

void ThreadPoolManager::addCompleteTask(list<tp_task*> *cont)
{
	completetasklistmutex.lock();
	completesearchtasklist.insert(completesearchtasklist.begin(),cont->begin(),cont->end());
	completetasklistmutex.unlock();
	complete_ready_condition.notify_all();

}

void ThreadPoolManager::setMaxThreadNumber(int num)
{
	maxthreadnumber=num;
}

void ThreadPoolManager::setMaxThreadNumberForChildren(int num)
{
	maxthreadnumberforchildren=num;

	//free(innerthreadlist);
	//free(completethreadlist);
	//innerthreadlist=(thread *)malloc(maxthreadnumberforchildren*sizeof(thread));
	//completethreadlist=(thread *)malloc((maxthreadnumber-maxthreadnumberforchildren)*sizeof(thread));	
}
