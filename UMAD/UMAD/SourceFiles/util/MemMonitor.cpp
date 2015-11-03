#include "../../HeaderFiles/util/MemMonitor.h"





CMemMonitor::CMemMonitor(void)

{
#ifdef __GNUC__
	memoryFile="/proc/self/status";

	memoryType = "";

	memoryUnit = "";

	virtMemorySize = 0.0;

	resMemorySize = 0.0;

	swapMemorySize = 0.0;

	peekVirtMemorySize = 0.0;

	peekResMemorySize = 0.0;
#endif

}





CMemMonitor::~CMemMonitor(void)

{



}



double CMemMonitor::maxMem =0.0;

double CMemMonitor::maxWorkingSet=0.0;

double CMemMonitor::maxPageUsing=0.0;



int CMemMonitor::callTime=0;



#ifdef _WIN32
PROCESS_MEMORY_COUNTERS CMemMonitor:: pmc;
#endif



void CMemMonitor::updateMem()

{

#ifdef __GNUC__
	CMemMonitor *memoryMonitor = new CMemMonitor();

	memoryMonitor->getMemorySize();

	if (memoryMonitor->virtMemorySize > maxMem)

	{

		maxMem = memoryMonitor->virtMemorySize;

	}

	if(memoryMonitor->resMemorySize > maxWorkingSet)

	{

		maxWorkingSet = memoryMonitor->resMemorySize;

	}

	if(memoryMonitor->swapMemorySize > maxPageUsing)

	{

		maxPageUsing = memoryMonitor->swapMemorySize;

	}

	callTime++;

	delete memoryMonitor;
#else
#ifdef _WIN32
	GetProcessMemoryInfo(GetCurrentProcess(),&pmc,sizeof(pmc));
	callTime++;
	if(pmc.WorkingSetSize+pmc.PagefileUsage >  maxMem)
		maxMem = pmc.WorkingSetSize+pmc.PagefileUsage;
	if(pmc.WorkingSetSize > maxWorkingSet)
		maxWorkingSet = pmc.WorkingSetSize;
	if(pmc.PagefileUsage > maxPageUsing)
		maxPageUsing = pmc.PagefileUsage; 
#endif
#endif
}



void CMemMonitor::reset()

{

	maxMem = 0;

	maxWorkingSet = 0;

	maxPageUsing = 0;

}



void CMemMonitor::getMemorySize()

{

#ifdef __GNUC__
	string str;
	ifstream in(memoryFile.c_str());
	int i = 1;
	while (in !=NULL)
	{
		getline(in,str);
		stringstream newStr(str);
		switch (i)
		{
		case 12:
			newStr >> memoryType >> peekVirtMemorySize >> memoryUnit;
			peekVirtMemorySize = peekVirtMemorySize /1024.0;
			break;
		case 13:
			newStr >> memoryType >> virtMemorySize >> memoryUnit;
			virtMemorySize = virtMemorySize /1024.0;
			break;
		case 15:
			newStr >> memoryType >> peekResMemorySize >> memoryUnit;
			peekResMemorySize = peekResMemorySize /1024.0;
			break;
		case 16:
			newStr >> memoryType >> resMemorySize >> memoryUnit;
			resMemorySize = resMemorySize /1024.0;
			break;
		case 22:
			newStr >> memoryType >> swapMemorySize >> memoryUnit;
			swapMemorySize = swapMemorySize /1024.0;
			break;
		}

		i++;

	}
#endif

}