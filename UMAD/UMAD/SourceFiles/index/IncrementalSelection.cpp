/**@file   IncrementalSelection.cpp

 * @brief  select pivots with IncrementalSelection

 * @author WeiPeng Zhang

 * @date   2013-4-18

*/



#include "../../HeaderFiles/index/IncrementalSelection.h"

#include "../../HeaderFiles/metricdata/DoubleVector.h"

/**

 *this is a threshold that uesd to calcuate A and N

 */

const double threshold = 0.85 ; 



CIncrementalSelection::CIncrementalSelection(int a, int n)

{

	this->constantA = a;

	this->constantN = n;

}



CIncrementalSelection::~CIncrementalSelection(){}



vector<int> CIncrementalSelection::selectPivots(CMetricDistance *metric, vector<shared_ptr<CMetricData> > &data, int numPivots)

{

	return selectPivots(metric, data, 0, data.size(), numPivots);

}

vector<int> CIncrementalSelection::selectPivots(CMetricDistance *metric, vector<shared_ptr<CMetricData> > &data, int first, int size, int numPivots)

{

	const int NP = (numPivots > size)?size : numPivots;

	vector<int> pivots(NP,0);  



	//if the number of pivots required is equal to the dataSize then return all the points in the data(duplicate removed).

	if( NP == size)

	{

		for(int i = first; i < size + first; i++)

			pivots[i - first] = i;

		return removeDuplicate( metric , data , pivots);

	}

	

    /*final int A =  (constantA > m) ? m : constantA;

	const int A = ( constantA > ( size * ( size + 1 ) / 2) * threshold ) ? size : constantA;*/



	int       m = ( (size * size / 100) > size ) ? (size*size/100) : size ; 

	const int A = ( constantA > m ) ? m : constantA ;

	const int N = ( constantN > size * threshold ) ? 2:constantN;



	int** setA = new int*[2];

	for(int i = 0; i < 2; i++)

		setA[i] = new int[A];

	

	srand(time(0));

	for(int i = 0; i < 2; i++)

	{

		if(A != size)

		{

			for(int j = 0; j < A; j++)

				setA[i][j] = rand() % size + first;

		}

		else if(A == size)

			for(int j = 0; j < A; j++)

				setA[i][j] = j + first;

	}



	double *D = new double[A];

	for(int i = 0; i < A; i++)

		D[i] = -std::numeric_limits<double>::max();



	double** ND = new double*[N];   //store the distances for the candidates in setN to samples in set A

	for(int i = 0; i < N; i++)

		ND[i] = new double[A];





	int *setN = new int[N];

	double sum = 0;

	double largestSum = -1;

    

	//cout<<"NP:"<<NP<<endl;



	for(int k = 0; k < NP; k++)

	{

		//generate set N

		if(N != size)

		{
			
			for(int i = 0; i < N; i++)

				setN[i] =  rand() % size + first;

		}

		else if(N == size)

			for(int i = 0; i < N; i++)

				setN[i] = i + first;



		//compute ND	

		for(int i = 0; i < N; i++)

		{

			sum = 0;

			for(int j = 0; j < A; j++)

			{

				double temM = 0.0;

				temM = abs(metric->getDistance( (data[setA[0][j]]).get() , (data[setN[i]]).get() ) - metric->getDistance( (data[setA[1][j]]).get() , (data[setN[i]]).get() ) );

				ND[i][j] = (temM > D[j]) ? temM : D[j];

				sum += ND[i][j];

			}



			if(sum > largestSum)

			{

				largestSum = sum;

				pivots.at(k)=i; //stores the largest row of ND, but not the offset of the pivot yet

			}

		}

		for(int l = 0; l < A; l++)

			 D[l] = ND[pivots.at(k)][l];

		pivots.at(k) = setN[pivots[k]]; //now really stores the offset of the pivot



	}

	return removeDuplicate(metric, data, pivots);



}



/**

 *to check the array of pivots, remove the duplicate.

 */

vector<int> CIncrementalSelection::removeDuplicate(CMetricDistance *metric, vector<shared_ptr<CMetricData> > &data,vector<int> pivots)

{

    	const int size = pivots.size();                  



		bool *isDuplicate = new bool[size];

        for (int i=0; i<size; i++)

            isDuplicate[i] = false;

        for (int i=0; i< size-1; i++)

        {

            if (isDuplicate[i])

                continue;

            for (int j=i+1; j<size; j++)

            {

                if (isDuplicate[j])

                    continue;

				if (metric->getDistance((data.at(pivots.at(i))).get(), (data.at(pivots.at(j))).get()) == 0)

                    isDuplicate[j] = true;

            }

        }



        int counter = 0;

        for (int i=0; i< size; i++)

            if (!isDuplicate[i])

			 counter ++;



        if (counter == size)

			return pivots;



        else

        {

            vector<int> temp(counter,0);                        

		    counter = 0;

            for (int i=0; i<size; i++)

                  if (!isDuplicate[i])

                  temp[counter++] = pivots[i];

			return temp;

        }

}