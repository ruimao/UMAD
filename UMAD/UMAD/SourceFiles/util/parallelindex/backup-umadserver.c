#include "umadserver.h"
#include "servertransport.h"

void
initialization();


int
main(int argc, char **argv)
{
	//if (fork())
	//{
	//	exit(0);
	//}

	//setsid();

	//if (fork())
	//{
	//	exit(0);
	//}

	signal(SIGCHLD, SIG_IGN);

	//umask(0);
	//initialization();

	//for read and write from socket
	int listenfd, connfd;
	socklen_t caddrlen;
	struct sockaddr_in caddr, saddr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&saddr, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(MYPORT);
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);

	//set S0_PORTREUSE option		
	int flag = 1;
	int flags = sizeof(int);

	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &flag, flags) < 0)
	{
		cout << "Failed to set socket option." << endl;
		return -1;
	}


	if (bind(listenfd, (SA *)(&saddr), sizeof(saddr)) < 0)
	{
		if (errno = EADDRINUSE)
		{
			cout << "Error \"EADDRINUSE\" arised when bind to " << MYPORT << endl;
		}

		cout << "Failed to band to port" << MYPORT << endl;
		return -1;
	}
	
	listen(listenfd, LISTEN_QUEUE);

	while (1)
	{
		caddrlen = sizeof(caddr);
		connfd = accept(listenfd, (SA *)(&caddr), &caddrlen);
		

		if (fork() == 0)
		{
			close(listenfd);

			//parameters
			string task;
			string dataFileName;
			string pivotSelectionMethod;
			string partitionMethod;
			string partitionByOnePivot;
			string selectOptimalPivots;
			
			int buildMode = 0;
			int numPivot = 2;
			int setE = 10000;
			int setC = 50;
			int singlePivotFanout = 3;
			int fftscale = 100;
			double trisectionRadius = 0.2;
			int maxLeafSize = 100;
			int initialSize = 20000;
			int finalSize = 20000;
			int stepSize = 20000;
			bool bucket = false;
			int fragmentLength = 6;
			int dim = 20;
			double maxR = 1;

			string dataType;
			string indexType;
			string indexName;
			string queryFileName;
			string runOption;
			string resultsFileName;

			int firstQuery  = 0;
			int fftopt = 0;
			int lastQuery = 1; //--l index
			double maxRadius = 2; //--a index 
			double minRadius = 0; //--i index
			double step = 0.2;  //--y index
			bool verify = false;  //--ver index
			int searchMode = 1; //--searchMode index
			double MF_maxRadius = 2.0;    //--maxR index
			double MF_middleProportion = 0.5; //--minP index
			bool putinfo = true; //--putinfo index 
			int cacheHeight = 3; //--cacheHeight index
			bool cacheAll = 0; //--cacheAll index
			int tsThreadNum = 10;//number of maximum parallel searching threads  --tsThreadNum
			int csThreadNum = 7;//number of children path searching threads  --csThreadNum
			int bThreadNum = 32;//for parallel build  --maxThreadNum
			
			//***classification parameters
			string disfun;
			string testFileName;
			string classifyMethod;
			string dataFileNameInMetricSpace;
			string showResultFileName;
			string testDataSetLabel;

		 	char buffer[MAXLINE];
			bzero(buffer, MAXLINE);

			ssize_t n = 0;

			n = read(connfd, buffer, MAXLINE);

			stringstream ss;
			ss << buffer;
		    
			//define 49 variable as parameters for further use
			ss >> task >>  buildMode >>  dataFileName >>  numPivot >>  setE >>  setC >>  singlePivotFanout >>  fftscale >>  pivotSelectionMethod >>  partitionMethod >>  partitionByOnePivot 
		>>  selectOptimalPivots >>  trisectionRadius >>  maxLeafSize >>  dataType >>  initialSize >>  finalSize >>  stepSize >>  bucket >>  fragmentLength >>  dim >>  maxR >> indexType >>  fftopt >>  indexName >>  queryFileName >>  firstQuery >>  lastQuery >>  maxRadius >>  minRadius >>  step >>  verify >>  resultsFileName >>  runOption >> 
		 searchMode >>  MF_maxRadius >>  MF_middleProportion >>  putinfo >>  cacheHeight >>  cacheAll >>  tsThreadNum >>  csThreadNum >>  bThreadNum >>  disfun >>  testFileName 
		>>  classifyMethod >>  dataFileNameInMetricSpace >>  showResultFileName >>  testDataSetLabel;

			cout << "=============================================================================================================" << endl;
			cout << "Arguments Received:\n" << ss.str() << endl;
			cout << "=============================================================================================================" << endl;

	

			struct sigaction act, oact;
			act.sa_handler = s_fun;
			sigemptyset(&act.sa_mask);
			act.sa_flags = 0;

			if (sigaction(SIGALRM, &act, &oact) == -1)
			{
				cout << " Error arised in sigaction function" << endl;
				return -1;
			}

			//cout << "Dim, inital and final size received from client is " << dim << ", " << initialSize << " and " << finalSize << "respectively" << endl;

			if (task.compare("index") == 0)
			{
				if (runOption.compare("buildandsearch") == 0)
				{
					if (dataType.compare("vector") == 0)
					{
						recvvectorforbuildindex(connfd, dataFileName, dim, finalSize);
						recvvectorforsearching(connfd, queryFileName, dim, lastQuery);
					}

					//begening build and search operation
					string results;
					bastask(dataFileName,numPivot,setE,setC,singlePivotFanout,fftscale,pivotSelectionMethod,partitionMethod,maxLeafSize,dataType,initialSize,finalSize,stepSize,bucket,fragmentLength,dim,maxR,indexType,fftopt,queryFileName,firstQuery,lastQuery,maxRadius,minRadius,step,verify,resultsFileName,buildMode,indexName,searchMode,MF_maxRadius,MF_middleProportion,putinfo,cacheHeight,tsThreadNum,cacheAll,bThreadNum,trisectionRadius,selectOptimalPivots,partitionByOnePivot,csThreadNum, results);

					write(connfd, results.c_str(), results.size());
					cout << "=============================================================================================================" << endl;
					cout << "result.size:" << results.size() << " \nresult:" << results.c_str() << endl;
					
					shutdown(connfd, SHUT_RDWR);
					//close(listenfd);

					//return 0;
				}
				else if (runOption.compare("build") == 0)
				{
					if (dataType.compare("vector") == 0)
					{
						recvvectorforbuildindex(connfd, dataFileName, dim, finalSize);
					}

					//build
					string results;
					btask(dataFileName,numPivot,setE,setC,singlePivotFanout,fftscale,pivotSelectionMethod,partitionMethod,maxLeafSize,dataType,initialSize,finalSize,stepSize,bucket,fragmentLength,dim,maxR,indexType,fftopt,resultsFileName,buildMode,indexName,MF_maxRadius,MF_middleProportion,putinfo,bThreadNum,trisectionRadius,selectOptimalPivots,partitionByOnePivot, results);

					write(connfd, results.c_str(), results.size());
					cout << "=============================================================================================================" << endl;
					cout << "result.size:" << results.size() << " \nresult:" << results.c_str() << endl;
					
					shutdown(connfd, SHUT_RDWR);
					//close(listenfd);

					//return 0;

				}
				else if (runOption.compare("search") == 0)
				{
					if (dataType.compare("vector") == 0)
					{
						recvvectorforsearching(connfd, queryFileName, dim, lastQuery);
					}
					
					cout << "Start searching" << endl;
					//search operation
					string results;
					stask(initialSize,queryFileName,dataType,firstQuery,lastQuery,dim,fragmentLength,maxRadius,minRadius,step,verify,resultsFileName,buildMode,indexName,searchMode,cacheHeight,tsThreadNum,cacheAll,csThreadNum, results);
					
					write(connfd, results.c_str(), results.size());
					cout << "=============================================================================================================" << endl;
					cout << "result.size:" << results.size() << " \nresult:" << results.c_str() << endl;
					
					shutdown(connfd, SHUT_RDWR);
					//close(listenfd);

					//return 0;
				}
			}
			else if (task.compare("classification") == 0)
			{
				write(connfd, "Sorry, service classification has not been constructed completely, we will finish and publish it in the near future", strlen("Sorry, service classification has not been constructed completely, we will finish and publish it in the near future"));
			}
			else if (task.compare("outlierdetection") == 0)
			{
				write(connfd, "Sorry, service classification has not been constructed completely, we will finish and publish it in the near future", strlen("Sorry, service classification has not been constructed completely, we will finish and publish it in the near future"));
			}
			else
			{
				write(connfd, "Sorry, we could not find the service you requested, please check out your parameter and try again.", strlen("Sorry, we could not find the service you requested, please check out your parameter and try again."));
			}
			close(connfd);

			exit(0);
		}
		
		close(connfd);
	}

	close(listenfd);

	return 0;
}


void 
initialization()
{
	int ipid = 0;
	if ((ipid = fork()) > 0)
	{
		exit(0);
	}
	else if (ipid < 0)
	{
		cout << "Failed to create process." << endl;
		exit(-1);
	}

	setsid();

	if ((ipid = fork()) > 0)
	{
		exit(0);
	}
	else if (ipid < 0)
	{
		cout << "Failed to create process." << endl;
		exit(-1);
	}

	close(0);
	close(1);
	close(2);

	umask(0);

	signal(SIGCHLD, SIG_IGN);
}
	
