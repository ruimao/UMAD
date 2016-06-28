#ifndef SQAI_OBJECTS_DNA_CLASSIFY_H

#define SQAI_OBJECTS_DNA_CLASSIFY_H



/**@file

 * @brief This is a Index object of DNA.

 * @author Ping Li

 * @date 2016-6-27

 *

 * This class define DNA index object. It load data from a format file.

*/



#include <iostream>

#include <fstream>

#include <string>

#include <vector>

#include <sstream>

#include "MetricData.h"

#include "../util/ObjectFactory.h"


#include <memory>


using namespace std;



/** 

 * The total number of DNA symbols. 

 */

#define DNASYMBOLNUMBER 15



/**

 * @struct

 * @author   Yaoda Liu.

 * @date     2012/3/18

 * @brief This is a struct defined for DNA symbols.

 *

 * This is a struct defined for DNA symbols.

 *  - sid            The unique identity of each DNA symbols.

 *  - abbr           Abbreviation of each symbols.

 *  - description    Full name of each symbols.

*/

struct CDNA_Classify_Symbol{



    /**

     * The unique identity of each DNA symbols.  

     */

    int sid;



    /**

     * Abbreviation of each symbols.

     */

    char abbr;



    /**

     * Full name of each symbols. 

     */

    string description;

};



/**

 * @class

 * @author   Yaoda Liu.

 * @date     2012/3/18

 * @brief    The CDNA class define a DNA tpye to contain DNA information.

 *           the method loadData will load data from a file.

*/

class CDNA_CLASSIFY:

    public CMetricData

{

private:



    /**

     * Record the identity of DNA sequence.

     */

    string _sequenceid;



    /** 

     * The sequence of the DNA.

     */

    string _sequence;



    /**

     * A integer list stored identity numbers of this DNA sequence's symbol. 

     */

    vector<int> _symbolIDs;



    /**

     * Length of this DNA sequence.

     */

    int _size;


public:

	

    /**

     * @brief A static data set stored DNA symbols.

     */

    static CDNA_Classify_Symbol DNASymbols[DNASYMBOLNUMBER];



    /**

     * @brief No parameters constructor. Doing nothing.

     */

    CDNA_CLASSIFY();


    /**

     * @brief This constructor initial DNA sequence.

     * @param sid          Identity string of this DNA sequence.

     * @param sequence     Sequence of this DNA.

     *

     * The first string parameter will be assigned to _sequenceid.

     * The second string parameter will be assigned to _sequence.

     * Property _size will be assigned by _sequence's length.

     * Each characters in sequence will translate into symbol id and stored in _symbolIDs.

    */

    CDNA_CLASSIFY(string sid, string sequence);

	CDNA_CLASSIFY(string sid,vector<int> seq);

    /** 

     * A destructor, do nothing. 

     */

    virtual ~CDNA_CLASSIFY();


    /**

     * @brief   Get the size of this DNA sequence.

     * @return  Return the length of this DNA sequence.

    */

    virtual int getSize()const;



    /**

     * @brief  Get the list of symbolID.

     * @return Return a integer vector contain symbols of this DNA sequence.

    */

     vector<int> getSymbolIDs()const;



    /**

     * @brief A static function to return symbol's ID.

     *        Get the symbol's id according to input symbol character.

     *        This function required a input param which is existed in DNASymbol data set.

     * @param symbol    A symbol waiting to get its ID.

     * @return  Return a int ID which is stand for input char.

    */

    static int getSymbolID(char symbol);


    /**

     * @brief    A static function to load data from a file.

     * This function will get data from a format file, which contain some DNA informations,

     *  and then save as a CDNA type and store in a vector.

     *

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

    static vector<shared_ptr<CMetricData> >* loadData(string filename, int maxSize, int fragmentLength);


    /**

     * @brief

    */

    virtual int writeExternal(ofstream &out);



    /**

     * @brief

    */

    virtual int readExternal(ifstream &in);



    /**

     * @brief This method return a DNA object.

     * @return return a CDNA instance.

    */

    static CreateFuntion getConstructor();

    static void* CreateInstance();

};



#endif

//SQAI_OBJECTS_DNA_H

