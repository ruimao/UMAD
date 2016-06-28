/** @file TimeSeries.cpp

* @describe a kind of object

* @author Ping Li

* @version 2015-5-10

*/

#include "../../HeaderFiles/metricdata/TimeSeries.h"

#include <sstream>

/**@brief none parameter constructor*/

CTimeSeries::CTimeSeries()
{

    dim = 0;

}

/**@brief constructor with two parameters

* @param data a double array represents the liner structure

* @param length length of the liner structure

*/
CTimeSeries::CTimeSeries(vector<TimeSeries> data,int length,int coordinate)
{

    dim = length;

	coord = coordinate;

	//for (int j = 0; j<data.size(); j++)
	//{

	//	//TimeSeries sample;
	//	
	//	//sample = data[j];

	//	dataList.push_back(data[j]);

	//}

	dataList.push_back(data[0]);

	dataList = data;

}

/**@brief destructure*/
CTimeSeries::~CTimeSeries()
{

}


vector<string> CTimeSeries::split(string &s,char delim)
{
	stringstream ss(s);
	string str;
	vector<string> results;
	while (getline(ss,str,delim))
		results.push_back(str);
	return results;
}


double CTimeSeries::str2num(string s)
{   
	double num;
	stringstream ss(s);
	ss >> num;
	return num;
}

/**@brief get the data list encapsulated in the objects

* @return return the memory address of the data list

*/
vector<TimeSeries> CTimeSeries::getData() const
{

    return dataList;

}



/**@brief get the length of the data list

*@return return an value of int represents the length of the data list.

*/
int CTimeSeries::getLen() const
{

    return dim;

}

int CTimeSeries::getCoord() const
{
	return coord;
}

/**@brief load raw data from hard disk file and package the data into a objects of CTimeSeries,then return the vector with all generated objects

* @param fileName name of the file that contains all the raw data waiting to be load

* @param maxDataNum maximum number of data list to be load from the file

* @param dimension length of each data list

* @return  return a vector contains all the objects generated before.

* @Notice: when coord>1 every sample point split by ','

*/

vector<shared_ptr<CMetricData> >* CTimeSeries::loadData(string fileName,int maxDataNum,int dimension)
{

	ifstream in(fileName.c_str(),std::ios_base::in);


    if(!in)
    {
		cout<<__FILE__<<"¡¡"<<__LINE__<<"open raw data file:"<<fileName<<" failed!"<<endl;

        exit(0);
    }

    int num, dim, coordinate, i, j;  //num:the total size of the file. dim:the length of time series. coord:the number of coordinate
 
    vector<shared_ptr<CMetricData> > *a=new vector<shared_ptr<CMetricData> >;

    vector<TimeSeries> data;

    shared_ptr<CTimeSeries> temp=shared_ptr<CTimeSeries>();

    string str="";

    in >> dim >> num >> coordinate;	

    getline(in,str);

    dim = dim>dimension ? dimension:dim;
    num = num>maxDataNum ? maxDataNum:num;

	if(coordinate == 1)
	{
		 for(i=0;i<num;i++)
		{
			TimeSeries sample;

			string point;

			getline(in,str);

			stringstream newStr(str);

			for(j=0; j<dim; j++)
			{

				newStr >> point;
				sample.x_coord = str2num(point);
				sample.y_coord = 0;
				sample.z_coord = 0;
				sample.time = 0;
				data.push_back(sample);
			}

			temp.reset(new CTimeSeries(data, dim,coordinate));

			a->push_back(temp);

			data.clear();

		}
	}

	else
	{
		vector<string> temp1;

		for(int i=0; i<num; ++i)
		{
			TimeSeries sample;

			string point;

			getline(in,str);

			temp1 = split(str,' ');

			for(int i=0; i<(temp1.size()-1); i+=coordinate)
			{
				stringstream newStr;

				TimeSeries smaple;

				if(coordinate == 2)
				{
					newStr << temp1[i];
					newStr >> point;
					smaple.x_coord = str2num(point);
					newStr.clear();

					newStr << temp1[i+1];
					newStr >> point;
					smaple.y_coord = str2num(point);
					newStr.clear();

					smaple.z_coord = 0;
					smaple.time = 0;
					data.push_back(smaple);
				}
				
				else if(coordinate == 3)
				{
					newStr << temp1[i];
					newStr >> point;
					smaple.x_coord = str2num(point);
					newStr.clear();

					newStr << temp1[i+1];
					newStr >> point;
					smaple.y_coord = str2num(point);
					newStr.clear();
					
					newStr << temp1[i+2];
					newStr >> point;
					smaple.z_coord = str2num(point);
					newStr.clear();

					smaple.time = 0;
					data.push_back(smaple);
				}

				else
				{
					newStr << temp1[i];
					newStr >> point;
					smaple.x_coord = str2num(point);
					newStr.clear();

					newStr << temp1[i+1];
					newStr >> point;
					smaple.y_coord = str2num(point);
					newStr.clear();
					
					newStr << temp1[i+2];
					newStr >> point;
					smaple.z_coord = str2num(point);
					newStr.clear();

					newStr << temp1[i+3];
					newStr >> point;
					smaple.time = str2num(point);
					newStr.clear();
					data.push_back(smaple);
				}
		
			}

			temp.reset(new CTimeSeries(data, dim,coordinate));

			a->push_back(temp);
		
			data.clear();

		}
	}

    return a;
}


/**@load a instance of this class from the file stored in hard disk

*@param in input stream,used to read information from hard disk

*@return return the size of information which has been read from the hard disk

*/
int CTimeSeries ::readExternal(ifstream &in)

{
	return 0;
}


/**@brief write the instance of this class to hard disk

     *@param out out putstream,used to write inforamtion to the hard disk

     *@return return the size of information which has been wrote to the hard disk

     */
int CTimeSeries::writeExternal(ofstream &out)
{
	return 0;
}


/**@brief return the name of a instance of this class

*@return return the name of a instance of this class

*/
CreateFuntion CTimeSeries::getConstructor()

{

    CreateFuntion constructor =& CreateInstance;

    return constructor;

}



void* CTimeSeries:: CreateInstance()

{

    return new CTimeSeries();

}