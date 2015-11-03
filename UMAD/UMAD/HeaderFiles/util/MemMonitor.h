#ifndef CMemMonitor_H

#define CMemMonitor_H



#include <sstream>

#include <iostream>

#include <fstream>

using namespace std;

#ifdef _WIN32
#include <Windows.h>
#include <psapi.h>
#pragma comment(lib,"psapi.lib") 
#endif

class CMemMonitor
{

public:

    CMemMonitor(void);

    ~CMemMonitor(void);



    static void updateMem();

    static void reset();

	void getMemorySize();



    static double maxMem;

    static double maxWorkingSet;

    static double maxPageUsing;



    static int callTime;


#ifdef __GNUC__
	string memoryFile,memoryType,memoryUnit;
	double virtMemorySize,resMemorySize,swapMemorySize,peekVirtMemorySize,peekResMemorySize;
#else
#ifdef _WIN32
	 static PROCESS_MEMORY_COUNTERS pmc;
#endif
#endif
};
#endif