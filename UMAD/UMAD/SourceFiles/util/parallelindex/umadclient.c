#include "umadclient.h"

extern void s_pipfun(int);
//change argument from string to other types
template<typename type>
type 
stringToNumber(const char *str)
{
	istringstream iss(str);

	type data;
	iss >> data;
	
	return data;
}


//merge arguments list
string argstr;
stringstream ss;

//deprecated
void
mergeargument(const char *str)
{
	argstr.append(str);
	argstr.append(" ");
}

int
main(int argc, char **argv)
{
	char *task = "";
	int buildMode = 0;
	char *dataFileName = "uniformvector-20dim-1m.txt";
	int numPivot = 2;
	int setA = 10000;
	int setN = 50;
	int singlePivotFanout = 3;
	int fftscale = 100;
	char *pivotSelectionMethod = "fft";
	char *partitionMethod = "PivotWise";
	char *partitionByOnePivot = "BalanceOne";
	char *selectOptimalPivots = "Variance";
	double trisectionRadius = 0.1;
	int maxLeafSize = 1;
	char *dataType = "vector";
	int initialSize = 200;
	int finalSize = 200;	
	int stepSize = 20000;	
	bool bucket = false;
	int fragmentLength = 6;
	int dim = 20;
	double maxR = 1;
	char *indexType = "MVPIndex";
	int fftopt = 0;
	char *indexName = "vector_index_PivotWise";
	char* queryFileName = "uniformvector-20dim-1m.txt";
	int firstQuery = 0;
	int lastQuery = 10;
	double maxRadius = 2;
	double minRadius = 0;
	double step = 0.2;
	bool verify = false;
	char *resultsFileName = "resultsfile.txt";

	//char resultsFileName[500];
	//strcpy(resultsFileName,"result/");

	char *runOption = "buildandsearch";
	int searchMode = 1;
	double MF_maxRadius = 2.0;
	double MF_middleProportion = 0.5; 
	bool putinfo = true;

	int cacheHeight = 3;
	bool cacheAll = 0;

	int tsThreadNum = 32;  //number of maximum parallel searching threads --tsThreadNum
	int csThreadNum = 27;  //number of children path searching threads --csThreadNum
	int bThreadNum = 32;   //for parallel build --maxThreadNum
	
	//classification parameters
	char *disfun = "EuclideanDistance";
	char *testFileName = "diabetes-test.data";
	char *classifyMethod = "knn";
	char *dataFileNameInMetricSpace = "metricDistance.data";
	char *showResultFileName = "OutPut.txt";
	char *testDataSetLabel = "iristestlabel.data";

	static struct option_a long_options[] =
	{
		{"task", ARG_REQ, 0 , 'M'},
		{"maxR", ARG_REQ, 0 , 'Y'},
		{"midP", ARG_REQ, 0 , 'Z'},
		{"r",    ARG_REQ, 0, 'a'},
		{"buildMode",    ARG_REQ, 0, 'A'},
		{"b", ARG_REQ,0,'b'},
		{"indexName", ARG_REQ,0,'B'},
		{"searchMode", ARG_REQ,0,'c'},
		{"tR", ARG_REQ,0,'C'},
		{"n",    ARG_REQ, 0 , 'd'},
		{"t",    ARG_REQ, 0 , 'D'},
		{"e",    ARG_REQ, 0 , 'e'},
		{"fftscale", ARG_REQ, 0, 'f'},
		{"F",    ARG_REQ, 0 , 'F'},
		{"frag",    ARG_REQ, 0 , 'g'},
		{"sop",    ARG_REQ, 0 , 'h'},
		{"pbop",    ARG_REQ, 0 , 'H'},
		{"init",    ARG_REQ, 0 , 'i'},
		{"dim",    ARG_REQ, 0 , 'I'},
		{"l",    ARG_REQ, 0 , 'l'},
		{"res",    ARG_REQ, 0 , 'L'},
		{"m",    ARG_REQ, 0 , 'm'},
		{"v",    ARG_REQ, 0 , 'n'},
		{"psm",    ARG_REQ, 0 , 'p'},
		{"dpm",    ARG_REQ, 0 , 'P'},
		{"q",    ARG_REQ, 0 , 'q'},
		{"runOption",ARG_REQ, 0,'r'},
		{"fq",  ARG_REQ, 0 , 'R'},
		{"sete",    ARG_REQ, 0 , 's'},
		{"setc",    ARG_REQ, 0 , 'S'},
		{"s",    ARG_REQ, 0 , 't'},
		{"fftopt",     ARG_REQ, 0, 'T'},
		{"i",    ARG_REQ, 0 , 'U'},
		{"ver",    ARG_REQ, 0 , 'v'},
		{"a",    ARG_REQ, 0 , 'x'},
		{"y",    ARG_REQ, 0 , 'y'},
		{"f",    ARG_REQ, 0 , 'z'},
		{"putinfo",    ARG_REQ, 0 , 'O'},
		{"bThreadNum",   ARG_REQ, 0 , 'G'},
		{"cacheAll",  ARG_REQ, 0 , 'X'},
		{"cacheHeight", ARG_REQ, 0 , 'E'},
		{"tsThreadNum",   ARG_REQ, 0 , 'u'},
		{"csThreadNum",   ARG_REQ, 0 , 'J'},
		{"disfun", ARG_REQ, 0, 'W'},
		{"testFileName", ARG_REQ, 0, 'w'},
		{"classifyMethod", ARG_REQ, 0, 'j'},
		{"dFNIMS", ARG_REQ, 0, 'K'},
		{"sRFN", ARG_REQ, 0, 'k'},
		{"tDSL", ARG_REQ, 0, 'N'},
		{0, 0, 0, 0}
	};
	
	int c = 0;

	while (1)
	{		
		int option_index = 0; //argument option index in long_options
		c = getopt_long_a(argc, argv, ("M:a:A:b:B:c:C:d:D:e:f:F:g:h:H:i:I:l:L:m:n:p:P:q:r:R:s:S:t:T:U:v:x:y:z:Y:Z:G:O:X:E:u:J:W:w:j:K:k:N"), long_options, &option_index); 
		// Check for end of operation or error
		if (c == -1)
			break;


		switch (c)
		{

		case 0:

			/* If this option set a flag, do nothing else now. */
			if (long_options[option_index].flag != 0)
				break;
		
			printf (("option %s"), long_options[option_index].name);
			if (optarg_a)
				printf ((" with arg %s"), optarg_a);
			printf (("\n"));

			break;

		case('M'):
			//mergeargument(optarg_a);
			task = new char[strlen(optarg_a)+1];
			strcpy(task, optarg_a);
			printf(("option -task with value `%s'\n"), task);

			break;
		case ('a'):	
			//mergeargument(optarg_a);
			maxR = stringToNumber<double>(optarg_a);
			cout<<"option -maxR with value " << maxR << endl;

			break;
		case ('A'):	
			//mergeargument(optarg_a);
			buildMode = stringToNumber<int>(optarg_a);
			cout<<"option -buildMode with value " << buildMode << endl;

			break;
		case ('b'):	
			//mergeargument(optarg_a);
			bucket = stringToNumber<bool>(optarg_a);
			cout<<"option -bucket with value " << bucket << endl;

			break;
		case ('B'):	
			//mergeargument(optarg_a);
			indexName = new char[strlen(optarg_a)+1];
			strcpy(indexName,optarg_a);
			cout<<"option -indexName with value " << indexName << endl;

			break;
		case ('c'):	
			//mergeargument(optarg_a);
			searchMode = stringToNumber<int>(optarg_a);
			cout<<"option -searchMode with value " << searchMode << endl;

			break;
		case ('C'):	
			//mergeargument(optarg_a);
			trisectionRadius = stringToNumber<double>(optarg_a);
			cout<<"option -trisectionRadius with value " << trisectionRadius << endl;

			break;
		case ('d'):	
			//mergeargument(optarg_a);
			dataFileName = new char[strlen(optarg_a)+1];
			strcpy(dataFileName,optarg_a);
			printf (("option -dataFileName with value `%s'\n"), dataFileName);

			break;
		case ('D'):	
			//mergeargument(optarg_a);
			dataType = new char[strlen(optarg_a)+1];
			strcpy(dataType,optarg_a);
			printf (("option -dataType with value `%s'\n"), dataType);

			break;
		case ('e'):	
			//mergeargument(optarg_a);
			indexType = new char[strlen(optarg_a)+1];
			strcpy(indexType,optarg_a);
			printf (("option -listType with value `%s'\n"), indexType);

			break;
		case ('f'):	
			//mergeargument(optarg_a);
			fftscale = stringToNumber<int>(optarg_a);
			printf (("option -fftscale with value `%d'\n"), fftscale);

			break;
		case ('F'):	
			//mergeargument(optarg_a);
			finalSize = stringToNumber<int>(optarg_a);
			printf (("option -finalSize with value `%d'\n"), finalSize);

			break;
		case ('g'):	
			//mergeargument(optarg_a);
			fragmentLength = stringToNumber<int>(optarg_a);
			printf (("option -fragmentLength with value `%d'\n"), fragmentLength);

			break;
		case ('h'):	
			//mergeargument(optarg_a);
			selectOptimalPivots = new char[strlen(optarg_a)+1];
			strcpy(selectOptimalPivots,optarg_a);
			printf (("option -selectOptimalPivots with value `%s'\n"), selectOptimalPivots);

			break;
		case ('H'):
			//mergeargument(optarg_a);
			partitionByOnePivot = new char[strlen(optarg_a)+1];
			strcpy(partitionByOnePivot,optarg_a);
			printf (("option -partitionByOnePivot with value `%s'\n"), partitionByOnePivot);

			break;
		case ('i'):
			//mergeargument(optarg_a);
			initialSize = stringToNumber<int>(optarg_a);
			printf (("option -initialSize with value `%d'\n"), initialSize);

			break;
		case ('I'):	
			//mergeargument(optarg_a);		
			dim=stringToNumber<int>(optarg_a);
			printf (("option -dim with value `%d'\n"), dim);

			break;
		case ('l'):
			//mergeargument(optarg_a);	
			lastQuery=stringToNumber<int>(optarg_a);
			printf (("option -lastQuery with value `%d'\n"), lastQuery);

			break;
		case ('L'):	
			//mergeargument(optarg_a);
			resultsFileName=new char[strlen(optarg_a)+1];
			strcpy(resultsFileName,optarg_a);
			printf (("option -resultsFileName with value `%s'\n"), resultsFileName);

			break;
		case ('m'):
			//mergeargument(optarg_a);	
			maxLeafSize=stringToNumber<int>(optarg_a);
			printf (("option -maxLeafSize with value `%d'\n"), maxLeafSize);

			break;
		case ('n'):
			//mergeargument(optarg_a);	
			numPivot=stringToNumber<int>(optarg_a);
			printf (("option -numPivot with value `%d'\n"), numPivot);

			break;
		case ('p'):
			//mergeargument(optarg_a);	
			pivotSelectionMethod = new char[strlen(optarg_a)+1];
			strcpy(pivotSelectionMethod,optarg_a);
			printf (("option -pivotSelectionMethod with value `%s'\n"), pivotSelectionMethod);

			break;
		case ('P'):
			//mergeargument(optarg_a);	
			partitionMethod = new char[strlen(optarg_a)+1];
			strcpy(partitionMethod,optarg_a);
			printf (("option -partitionMethod with value `%s'\n"), partitionMethod);

			break;
		case ('q'):
			//mergeargument(optarg_a);	
			queryFileName = new char[strlen(optarg_a)+1];
			strcpy(queryFileName,optarg_a);
			printf (("option -queryFileName with value `%s'\n"), queryFileName);

			break;
		case ('r'):
			//mergeargument(optarg_a);	
			runOption = new char[strlen(optarg_a)+1];
			strcpy(runOption,optarg_a);
			printf (("option -runOption with value `%s'\n"), optarg_a);

			break;
		case ('R'):
			//mergeargument(optarg_a);	
			firstQuery = stringToNumber<int>(optarg_a);
			printf (("option -firstQuery with value `%d'\n"), firstQuery);

			break;
		case ('s'):
			//mergeargument(optarg_a);
			setA = stringToNumber<int>(optarg_a);
			printf (("option -setA with value `%d'\n"), setA);

			break;
		case ('S'):
			//mergeargument(optarg_a);	
			setN = stringToNumber<int>(optarg_a);
			printf (("option -setN with value `%d'\n"), setN);

			break;
		case ('t'):
			//mergeargument(optarg_a);	
			stepSize = stringToNumber<int>(optarg_a);
			printf (("option -stepSize with value `%d'\n"), stepSize);

			break;
		case ('T'):
			//mergeargument(optarg_a);	
			fftopt = stringToNumber<int>(optarg_a);
			printf (("option -fftopt with value `%d'\n"), fftopt);

			break;
		case ('U'):
			//mergeargument(optarg_a);	
			minRadius = stringToNumber<double>(optarg_a);
			printf (("option -minRadius with value `%f'\n"), minRadius);

			break;
		case ('v'):
			//mergeargument(optarg_a);
			verify = stringToNumber<bool>(optarg_a);
			printf (("option -verify with value `%d'\n"), verify);

			break;
		case ('x'):
			//mergeargument(optarg_a);
			maxRadius = stringToNumber<float>(optarg_a);
			printf (("option -maxRadius with value `%f'\n"), maxRadius);

			break;
		case ('O'):
			//mergeargument(optarg_a);
			putinfo = stringToNumber<bool>(optarg_a);
			printf (("option -putinfo with value `%d'\n"), putinfo);

			break;
		case ('y'):
			//mergeargument(optarg_a);
			step=stringToNumber<double>(optarg_a);
			printf (("option -step with value `%f'\n"), step);

			break;
		case ('z'):
			//mergeargument(optarg_a);
			singlePivotFanout=stringToNumber<int>(optarg_a);
			printf (("option -singlePivotFanout with value `%d'\n"), singlePivotFanout);

			break;
		case ('Y'):
			//mergeargument(optarg_a);
			MF_maxRadius = stringToNumber<double>(optarg_a);
			cout<<"option -MF_maxRadius with value " << MF_maxRadius << endl;

			break;
		case ('Z'):
			//mergeargument(optarg_a);
			MF_middleProportion = stringToNumber<double>(optarg_a);
			cout<<"option -MF_middleProportion with value " << MF_middleProportion << endl;

			break;
		case ('G'):
			//mergeargument(optarg_a);
			bThreadNum = stringToNumber<int>(optarg_a);
			cout<<"option -maxThreadNum with value "<<bThreadNum<<endl;

			break;
		case ('X'):
			//mergeargument(optarg_a);
			cacheAll=stringToNumber<bool>(optarg_a);
			cout<<"option -cacheAll with value "<<cacheAll <<endl;

			break;
		case ('E'):
			//mergeargument(optarg_a);
			cacheHeight = stringToNumber<int>(optarg_a);
			cout<<"option -cacheHeight with value "<<cacheHeight<<endl;

			break;
		case ('u'):
			//mergeargument(optarg_a);
			tsThreadNum = stringToNumber<int>(optarg_a);
			cout<<"option -tsthreadNum with value "<< tsThreadNum <<endl;

			break;
		case ('J'):
			//mergeargument(optarg_a);
			csThreadNum = stringToNumber<int>(optarg_a);
			cout<<"option -csthreadNum with value "<< csThreadNum <<endl;

			break;
		case ('W'):
			//mergeargument(optarg_a);
			disfun = new char[strlen(optarg_a)+1];
			strcpy(disfun,optarg_a);
			printf (("option -disfun with value `%s'\n"), disfun);

			break;
		case ('w'):
			//mergeargument(optarg_a);
			testFileName = new char[strlen(optarg_a)+1];
			strcpy(testFileName,optarg_a);
			printf (("option -testFileName with value `%s'\n"), testFileName);

			break;
		case ('j'):
			//mergeargument(optarg_a);
			classifyMethod = new char[strlen(optarg_a)+1];
			strcpy(classifyMethod,optarg_a);
			printf (("option -classifyMethod with value `%s'\n"), classifyMethod);

			break;
		case ('K'):
			//mergeargument(optarg_a);
			dataFileNameInMetricSpace = new char[strlen(optarg_a)+1];
			strcpy(dataFileNameInMetricSpace,optarg_a);
			printf (("option -dataFileNameInMetricSpace with value `%s'\n"), dataFileNameInMetricSpace);

			break;
		case ('k'):
			//mergeargument(optarg_a);
			showResultFileName = new char[strlen(optarg_a)+1];
			strcpy(showResultFileName,optarg_a);
			printf (("option -showResultFileName with value `%s'\n"), showResultFileName);

			break;
		case ('N'):
			//mergeargument(optarg_a);
			testDataSetLabel = new char[strlen(optarg_a)+1];
			strcpy(testDataSetLabel,optarg_a);
			printf (("option -testDataSetLabel with value `%s'\n"), testDataSetLabel);

			break;
		case '?':
			/* getopt_long already printed an error message. */
			break;
		default:

			abort();
		}
		
	}

	if (optind < argc)
	{
		printf (("non-option ARGV-elements: "));
		while (optind < argc) printf (("%s "), argv[optind++]);

		printf (("\n"));
	}
	
	ss << task << " " << buildMode << " " << dataFileName << " " << numPivot << " " << setA << " " << setN << " " << singlePivotFanout << " " << fftscale << " " << \
		pivotSelectionMethod << " " << partitionMethod << " " << partitionByOnePivot << " " << selectOptimalPivots << " " << trisectionRadius << " " << maxLeafSize << \
		" " << dataType << " " << initialSize << " " << finalSize << " " << stepSize << " " << bucket << " " << fragmentLength << " " << dim << " " << maxR << " " << \
		indexType << " " << fftopt << " " << indexName << " " << queryFileName << " " << firstQuery << " " << lastQuery << " " << maxRadius << " " << minRadius << \
		" " << step << " " << verify << " " << resultsFileName << " " << runOption << " " << searchMode << " " << MF_maxRadius << " " << MF_middleProportion << " " << \
		putinfo << " " << cacheHeight << " " << cacheAll << " " << tsThreadNum << " " << csThreadNum << " " << bThreadNum << " " << disfun << " " << testFileName << \
		" " << classifyMethod << " " << dataFileNameInMetricSpace << " " << showResultFileName << " " << testDataSetLabel;

    	argstr = ss.str();
	//cout << "ss.size:" << argstr.size() << endl;


	if (argstr.size() > MAXLINE)
	{
		perror("too many arguments£¬ please modify the MAXLINE in unp.h to a larger value and rebuild the server and client\n");
		return -1;
	}
	

	vector<string> iplist;
	char ipstr[32] = {0};
	int servernum = 0;

	fstream ipfile("servers");
	while (ipfile.getline(ipstr, 32))
	{
		string tmp(ipstr);
		iplist.push_back(tmp);
		servernum ++;
	}

	cout << "Number of servers is: " << iplist.size() << endl;
	for (int i=0; i<iplist.size(); i++)
	{
		cout << "Server[" << i << "]: " << iplist[i] << endl;
	}

	fd_set rset, allset;
	FD_ZERO(&allset);

	int maxfd = 0;
	
	int *socketfdlist = new int[servernum];
	for (int i=0; i<servernum; i++)
	{
		int sockfd = socket(AF_INET, SOCK_STREAM, 0);

		struct sockaddr_in addr;
		bzero(&addr, sizeof(addr));

		addr.sin_family = AF_INET;
		addr.sin_port = htons(MYPORT);
		inet_pton(AF_INET, iplist[i].c_str(), &addr.sin_addr);		

		if (connect(sockfd, (SA *)(&addr), sizeof(addr)) == -1)
		{
			perror("Connect to server failed");
			cout << "errno:" << errno << endl;
			cout << "Connect to " << iplist[i] << " failed." << endl;
		}
		
		socketfdlist[i] = sockfd;
		FD_SET(sockfd, &allset);
		if (maxfd < sockfd)
		{
			maxfd = sockfd;
		}
		//write(socketfdlist[i], argstr.c_str(), strlen(argstr.c_str()));
	}
	
	//space for receiving and sending data
	char buffer[MAXLINE] = {0};
	
	struct sigaction act, oact;
	act.sa_handler = s_pipfun;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	
	if (sigaction(SIGPIPE, &act,&oact) < 0)
	{
		cout << "SIG_ERR for socket connect" << endl;
	} 

	int niniS = initialSize, nfinalS = finalSize;

	if (strcmp(task, "index") == 0)
	{
		
		if (strcmp(runOption, "buildandsearch") == 0)
		{
			clock_t localbegt, localendt;
			localbegt = times(NULL);
			
			for (int i=0; i<servernum; i++)
			{
				int startfi = (nfinalS / servernum) * i;
				int endfi = 0;
				if (i != (servernum -1))
				{
					endfi = (nfinalS / servernum) * (i + 1);
				}
				else
				{
					endfi = nfinalS;
				}

				initialSize = endfi - startfi;
				finalSize = endfi - startfi;

				ss.str("");
				ss << task << " " << buildMode << " " << dataFileName << " " << numPivot << " " << setA << " " << setN << " " << singlePivotFanout << " " << fftscale << " " << \
					pivotSelectionMethod << " " << partitionMethod << " " << partitionByOnePivot << " " << selectOptimalPivots << " " << trisectionRadius << " " << maxLeafSize << \
					" " << dataType << " " << initialSize << " " << finalSize << " " << stepSize << " " << bucket << " " << fragmentLength << " " << dim << " " << maxR << " " << \
					indexType << " " << fftopt << " " << indexName << " " << queryFileName << " " << firstQuery << " " << lastQuery << " " << maxRadius << " " << minRadius << \
					" " << step << " " << verify << " " << resultsFileName << " " << runOption << " " << searchMode << " " << MF_maxRadius << " " << MF_middleProportion << " " << \
					putinfo << " " << cacheHeight << " " << cacheAll << " " << tsThreadNum << " " << csThreadNum << " " << bThreadNum << " " << disfun << " " << testFileName << \
					" " << classifyMethod << " " << dataFileNameInMetricSpace << " " << showResultFileName << " " << testDataSetLabel;

				//cout <<"Inital and final size send to server " << i << " is " << initialSize << " and " << finalSize << "respectively" << endl;

				if (write(socketfdlist[i], ss.str().c_str(), strlen(ss.str().c_str())) == -1)
				{
					cout << "write args to server failed" << endl;
					if (errno = EPIPE)
					{
						cout << "Errer \"EPIPE\" arised and signal SIGPIPE was cached, which means that the server isn't listening on the port we connect to" << endl;
					}
				}

			}

			finalSize = nfinalS;
			initialSize = niniS;

			//variable for recording time elapsed to transporting data
			double sendid = 0, sendqd = 0;

			clock_t begt, endt;
			
			begt = times(NULL);
			sendrawdatatoserver(socketfdlist, servernum, dataFileName, finalSize, dataType, fragmentLength);
			endt = times(NULL);

			sendid = (endt - begt)/100.0;
			cout << "Sending data line to all servers consumes " << sendid << " senconds." << endl;

			begt = times(NULL);
			sendquerydatatoserver(socketfdlist, servernum, queryFileName, lastQuery, dataType, fragmentLength);
			endt = times(NULL);

			sendqd = (endt - begt)/100.0;
			cout << "Sending query data line to all servers consumes " << sendqd << " senconds." << endl;
			
			int rnum = 0;
			maxfd++;

			vector<string> rlist;
			int *serverf = new int[servernum];
			bzero(serverf, servernum * sizeof(int));
			while (1)
			{
				rset = allset;

				int n = select(maxfd, &rset, NULL, NULL, NULL);

				for(int i=0; i<servernum; i++)
				{
					if (!serverf[i] && FD_ISSET(socketfdlist[i], &rset))
					{
						bzero(buffer, MAXLINE);
						int rn = read(socketfdlist[i], buffer, MAXLINE);
						
						string str(buffer);
						//cout << __LINE__ << " result information got from server is:" << str << endl;
						rlist.push_back(str);

						serverf[i] = 1;
						FD_CLR(socketfdlist[i], &allset);
						close(socketfdlist[i]); 
						rnum++;
					}
				}

				if (rnum == servernum)
				{
					break;
				}
			}
			
			clock_t resultcbeg, resultcend;
			resultcbeg = times(NULL);

			//index name
			string findexn;
			//time elapsed to build MVPTree
			double findext = 0;

			double **resultl = 0;
			int n = 0;

			for (int i=0; i<rlist.size(); i++)
			{
				cout << "Result from server[" << i << "]: " << rlist[i] << endl;
				
				ss.str("");
				ss << rlist[i];
				
				string indexn;
				double indext = 0;


				double indexrad = 0;

				double searchr = 0;
				double ttt = 0;
				double respt = 0;

				ss >> indexn >> indext >> n;
				
				if (i == 0)
				{
					findexn.append(indexn);
					resultl = new double*[n];
					
					for (int j=0; j<n; j++)
					{
						resultl[j] = new double[4];
						for (int k=0; k<4; k++)
						{
							resultl[j][k] = 0;
						}
					}

				}

				findext = (findext > indext) ? findext:indext;

				cout << "============================BUILD RESULT=============================" << endl;
				cout << "|Index Name" << "|------|" << "Time elapsed|" << endl;
				cout << indexn << "       " << indext << endl;
				cout << "=====================================================================" << endl;
				
				cout << "============================SEARCH RESULT============================" << endl;
				cout << "|radius|------|Number of result|------|Throughput time|------|Response time|" << endl;
				
				for (int j=0; j<n; j++)
				{
					ss >> indexrad >> searchr >> ttt >> respt;
					cout << indexrad << "            " << searchr  << "                " << ttt  << "                " << respt << endl;
					
					resultl[j][0] = indexrad;
					resultl[j][1] += searchr;
					resultl[j][2] = (resultl[j][2] > ttt) ? resultl[j][2] : ttt;
					resultl[j][3] = (resultl[j][3] > respt) ? resultl[j][3] : respt;
				}
				
				cout << "=====================================================================" << endl;
			}
			
			resultcend = times(NULL);			
			cout << "Time elapsed for result information extraction is:" << (resultcend - resultcbeg)/100.0 << endl;
			localendt = times(NULL);

			string nresf(resultsFileName);
			nresf.insert(0, "../result/");

			fstream resfs(nresf.c_str(), ios::out|ios::app);

			resfs << "=====================================================================================" << endl;
			resfs << "Index Name" << endl;
			resfs << findexn << endl;
			resfs << "Database Size" << endl;
			resfs << finalSize << endl;
			resfs << "Build Mode" << endl;
			resfs << buildMode << endl;
			resfs << "Time Elapsed" << endl; 
			resfs << findext << endl;
			resfs << "======================================================================================" << endl;
			resfs << "QueryNum  " << " SearchMode " << " Searchradius   " << "Searchresultsnum" << " Throughputtime " << "Responsetime  " << endl;
			for (int i=0; i<n; i++)
			{
				resfs << lastQuery << setw(19) << searchMode << setw(19) << resultl[i][0] << setw(19) << resultl[i][1] / lastQuery << setw(15) << resultl[i][2] << setw(15) << resultl[i][3] / lastQuery << endl;
			}

			resfs << "\nLocal total elapsed time:" << (localendt - localbegt) / 100.0 << endl;
			resfs << "======================================================================================" << endl;

			resfs.clear();
			resfs.close();
			//print out building time of every server and summation of them

			//print out index file name in every server

			//print out total searching time of query and summation of them

			//print out everage reponse time of query

			//print out total number of results and the one from every server

		}
		else if (strcmp(runOption, "build") == 0)
		{
			clock_t localbegt, localendt;
			localbegt = times(NULL);

			for (int i=0; i<servernum; i++)
			{
				int startfi = (nfinalS / servernum) * i;
				int endfi = 0;
				if (i != (servernum -1))
				{
					endfi = (nfinalS / servernum) * (i + 1);
				}
				else
				{
					endfi = nfinalS;
				}

				initialSize = endfi - startfi;
				finalSize = endfi - startfi;

				ss.str("");
				ss << task << " " << buildMode << " " << dataFileName << " " << numPivot << " " << setA << " " << setN << " " << singlePivotFanout << " " << fftscale << " " << \
					pivotSelectionMethod << " " << partitionMethod << " " << partitionByOnePivot << " " << selectOptimalPivots << " " << trisectionRadius << " " << maxLeafSize << \
					" " << dataType << " " << initialSize << " " << finalSize << " " << stepSize << " " << bucket << " " << fragmentLength << " " << dim << " " << maxR << " " << \
					indexType << " " << fftopt << " " << indexName << " " << queryFileName << " " << firstQuery << " " << lastQuery << " " << maxRadius << " " << minRadius << \
					" " << step << " " << verify << " " << resultsFileName << " " << runOption << " " << searchMode << " " << MF_maxRadius << " " << MF_middleProportion << " " << \
					putinfo << " " << cacheHeight << " " << cacheAll << " " << tsThreadNum << " " << csThreadNum << " " << bThreadNum << " " << disfun << " " << testFileName << \
					" " << classifyMethod << " " << dataFileNameInMetricSpace << " " << showResultFileName << " " << testDataSetLabel;

				//cout <<"Inital and final size send to server " << i << " is " << initialSize << " and " << finalSize << "respectively" << endl;

				if (write(socketfdlist[i], ss.str().c_str(), strlen(ss.str().c_str())) == -1)
				{
					cout << "write args to server failed" << endl;
					if (errno = EPIPE)
					{
						cout << "Errer \"EPIPE\" arised and signal SIGPIPE was cached, which means that the server isn't listening on the port we connect to" << endl;
					}
				}

			}

			finalSize = nfinalS;
			initialSize = niniS;

			//variable for recording time elapsed to transporting data
			double sendid = 0;

			clock_t begt, endt;
			
		
			begt = times(NULL);
			sendrawdatatoserver(socketfdlist, servernum, dataFileName, finalSize, dataType, fragmentLength);
			endt = times(NULL);

			sendid = (endt - begt)/100.0;


			cout << "Sending data line to all servers consumes " << sendid << " senconds." << endl;
		
			
			int rnum = 0;
			maxfd++;

			vector<string> rlist;
			int *serverf = new int[servernum];
			bzero(serverf, servernum * sizeof(int));
			while (1)
			{
				rset = allset;
			
				int n = select(maxfd, &rset, NULL, NULL, NULL);
				for(int i=0; i<servernum; i++)
				{
					if (!serverf[i] && FD_ISSET(socketfdlist[i], &rset))
					{
						bzero(buffer, MAXLINE);
						int rn = read(socketfdlist[i], buffer, MAXLINE);
						
						string str(buffer);
						rlist.push_back(str);

						serverf[i] = 1;
						FD_CLR(socketfdlist[i], &allset);
						close(socketfdlist[i]); 
						rnum++;
					}
				}

				if (rnum == servernum)
				{
					break;
				}
			}

			clock_t resultcbeg, resultcend;
			resultcbeg = times(NULL);
			//index name
			string findexn;
			//time elapsed to build MVPTree
			double findext = 0;

			for (int i=0; i<rlist.size(); i++)
			{
				cout << "Result from server[" << i << "]: " << rlist[i] << endl;
				
				ss.str("");
				ss << rlist[i];
				
				string indexn;
				double indext = 0;


				double indexrad = 0;

				ss >> indexn >> indext;
				
				if (i == 0)
				{
					findexn.append(indexn);
				}

				findext = (findext > indext) ? findext : indext;

				cout << "============================BUILD RESULT=============================" << endl;
				cout << "|Index Name" << "|------|" << "Time elapsed|" << endl;
				cout << indexn << "       " << indext << endl;
				cout << "=====================================================================" << endl;
			}
			resultcend = times(NULL);			
			cout << "Time elapsed for result information extraction is:" << (resultcend - resultcbeg)/100.0 << endl;

			localendt = times(NULL);
			
			string nresf(resultsFileName);
			nresf.insert(0, "../result/");

			fstream resfs(nresf.c_str(), ios::out|ios::app);

			resfs << "=====================================================================================" << endl;
			resfs << "Index Name" << endl;
			resfs << findexn << endl;
			resfs << "Database Size" << endl;
			resfs << finalSize << endl;
			resfs << "Build Mode" << endl;
			resfs << buildMode << endl;
			resfs << "Time Elapsed" << endl; 
			resfs << findext << endl;

			resfs << "\nLocal total elapsed time:" << (localendt - localbegt) / 100.0 << endl;
			resfs << "======================================================================================" << endl;

			resfs.clear();
			resfs.close();

			////print out building time of every server and summation of them

			//print out index file name in every server
		}
		else if (strcmp(runOption, "search") == 0)
		{
			clock_t localbegt, localendt;
			localbegt = times(NULL);

			for (int i=0; i<servernum; i++)
			{
				write(socketfdlist[i], ss.str().c_str(), strlen(ss.str().c_str()));
			}

			double sendqd = 0;

			clock_t begt, endt;

			begt = times(NULL);
			sendquerydatatoserver(socketfdlist, servernum, queryFileName, lastQuery, dataType, fragmentLength);
			endt = times(NULL);

			sendqd = (endt - begt)/100.0;
			cout << "Sending query data line to all servers consumes " << sendqd << " senconds." << endl;
			
			int rnum = 0;
			maxfd++;

			vector<string> rlist;
			int *serverf = new int[servernum];
			bzero(serverf, sizeof(int) * servernum);
			while (1)
			{
				rset = allset;
			
				int n = select(maxfd, &rset, NULL, NULL, NULL);
				for(int i=0; i<servernum; i++)
				{
					if (!serverf[i] && FD_ISSET(socketfdlist[i], &rset))
					{
						bzero(buffer, MAXLINE);
						int rn = read(socketfdlist[i], buffer, MAXLINE);
						
						string str(buffer);
						rlist.push_back(str);

						serverf[i] = 1;
						FD_CLR(socketfdlist[i], &allset);
						close(socketfdlist[i]); 
						rnum++;
					}
				}

				if (rnum == servernum)
				{
					break;
				}
			}

			clock_t resultcbeg,resultcend;
			resultcbeg = times(NULL);
			double **resultl = 0;
			int n = 0;

			for (int i=0; i<rlist.size(); i++)
			{
				cout << "Result from server[" << i << "]: " << rlist[i] << endl;
				
				ss.str("");
				ss << rlist[i];

				double indexrad = 0;

				double searchr = 0;
				double ttt = 0;
				double respt = 0;

				ss >> n;
				
				if (i == 0)
				{
					resultl = new double*[n];
					
					for (int j=0; j<n; j++)
					{
						resultl[j] = new double[4];
						for (int k=0; k<4; k++)
						{
							resultl[j][k] = 0;
						}
					}

				}
				
				cout << "============================SEARCH RESULT============================" << endl;
				cout << "|radius|------|Number of result|------|Throughput time|------|Response time|" << endl;	
				for (int j=0; j<n; j++)
				{
					ss >> indexrad >> searchr >> ttt >> respt;
					cout << indexrad << "            " << searchr << "                " << ttt << "                " << respt << endl;
					
					resultl[j][0] = indexrad;
					resultl[j][1] += searchr;
					resultl[j][2] = (resultl[j][2] > ttt) ? resultl[j][2] : ttt;
					resultl[j][3] = (resultl[j][3] > respt) ? resultl[j][3] : respt;
				}
				cout << "=====================================================================" << endl;
			}
			resultcbeg = times(NULL);
			cout << "Time elapsed for result information extraction is:" << (resultcend - resultcbeg)/100.0 << endl;
			localendt = times(NULL);
			
			string nresf(resultsFileName);
			nresf.insert(0, "../result/");

			fstream resfs(nresf.c_str(), ios::out|ios::app);
			resfs << "======================================================================================" << endl;
			resfs << "QueryNum  " << " SearchMode " << " Searchradius   " << "Searchresultsnum" << " Throughputtime " << "Responsetime  " << endl;
			for (int i=0; i<n; i++)
			{
				resfs << lastQuery << setw(19) << searchMode << setw(19) << resultl[i][0] << setw(19) << resultl[i][1] / lastQuery << setw(15) << resultl[i][2] << setw(15) << resultl[i][3] / lastQuery << endl;
			}

			resfs << "\nLocal total elapsed time:" << (localendt - localbegt) / 100.0 << endl;
			resfs << "======================================================================================" << endl;

			resfs.clear();
			resfs.close();

			//print out total throughput time of searching and the one from every server

			//print out everage response time of query

			//print out total number results and the one from every server
		}
	}
	else if (strcmp(task, "classification") == 0)
	{
			
	}
	else if (strcmp(task, "outlier") == 0)
	{
		
	}
	else 
	{
		printf("Please point out the operation with parameter --task\n");

		for (int i=0; i<servernum; i++)
		{
			close(socketfdlist[i]);
		}
	}

	return 0;
}
