#ifndef UMADSERVER_H
#define UMADSERVER_H



#include <sstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <string>
#include <vector>
#include <fstream>

using std::cin;
using std::cout;
using std::endl;
using std::cerr;
using std::ios;
using std::fstream;
using std::ofstream;
using std::stringstream;
using std::string;


#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>

#define MYPORT 7777
#define LISTEN_QUEUE 10
#define MAXLINE 1024

#define SA struct sockaddr

extern void btask(string &dataFileName, int &numPivot, int &setE, int &setC, int &singlePivotFanout, int &fftscale, string &pivotSelectionMethod, string &partitionMethod,int &maxLeafSize, string &dataType, int &initialSize, int &finalSize, int &stepSize, bool &bucket, int &fragmentLength, int &dim, double &maxR, string &indexType, int &fftOpt, string &buildingInformationFile, int &buildMode, string &indexName, double &MF_maxRadius, double &MF_middleProportion, bool &putInfo, int &bThreadNum, double &trisectionRadius,string &selectOptimalPivots,string &partitionByOnePivot, string &resultcontainer);
extern void stask(int &dataBasehSize, string &queryFileName, string &dataType, int &firstQuery, int &lastQuery, int &dim, int &fragmentLength, double &maxRadius, double &minRadius, double &step, bool &verify, string &resultsFileName, int &buildMode, string &indexName, int &searchMode, int &cacheHeight, int &sThreadNum, bool &cacheAll, int &numberOfChildrenPathSearchingThreads, string &resultcontainer);
extern void bastask(string &dataFileName, int &numPivot, int &setE, int &setC, int &singlePivotFanout, int &fftscale, string &pivotSelectionMethod, string &partitionMethod, int &maxLeafSize, string &dataType, int &initialSize, int &finalSize, int &stepSize, bool &bucket, int &fragmentLength, int &dim, double &maxR, string &indexType, int &fftOpt, string &queryFileName, int &firstQuery, int &lastQuery, double &maxRadius, double &minRadius, double &step, bool &verify, string &resultsFileName, int &buildMode, string &indexName, int &searchMode, double &MF_maxRadius, double &MF_middleProportion, bool &putInfo, int &cacheHeight, int &sThreadNum, bool &cacheAll, int &bThreadNum, double &trisectionRadius, string &selectOptimalPivots, string &partitionByOnePivot,int &numberOfChildrenPathSearchingThreads, string &resultcontainer);

extern void s_fun(int signo);
extern void recvquerydatafromclient(int connfd, string &queryFileName, int dim, string &datatype, int lastQuery);
extern void recvrawdatafromclient(int connfd, string &dataFileName, int dim, string &dattype, int finalSize);

#endif
