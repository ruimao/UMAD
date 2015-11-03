#ifndef STRINGOBJECT_H

#define STRINGOBJECT_H



/** @file

* @brief This class is a wrapper of over String to implement IndexObject

* @author Qihang Li,Zhiyong Xu

* @date 2013.3.24

* @version <version number>

* @note

* detailed description

*/



#include "MetricData.h"

#include "../util/ObjectFactory.h"

#include<string>

#include<fstream>

#include<vector>


#include <memory>

#ifdef __GNUC__
#include "../util/parallelindex/transport.h"
#endif

using namespace std;



/**

* @class CStringObject

* @brief This class is a wrapper of over String to implement IndexObject

* @author Qihang Li,Zhiyong Xu

* @note

* detailed description

*/

class CStringObject:public CMetricData

{

private:

	static const long serialVersionUID = 7630078213101669086L;



	string data;







public:

	/**

	* @brief a constructor.

	*/

	CStringObject(void);



	/**

	* @brief Builds an instance from a double array.

	*/

	CStringObject(string line);



	/**

	* @return the data

	*/

	string getData();



	/**

	* @brief A static function to load data from a file.

	*/	

	static vector<shared_ptr<CMetricData> >* loadData(string filename,int maxsize);



	/**@brief write the instance of this class to hard disk

	*@param out out putstream,used to write inforamtion to the hard disk

	*@return return the size of information which has been wrote to the hard disk

	*/

	virtual int writeExternal(ofstream &out);

	/**@load a instance of this class from the file stored in hard disk

	*@param in input stream,used to read information from hard disk

	*@return return the size of information which has been read from the hard disk

	*/

	virtual int readExternal(ifstream &in);

	
	#ifdef __GNUC__
	/**@brief 

     *@param 

     *@return 

     */
	static int sendrawdata(int *serverlist, int servernum, char *filename, int size);

	/**@brief 

     *@param 

     *@return 

     */
	static int sendquerydata(int *serverlist, int servernum, char *filename, int num);



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

	/**@brief return the name of a instance of this class

	*@return return the name of a instance of this class

	*/

	static CreateFuntion getConstructor();

	static void* CreateInstance();

	/** @brief a destructor, do nothing.  */

	virtual ~CStringObject(void);

};



#endif