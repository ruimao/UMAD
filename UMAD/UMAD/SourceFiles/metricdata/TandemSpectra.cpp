/** @file

* @brief TandemSpectra represents a given spectra and its attached precursor mass.

* @author Qihang Li,Zhiyong Xu

* @date 2013.1.26

* @version <version number>

* @note

* detailed description

*/



#include "../../HeaderFiles/metricdata/TandemSpectra.h"



#include <iomanip>

#include <fstream>

#include <vector>

#include <cmath>

#include <string>

using namespace std;



/**

* Necessary for readExternal() and writeExternal().

*/

CTandemSpectra::CTandemSpectra(void)

{



}



/**

* @brief Constructs a TandemSpectra object from a {@link String representing

* the tandem spectra. The only difference between a Spectra and a

* TandemSpectra is that a TandemSpectra includes the precursor mass.

* 

* @param _data

*        

* @param _length

* 

* @param _precursorMass

*        the precursor mass of the spectra itself.

*/

CTandemSpectra::CTandemSpectra(double* _data,int _length,double _precursorMass)

{

	precursorMass=_precursorMass;

	length=_length;

	dataList = new double[length];

	for (int i = 0;i<length;i++)

	{

		dataList[i]=_data[i];

	}

}



/**

* @return the precursor mass for this tandem spectra.

*/

double CTandemSpectra::getPrecursorMass()

{

	return precursorMass;

}



/**

* @param[in] fileName the name of a format file.

* @exception <exception-object> <exception description>

* @return a vector stored tandem spectra fragments.

* @note

* This function will get data from a format file, which contain some tandem spectra informations,

*  and then save as a CTandemSpectra type and store in a vector.

* Details:

*/

vector<shared_ptr<CMetricData> >* CTandemSpectra::loadData(string fileName,int maxSize)

{

	ifstream in(fileName);

	int num,  i;

	in >> num;	

	int rowID;

	in>>rowID;



	vector<shared_ptr<CMetricData> > *a =new vector<shared_ptr<CMetricData> >;

	shared_ptr<CMetricData> temp;

	vector< double > *data;

	for(i=0; i<maxSize; i++)

	{	

		data=new vector<double>;



		double precursorMass;

		in>>precursorMass;



		double _data;

		while(in>>_data)

		{

			if(_data-(int)_data==0) break;

			//	cout<<_data<<" ";

			data->push_back(_data);

			//	cout<<data->back();

		}



		//	cout<<endl;



		int length=data->size();



		double* spectra=new double [length];

		int i;

		for(i=0;i<length;i++)

		{

			spectra[i]=data->at(i);

		}



		temp.reset(new CTandemSpectra(spectra,length,precursorMass));

		a->push_back(temp);



	}



	return a;



}



/**

* @return the dataList for this tandem spectra.

*/

double* CTandemSpectra::getSpectra()

{

	return dataList;

}



/**

* @return the length for this tandem spectra.

*/

int	CTandemSpectra::getLen()

{

	return length;

}



/**

* @param [in|out] <parameter-name> <parameter description>

* @exception <exception-object> <exception description>

* @return a string representation of the object.

* @note

* Returns a string representation of the object. In general, the 

* toString method returns a string that 

* "textually represents" this object. The result should 

* be a concise but informative representation that is easy for a 

* person to read.

* It is recommended that all subclasses override this method.

* The toString method for class Object

* returns a string consisting of the name of the class of which the 

* object is an instance, the at-sign character @', and 

* the unsigned hexadecimal representation of the hash code of the 

* object. In other words, this method returns a string equal to the 

* value of:getClass().getName() + '@' + Integer.toHexString(hashCode())

* @remarks <remark text>

*/

void   CTandemSpectra::toString()

{

	//cout<<"data(size=" <<getLen()<<", pMass= " <<precursorMass <<") :[";



	//int i;

	//for(i=0;i<getLen();i++)

	//	cout<<getSpectra()[i]<<",";



	//cout<<"]"<<endl;

	ofstream output("data/spectra_test.txt",ios::app);

	output<<"data(size=" <<getLen()<<", pMass= " << setprecision(12) <<precursorMass <<") :[";



	int i;

	for(i=0;i<getLen();i++)

		output<<setprecision(15)<<getSpectra()[i]<<",";



	output<<"]"<<endl;

}



CTandemSpectra::~CTandemSpectra()

{



}

const int CTandemSpectra::size= 0;



