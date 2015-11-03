#include "../../HeaderFiles/index/MVPInternalNode.h"





/** @file MVPInternalNode.cpp

* @classes about index structure

* @author Fuli Lei

* @version 2012-12-09

*/

/**none parameter constructor*/

CMVPInternalNode::CMVPInternalNode()

{

	/*pivots.clear();*/

}



/**constructor with parameters parameters

@param pivots a vector contains all the memory address of pivots

@param lower this is a two dimension vector contains the maximum distance value from every pivot to the object in each sub-tree. the total number of rows is equal to the number of all pivots in this node while the number of column is equal to the number of child nodes

@param upper this is a two dimension vector contains the maximum distance value from every pivot to the object in each sub-tree. the total number of rows is equal to the number of all pivots in this node while the number of column is equal to the number of child nodes

@param child a sub-tree address vector

@param myHeight the layer of this node

*/

CMVPInternalNode::CMVPInternalNode(vector<shared_ptr<CMetricData> > pivots,vector<vector<double> > lower,vector<vector<double> > upper,vector<shared_ptr<CIndexNode> > child,int myHeight):CIndexNode(pivots,myHeight)
{
	this->child = child;

	this->upper = upper;

	this->lower = lower;

	this->childSize = 0;
}



/**destructor*/

CMVPInternalNode::~CMVPInternalNode()

{

	delete (childAddress);

}



/**get address of a child according the child index

* @return the root address of a child

*/

vector<shared_ptr<CIndexNode> >& CMVPInternalNode::getSubTree()

{

	return this->child;

}





/**get the toltal number of child

* @return return the toltal number of child

*/

int CMVPInternalNode::numChildren()

{

	return child.size();

}



/**get address of a child according the child index

* @return the root address of a child

*/

shared_ptr<CIndexNode> CMVPInternalNode::getChildAddress(int subTreeRootAddress)

{

	return child[subTreeRootAddress];

}

/**get address of a child according the child index

* @return the root address of a child

*/
long CMVPInternalNode::getChildTrueAddress(int childIndex)
{
	return childAddress[childIndex];
}


/**get the lower bound value of every distance range, each element of the contain represent the distance from every object in sub-tree to every pivots

* @param pivotIndex the index of pivot in the pivot list

* @return the double value list of lower bound distance

*/

vector<double>& CMVPInternalNode::getChildLowerRange(int pivotIndex)

{

	return lower[pivotIndex];

}



/**get the upper band value of every distance range, each element of the contain represent the distance from every object in sub-tree to every pivots

* @param pivotIndex the index of pivot in the pivot list

* @return the double value list of upper bound distance

*/

vector<double>& CMVPInternalNode::getChildUpperRange(int pivotIndex)

{

	return upper[pivotIndex];

}



/**traversal from this internal node to its every sub-tree root to search out the proper objects base on the parameters

@param q this object is given by the user according which kind of query does the user want to do later, there is some basic information packaged in the object  like the search radius if the user want do a range query search latter.

@param metric this object will be use to calculate the distance of two objects

@return the proper objects address list through a vector

*/

vector<CMetricData*>  CMVPInternalNode::search(CRangeQuery &q,CMetricDistance &metric)
{



	vector<CMetricData*> rs;



	vector<CMetricData*> temprs;





	int numpivot = pivots.size();



	double r=q.getRadius();



	double* tempd=new double[numpivot];



	int shouldBeSearched = 1;


	int i;
	unsigned j;


	for(i=0;i<numpivot;i++)
	{

		tempd[i] = metric.getDistance(q.getQueryObject().get(),CIndexNode::getPivot(i).get());

		//tempd[i]: the distance between query and pivot[i]

		if(tempd[i]<=r)

		{

			rs.push_back(pivots[i].get());

		}



	}









	for(j=0;j<child.size();j++)

	{



		shouldBeSearched = 1;



		for(i=0;i<numpivot;i++)

		{





			if(tempd[i]-lower.at(i).at(j)<r&&tempd[i]+r>=upper.at(i).at(j))

			{

				shouldBeSearched=-1;

			}

			else

			{

				shouldBeSearched=1;

			}









			if((tempd[i]+r)<lower.at(i).at(j)||(tempd[i]-r)>upper.at(i).at(j))

			{

				shouldBeSearched =0;

				break;



			}

		}







		if(shouldBeSearched == -1)

		{

			/*child.at(j)->getAlldata(rs);*/

		}

		else if(shouldBeSearched==1)

		{

			temprs = getChildAddress(j)->search(q,metric);

			rs.insert(rs.begin(),temprs.begin(),temprs.end());

		}



	}





	delete(tempd);



	return rs;



}

list<shared_ptr<CMetricData> >  CMVPInternalNode::search(CRangeQuery &q,CMetricDistance &metric,deque<long> *searchlist)
{

    list<shared_ptr<CMetricData> > rs;



    vector<CMetricData*> temprs;





    int numpivot = pivots.size();



    double r=q.getRadius();



    double* tempd=new double[numpivot];



    int shouldBeSearched = 1;







    int i,j;





    for(i=0;i<numpivot;i++)
    {

        tempd[i] = metric.getDistance(q.getQueryObject().get(),CIndexNode::getPivot(i).get());

        if(tempd[i]<=r)

        {

            rs.push_back(pivots[i]);

        }



    }









    for(j=0;j<childSize;j++)
    {



        shouldBeSearched = 1;



        for(i=0;i<numpivot;i++)

        {





            if(tempd[i]+r<lower.at(i).at(j)||tempd[i]-r>upper.at(i).at(j))

            {

                shouldBeSearched=0;

				break;

            }



        }



	if(shouldBeSearched)

	     searchlist->push_back(childAddress[j]);







    }





    delete [] tempd;



    return rs;



}

void CMVPInternalNode::search(CRangeQuery &q,CMetricDistance &metric,deque<long> *searchlist,list<shared_ptr<CMetricData> > &re)
{

	int numpivot=pivots.size();



	double r=q.getRadius();

	double *tempd=new double[numpivot];



	int shouldbesearched = 1;



	int i,j;



	for(int i=0;i<numpivot;i++)
	{

		tempd[i]=metric.getDistance(q.getQueryObject().get(),CIndexNode::getPivot(i).get());

		if(tempd[i]<=r)

			re.push_back(pivots[i]);

	}



	for(int i=0;i<childSize;i++)
	{

		shouldbesearched=1;

		for(int j=0;j<numpivot;j++)

		{

			if((tempd[j]+r)<lower[j][i]||(tempd[j]-r)>upper[j][i])

			{

				if((tempd[j]+upper.at(j).at(i))<=r)

                {

                    shouldbesearched=true;

                    break;

                }

                else

                {

                    shouldbesearched = false;

                    break;

                }

				//shouldbesearched=0;

				//break;

			}

		}



		if(shouldbesearched)

		{

				searchlist->push_back(childAddress[i]);

		}

	}



	delete [] tempd;

}


void CMVPInternalNode::getAlldata(vector<shared_ptr<CMetricData> > &result)
{

	int i;



	for (i=0;i<pivots.size();i++)

	{

		result.push_back(pivots.at(i));

	}



	for (i=0;i<child.size();i++)

	{

		child.at(i)->getAlldata(result);

	}





}



void CMVPInternalNode::travelSearch(CRangeQuery &q,CMetricDistance &metric,vector<shared_ptr<CMetricData> > &result)
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



	for(j=0;j<child.size();j++)

	{



		child.at(j)->travelSearch(q,metric,result);

	}







}



int CMVPInternalNode ::writeExternal(ofstream &out)
{
	/* the size of content will be written into file*/
	int size = 0;

	int i, j, tempsize;

	/* write the number of child into index file */
	out.write((char*)(&childSize),sizeof(int));

	size += sizeof(int);

	/* write all children address */
	out.write((char*)childAddress,childSize*sizeof(long));

	size+=childSize*sizeof(long);
	
	/* write this node height */
	out.write((char*)(&height),sizeof(int));

	size+=sizeof(int);


	
	/* write the number of pivots */
	tempsize = pivots.size();
	out.write((char*)(&tempsize),sizeof(int));

	size+=sizeof(int);

	/* write all pivots */
	for(i=0;i<pivots.size();i++)
	{
		size+=pivots.at(i)->writeExternal(out);//this writeExternal is in like : DoubleVector.cpp
	}

	/* write upper and lower */
	int lineSize = upper.size();//

	int rowSize = upper.at(0).size();

	out.write((char*)(&lineSize),sizeof(int));

	out.write((char*)(&rowSize),sizeof(int));

	size+=2*sizeof(int);

	for(i=0;i<lineSize;i++)
	{     
		for(j=0;j<rowSize;j++)
		{

			out.write((char*)(&lower.at(i).at(j)),sizeof(double));

			out.write((char*)(&upper.at(i).at(j)),sizeof(double));

			size+=2*sizeof(double);

		}
	}

	return size;
}



int CMVPInternalNode ::readExternal(ifstream &in,string &objectType)
{

	int size=0;

	int tempSize=0;

	int lineSize=0,rowSize=0;



	int i,j;

	//read children

	in.read((char*)(&childSize),sizeof(int));

	size+=sizeof(int);



	childAddress = new long[childSize];

	in.read((char*)childAddress,childSize*sizeof(long));

	size+=childSize*sizeof(long);



	//read height

	in.read((char*)(&height),sizeof(int));

	size+=sizeof(int);



	//read pivots

	in.read((char*)(&tempSize),sizeof(int));

	size+=sizeof(int); 



	for(i=0;i<tempSize;i++)
	{

		shared_ptr<CMetricData> pivot;



		//CMetricData *obj=(CMetricData*)CObjectFactory::getClassByName(objectType);

		pivot.reset((CMetricData*)CObjectFactory::getClassByName(objectType));

		pivots.push_back(pivot);

		size+=pivots.at(i)->readExternal(in);

	}





	//read upper and lower

	in.read((char*)(&lineSize),sizeof(int));

	in.read((char*)(&rowSize),sizeof(int));

	size+=2*sizeof(int);



	for(i=0;i<lineSize;i++)

	{   

		vector<double> line(rowSize);

		upper.push_back(line);

		lower.push_back(line);



		for(j=0;j<rowSize;j++)

		{

			in.read((char*)(&lower.at(i).at(j)),sizeof(double));

			in.read((char*)(&upper.at(i).at(j)),sizeof(double));

			size+=2*sizeof(double);

		}



	}



	//showMemoryInfo2("testInternal.txt","readEnd");



	return size;

}



long* CMVPInternalNode::getSubTreeAddress()
{

	return childAddress; 

}



long CMVPInternalNode::getSubTreeRootAddress(int subTreeIndex)
{

	return childAddress[subTreeIndex];

}



void CMVPInternalNode::setChildSize(int size)
{

	if(childSize<=0)

	{

		childSize = size;

		childAddress = new long[size];

	}

}



int CMVPInternalNode::getChildeSize()
{

	return childSize;

}

void CMVPInternalNode::searchIndex(CRangeQuery &q,long filePointer,ifstream &in,CMetricDistance& metric,list<shared_ptr<CMetricData> > *rs,string &dataType)
{

	in.seekg(filePointer,ios::beg);



	this->readExternal(in,dataType);



	int numpivot = pivots.size();


	double r=q.getRadius();



	double* tempd=new double[numpivot];


	
	bool shouldBeSearched = true;  //shouldBeSearched=true : the current child node should be searched, i.e. can not exclude by triangle inequality



	int i,j;




	//judge the pivots is or not in range r
	for(i=0;i<pivots.size();i++)
	{

		tempd[i] = metric.getDistance(q.getQueryObject().get(),CIndexNode::getPivot(i).get());

		//tempd[i]: the distance between query and pivot[i]

		if(tempd[i]<=r)
		{

			rs->push_back(pivots[i]);

		}
		else
		{

			/*pivots[i].reset();*/

		}



	}





	//if d(q,p)+d(p,x)<=r, directly insert internal-node's all child nodes into result without distance computation
	bool readChildDirectly = false;  //readChildDirectly=true :  d(q,p)+d(p,x)<=r


	//judge each child nodes is or not be search. if should, recursively search each node; if not should, directly exclude
	for(j=0;j<childSize;j++)
	{


		readChildDirectly = false;

		shouldBeSearched = true;



		for(i=0;i<numpivot;i++)

		{


			
			if((tempd[i]+r)<lower.at(i).at(j) || tempd[i]-r>upper.at(i).at(j))
			//exclude child node by triangle inequality
			{

				shouldBeSearched = false;
				break;


			}

			//judge d(q,p)+d(p,x)<=r is true
			if(tempd[i]+upper.at(i).at(j) <= r)
			{
				readChildDirectly = true;
				break;
			}

		}

		//the current node is the j-th child node of the object to which "this" pointer point 
		if(shouldBeSearched)//current node should be searched
		{

			in.seekg(childAddress[j],ios::beg);  //move file pointer to the position of current node

			char *type = new char[6];

			in.read((char*)type,6*sizeof(char));  //read the type of current node, include two types: INODE(internal node) and LNODE(leaf node)


			if(readChildDirectly)
			{//directly read all data from current node and its child nodes

				if(!strcmp(type,"INODE"))
				{//if current node is an internal node, move it and its child nodes into a queue, the queue is used to visit each node hierarchically

					deque<long> internalQueue;  //the queue is only save internal node, and its elements are a "long" type address of node, we use the address to visit each node

					internalQueue.push_back(childAddress[j]+6*sizeof(char));//push the address of current node into queue

					readInternalNodeDirectly(internalQueue,in,rs,dataType);//use "readInternalNodeDirectly" function to achieve the visit to each node
				}
				else
				{//if current node is a leaf node, directly visit all data in it

					readLeafNodeDirectly(childAddress[j]+6*sizeof(char),in,rs,dataType);
				}
				
			}
			else
			{// "d(q,p)+d(p,x)<=r" is not true

				shared_ptr<CIndexNode> child;

				if(!strcmp(type,"INODE"))
				{
					child.reset(new CMVPInternalNode());
				}
				else
				{
					child.reset(new CMVPLeafNode());
				}

				child->searchIndex(q,childAddress[j]+6*sizeof(char),in,metric,rs,dataType);
			}

			CMemMonitor::updateMem();
			delete(type);

		}



	}

	CMemMonitor::updateMem();

	delete(tempd);

}


/**
* @brief when d(q,p)+d(p,x)<=r, use this function to visit all nodes without distance computation

* @param internalQueue is a queue which saves the address of each internal nodes

* @param in the input file stream of database

* @param rs the result set in range r of Query(q,r)

* @param dataType the type of data

**/
void CMVPInternalNode::readInternalNodeDirectly(deque<long> &internalQueue, ifstream &in, list<shared_ptr<CMetricData> > *rs, string &dataType)
{
	int i = 0, j = 0;
	while(!internalQueue.empty())
	{
		shared_ptr<CMVPInternalNode> internalNode(new CMVPInternalNode());  //create an new internal node, because there is only internal node in queue(internalQueue)

		long filePointer = internalQueue.front();  //get the address of front node in queue

		internalQueue.pop_front();//pop from queue

		in.seekg(filePointer,ios::beg);//find the position of node in input file stream

		internalNode->readExternal(in,dataType);//initial current node

		vector<shared_ptr<CMetricData> > internalPivots = internalNode->getPivots(); //get pivots in current node

		int numpivots = internalPivots.size();// the size of pivots

		//push every pivot into rs(result set) without distance computation
		for(i=0;i<numpivots;i++)
		{
			rs->push_back(internalPivots.at(i));//
		}

		int childSize = internalNode->getChildeSize();//get the number of child

		for(i=0;i<childSize;i++)
		{//visit each child node

			long childAddress_t = internalNode->getChildTrueAddress(i);//get the address of current child node

			in.seekg(childAddress_t,ios::beg); //get the position in stream

			char *type = new char[6];

			in.read((char*)type,6*sizeof(char));//get type of current child node

			if(!strcmp(type,"INODE"))
			{//is an internal node, directly push its address into queue, wait to be visited
				internalQueue.push_back(childAddress_t+6*sizeof(char));
			}
			else
			{//is a leaf node, directly read all data(include pivots and dataObjects) in it
				readLeafNodeDirectly(childAddress_t+6*sizeof(char),in,rs,dataType);
			}
			delete(type);
		}

	}
	CMemMonitor::updateMem();
}

/**
* @brief when d(q,p)+d(p,x)<=r, directly read all data of current leaf node without distance computation

* @param childAddress the address of leaf node

* @param in the input file stream of database

* @param rs the result set in range r of Query(q,r)

* @param dataType the type of data

**/
void CMVPInternalNode::readLeafNodeDirectly(long childAddress, ifstream &in, list<shared_ptr<CMetricData> > *rs, string &dataType)
{
	int j = 0;
	shared_ptr<CMVPLeafNode> leafNode(new CMVPLeafNode()); //create a new leaf node

	in.seekg(childAddress,ios::beg);//find the position in stream

	leafNode->readExternal(in,dataType);//initial leaf node

	vector<shared_ptr<CMetricData> > leafPivots = leafNode->getPivots();//get pivots in node

	//push every pivots into rs
	for(j=0;j<leafPivots.size();j++)
		rs->push_back(leafPivots.at(j));

	vector<shared_ptr<CMetricData> > dataObjects = leafNode->getObjects();//get dataObjects in node

	//push every dataObjects into rs without distance computation
	for(j=0;j<dataObjects.size();j++)
		rs->push_back(dataObjects.at(j));
}




void CMVPInternalNode::searchExternal(CRangeQuery &q,long filePointer,ifstream &in,CMetricDistance &metric,list<shared_ptr<CMetricData> > &rs,deque<long> &childrenAddress,string &dataType)
{
	in.seekg(filePointer,ios::beg);

	this->readExternal(in,dataType);

	int numpivot = pivots.size();

	double r=q.getRadius();

	double* tempd=new double[numpivot];

	bool shouldBeSearched = true;

	int i,j;

	for(i=0;i<pivots.size();i++)

	{

		tempd[i] = metric.getDistance(q.getQueryObject().get(),CIndexNode::getPivot(i).get());

		//tempd[i]: the distance between query and pivot[i]

		if(tempd[i]<=r)
		{

			rs.push_back(pivots[i]);

		}
	}

	//if d(q,p)+d(p,x)<=r, directly insert internal-node's all child nodes into result without distance computation
	bool readChildDirectly = false;  //readChildDirectly=true :  d(q,p)+d(p,x)<=r

	for(j=0;j<childSize;j++)
	{

		shouldBeSearched = true;

		readChildDirectly = false;

		for(i=0;i<numpivot;i++)
		{

			if((tempd[i]+r)<lower.at(i).at(j)||(tempd[i]-r)>upper.at(i).at(j))
			{
				shouldBeSearched = false;
				break;
			}

			//judge d(q,p)+d(p,x)<=r is true
			if(tempd[i]+upper.at(i).at(j) <= r)
			{
				readChildDirectly = true;
				break;
			}

		}

		if(shouldBeSearched)
		{

			in.seekg(childAddress[j],ios::beg);  //move file pointer to the position of current node

			char *type = new char[6];

			in.read((char*)type,6*sizeof(char));  //read the type of current node, include two types: INODE(internal node) and LNODE(leaf node)

			if(readChildDirectly)
			{//directly read all data from current node and its child nodes

				if(!strcmp(type,"INODE"))
				{//if current node is an internal node, move it and its child nodes into a queue, the queue is used to visit each node hierarchically

					deque<long> internalQueue;  //the queue is only save internal node, and its elements are a "long" type address of node, we use the address to visit each node

					internalQueue.push_back(childAddress[j]+6*sizeof(char));//push the address of current node into queue

					readInternalNodeDirectly(internalQueue,in,&rs,dataType);//use "readInternalNodeDirectly" function to achieve the visit to each node
				}
				else
				{//if current node is a leaf node, directly visit all data in it

					readLeafNodeDirectly(childAddress[j]+6*sizeof(char),in,&rs,dataType);
				}
	
			}
			else
			{// "d(q,p)+d(p,x)<=r" is not true
			
				childrenAddress.push_back(childAddress[j]);
				
			}

		}

	}

	CMemMonitor::updateMem();

	delete(tempd);

}

void CMVPInternalNode::SMTSearchExternal(CRangeQuery &q,long filePointer,ifstream &in,CMetricDistance &metric,list<shared_ptr<CMetricData> > *rs,deque<long> &childrenAddress,string &dataType,mutex &mux,mutex &r_mutex)
{

	mux.lock();

	//showMemoryInfo2("testInternal.txt","searchBegin");

	in.seekg(filePointer,ios::beg);



	this->readExternal(in,dataType);

	mux.unlock();



	int numpivot = pivots.size();



	double r=q.getRadius();



	double* tempd=new double[numpivot];



	bool shouldBeSearched = true;









	int i,j;





	for(i=0;i<pivots.size();i++)
	{

		tempd[i] = metric.getDistance(q.getQueryObject().get(),CIndexNode::getPivot(i).get());

		//tempd[i]: the distance between query and pivot[i]

		if(tempd[i]<=r)
		{

			r_mutex.lock();

			rs->push_back(pivots[i]);

			r_mutex.unlock();

		}
		else
		{

			//pivots[i].reset();

		}



	}









	for(j=0;j<childSize;j++)
	{



		shouldBeSearched = true;



		for(i=0;i<numpivot;i++)
		{



			if((tempd[i]+r)<lower.at(i).at(j)||(tempd[i]-r)>upper.at(i).at(j))
			{





				if((tempd[i]+upper.at(i).at(j))<=r)
				{

					shouldBeSearched=true;

					break;

				}
				else
				{

					shouldBeSearched = false;

					break;

				}

			}

		}



		if(shouldBeSearched)
		{

			childrenAddress.push_back(childAddress[j]);

		}



	}



	CMemMonitor::updateMem();

	delete [] tempd;



	//showMemoryInfo2("testInternal.txt","searchEnd");

}
