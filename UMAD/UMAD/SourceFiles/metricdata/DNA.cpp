/**@file DNA.h

* @brief This is a Index object of DNA.

* @author Yaoda Liu(2011150337@email.szu.edu.cn)

* @date 2013/3/18

*

* This class define DNA index object. It load data from a format file.

*/



#include "../../HeaderFiles/metricdata/DNA.h"





/** A  static data set stored DNA symbols.  */

CDNASymbol CDNA::DNASymbols[DNASYMBOLNUMBER] = {

    {0, 'A', "Adenine"}, {1, 'C', "Cytosine"}, {2, 'G', "Guanie"}, {3, 'T', "Thymine"},

    {4, 'R', "Purine"}, {5, 'Y', "Pyrimidine"}, {6, 'M', "C or A"}, {7, 'K', "T, U, or G"},

    {8, 'W', "T, U or A"}, {9, 'S', "C or G"}, {10, 'G', "not A"}, {11, 'D', "not C"},

    {12, 'H', "not G"}, {13, 'V', "not T, U"}, {14, 'N', "Any base"}

};



/**

* @brief No parameters constructor. Doing nothing.

*/

CDNA::CDNA(){



}



/**

* @brief This constructor initial DNA sequence.

* @param sid          Identity string of this DNA sequence.

* @param sequence     Sequence of this DNA.

*

* @note

* The first string parameter will be assigned to _sequenceid.

* The second string parameter will be assigned to _sequence.

* Property _size will be assigned by _sequence's length.

* Each characters in sequence will translate into symbol id and stored in _symbolIDs.

*/

CDNA::CDNA(string sid, string sequence)

{

    int i;

    int temp;



    this->_sequenceid = sid;

    this->_sequence = sequence;

    this->_size = this->_sequence.size();



    for(i=0;i<this->_size;i++){

        temp = CDNA::getSymbolID(this->_sequence.at(i));

        this->_symbolIDs.push_back(temp);

    }



}



/** 

* A destructor, do nothing. 

*/

CDNA::~CDNA()

{



}



/**

* @return Return the size of DNA sequence.

*/

int CDNA::getSize()const{

    return this->_size;

}



/**

* @brief  Get the sysbolID list.

* @return Return a integer vector contain symbols of DNA sequence.

*/

vector<int> CDNA::getSymbolIDs()const{

    return this->_symbolIDs;

}



/**

* @brief A static function to return symbol's ID.

*        Get the symbol's id according to input symbol character.

*        This function required a input param which is existed in DNASymbol data set.

* @param symbol    A symbol waiting to get its ID.

* @return  Return a int ID which is stand for input char.

*/

int CDNA::getSymbolID(char symbol)

{

    int i;

    for(i=0;i<DNASYMBOLNUMBER;i++)

        if(symbol == DNASymbols[i].abbr)

            return DNASymbols[i].sid;

    return -1;

}





/**

* @brief    A static function to load data from a file.

* This function will get data from a format file, which contain some DNA informations,

*  and then save as a CDNA type and store in a vector.

*

* @note

* Firstly, load each DNA sequence according to the characters from the file,

*  the char '>' in the format files, stand for the beginning of DNA,

*  if the length of total characters more than maxSize the function will

*  stop loadding.

* Then, split DNA sequenct into many pieces, each piece's length is fragmentLength.

* Finally, save all pieces in a vector, and return this vector.

* A object defined shared_ptr<T>(a kind of smart pointer, existed in <memory>) will count

*  how much pointer point to it, and this counting called reference counting.

*  When reference counting reduces to 0, this object will destory automatically.

* By using shared_ptr<T>, we can ensure no memory leak.

* 

* @param filename           The file path of input data.

* @param maxSize            The max size of loaded data.

* @param fragmentLength     The length of each DNA fragment.

* @return return a vector stored DNA fragments.

*/

vector<shared_ptr<CMetricData> >* CDNA::loadData(string filename, int maxSize, int fragmentLength){


 vector<shared_ptr<CMetricData> > *data = new vector<shared_ptr<CMetricData> >;

    vector<shared_ptr<CDNA> > dnas;

	ifstream infile(filename.c_str(), ios::in);



    if(!infile.is_open()){

        cerr << "Stop! Cannot open the file." << endl;

        return data;

    }



    string ident = "";

    int i;

    int counter = 0;

    int sequenceLengthCounter = 0;

    string currentSequence;

    string line;

    char buffer[65];



    while(!infile.eof() && counter < maxSize && sequenceLengthCounter < maxSize)

    {

        infile.getline(buffer, 65);

        line = string(buffer);

        if(line.size() >= 1){

            if(line.at(0) == '>'){

                if(currentSequence.size() != 0)

                {

                    shared_ptr<CDNA> temp(new CDNA(ident, currentSequence));

                    dnas.push_back(temp);

                    counter += currentSequence.size();

                    currentSequence.clear();

                }

                ident = line;

            }else{

                currentSequence = currentSequence.append(line);

                sequenceLengthCounter = currentSequence.size();

            }

        }

    }



    if(currentSequence.size() != 0){

        shared_ptr<CDNA> temp(new CDNA(ident, currentSequence));

        dnas.push_back(temp);

    }



    for(i=0;i<dnas.size();i++){

        shared_ptr<CDNA> dna = dnas.at(i);

        int totalSize = (dna->getSize() - fragmentLength) > maxSize ? maxSize : (dna->getSize() - fragmentLength);

        string seq_id = dna->_sequenceid;

        string seq = dna->_sequence;

        for(int j=0;j<totalSize;j++){

            shared_ptr<CDNA> temp(new CDNA(seq_id, seq.substr(j, fragmentLength)));

            data->push_back(temp);

        }

		string combineStr = seq.substr(totalSize,fragmentLength) + seq.substr(0,fragmentLength);

		totalSize = combineStr.size() - fragmentLength;

		for(int j=0;j<totalSize;j++)
		{
			shared_ptr<CDNA> temp(new CDNA(seq_id, combineStr.substr(j, fragmentLength)));

            data->push_back(temp);
		}

    }

	if (data->size() > maxSize){

        data->resize(maxSize);

    }


    return data;
}



/**

* @brief Write this CDNA instance into the hard disk.

* The format of file:

*  - The first line is an integer show how long the sequenceid is.

*  - The second line is sequenceid.

*  - The third line is the size of sequence.

*  - The forth line is sequence.

*  - The fifth lines is the data of sysbom IDs.

* @param A file output stream, which used to write the information of this instance into hard disk.

* @return return an integer refers to the size of information has been wrote into the hard disk.

*/

int CDNA::writeExternal(ofstream &out){

    int size = 0;

    int temp;



    temp = _sequenceid.size() + 1;

    out.write((char*) (&(temp)), sizeof(int));

    size += sizeof(int);



    out.write((char*) (_sequenceid.c_str()), (temp) * sizeof(char));

    size += (_sequenceid.size()+1) * sizeof(char);



    temp = _sequence.size() + 1;

    out.write((char*) (&(temp)), sizeof(int));

    size += sizeof(int);



    out.write((char*) (_sequence.c_str()), (temp) * sizeof(char));

    size += sizeof(char) * (_size+1);



    temp = _symbolIDs.size();

    out.write((char*) (&(temp)), sizeof(int));

    size += sizeof(int);



    for (int i=0; i<_symbolIDs.size(); ++i){

        out.write((char*) ((&_symbolIDs.at(i))), sizeof(int));

        size += sizeof(int);

    }



    return size;

}

 

/**

* @brief Read this CDNA instance from the hard disk.

* The format of file:

*  - The first line is an integer show how long the sequenceid is.

- The second line is sequenceid.

- The third line is the size of sequence.

- The forth line is sequence.

- The fifth lines is the data of sysbom IDs.

* @param A file input stream, which used to read the information from hard disk.

* @return return an integer refers to the size of information has been read from the hard disk.

*/

int CDNA::readExternal(ifstream &in){



    _sequence.clear();

    _sequenceid.clear();

    _symbolIDs.clear();



    int size = 0;

    int tempSize;

    int tempSymbol;



    in.read((char*) (&tempSize), sizeof(int));

    size += sizeof(int);



    char* tempSequenceid = new char[tempSize];

    in.read(tempSequenceid, sizeof(char) * tempSize);

    size += sizeof(char) * tempSize;

    _sequenceid = string(tempSequenceid);

    delete [] tempSequenceid;



    in.read((char*) (&tempSize), sizeof(int));

    size += sizeof(int);



    char* tempSequence = new char[tempSize];

    in.read(tempSequence, sizeof(char) * tempSize);

    size += sizeof(char) * tempSize;

    _sequence = string(tempSequence);

    _size = _sequence.size();

    delete [] tempSequence;



    in.read((char*) (&tempSize), sizeof(int));

    size += sizeof(int);



    for (int i=0; i<tempSize; ++i){

        in.read((char*) (&tempSymbol), sizeof(int));

        _symbolIDs.push_back(tempSymbol);

        size += sizeof(int);

    }



    return size;

}



/**

 * @brief return the name of a instance of this class

 * @return return the name of a instance of this class

*/

CreateFuntion CDNA::getConstructor()

{

    return &CreateInstance;

}



void* CDNA::CreateInstance()

{

    return new CDNA();

}

#ifdef __GNUC__
int
CDNA::sendrawdata(int *serverlist, int servernum, char *filename, int size, int fragmentlength)
{
	char buffer[65];
	
	char *nfilename = "../data/";
	joinCharArray(nfilename, filename);

	fstream ifs(nfilename);
	if (!ifs)
	{
		cout << "Failed to open file " << filename << endl;
		return -1;
	}

	//bzero(buffer, 65);
	//ifs.getline(buffer, 65);
	//cout << "first line:" << buffer << endl;
	//cout << __FILE__ << __LINE__ << endl;	
	string temp;

	for (int i=0; i<servernum; i++)
	{
		//cout << __FILE__ << __LINE__ << " serer id:" << i << endl;	
		bzero(buffer, 65);
		read(serverlist[i], buffer, 65);

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
		//	ifs.getline(buffer, 65);
		//}

		//cout << "server[" << i << "] start:" << i * (size / servernum) << " end:" << nsize << endl;

		int temps = nsize - (i * (size / servernum)) + fragmentlength - 1;
		int linenum = 0;
		//cout << __FILE__ << __LINE__ << " temps:" << temps << " size:" << size << " server num:" << servernum << " fragmentlenght:" << fragmentlength << endl;
		
		if ( temp.size() > 0 )
		{
			//pan duan gang kai shi, shi fou yi jing you zi fu chuang le, ru guo you, name xian tian jia yi ge dou hao, yi miang mei yi hang de zi fu shu mu chao guo 65
			temp.append(1, '\n');
			// hang shu jia yi
			linenum++;
		}
		
		while (temp.size() < temps)
		{
			bzero(buffer, 65);
			ifs.getline(buffer, 65);

			if ( (strlen(buffer) >= 65) && (buffer[0] != '>') )
			{
				cout << "buffer overflow" << endl;
				return -1;
			}

			if (buffer[0] == '>' || strlen(buffer) == 0)
			{
				continue;
			}
			else
			{
				temp.append(buffer);
				
				//zhi you zai fa song de zi fu chuang zhong jian cai xu yao hui che jiang
				if (temp.size() < temps)
				{
					temp.append(1, '\n');
					temps = temps + 1;
				}
				
				linenum ++;
			}
		}
		//cout << __FILE__ << __LINE__ << " linenum:" << linenum << " new temp size:" << temps <<  endl;	
		char * nb = new char[temps + 1];
		string subtemp = temp.substr(0, temps);
		
		//cout << __FILE__ << __LINE__ << " temp.size:" << temp.size() << " subtemp.size:" << subtemp.size() << endl;

		strcpy(nb, subtemp.c_str());
		//cout << __FILE__ << __LINE__ << endl;
		nb[temps] = '\n';
		//cout << __FILE__ << __LINE__ << endl;
		
		int len = write(serverlist[i], nb, temps+1);
		if (len != (temps + 1))
		{
			cout << "Failed to write data to server " << i << endl;
			return -1;
		}

		cout << "Number of data line sent to server " << i << " is: " << linenum << ", charactor has been sent is:" << len << ". File location now is:" << ifs.tellg() << endl;

		sleep(1);
		bzero(buffer, 65);
		strcpy(buffer, "over");
		write(serverlist[i], buffer, strlen(buffer));

		//que bao temps de zui hou fragmentlength -1 ge zi fu dou wei DNA zi fu, er bu bao han hui che.
		for (int i=temps - (fragmentlength -1); i<temps; i++)
		{
			if (temp[i] == '\n')
			{
				temp.erase(i, 1);
				temps --;
				i = temps - (fragmentlength - 1);
			}
		}
		//erase 0 to temps of temp
		temp.erase(0, temps - (fragmentlength - 1) );
		//erase temp's last element '\n'
		if ( temp[temp.size() - 1 ] == '\n' )
		{
			temp.erase(temp.size() - 1, 1);
		}
	}
	
	ifs.close();
}
#else
			//windows
		
#endif

#ifdef __GNUC__
int
CDNA::sendquerydata(int *serverlist, int servernum, char *filename, int num, int fragmentlength)
{
	char buffer[65];

	char *nfilename = "../data/";
	joinCharArray(nfilename, filename);
	
	fstream ifs(nfilename);
	if (!ifs)
	{
		cout << "Failed to open file " << filename << endl;
		return -1;
	}


	//bzero(buffer, 65);
	//ifs.getline(buffer, 65);
	//cout << "first line:" << buffer << endl;
	string temp;
	int temps = num + fragmentlength - 1;
	int linenum = 0;

	while (temp.size() < temps)
	{
		bzero(buffer, 65);
		ifs.getline(buffer, 65);

		if ( (strlen(buffer) >= 65) && (buffer[0] != '>') )
		{
			cout << "buffer overflow" << endl;
			return -1;
		}

		if (buffer[0] == '>' || strlen(buffer) == 0)
		{
			continue;
		}
		else
		{
			temp.append(buffer);
				
			//zhi you zai fa song de zi fu chuang zhong jian cai xu yao hui che jiang
			if (temp.size() < temps)
			{
				temp.append(1, '\n');
				temps = temps + 1;
			}
				
			linenum ++;
		}
	}

	//cout << __FILE__ << __LINE__ << " linenum:" << linenum << " new temp size:" << temps <<  endl;	
	char * nb = new char[temps + 1];
	string subtemp = temp.substr(0, temps);
	//cout << __FILE__ << __LINE__ << " temp.size:" << temp.size() << " subtemp.size:" << subtemp.size() << endl;

	strcpy(nb, subtemp.c_str());
	nb[temps] = '\n';

	for (int i=0; i<servernum; i++)
	{
		bzero(buffer, 65);
		read(serverlist[i], buffer, 65);

		if (strcmp(buffer, filename) != 0)
		{
			cout << "Filename from server is not equal to the one send to it" << endl;
			return -1;
		}

		int len = write(serverlist[i], nb, (temps + 1));
		if (len != (temps + 1))
		{
			cout << "Failed to write data to server " << i << endl;
			return -1;
		}

		cout << "Number of data line sent to server " << i << " is: " << linenum << ", charactor has been sent is:" << len << ". File location now is:" << ifs.tellg() << endl;

		bzero(buffer, 65);
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
CDNA::recvrawdata(int connfd, string &dataFileName, int dim, int finalSize)
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
CDNA::recvquerydata(int connfd, string &queryFileName, int dim, int lastQuery)
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
CDNA::s_pipfun(int signo)
{
	cerr << "signo:" << signo << ", the server does not listen on the target port." << endl;
	return;
}

void 
CDNA::s_fun(int signo)
{
	cerr << "signo:" << signo << endl;
	alarm(5);
	return;
}
#else
			//windows
		
#endif