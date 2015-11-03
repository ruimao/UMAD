#ifndef SQAI_OBJECTS_Peptide_H

#define SQAI_OBJECTS_Peptide_H



/**@file

* @brief This is a Index object of Peptide.

* @author Yadi Cai

* @date 2013/5/30

*

* @note

* This class define Peptide index object. It load data from a format file.

*/



#include <iostream>

#include <fstream>

#include <string>

#include <vector>




#include "MetricData.h"

#include "../util/ObjectFactory.h"


#include <memory>

#ifdef __GNUC__
#include "../util/parallelindex/transport.h"
#endif
using namespace std;



/** 

* The total number of Peptide symbols. 

*/

#define PeptideSYMBOLNUMBER 24



/**

* @struct

* @author   Yadi Cai

* @version  1.0

* @date     2013/5/30

* @brief This is a struct defined for Peptide symbols.

* 

* This is a struct defined for Peptide symbols.

*  - sid            The unique identity of each Peptide symbols.

*  - abbr           Abbreviation of each symbols.

*  - description    Full name of each symbols.

*/



struct CPeptideSymbol

{

	/*

	* Identity number of the symbol  

	*/

	int sid;



	/*

	* Abbreviation of the symbol  

	*/

	char abbr;



	/*

	* Description of symbol  

	*/

	string description;

};



/**

* @class

* @author   Yaoda Liu.

* @date     2012/3/18

* @brief    The Cpeptide class define a peptide tpye to contain peptide information.

*           the method loadData will load data from a file.

*/

class CPeptide:

	public CMetricData

{

private:

	/*

	* Record the identity of Peptide sequence.  

	*/

	string _sequenceid;



	/*

	* The sequence of the Peptide.  

	*/

	string _sequence;



	/*

	* A integer list stored identity numbers of this Peptide sequence's symbol.  

	*/

	vector<int> _symbolIDs;



	/*

	* Length of this Peptide sequence.  

	*/

	int _size;



public:

	/*

	* @brief A static data set stored Peptide symbols.  

	*/

	static CPeptideSymbol PeptideSymbols[PeptideSYMBOLNUMBER];



	/**

	* @brief This constructor initial Peptide sequence.

	* @param sid          Identity string of this Peptide sequence.

	* @param sequence     Sequence of this Peptide.

	*

	* @note

	* The first string parameter will be assigned to _sequenceid.

	* The second string parameter will be assigned to _sequence.

	* Property _size will be assigned by _sequence's length.

	* Each characters in sequence will translate into symbol id and stored in _symbolIDs.

	*/

	CPeptide(string sid, string sequence);



	/**

	* @brief   Get the size of this Peptide sequence.

	* @return  Return the length of this Peptide sequence.

	*/

	virtual int getSize()const;



	/**

	* @brief  Get the list of symbolID.

	* @return Return a integer vector contain symbols of this Peptide sequence.

	*/

	vector<int> getSymbolIDs() const;



	/**

	* @brief A static function to return symbol's ID.

	*        Get the symbol's id according to input symbol character.

	*        This function required a input param which is existed in PeptideSymbol data set.

	* @param symbol    A symbol waiting to get its ID.

	* @return  Return a int ID which is stand for input char.

	*/

	static int getSymbolID(char symbol);



	/**

	* @brief    A static function to load data from a file.

	* This function will get data from a format file, which contain some Peptide informations,

	*  and then save as a CPeptide type and store in a vector.

	*

	* @note

	* Firstly, load each Peptide sequence according to the characters from the file,

	*  the char '>' in the format files, stand for the beginning of Peptide,

	*  if the length of total characters more than maxSize the function will

	*  stop loadding.

	* Then, split Peptide sequenct into many pieces, each piece's length is fragmentLength.

	* Finally, save all pieces in a vector, and return this vector.

	* A object defined shared_ptr<T>(a kind of smart pointer, existed in <memory>) will count

	*  how much pointer point to it, and this counting called reference counting.

	*  When reference counting reduces to 0, this object will destory automatically.

	* By using shared_ptr<T>, we can ensure no memory leak.

	* 

	* @param filename           The file path of input data.

	* @param maxSize            The max size of loaded data.

	* @param fragmentLength     The length of each Peptide fragment.

	* @return return a vector stored Peptide fragments.

	*/

	static vector<shared_ptr<CMetricData> >* loadData(string filename, int maxSize, int fragmentLength);



	/*

	* a no parameter constructor, do nothing.  

	*/

	CPeptide();



	/*

	* a destructor, do nothing.  

	*/

	virtual ~CPeptide();



	/**

	* @brief

	*/

	virtual int writeExternal(ofstream &out);



	/**

	* @brief

	*/

	virtual int readExternal(ifstream &in);

	
	#ifdef __GNUC__
	/**@brief 

     *@param 

     *@return 

     */
	static int sendrawdata(int *serverlist, int servernum, char *filename, int size, int fragmentlength);

	/**@brief 

     *@param 

     *@return 

     */
	static int sendquerydata(int *serverlist, int servernum, char *filename, int num, int fragmentlength);



	/**@brief 

     *@param 

     *@return 

     */
	static void recvrawdata(int connfd, string &dataFileName, int dim, int finalSize);



	
	/**@brief write the instance of this class to hard disk

     *@param 

     *@return 

     */
	static void recvquerydata(int connfd, string &queryFileName, int dim, int lastQuery);




    /**@brief return the name of a instance of this class

     *@return return the name of a instance of this class

     */

	void s_fun(int);
	void s_pipfun(int);
	#else
	// windows

	#endif

	/**

	* @brief This method return a DNA object.

	* @return return a CDNA instance.

	*/

	static CreateFuntion getConstructor();

	static void* CreateInstance();



    string getSequence();

};



#endif

//SQAI_OBJECTS_Peptide_H