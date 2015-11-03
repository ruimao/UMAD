/** @file DoubleVector.cpp

* @describe a kind of object

* @author Fuli Lei

* @version 2012-12-09

*/

#include "../../HeaderFiles/metricdata/DoubleVector.h"





#include <sstream>



/**@brief none parameter constructor*/

CDoubleVector::CDoubleVector()

{

    dim=0;

    dataList=0;



}



/**@brief constructor with two parameters

* @param data a double array represents the liner structure

* @param length length of the liner structure

*/

CDoubleVector::CDoubleVector(double *data,int length)

{

    dim = length;

    dataList = new double[dim];

    for (int i = 0;i<dim;i++)

    {

        dataList[i]=data[i];

    }





}



/**@brief destructure*/

CDoubleVector::~CDoubleVector()

{

    delete[](dataList);        

    /*cout<<"release CDoubleVector"<<endl;*/

}



/**@brief get the data list encapsulated in the objects

* @return return the memory address of the data list

*/

double* CDoubleVector::getData() const

{

    return dataList;

}



/**@brief get the length of the data list

*@return return an value of int represents the length of the data list.

*/

int CDoubleVector::getLen() const

{

    return dim;

}





/**@brief load raw data from hard disk file and package the data into a objects of CDoubleVector,then return the vector with all generated objects

* @param fileName name of the file that contains all the raw data waiting to be load

* @param maxDataNum maximum number of data list to be load from the file

* @param dimension length of each data list

* @return  return a vector contains all the objects generated before.

*/

vector<shared_ptr<CMetricData> >* CDoubleVector::loadData(string fileName,int maxDataNum,int dimension)
{

	ifstream in(fileName.c_str(),std::ios_base::in);


    if(!in)
    {

		cout<<__FILE__<<"　"<<__LINE__<<"open raw data file:"<<fileName<<" failed!"<<endl;

        exit(0);

    }



    int num, dim, i, j;

    vector<shared_ptr<CMetricData> > *a=new vector<shared_ptr<CMetricData> >;

    double* data=NULL;

    shared_ptr<CDoubleVector> temp=shared_ptr<CDoubleVector>();

    string str="";

    in >> dim >> num;	

    getline(in,str);

    dim = dim>dimension ? dimension:dim;
    num = num>maxDataNum ? maxDataNum:num;


    for(i=0;i<num;i++)
    {

        getline(in,str);

        stringstream newStr(str);

        data = new double[dim];



        for(j=0; j<dim; j++)
        {

            newStr>>data[j];

        }



        temp.reset(new CDoubleVector(data, dim));

        a->push_back(temp);

    }


    return a;

}


/**@brief write the instance of this class to hard disk

*@param out out putstream,used to write inforamtion to the hard disk

*@return return the size of information which has been wrote to the hard disk

*/

int CDoubleVector::writeExternal(ofstream &out)

{
	/* the size will be written into index file in this function */
    int size=0;

	/* write "dim" into file, "dim" takes up the size of a "int" */
    out.write((char*) (&dim),sizeof(int));

    size += sizeof(int);

    /*for(int i =0;i<dim;i++)

    {

    out.write((char*)(&dataList[i]),sizeof(double));

    size += sizeof(double);

    }*/

	/* write a data object into file, only one data object*/
    out.write((char*)dataList,dim*sizeof(double));

    size += dim*sizeof(double);

    return size;
}



/**@load a instance of this class from the file stored in hard disk

*@param in input stream,used to read information from hard disk

*@return return the size of information which has been read from the hard disk

*/

int CDoubleVector ::readExternal(ifstream &in)

{

    int size=0;     



    in.read((char*)(&dim),sizeof(int));    

    size+=sizeof(int);



    dataList = new double[dim];



    in.read((char*)dataList,dim*sizeof(double));    

    size+=dim*sizeof(double);



    return size;



}



/**@brief return the name of a instance of this class

*@return return the name of a instance of this class

*/

CreateFuntion CDoubleVector::getConstructor()

{

    CreateFuntion constructor =& CreateInstance;

    return constructor;

}



void* CDoubleVector:: CreateInstance()

{

    return new CDoubleVector();

}

#ifdef __GNUC__
int
CDoubleVector::sendrawdata(int *serverlist, int servernum, char *filename, int size)
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

	bzero(buffer, MAXLINE);
	ifs.getline(buffer, MAXLINE);
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
CDoubleVector::sendquerydata(int *serverlist, int servernum, char *filename, int num)
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


	bzero(buffer, MAXLINE);
	ifs.getline(buffer, MAXLINE);
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
CDoubleVector::recvrawdata(int connfd, string &dataFileName, int dim, int finalSize)
{
	stringstream ss;

	write(connfd, dataFileName.c_str(), strlen(dataFileName.c_str()));

	char buffer[MAXLINE];

	dataFileName.insert(0, "n");
	string ndfs = dataFileName;
	ndfs.insert(0, "../data/");
	fstream fs(ndfs.c_str(), ios::out);
	
	string firstl;
	ss.str("");
	ss.clear();
	ss << dim << " " << finalSize << "\n";
	firstl.append(ss.str());
	
	//cout << "Dim and finalSize writen to " << dataFileName << " is " << dim << " and " << finalSize << " respectively." << endl;



	fs.write(firstl.c_str(), firstl.size());
	
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
CDoubleVector::recvquerydata(int connfd, string &queryFileName, int dim, int lastQuery)
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

	string firstl;
	ss.str("");
	ss.clear();
	ss << dim <<" " << lastQuery << "\n";
	firstl.append(ss.str());

	//cout << "Dim and finalSize writen to " << queryFileName << " is " << dim << " and " << lastQuery << " respectively." << endl;

	fs.write(firstl.c_str(), firstl.size());

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
CDoubleVector::s_pipfun(int signo)
{
	cerr << "signo:" << signo << ", the server does not listen on the target port." << endl;
	return;
}

void 
CDoubleVector::s_fun(int signo)
{
	cerr << "signo:" << signo << endl;
	alarm(5);
	return;
}
#else
			//windows
		
#endif