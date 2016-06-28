#ifndef TIME_SERIES_H
#define TIME_SERIES_H

/**@file

 * @brief This is a Data object of TimeSeries.

 * @author Ping Li

 * @date 2015-5-10

 * This class define Data object. It load data from a format file.

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

struct TimeSeries
{
	double x_coord;  // longitude
	double y_coord;  // latitude
	double z_coord;  // height
	double time;     // sampling time
};

class CTimeSeries: public CMetricData
{
public:

	 /**@brief none parameter constructor*/
    CTimeSeries();


    /**@brief constructor with two parameters

     * @param data a double array represents the liner structure

     * @param length length of the liner structure

     */

    CTimeSeries(vector<TimeSeries> data,int length,int coordinate);

    /**@brief destructure*/

    virtual ~CTimeSeries();



    /**@brief load raw data from hard disk file and package the data into a objects of CTimeSeries,then return the vector with all generated objects

     * @param fileName name of the file that contains all the raw data waiting to be load

     * @param maxDataNum maximum number of data list to be load from the file

     * @param dimension length of each data list

     * @return  return a vector contains all the objects generated before.

     */

	static vector<shared_ptr<CMetricData> >* loadData(string fileName,int maxDataNum,int dimension);



    /**@brief get the data list encapsulated in the objects

     * @return return the memory address of the data list

     */

    vector<TimeSeries> getData() const;

    /**@brief get the length of the data list

     *@return return an value of int represents the length of the data list.

     */

    int getLen() const;

	int getCoord() const;

	static vector<string> split(string &s,char delim);

	static double CTimeSeries::str2num(string s);

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



    /**@brief return the name of a instance of this class

     *@return return the name of a instance of this class

     */

    static CreateFuntion getConstructor();

    static void* CreateInstance();

private:

	/**length of the data list*/
    int dim;

	/**number of the sampling point coordinate*/
	int coord;

    /**the data list*/
	vector<TimeSeries> dataList;
};

#endif