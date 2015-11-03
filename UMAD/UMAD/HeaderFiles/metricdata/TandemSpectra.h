#ifndef TANDEMSPECTRA_H

#define TANDEMSPECTRA_H



/** @file

* @brief TandemSpectra represents a given spectra and its attached precursor mass.

* @author Qihang Li,Zhiyong Xu

* @date 2013.1.26

* @version <version number>

* @note

* detailed description

*/





#include "MetricData.h"



#include <iostream>

#include <fstream>

#include <queue>

#include <vector>

using namespace std;



/**

* @class CTandemSpectra

* @brief TandemSpectra represents a given spectra and its attached precursor mass.

* @author Qihang Li,Zhiyong Xu

* @note

* detailed description

*/

class CTandemSpectra:

	public CMetricData

{

public :



	/**

	* @brief Necessary for readExternal() and writeExternal().

	*/

	CTandemSpectra();



	/**

	* @brief Constructs a TandemSpectra object from a {@link String representing

	* the tandem spectra. The only difference between a Spectra and a

	* TandemSpectra is that a TandemSpectra includes the precursor mass.

	*/

	CTandemSpectra(double *,int,double);



	/**

	* @return the precursor mass for this tandem spectra.

	*/

	double getPrecursorMass();



	/**

	* @return the dataList of the tandem spectra.

	*/

	double* getSpectra();



	/**

	* @return the length for this tandem spectra.

	*/

	int getLen();



	static const int size ;



	/**

	* @brief A static function to load data from a file.

	*/

	static vector<shared_ptr<CMetricData> >* loadData(string ,int);



	/**

	* @return a string representation of the object.

	*/

	void toString();



	/** @brief a destructor, do nothing.  */

	~CTandemSpectra();







private:



	double precursorMass;



	double *dataList;



	int length;



};

#endif