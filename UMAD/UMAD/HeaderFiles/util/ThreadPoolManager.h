#ifndef THREADPOOLMANAGER_H
#define THREADPOOLMANAGER_H
#include<thread>
#include<list>
#include<deque>
#include "TaskDeclaration.h"

class ThreadPoolManager;
void innerroutine(ThreadPoolManager *p);
void completeroutine(ThreadPoolManager*p);

using std::list;
using std::deque;

class ThreadPoolManager
{
public:
	ThreadPoolManager();
	ThreadPoolManager(int maxthreadnumber,int maxthreadnumberforinnerquery);

	void setMaxThreadNumber(int num);
	void setMaxThreadNumberForChildren(int num);

	void run();
	void stop();

	void addCompleteTask(tp_task *task);
	void addCompleteTask(list<tp_task*> *cont);

	void addChildTask(tp_task *task);
	void addChildTask(list<tp_task*> *cont);

	friend void innerroutine(ThreadPoolManager *tp);
	friend void completeroutine(ThreadPoolManager *tp);

private:
	list<tp_task*> innersearchtasklist;
	list<tp_task*> completesearchtasklist;

	condition_variable inner_ready_condition;
	condition_variable complete_ready_condition;

	vector<std::thread> innerthreadlist;
	vector<std::thread> completethreadlist;

	int shutdownflag;

	mutex innertasklistmutex;
	mutex completetasklistmutex;

	int maxthreadnumber;
	int maxthreadnumberforchildren;
};

#endif
