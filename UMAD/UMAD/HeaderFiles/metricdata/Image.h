#ifndef IMAGE_H

#define IMAGE_H



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



#include "MetricData.h"

#include "../util/ObjectFactory.h"



#include <iostream>

#include <fstream>

#include <string>

#include <math.h>


#include <memory>

#ifdef __GNUC__
#include "../util/parallelindex/transport.h"
#endif
using namespace std;



/**

 * @class

 * @brief   An image class to contain the information of image.

 * @author  Arthur Fu

 * @note    

 * detailed description

 * The Image class define a CImage type to contain the information of image.

 *  - The method getFeature(int) will return the index data in 'm_Feas'.

 *  - The method getSize() will return the size of 'm_Feas'.

 *  - The method compareTo(CMetricData*) will compare the object of CMetricData and the one of CImage.

 *  - The method loadData will load data from a file.

*/

class CImage :

	public CMetricData

{

public: 

	/** 

	 * @brief A constructor.

	 */

	CImage();



	/** 

	 * @brief A constructor with the features and the number of features.

	 */

	CImage(int FLen , float *feas); 



	/** 

	 * @brief A constructor with the features and the size of it, as well as the max distances and its size

	 */

	CImage(int FLen , float *feas, int DLen , double *maxDist); 



	/** 

	 * @brief A destructor.

	 */

	virtual ~CImage(void);



	/** 

	 * @brief The method taking an index and returning the index data in 'm_Feas'

	 */

	float getFeature(int index); 



	/** @brief The method returning the size of 'm_Feas'.*/

	int getSize(); 



	/** 

	 * @brief The method comparing the object of CMetricData and the one of CImage.

	 */

	int compareTo(CMetricData *oThat); 



	/** 

	 * @brief The method loadData will load datum from a file and return a vector containing the datum.

	 */

	//static vector<CMetricData*>* loadData(string fileName , int cImage_Num, int  feas_Num);

	static vector< shared_ptr<CMetricData> >* loadData(string fileName , int cImage_Num, int  feas_Num);



	/** 

	 * @brief The method loadData will load datum from a file and return a vector containing the datum.

	 */

	//static vector<CMetricData*>* loadData(string fileName , string fileNameMaxInfo, int cImage_Num, int  feas_Num, int dis_Num);

	static vector< shared_ptr <CMetricData> >* loadData(string fileName , string fileNameMaxInfo, int cImage_Num, int  feas_Num, int dis_Num);

	

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

    static CreateFuntion getConstructor();

    static void* CreateInstance();



private:



	float *m_Feas;///< Floating numbers represent the features for an image.

	int m_Feas_Len;///< The size of m_Feas



	double *max_Dist;///<Double numbers represent the max distance for each feature of an image.

	int max_Dist_Len;///< The size of max_Dist

};



#endif