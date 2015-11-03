/** @file

* @brief This class is a wrapper of over String to implement IndexObject

* @author Qihang Li,Zhiyong Xu

* @date 2013.3.24

* @version <version number>

* @note

* modified writeExternal() and readExternal() by Zhiyong Xu 2013/07/24

* detailed description

*/



#include "../../HeaderFiles/metricdata/StringObject.h"





CStringObject::CStringObject(void)

{

}



CStringObject::CStringObject(string line)

{

	data=line;

}



/**

* @return the data

*/

string CStringObject:: getData()

{

	return data;

}



CStringObject::~CStringObject(void)

{

	

}



/**

* @param[in] fileName the filename of the source file

* @exception <exception-object> <exception description>

* @return a vector stored string fragments.

* @note

* This function will get data from a format file, which contain some string informations,

*  and then save as a CStringObject type and store in a vector.

* Details:

* The first line of the file should have two integers, separated by white

* space. The first is the dimension of the DoubleVector, the second is the

* total number of data points. Each following line is a DoubleVector, with

* each dimension separated by white space.

*/

vector<shared_ptr<CMetricData> >* CStringObject ::loadData(string filename,int maxsize)

{

	ifstream infile(filename.c_str());

	vector<shared_ptr<CMetricData> >* data=new vector<shared_ptr<CMetricData> >;



	if(!infile.is_open()){

		cerr << "Stop! Cannot open the file." << endl;

		return data;

	}



	int numData=0;

	string Line;	//a DoubleVector, with each dimension separated by white space

	char buffer[100];

	infile.getline(buffer, 100);

	Line = string(buffer);

	while(Line!=""&&numData<maxsize)

	{

		shared_ptr<CStringObject> temp((new CStringObject(Line)));

		data->push_back(temp);

		infile.getline(buffer, 100);

		Line = string(buffer);

		numData++;

	}

	return data;

}





/**@brief write the instance of this class to hard disk

*@param out out putstream,used to write inforamtion to the hard disk

*@return return the size of information which has been wrote to the hard disk

*/

int CStringObject ::writeExternal(ofstream &out)

{

	int size=0;



	int length=data.size()+1;// the '\0' char.



	out.write((char*)(&length),sizeof(int));

	size += sizeof(int);



	out.write(data.c_str(),length*sizeof(char));

	size += length*sizeof(char);



	return size;

}



/**@load a instance of this class from the file stored in hard disk

*@param in input stream,used to read information from hard disk

*@return return the size of information which has been read from the hard disk

*/

int CStringObject ::readExternal(ifstream &in)

{

	int size=0;

	int length=0;

	data.empty();



	in.read((char*)(&length),sizeof(int));

	size += sizeof(int);



	char *temp=new char[length];

	in.read(temp,length*sizeof(char));

	size += length*sizeof(char);



	data=string(temp);

	delete [] temp;

	return size;

}



/**@brief return the name of a instance of this class

*@return return the name of a instance of this class

*/

CreateFuntion CStringObject::getConstructor()

{

	return &CreateInstance;

}



void* CStringObject::CreateInstance()

{

	return new CStringObject();

}
#ifdef __GNUC__
int
CStringObject::sendrawdata(int *serverlist, int servernum, char *filename, int size)
{
	char buffer[MAXLINE];
	
	char *nfilename = "../data/";
	joinCharArray(nfilename, filename);

	fstream ifs(nfilename);
	if (!ifs)
	{
		cout << "Failed to open file " << filename << endl;
		return -1;
	}

	//bzero(buffer, MAXLINE);
	//ifs.getline(buffer, MAXLINE);
	//cout << "first line:" << buffer << endl;
	
	for (int i=0; i<servernum; i++)
	{
		bzero(buffer, MAXLINE);
		read(serverlist[i], buffer, MAXLINE);

		if (strcmp(buffer, filename) != 0)
		{
			cout << "There is no file names " << buffer << " located in this client" << endl;
			return -1;
		}

		//sleep(5);

		int nsize = 0;
		if (i == servernum - 1)
		{
			nsize = size;
		}
		else
		{
			nsize = (i + 1) * (size / servernum);
		}

		//TODO: 每一次循环结束都记录当前的location，供下次使用。
		//for (int j=0; j<i * (size / servernum); j++)
		//{
		//	ifs.getline(buffer, MAXLINE);
		//}

		//cout << "server[" << i << "] start:" << i * (size / servernum) << " end:" << nsize << endl;
		
		int senddn = 0;
		for (int j=i * (size / servernum); j<nsize; j++)
		{
			bzero(buffer, MAXLINE);
			ifs.getline(buffer, MAXLINE);

			if (strlen(buffer) >= MAXLINE)
			{
				cout << "buffer overflow" << endl;
				return -1;
			}
			else
			{
				buffer[strlen(buffer)] = '\n';
			}

			if (strlen(buffer) > 0)
			{
				int len = write(serverlist[i], buffer, strlen(buffer));
				if (len != strlen(buffer))
				{
					cout << "Failed to write data to server " << i << endl;
					return -1;
				}
				senddn++;
			}
			else
			{
				j--;
				continue;
			}
		}
		cout << "Number of data line sent to server " << i << " is: " << senddn << ". File location now is:" << ifs.tellg() << endl;

		sleep(1);
		bzero(buffer, MAXLINE);
		strcpy(buffer, "@@@@");
		write(serverlist[i], buffer, strlen(buffer));
	}
	
	ifs.close();
}
#else
			//windows
		
#endif

#ifdef __GNUC__
int
CStringObject::sendquerydata(int *serverlist, int servernum, char *filename, int num)
{
	char buffer[MAXLINE];

	char *nfilename = "../data/";
	joinCharArray(nfilename, filename);
	
	fstream ifs(nfilename);
	if (!ifs)
	{
		cout << "Failed to open file " << filename << endl;
		return -1;
	}


	//bzero(buffer, MAXLINE);
	//ifs.getline(buffer, MAXLINE);
	//cout << "first line:" << buffer << endl;

	long flength = ifs.tellg();

	for (int i=0; i<servernum; i++)
	{
		ifs.seekg(flength, ios::beg);

		bzero(buffer, MAXLINE);
		read(serverlist[i], buffer, MAXLINE);

		if (strcmp(buffer, filename) != 0)
		{
			cout << "Filename from server is not equal to the one send to it" << endl;
			return -1;
		}
		
		//cout << "server[" << i << "] query object:" << num << endl;

		int sendqn = 0;
		for (int j=0; j<num; j++)
		{
			bzero(buffer, MAXLINE);
			ifs.getline(buffer, MAXLINE);

			if (strlen(buffer) >= MAXLINE)
			{
				cout << "buffer overflow" << endl;
				return -1;
			}
			else
			{
				buffer[strlen(buffer)] = '\n';
			}

			if (strlen(buffer) > 0)
			{
				int len = write(serverlist[i], buffer, strlen(buffer));
				if (len != strlen(buffer))
				{
					cout << "Failed to write data to server " << i << endl;
					return -1;
				}
				sendqn++;
			}
		}

		cout << "Number of query data line sent to server " << i << " is: " << sendqn << endl;

		bzero(buffer, MAXLINE);
		strcpy(buffer, "@@@@");
		write(serverlist[i], buffer, strlen(buffer));
	}

	ifs.close();
}

#else
			//windows
		
#endif



#ifdef __GNUC__
void 
CStringObject::recvrawdata(int connfd, string &dataFileName, int dim, int finalSize)
{
	stringstream ss;

	write(connfd, dataFileName.c_str(), strlen(dataFileName.c_str()));

	char buffer[MAXLINE];

	dataFileName.insert(0, "n");
	string ndfs = dataFileName;
	ndfs.insert(0, "../data/");
	fstream fs(ndfs.c_str(), ios::out);
	
	/*string firstl;
	ss.str("");
	ss.clear();
	ss << dim << " " << finalSize << "\n";
	firstl.append(ss.str());*/
	
	//cout << "Dim and finalSize writen to " << dataFileName << " is " << dim << " and " << finalSize << " respectively." << endl;



	//fs.write(firstl.c_str(), firstl.size());
	
	int recvdata = 0;
	while (1) 
	{
		alarm(5);	
		bzero(buffer, MAXLINE);
		int rlen = read(connfd, buffer, 1000);	
		
		//cnt++;
		//cout << "buffer[" << cnt << "]:" << buffer << " readn:" << rlen << " strlen(buffer):" << strlen(buffer) << endl;
		//cout.flush();
		
		char *t = buffer;
		if (rlen == 4 && strcmp(buffer, "@@@@") == 0)
		{
			int ar = alarm(0);
			//cerr << __LINE__ << " Remaining alarm time:" << ar << endl;
			break;
		}
		if (rlen > 4 && strcmp(t + (strlen(buffer) - 4), "@@@@") == 0)
		{
			fs.write(buffer, strlen(buffer) - 4);

			int ar = alarm(0);
			//cerr << __LINE__ << " Remaining alarm time:" << ar << endl;
		    break;	
		}

		if (strlen(buffer) > 0)
		{
			recvdata++;
			fs.write(buffer, strlen(buffer));
			fs.flush();
		}
		
		int ar = alarm(0);
		//cerr << __LINE__ << " Remaining alarm time:" << ar << endl;

	}
	//cout << "Received raw data line is: " << recvdata << endl;
	fs.clear();
	fs.close();
}
#else
			//windows
		
#endif

#ifdef __GNUC__
void 
CStringObject::recvquerydata(int connfd, string &queryFileName, int dim, int lastQuery)
{
	stringstream ss;

	char buffer[MAXLINE];

	write(connfd, queryFileName.c_str(), strlen(queryFileName.c_str()));
		
	queryFileName.insert(0, "q");
	string nqfn = queryFileName;
	nqfn.insert(0, "../data/");

	fstream fs(nqfn.c_str(), ios::out);
	if (!fs)
	{
		cout << "Failed to open file " << queryFileName << endl; 
	}

	/*string firstl;
	ss.str("");
	ss.clear();
	ss << dim <<" " << lastQuery << "\n";
	firstl.append(ss.str());*/

	//cout << "Dim and finalSize writen to " << queryFileName << " is " << dim << " and " << lastQuery << " respectively." << endl;

	//fs.write(firstl.c_str(), firstl.size());

	int recvqnum = 0;
	while (1) 
	{
		alarm(5);	
		bzero(buffer, MAXLINE);
		int rlen = read(connfd, buffer, 1020);	
		
		//cnt++;
		//cout << "buffer[" << cnt << "]:" << buffer << " readn:" << rlen << " strlen(buffer):" << strlen(buffer) << endl;
		//cout.flush();
		
		char *t = buffer;
		if (rlen == 4 && strcmp(buffer, "@@@@") == 0)
		{
			int ar = alarm(0);
			//cerr << __LINE__ << " Remaining alarm time:" << ar << endl;
			break;
		}
		if (rlen > 4 && strcmp(t + (strlen(buffer) - 4), "@@@@") == 0)
		{
			int ar = alarm(0);
			fs.write(buffer, strlen(buffer) - 4);
			recvqnum++;

			//cout << __LINE__ << " Last buffer[" << recvqnum << "]:" << buffer << " strlen buffer:" << strlen(buffer) <<endl;

			//cerr << __LINE__ << " Remaining alarm time:" << ar << endl;
		    	break;	
		}
		else if(strcmp(buffer, "@@@@") == 0)
		{
			int ar = alarm(0);
			break;
		}

		if (strlen(buffer) > 0)
		{
			recvqnum++;
			//cout << __LINE__ << " buffer[" << recvqnum << "]:" << buffer << " strlen buffer:" << strlen(buffer) << endl;
			fs.write(buffer, strlen(buffer));
			fs.flush();
		}
		
		int ar = alarm(0);
		//cerr << __LINE__ << " Remaining alarm time:" << ar << endl;
	}
	//cout << "Received query data line is:" << recvqnum << endl;
	fs.clear();
	fs.close();
}
#else
			//windows
		
#endif

#ifdef __GNUC__
void
CStringObject::s_pipfun(int signo)
{
	cerr << "signo:" << signo << ", the server does not listen on the target port." << endl;
	return;
}

void 
CStringObject::s_fun(int signo)
{
	cerr << "signo:" << signo << endl;
	alarm(5);
	return;
}
#else
			//windows
		
#endif