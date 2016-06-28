/**@file DNA_Classify.h

* @brief This is a Index object of DNA.

* @author Ping Li

* @date 2016/6/27

*

* This class define DNA index object. It load data from a format file.

*/



#include "../../HeaderFiles/metricdata/DNA_Classify.h"





/** A  static data set stored DNA symbols.  */

CDNA_Classify_Symbol CDNA_CLASSIFY::DNASymbols[DNASYMBOLNUMBER] = {

    {0, 'A', "Adenine"}, {1, 'C', "Cytosine"}, {2, 'G', "Guanie"}, {3, 'T', "Thymine"},

    {4, 'R', "Purine"}, {5, 'Y', "Pyrimidine"}, {6, 'M', "C or A"}, {7, 'K', "T, U, or G"},

    {8, 'W', "T, U or A"}, {9, 'S', "C or G"}, {10, 'G', "not A"}, {11, 'D', "not C"},

    {12, 'H', "not G"}, {13, 'V', "not T, U"}, {14, 'N', "Any base"}

};



/**

* @brief No parameters constructor. Doing nothing.

*/

CDNA_CLASSIFY::CDNA_CLASSIFY(){



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

CDNA_CLASSIFY::CDNA_CLASSIFY(string sid, string sequence)

{

    int i;

    int temp;



    this->_sequenceid = sid;

    this->_sequence = sequence;

    this->_size = this->_sequence.size();



    for(i=0;i<this->_size;i++){

        temp = CDNA_CLASSIFY::getSymbolID(this->_sequence.at(i));

        this->_symbolIDs.push_back(temp);

    }
}

CDNA_CLASSIFY::CDNA_CLASSIFY(string sid,vector<int> seq)
{
	this->_sequenceid = sid;
	 this->_size = seq.size();
	for(int i=0; i<seq.size(); ++i)
	{
		this->_symbolIDs.push_back(seq[i]);
	}
}
/** 

* A destructor, do nothing. 

*/

CDNA_CLASSIFY::~CDNA_CLASSIFY()

{



}



/**

* @return Return the size of DNA sequence.

*/

int CDNA_CLASSIFY::getSize()const{

    return this->_size;

}



/**

* @brief  Get the sysbolID list.

* @return Return a integer vector contain symbols of DNA sequence.

*/

vector<int> CDNA_CLASSIFY::getSymbolIDs()const{

    return this->_symbolIDs;

}



/**

* @brief A static function to return symbol's ID.

*        Get the symbol's id according to input symbol character.

*        This function required a input param which is existed in DNASymbol data set.

* @param symbol    A symbol waiting to get its ID.

* @return  Return a int ID which is stand for input char.

*/

int CDNA_CLASSIFY::getSymbolID(char symbol)

{

    int i;

    for(i=0;i<DNASYMBOLNUMBER;i++)

        if(symbol == DNASymbols[i].abbr)

            return DNASymbols[i].sid;

    return -1;

}

string split(string &s,char delim)
{
	stringstream ss(s);
	string str1;
	getline(ss,str1,delim);
	return str1;
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

vector<shared_ptr<CMetricData> >* CDNA_CLASSIFY::loadData(string filename, int maxSize, int fragmentLength){



    vector<shared_ptr<CMetricData> > *data = new vector<shared_ptr<CMetricData> >;

    vector<shared_ptr<CDNA_CLASSIFY> > dnas;

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

	/* Classification parameter */
	string splitline1;
	/* ************************ */

    while(!infile.eof() && counter < maxSize && sequenceLengthCounter < maxSize)

    {

        infile.getline(buffer, 65);

		/* classification parameter */
        splitline1 = string(buffer);
		line = split(splitline1,' ');
		/* ************************ */

        if(line.size() >= 1)
		{

            if(line.at(0) == '>')
			{

                if(currentSequence.size() != 0)

                {

                    shared_ptr<CDNA_CLASSIFY> temp(new CDNA_CLASSIFY(ident, currentSequence));

                    dnas.push_back(temp);

                    counter += currentSequence.size();

                    currentSequence.clear();

                }

                ident = line;

            }
			else
			{

                currentSequence = currentSequence.append(line);

                sequenceLengthCounter = currentSequence.size();

            }

        }

    }



    if(currentSequence.size() != 0){

        shared_ptr<CDNA_CLASSIFY> temp(new CDNA_CLASSIFY(ident, currentSequence));

        dnas.push_back(temp);

    }



    for(i=0;i<dnas.size();i++){

        shared_ptr<CDNA_CLASSIFY> dna = dnas.at(i);

		int totalSize = maxSize;

        string seq_id = dna->_sequenceid;

        string seq = dna->_sequence;

		for(int j=0;j<totalSize;j+=fragmentLength){

			/* classification parameter */
			shared_ptr<CDNA_CLASSIFY> temp(new CDNA_CLASSIFY(seq_id, seq.substr(j, fragmentLength)));
			/* ************************ */

            data->push_back(temp);

        }

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

int CDNA_CLASSIFY::writeExternal(ofstream &out){

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

int CDNA_CLASSIFY::readExternal(ifstream &in){



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

CreateFuntion CDNA_CLASSIFY::getConstructor()

{

    return &CreateInstance;

}



void* CDNA_CLASSIFY::CreateInstance()

{

    return new CDNA_CLASSIFY();

}

