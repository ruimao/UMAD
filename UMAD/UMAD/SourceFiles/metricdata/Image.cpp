/**@file    

 * @brief   This is a Index object of Image.

 * @author  Arthur Fu

 * @date    2013/05/29

 * @version Bate_0.9

 *

 * detailed description

 * This class define Image index object. 

 * This is the key of Image Object. 

 * This object stores an array of floating numbers representing the key for image object. 

 * It is designed only for MoBIoS image dataset, might not work for other datasets.

*/



int i=0;



#include "../../HeaderFiles/metricdata/Image.h"



/**

 * @brief A constructor.

 * @author Arthur Fu

 * @return none

 * @note

 * detailed description

 * - The m_Feas_Len will be initialized to '66'.

 * - The m_Feas will be initialized to '0'.

 * - The max_Dist_Len will be initialized to '3'. 

 * - The max_Dist will be initialized to '1.0', '60.0', '1.0'.

*/

CImage::CImage()

{

	m_Feas_Len = 66;

	m_Feas = new float[66];

	for(int i = 0 ; i < 66; i++)

		m_Feas[i] =  0.0 ; 



	max_Dist_Len = 3;

	max_Dist = new double[3];

	max_Dist[0]=1.0;

	max_Dist[1]=60.0;

	max_Dist[2]=1.0;

}



/**

 * @brief A constructor.

 * @author Arthur Fu

 * @param FLen : The length of the feas[]

 * @param feas : An array of floats over which the feature values are defined.

 * @return none

 * @note

 * detailed description

 * - The FLen will be initialized to m_Feas_Len. 

 * - The feas will be initialized to m_Feas.

*/

CImage::CImage(int FLen, float *feas)

{

	int i;



	m_Feas_Len = FLen ; 

    m_Feas = new float[m_Feas_Len];

	for ( i = 0; i < m_Feas_Len; i++)

		m_Feas[i] = feas[i];



	max_Dist_Len = 3;

	max_Dist = new double[3];

	max_Dist[0]=1.0;

	max_Dist[1]=60.0;

	max_Dist[2]=1.0;

}



/**

 * @brief A constructor.

 * @author Arthur Fu

 * @param FLen : The length of the feas[]

 * @param DLen : The length of the maxDist[]

 * @param feas : an array of floats over which the feature values are defined.

 * @param maxDist : an array of floats over which the dis values are defined.

 * @return none

 * @note

 *

 * detailed description

 * - The FLen will be initialized to m_Feas_Len. 

 * - The feas will be initialized to m_Feas.

 * - The DLen will be initialized to max_Dist_Len.

 * - The maxDist will be initialized to max_Dist.

*/

CImage::CImage(int FLen, float *feas, int DLen, double *maxDist)

{  

	int i;

	m_Feas_Len = FLen ; 

	max_Dist_Len = DLen;

	

    m_Feas = new float[m_Feas_Len];

	for (i = 0; i < m_Feas_Len; i++)

		m_Feas[i] = feas[i];



	max_Dist = new double[max_Dist_Len];

	for (i = 0; i < max_Dist_Len; i++)

		max_Dist[i] = maxDist[i];

}



/**

 * @brief A destructor, do nothing.

 * @author Arthur Fu

 * Details:

 * - The m_Feas and max_Dist will be deleted.

*/

CImage::~CImage()

{

	delete[] (m_Feas);

	delete[] (max_Dist);

    i++;

   /* cout<<i<<endl;*/

}



/**

 * @brief The method will return the value of index subscript corresponds in m_Feas.

 * @author Arthur Fu

 * @param index : An index will be used to find a value.

 * @return Return the value of index subscript corresponds in m_Feas.

*/

float CImage::getFeature(int index)

{

	return m_Feas[index];

}



/**

 * @brief The method will return the m_Feas_Len.

 * @author Arthur Fu

 * @return Return the m_Feas_Len.

*/

int CImage::getSize()

{

	return m_Feas_Len;

}



/**

 * @brief The method will compare the two object.

 * @author Arthur Fu

 * @param *oThat : The object of CMetricData.

 * @return '0', '1', '-1'.

 * @note

 * detailed description

 *  - The CMetricData types of object will be changed into CImage types.

 *  - Compare the CMetricData types of object to the CImage types of object.

 *  - If "this" is equal to "that", then returns '0'.

 *  - If the length of "this" is larger than the length of "that", then return '-1'.

 *  - If the length of "that" is larger than the length of "this", then return '1'.

 *  - If their length is the same, then compare the features one by one.

 *  - If the feature of "this" is larger than the feature of "that", then return '-1'.

 *  - If the feature of "that" is larger than the feature of "this", then return '1'.

*/

int CImage::compareTo(CMetricData *oThat)

{

	CImage  *that = static_cast<CImage *> (oThat);//将CMetricData对象指针转化为CImage对象指针 



	if (this == that) 

		return 0;



	if (this->m_Feas_Len < that->m_Feas_Len)

		return -1;

	else if (this->m_Feas_Len > that->m_Feas_Len)

		return 1;



	else

	{

		for (int i = 0; i < m_Feas_Len; i++)

		{

			if (this->m_Feas[i] < that->m_Feas[i])

				return -1;

			else if (this->m_Feas[i] > that->m_Feas[i])

				return 1;

		}

		return 0;

	}

}



/**

 * @brief The method will load the datum from the file, and store them in the vector.

 * @author Arthur Fu

 * @param string fileName : The name of the file containing the datum.

 * @param int cImage_Num : The number of datum.

 * @param int feas_Num : The number of features.

 * @return Return vector stored the datum from the file.

 * @note

 * detailed description

 *  - Open the file containing the datum.

 *  - Store 'feas_Num' datum in a temporary array named 'inFeas'.

 *  - Then using the 'inFeas' to create a new object of 'CImage'.

 *  - Add the object of 'CImage' into the vector.

 *  - Return the vector.

*/

vector< shared_ptr<CMetricData> >* CImage::loadData(string fileName , int cImage_Num, int  feas_Num)

{

	ifstream in(fileName.c_str());

	int i,j;

	string imageName;

	//vector<CMetricData*> *datas = new vector<CMetricData*>;

	vector< shared_ptr<CMetricData> > *datas = new vector< shared_ptr<CMetricData> >;



	float* inFeas = NULL;

	//CMetricData* temp = NULL;

	shared_ptr<CMetricData> temp;



	for(i = 0; i < cImage_Num; i++)

	{

		in>>imageName;

		inFeas = new float[feas_Num];

		for(j = 0; j < feas_Num; j++)

		{

			in>>inFeas[j];

		}

		//temp = new CImage(feas_Num,inFeas);

		temp.reset(new CImage(feas_Num,inFeas));

		datas->push_back(temp);

	}

	return datas;

}



/**

 * @brief The method will load the datum from the file, and store them in the vector.

 * @author Arthur Fu

 * @param string fileName : The name of the file containing the datum.

 * @param int cImage_Num : The number of datum.

 * @param int feas_Num : The number of features.

 * @return Return vector stored the datum from the file.

 * @note

 * detailed description

 *  - Open the file containing the datum.

 *  - Store 'feas_Num' datum in a temporary array named 'inFeas'.

 *  - Then using the 'inFeas' to create a new object of 'CImage'.

 *  - Add the object of 'CImage' into the vector.

 *  - Return the vector.

*/

vector< shared_ptr <CMetricData> >* CImage::loadData(string fileName , string fileNameMaxInfo, int cImage_Num, int  feas_Num, int maxDist_Num)

{

	ifstream in(fileName.c_str());

	ifstream inMaxInfo(fileNameMaxInfo.c_str());

	int i,j;

	string imageName;

	//vector<CMetricData*> *datas = new vector<CMetricData*>;

	vector< shared_ptr<CMetricData> > *datas = new vector< shared_ptr<CMetricData> >;



	float* inFeas = NULL;

	double* inMaxDist = NULL;

	//CMetricData* temp = NULL;

	shared_ptr<CMetricData> temp;



	for(i = 0; i < cImage_Num; i++)

	{

		in>>imageName;

		inFeas = new float[feas_Num];

		inMaxDist = new double[maxDist_Num];

		for(j = 0; j < feas_Num; j++)

		{

			in>>inFeas[j];

		}

		for(j = 0; j < maxDist_Num; j++)

		{

			inMaxInfo>>inMaxDist[j];

		}

		//temp = new CImage(feas_Num, inFeas, maxDist_Num, inMaxDist);

		temp.reset(new CImage(feas_Num, inFeas, maxDist_Num, inMaxDist));

		datas->push_back(temp);

	}

	return datas;

}



/**@brief write the instance of this class to hard disk

*@param out out put stream,used to write inforamtion to the hard disk

*@return return the size of information which has been wrote to the hard disk

*/

int CImage::writeExternal(ofstream &out)

{

    int size=0;



    out.write((char*) (&m_Feas_Len),sizeof(int));

    size += sizeof(int);

    

    out.write((char*)m_Feas,m_Feas_Len*sizeof(float));

    size += m_Feas_Len*sizeof(float);



    out.write((char*) (&max_Dist_Len),sizeof(int));

    size += sizeof(int);

    

    out.write((char*)max_Dist,max_Dist_Len*sizeof(double));

    size += max_Dist_Len*sizeof(double);



    return size;



}



/**@load a instance of this class from the file stored in hard disk

*@param in input stream,used to read information from hard disk

*@return return the size of information which has been read from the hard disk

*/

int CImage::readExternal(ifstream &in)

{

    int size=0;



    in.read((char*)(&m_Feas_Len),sizeof(int));    

    size+=sizeof(int);



    //m_Feas = new float[m_Feas_Len];



    in.read((char*)m_Feas,m_Feas_Len*sizeof(float));    

    size+=m_Feas_Len*sizeof(float);





    in.read((char*)(&max_Dist_Len),sizeof(int));    

    size+=sizeof(int);



    //max_Dist = new double[max_Dist_Len];



    in.read((char*)max_Dist,max_Dist_Len*sizeof(double));    

    size+=max_Dist_Len*sizeof(double);

    return size;

    

}



/**@brief return the name of a instance of this class

*@return return the name of a instance of this class

*/

CreateFuntion CImage::getConstructor()

{

    return & CreateInstance;

}



void* CImage:: CreateInstance()

{

    return new CImage();

}

#ifdef __GNUC__
int
CImage::sendrawdata(int *serverlist, int servernum, char *filename, int size)
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
		strcpy(buffer, "over");
		write(serverlist[i], buffer, strlen(buffer));
	}
	
	ifs.close();
}
#else
			//windows
		
#endif

#ifdef __GNUC__
int
CImage::sendquerydata(int *serverlist, int servernum, char *filename, int num)
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
		strcpy(buffer, "over");
		write(serverlist[i], buffer, strlen(buffer));
	}

	ifs.close();
}

#else
			//windows
		
#endif



#ifdef __GNUC__
void 
CImage::recvrawdata(int connfd, string &dataFileName, int dim, int finalSize)
{
	stringstream ss;

	write(connfd, dataFileName.c_str(), strlen(dataFileName.c_str()));

	char buffer[MAXLINE];

	dataFileName.insert(0, "n");
	string ndfs = dataFileName;
	ndfs.insert(0, "../data/");
	fstream fs(ndfs.c_str(), ios::out);
	
	//string firstl;
	//ss.str("");
	//ss.clear();
	//ss << dim << " " << finalSize << "\n";
	//firstl.append(ss.str());
	
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
		if (rlen == 4 && strcmp(buffer, "over") == 0)
		{
			int ar = alarm(0);
			//cerr << __LINE__ << " Remaining alarm time:" << ar << endl;
			break;
		}
		if (rlen > 4 && strcmp(t + (strlen(buffer) - 4), "over") == 0)
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
CImage::recvquerydata(int connfd, string &queryFileName, int dim, int lastQuery)
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
		if (rlen == 4 && strcmp(buffer, "over") == 0)
		{
			int ar = alarm(0);
			//cerr << __LINE__ << " Remaining alarm time:" << ar << endl;
			break;
		}
		if (rlen > 4 && strcmp(t + (strlen(buffer) - 4), "over") == 0)
		{
			int ar = alarm(0);
			fs.write(buffer, strlen(buffer) - 4);
			recvqnum++;

			//cout << __LINE__ << " Last buffer[" << recvqnum << "]:" << buffer << " strlen buffer:" << strlen(buffer) <<endl;

			//cerr << __LINE__ << " Remaining alarm time:" << ar << endl;
		    	break;	
		}
		else if(strcmp(buffer, "over") == 0)
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
CImage::s_pipfun(int signo)
{
	cerr << "signo:" << signo << ", the server does not listen on the target port." << endl;
	return;
}

void 
CImage::s_fun(int signo)
{
	cerr << "signo:" << signo << endl;
	alarm(5);
	return;
}
#else
			//windows
		
#endif