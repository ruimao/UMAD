#include "../../HeaderFiles/index/MVPLeafNode.h"

#include <cmath>

/** @file MVPLeafNode.cpp

* @classes about index structure

* @author Fuli Lei

* @version 2012-12-09

*/

/**constructor with no parameters*/

CMVPLeafNode::CMVPLeafNode()

{

}



/**constructor with four parameters.

* @param pivots a vector contains all the address of pivots

* @param dataobjects address list of all the objects composed in this node

* @param distance contains all the distance values from every pivots to each of the object in the object list of this node.the number of rows in this vector is euqual to the number of pivots in this node while the number of column is equal to the length of dataObjects, which is the second parameter of this function

* @param the height of current node

*/

CMVPLeafNode::CMVPLeafNode(vector<shared_ptr<CMetricData> > &pivots,vector<shared_ptr<CMetricData> > &dataObjects, vector<vector<double> > &distance,int height):CIndexNode(pivots,height)

{

    this->dataObjects=dataObjects;

    this->distance=distance;

}



/**destructor*/

CMVPLeafNode::~CMVPLeafNode()

{

    /*dataObjects.clear();

    pivots.clear();*/

}



/**return the distance value list according to the pivot index in the parameter of this function

* @param pivotIndex the index of pivot in the pivot list

*/

vector<double>& CMVPLeafNode::getDataPointPivotDistance(int pivotNumber)

{

    return distance[pivotNumber];

}



/**return the number of objects in this node*/

int CMVPLeafNode::numChildren()

{

    return dataObjects.size();

}



/**return the number of objects in this node*/

shared_ptr<CMetricData>  CMVPLeafNode::getObject(int childIndex)

{

    return dataObjects[childIndex];

}



/**return all the address of object in this node trough a vector*/

vector<shared_ptr<CMetricData> >& CMVPLeafNode::getObjects()

{

    return dataObjects;

}




/**traversal from this internal node to its every child to search out the proper objects base on the parameters

@param q this object is given by the user according which kind of query does the user want to do later, there is some basic information packaged in the object  like the search radius if the user want do a range query search latter.

@param metric this object will be use to calculate the distance of two objects

@return the proper objects address list through a vector

*/
vector<CMetricData*> CMVPLeafNode::search(CRangeQuery &q,CMetricDistance &metric)

{

    vector<CMetricData*> rs;

    int numpivots = pivots.size();

    int i,p=-1;

	unsigned j;

    double* tempd = new double[numpivots];

    double r = q.getRadius();



    for(i=0;i<numpivots;i++)

    {

        tempd[i] = metric.getDistance(pivots[i].get(),q.getQueryObject().get());

        if (tempd[i]<=r)

        {

            rs.push_back(pivots.at(i).get());

            if (tempd[i]==0)

                p = i;

        }



    }



    if (p>=0&&tempd[p]==0)

    {

        for(j=0;j<dataObjects.size();j++)

        {

            if(distance.at(p).at(j)<=r)

                rs.push_back(dataObjects.at(j).get());



        }



        return rs;

    }









    for(i=0;i<dataObjects.size();i++)

    {			

        for(j=0;j<numpivots;j++)

        {           



            if(abs(tempd[j]-distance.at(j).at(i))>r)

            {

                /*dataObjects.at(i).reset();*/

                break;    

            }

            else if(metric.getDistance(dataObjects.at(i).get(),q.getQueryObject().get())<=r)

            {

                rs.push_back(dataObjects.at(i).get());

                break;

            }

            else

            {

               /* dataObjects.at(i).reset()*/;

            }



        }

    }



    delete(tempd);



    return rs;

}

list<shared_ptr<CMetricData> > CMVPLeafNode::search(CRangeQuery &q,CMetricDistance &metric, deque<long> *searchlist)

{

	list<shared_ptr<CMetricData> > rs;

	

	int numpivots=pivots.size();



	double r=q.getRadius();

	double *tempd=new double[numpivots];



	for(int i=0;i<numpivots;i++)

	{

		tempd[i]=metric.getDistance(pivots[i].get(),q.getQueryObject().get());

		if(tempd[i]<=r)

			rs.push_back(pivots.at(i));

	}

	

	int shouldbesearch=1;



	for(int i=0;i<dataObjects.size();i++)

	{

		for(int j=0;j<numpivots;j++)

		{

			if(abs(tempd[j]-distance.at(j).at(i))>r)

			{

				shouldbesearch=0;

				break;

			}

		}

		

		if(shouldbesearch)

		{

			if(metric.getDistance(dataObjects.at(i).get(),q.getQueryObject().get())<=r)

			{

				rs.push_back(dataObjects.at(i));

			}

		}



		shouldbesearch=1;

	}



	delete [] tempd;



	return rs;

}

void CMVPLeafNode::search(CRangeQuery &q,CMetricDistance &metric, deque<long> *searchlist,list<shared_ptr<CMetricData> > &re)

{

	int numpivot=pivots.size();

	double r=q.getRadius();

	int p=-1;

	double *tempd=new double[numpivot];



	for(int i=0;i<numpivot;i++)

	{

		tempd[i]=metric.getDistance(q.getQueryObject().get(),pivots[i].get());

		if(tempd[i]<=r)

		{

			re.push_back(pivots[i]);

			if(tempd[i]==0)

				p=i;



		}

	}



	if(p>=0)

	{

		for(int i=0;i<dataObjects.size();i++)

		{

			if(distance.at(p).at(i)<=r)

			{

				re.push_back(dataObjects.at(i));

			}

		}



		delete [] tempd;

		return;

	}



	int shouldbesearched=1;



	for(int i=0;i<dataObjects.size();i++)

	{

		shouldbesearched=1;

		for(int j=0;j<numpivot;j++)

		{

			if(abs(tempd[j]-distance.at(j).at(i))>r)

			{

				shouldbesearched=0;

				break;

			}



		}



		if(shouldbesearched)

		{

			if(metric.getDistance(dataObjects.at(i).get(),q.getQueryObject().get())<=r)

			{

				re.push_back(dataObjects.at(i));

			}

		}



	}



	delete [] tempd;

}


void CMVPLeafNode::getAlldata(vector<shared_ptr<CMetricData> > &result)

{

    int i;



    for (i=0;i<pivots.size();i++)

    {

        result.push_back(pivots.at(i));

    }



    for (i=0;i<dataObjects.size();i++)

    {

        result.push_back(dataObjects.at(i));

    }





}



void CMVPLeafNode::travelSearch(CRangeQuery &q,CMetricDistance &metric,vector<shared_ptr<CMetricData> > &result)

{

    int numpivot = pivots.size();



    double r=q.getRadius();









    int i,j;





    for(i=0;i<numpivot;i++)

    {



        if(metric.getDistance(q.getQueryObject().get(),CIndexNode::getPivot(i).get())<=r)

        {

            result.push_back(pivots[i]);

        }



    }



    for(j=0;j<dataObjects.size();j++)

    {



        if(metric.getDistance(dataObjects.at(j).get(),q.getQueryObject().get())<=r)

            result.push_back(dataObjects.at(j));

    }







}



int CMVPLeafNode ::writeExternal(ofstream &out)
{
	/* the size of content will be written into file */
    int size=0;

    int i,j,tempsize;    

	/* write height */
    out.write((char*)(&height),sizeof(int));

    size += sizeof(int);

    /* write the number of pivots  */
    tempsize = pivots.size();
    out.write((char*)(&tempsize),sizeof(int));

    size+=sizeof(int);

	/* write pivots */
    for(i=0;i<pivots.size();i++)
    {
        size += pivots.at(i)->writeExternal(out);//this writeExternal function is data-type's, like DoubleVector, see in DoubleVector.cpp
    }

	/*************        write all distances between pivots and data objects          **************/
	/* the number of pivots */
    int lineSize = distance.size();

	/* the number of data objects */
    int rowSize = distance.at(0).size();

    out.write((char*)(&lineSize),sizeof(int));//

    out.write((char*)(&rowSize),sizeof(int));

    size += 2*sizeof(int);

	/* write distance */
    for(i=0;i<lineSize;i++)
    {
        for(j=0;j<rowSize;j++)
        {
            out.write((char*)(&distance.at(i).at(j)),sizeof(double));           

            size += sizeof(double);
        }
    }

	/* write the number of data objects */
    int dataSize = dataObjects.size();

    out.write((char*)&dataSize,sizeof(int));

    size += sizeof(int);

	/* write data objects */
    for(i=0;i<dataSize;i++)
    {
        size += dataObjects.at(i)->writeExternal(out);
    }

    return size;
}



int CMVPLeafNode::readExternal(ifstream &in,string &objectType)

{

    //showMemoryInfo3("testLeaf","readBegin");



    int size=0;

    int tempSize=0;

    int lineSize=0,rowSize=0;



    int i,j;









    //read height

    in.read((char*)(&height),sizeof(int));

    size+=sizeof(int);





    //read pivots

    in.read((char*)(&tempSize),sizeof(int));

    size+=sizeof(int);



    for(i=0;i<tempSize;i++)

    {

        shared_ptr<CMetricData>  pivot;



        pivot.reset((CMetricData*)CObjectFactory::getClassByName(objectType));  //objectType is dataType

        pivots.push_back(pivot);

        size+=pivots.at(i)->readExternal(in);

    }





    //read distance

    in.read((char*)(&lineSize),sizeof(int));

    in.read((char*)(&rowSize),sizeof(int));

    size+=2*sizeof(int);



    for(i=0;i<lineSize;i++)

    {   

        vector<double> line(rowSize);

        distance.push_back(line);



        for(j=0;j<rowSize;j++)

        {

            in.read((char*)(&distance.at(i).at(j)),sizeof(double));

            size+=sizeof(double);

        }

    }





    //read objects

    int dataSize;

    in.read((char*)&dataSize,sizeof(int));

    size+=sizeof(int);







    for(i=0;i<dataSize;i++)

    {

        shared_ptr<CMetricData>  data;



        data.reset( (CMetricData*)CObjectFactory::getClassByName(objectType));

        size+=data->readExternal(in);

        dataObjects.push_back(data);

    }



    //showMemoryInfo3("testLeaf","readEnd");

    return size;

}


void CMVPLeafNode::searchIndex(CRangeQuery &q,long filePointer,ifstream &in,CMetricDistance &metric,list<shared_ptr<CMetricData> > *rs,string &dataType)
{

    in.seekg(filePointer,ios::beg);



    this->readExternal(in,dataType);



    int numpivots = pivots.size();



    int i,j,p=-1;

    double* tempd = new double[numpivots];

    double r = q.getRadius();

    bool shouldBeSearched = true;



    for(i=0;i<numpivots;i++)
    {

        tempd[i] = metric.getDistance(pivots[i].get(),q.getQueryObject().get());

        if (tempd[i]<=r)
        {

			rs->push_back(pivots.at(i));

            if (tempd[i]==0)

                p = i;

        }
        else
        {

            /*pivots.at(i).reset();*/

        }



    }



    if (p>=0&&tempd[p]==0)
    {

        for(j=0;j<dataObjects.size();j++)
        {

            if(distance.at(p).at(j)<=r)

                rs->push_back(dataObjects.at(j));

            else

                /*dataObjects.at(j).reset();*/;

        }



        CMemMonitor::updateMem();



        delete(tempd);

        return;

    }











    for(i=0;i<dataObjects.size();i++)
    {			

        for(j=0;j<numpivots;j++)
        {           



            if(abs(tempd[j]-distance.at(j).at(i))>r)

            {

                shouldBeSearched = false;

                break;

            }

			// if d(q,p)+d(x,p)<=r, then directly insert x into result without distance computation.   q is query object, p is a pivot, x is a object in database
			if(tempd[j]+distance.at(j).at(i)<=r)
			{
				 rs->push_back(dataObjects.at(i));
				 shouldBeSearched = false;
				 break;
			}




        }



        if(shouldBeSearched)
        {

            if(metric.getDistance(dataObjects.at(i).get(),q.getQueryObject().get())<=r)
            {

                rs->push_back(dataObjects.at(i));



            }

        }



        shouldBeSearched = true;



    }

    CMemMonitor::updateMem();

    delete(tempd);



}

void CMVPLeafNode::searchExternal(CRangeQuery &q,long filePointer,ifstream &in,CMetricDistance &metric,list<shared_ptr<CMetricData> > &rs,deque<long> &childrenAddress,string &dataType)
{

    in.seekg(filePointer,ios::beg);

    this->readExternal(in,dataType);

    int numpivots = pivots.size();

    int i,j,p=-1;

    double* tempd = new double[numpivots];

    double r = q.getRadius();

    bool shouldBeSearched = true;

    for(i=0;i<numpivots;i++)
    {

        tempd[i] = metric.getDistance(pivots[i].get(),q.getQueryObject().get());

        if (tempd[i]<=r)
        {

            rs.push_back(pivots.at(i));

            if (tempd[i]==0)

                p = i;

        }


    }

    if (p>=0&&tempd[p]==0)
    {

        for(j=0;j<dataObjects.size();j++)
        {

            if(distance.at(p).at(j)<=r)
                rs.push_back(dataObjects.at(j));

            /* else

            dataObjects.at(j).reset();*/

        }

        CMemMonitor::updateMem();

        delete(tempd);

        return;

    }


    for(i=0;i<dataObjects.size();i++)
    {			

        for(j=0;j<numpivots;j++)
        {           

            if(abs(tempd[j]-distance.at(j).at(i))>r)
            {

                shouldBeSearched = false;                

                break;

            }

			// if d(q,p)+d(x,p)<=r, then directly insert x into result without distance computation.   q is query object, p is a pivot, x is a object in database
			if(tempd[j]+distance.at(j).at(i)<=r)
			{
				 rs.push_back(dataObjects.at(i));
				 shouldBeSearched = false;
				 break;
			}
        }

        if(shouldBeSearched)
        {

            if(metric.getDistance(dataObjects.at(i).get(),q.getQueryObject().get())<=r)
            {

                rs.push_back(dataObjects.at(i));

            }


        }

        shouldBeSearched = true;
    }

    //showMemoryInfo3("testLeafSearchExternal");

    CMemMonitor::updateMem();

    delete(tempd);
    //showMemoryInfo3("testLeaf","searchEnd");

}

void CMVPLeafNode::SMTSearchExternal(CRangeQuery &q,long filePointer,ifstream &in,CMetricDistance &metric,list<shared_ptr<CMetricData> > *rs,deque<long> &childrenAddress,string &dataType,mutex &mux,mutex &r_mutex)
{



	mux.lock();

    in.seekg(filePointer,ios::beg);



    this->readExternal(in,dataType);

	mux.unlock();



    int numpivots = pivots.size();



    int i,j,p=-1;

    double* tempd = new double[numpivots];

    double r = q.getRadius();

    bool shouldBeSearched = true;



    for(i=0;i<numpivots;i++)

    {

        tempd[i] = metric.getDistance(pivots[i].get(),q.getQueryObject().get());

        if (tempd[i]<=r)

        {

			r_mutex.lock();

            rs->push_back(pivots.at(i));

			r_mutex.unlock();

            if (tempd[i]==0)

                p = i;

        }

        /*else

        {

        pivots[i].reset();

        }*/



    }



    if (p>=0&&tempd[p]==0)

    {

        for(j=0;j<dataObjects.size();j++)

        {

            if(distance.at(p).at(j)<=r)

			{

				r_mutex.lock();

                rs->push_back(dataObjects.at(j));

				r_mutex.unlock();

			}

			/* else

            dataObjects.at(j).reset();*/

        }

        CMemMonitor::updateMem();

        delete(tempd);

        return;

    }



    for(i=0;i<dataObjects.size();i++)

    {			

        for(j=0;j<numpivots;j++)

        {           



            if(abs(tempd[j]-distance.at(j).at(i))>r)

            {

                shouldBeSearched = false;                

                //dataObjects.at(i).reset();



                break;

            }





        }



        if(shouldBeSearched)

        {

            if(metric.getDistance(dataObjects.at(i).get(),q.getQueryObject().get())<=r)

            {

				r_mutex.lock();

                rs->push_back(dataObjects.at(i));

				r_mutex.unlock();

            }

            /* else

            {

            dataObjects.at(i).reset();

            }*/



        }



        shouldBeSearched = true;



    }

    //showMemoryInfo3("testLeafSearchExternal");

    CMemMonitor::updateMem();

    delete(tempd);



    //showMemoryInfo3("testLeaf","searchEnd");

}