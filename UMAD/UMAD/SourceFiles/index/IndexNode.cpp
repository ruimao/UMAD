#include "../../HeaderFiles/index/IndexNode.h"

/** @file IndexNode.cpp

 * @classes about index

 * @author Fuli Lei

 * @version 2012-12-09

 */





/**none parameter constructor

this constructor will be called to create an instance of class CIndexNode

*/

CIndexNode::CIndexNode()

{



}



/**constructor with one parameter pivots. 

		this constructor will be called to create an instance of class CIndexNode



*@param pivots a vector include all the address of pivots belong to single node in the index structor

*@param height the height of current node

*/

CIndexNode::CIndexNode(vector<shared_ptr<CMetricData> > pivots,int height)

{

	this->pivots=pivots;

	this->height=height;



}



/**destructor

the space of this class's instance will be released if the instance is no longer used in the program

*/

CIndexNode::~CIndexNode()

{



}



/**set the height of this node

*@param degree the value of height of this node

*/

void CIndexNode::setHeight(int height)

{

	this->height=height;

}



/**get the height of this node

* @return the value of height

*/

int CIndexNode::getHeight()

{

	return this->height;

}



/** get the address of a pivot based on the pivot index in the parameter list

*@param pivot pivot index of a pivot in the pivot address vector in the node

*@return a pivot address in the main memory

*/

shared_ptr<CMetricData>  CIndexNode::getPivot(int pivotIndex)

{

	return pivots[pivotIndex];

}



/**one can get all the pivots belongs to a single node, which is internal node or leaf node, through this function.

*@return a vector consists of all the pivot address in a single node

*/

vector<shared_ptr<CMetricData> >& CIndexNode::getPivots()

{

    return pivots;

}



int CIndexNode::writeExternal(ofstream &out)

{

    return 0;

}



int CIndexNode::readExternal(ifstream &in,string &objectType)

{

    return 0;

}