#ifndef DOUBLEVECTOR_H

#define DOUBLEVECTOR_H



#include "MetricData.h"

#include "../util/ObjectFactory.h"





#include <fstream>

#include <string>



#include<memory>


#ifdef __GNUC__
#include "../util/parallelindex/transport.h"
#endif


/** @file DoubleVector.h

 * @describe a kind of object

 * @author Fuli Lei

 * @version 2012-12-09

*/





/**

* @class CDoubleVector

* @brief object with liner data structure

* @author Fuli Lei

*

* This class represents space vectors, where each element is a double.

*/

class CDoubleVector :

    public CMetricData

{

public:

    /**@brief none parameter constructor*/

    CDoubleVector();



    /**@brief constructor with two parameters

     * @param data a double array represents the liner structure

     * @param length length of the liner structure

     */

    CDoubleVector(double *data,int length);

    /**@brief destructure*/

    virtual ~CDoubleVector();



    /**@brief load raw data from hard disk file and package the data into a objects of CDoubleVector,then return the vector with all generated objects

     * @param fileName name of the file that contains all the raw data waiting to be load

     * @param maxDataNum maximum number of data list to be load from the file

     * @param dimension length of each data list

     * @return  return a vector contains all the objects generated before.

     */

	static vector<shared_ptr<CMetricData> >* loadData(string fileName,int maxDataNum,int dimension);



    /**@brief get the data list encapsulated in the objects

     * @return return the memory address of the data list

     */

    double* getData() const;

    /**@brief get the length of the data list

     *@return return an value of int represents the length of the data list.

     */

    int getLen() const;



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



private:





    /**length of the data list*/

    int dim;

    /**the data list*/

    double* dataList;



};

#endif