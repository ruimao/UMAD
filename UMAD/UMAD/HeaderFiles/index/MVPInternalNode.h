#ifndef MVPINTERNALNODE_H

#define MVPINTERNALNODE_H



#include "IndexNode.h"

#include "MVPLeafNode.h"



#include <cstring>



#include <list>


#include <memory>


using namespace std;



/** @file MVPInternalNode.h

 * @classes about index structure

 * @author Fuli Lei

 * @version 2012-12-09

 */



/**

* @class CMVPInternalNode

* @brief class about internal nodes in index structure

* @author Fuli Lei

*

* this class packages the basic information of an internal node of mvp-tree.

*/

class CMVPInternalNode:public CIndexNode

{

public:

	/**none parameter constructor*/

	CMVPInternalNode();

	

	/**constructor with parameters parameters

	   @param pivots a vector contains all the memory address of pivots

	   @param lower this is a two dimension vector contains the maximum distance value from every pivot to the object in each sub-tree. the total number of rows is equal to the number of all pivots in this node while the number of column is equal to the number of child nodes

	   @param upper this is a two dimension vector contains the maximum distance value from every pivot to the object in each sub-tree. the total number of rows is equal to the number of all pivots in this node while the number of column is equal to the number of child nodes

	   @param child a sub-tree address vector

	   @param myHeight the layer of this node

	*/

	CMVPInternalNode(vector<shared_ptr<CMetricData> > pivots,vector<vector<double> > lower,vector<vector<double> > upper,vector<shared_ptr<CIndexNode> > child,int myHeight);

	

	/**destructor*/

	virtual ~CMVPInternalNode();



	/**get the toltal number of child

	   @return return the toltal number of child

	 */

	int numChildren();



	/**get address of a child according the child index

		@return the root address of a child

	*/

	shared_ptr<CIndexNode> getChildAddress(int childIndex);




	/**get the address of child according the child index

		@return the address of a child, the value is a "long" type
	**/
	long getChildTrueAddress(int childIndex);

	/**when d(q,p)+d(p,x)<=r, read internal node directly without distance computation**/
	void readInternalNodeDirectly(deque<long> &internalQueue, ifstream &in, list<shared_ptr<CMetricData> > *rs, string &dataType);

	/**when d(q,p)+d(p,x)<=r, read leaf node directly withour distance computation**/
	void readLeafNodeDirectly(long childAddress, ifstream &in, list<shared_ptr<CMetricData> > *rs, string &dataType);



	/**get all the address of sub-trees

	   @return a sub-tree address container

	  */

	vector<shared_ptr<CIndexNode> >& getSubTree();





	/**get the lower bound value of every distance range, each element of the contain represent the distance from every object in sub-tree to every pivots

		@param pivotIndex the index of pivot in the pivot list

		@return the double value list of lower bound distance

	*/

	vector<double>& getChildLowerRange(int pivotIndex);

	/**get the upper band value of every distance range, each element of the contain represent the distance from every object in sub-tree to every pivots

		@param pivotIndex the index of pivot in the pivot list

		@return the double value list of upper bound distance

	*/

	vector<double>& getChildUpperRange(int pivotIndex);

	

	/**traversal from this internal node to its every sub-tree root to search out the proper objects base on the parameters

		@param q this object is given by the user according which kind of query does the user want to do later, there is some basic information packaged in the object  like the search radius if the user want do a range query search latter.

		@param metric this object will be use to calculate the distance of two objects

		@return the proper objects address list through a vector

	*/

	virtual vector<CMetricData*>  search(CRangeQuery &q,CMetricDistance &metric);



    virtual void getAlldata(vector<shared_ptr<CMetricData> > &result);

    virtual void travelSearch(CRangeQuery &q,CMetricDistance &metric,vector<shared_ptr<CMetricData> > &result);



    void setChildSize(int size);

    int getChildeSize();

    long* getSubTreeAddress();

    long getSubTreeRootAddress(int subTreeIndex);



    virtual int writeExternal(ofstream &out);

    virtual int readExternal(ifstream &in,string &objectType);

	virtual void search(CRangeQuery &q,CMetricDistance &metric,deque<long> *searchlist,list<shared_ptr<CMetricData> >&re);
	virtual list<std::shared_ptr<CMetricData> >  search(CRangeQuery &q,CMetricDistance &metric,deque<long> *searchlist);
			
    virtual void searchIndex(CRangeQuery &q,long filePointer,ifstream &in,CMetricDistance &metric,list<shared_ptr<CMetricData> > *result,string &dataType);
			
    virtual void searchExternal(CRangeQuery &q,long filePointer,ifstream &in,CMetricDistance &metric,list<shared_ptr<CMetricData> > &result,deque<long> &childrenAddress,string &dataType);

	void SMTSearchExternal(CRangeQuery &q,long filePointer,ifstream &in,CMetricDistance &metric,list<shared_ptr<CMetricData> > *rs,deque<long> &childrenAddress,string &dataType,mutex &mux,mutex &r_mutex);

private:



	/**this is a sub-tree root addresses list*/

	vector<shared_ptr<CIndexNode> > child;

	/**In this two dimension vector contains the minimum distance value of data objects in each sub-partition to every given pivots. The total  number of rows of this vector is equal to the number of all pivots in this node while the number of column is equal to the number of child nodes in this node*/

	vector<vector<double> > upper;

	/**In this two dimension vector contains the maximum distance value of data objects in each sub-partition to every given pivots. The total  number of rows of this vector is equal to the number of all pivots in this node while the number of column is equal to the number of child nodes in this node*/

	vector<vector<double> > lower;



    long* childAddress;

    int childSize;



  



};

#endif