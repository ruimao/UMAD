#include "../../HeaderFiles/index/MVPIndex.h"
//#include "../HeaderFiles/index/ListIndex.h"
#include "../../HeaderFiles/index/FFTPivotSelectionMethod.h"
#include "../../HeaderFiles/index/BalancePartitionMethod.h"
#include "../../HeaderFiles/metricdata/DoubleVector.h"
#include "../../HeaderFiles/metricdistance/EuclideanDistance.h"
#include "../../HeaderFiles/metricdistance/CountedMetric.h"


#include <string>
#include <iostream>
#include <fstream>
using namespace std;


/**
此函数简单的展示了一下开发人员如何使用本系统提供的API来进一步开发，为简单期间，只是用了MVP树索引结构，欧几里得距离类和向量数据类型
开发人员可以自己构造一个距离类和数据类型，采用本系统提供的mvp-tree索引结构或者线性索引结构进行研究
使用本系统大体上分为两步：
1 构建索引
2 在构建的索引中进行查找
*/
//int main()
//{
//	/******************************准备工作*****************************************/
//	string rawDatafileName=".\\data\\2000000rawdata.txt";                      //原始数据所在的文件的文件名
//	string indexFileName=".\\data\\vector-1000-5.db";						   //索引存储的文件名
//	CObjectFactory objectF();
//	CObjectFactory::objectRegister("vector",CDoubleVector::getConstructor());  //采用vector作为数据对象
//	CPartitionMethod *pm=new CBalancePartitionMethod();				           //采用平衡划分方法
//	CPivotSelectionMethod *psm=new CFFTPivotSelectionMethod();                 //采用first farthest支撑点选择方法
//	CountedMetric *m=new CountedMetric(new CEuclideanDistance);			       //采用欧几里得距离
//
//
//
//	/******************************构建索引*****************************************/
//	//1 从文件中读取原始数据,将原始数据封装成CDoubleVector对象，并存入对象列表中
//		vector<shared_ptr<CMetricData> > *dataObjectList=0;					//数据类型是CDoubleVector类型
//		dataObjectList=CDoubleVector::loadData(rawDatafileName,1000,5);
//
//	//2 用获取的对象列表构建索引,调用bulkload函数来实现索引的构建, 构建过程中并且写入硬盘
//		
//		//CIndex *index=new CListIndex(m);										//采用线性索引结构	
//		CIndex *index=new CMVPIndex(*dataObjectList,m,psm,pm,2,3,100);          //采用mvp树索引结构
//		index->setFileName(indexFileName);
//		index->setDataType("vector");
//
//		index->bulkLoad(*dataObjectList,0);
//
//
//
//	/*******************************在索引中查找*************************************/
//	//1 构建查询对象Q
//		//范围查询的目标对象
//		double data[5]={20,30,40,50,10};                                      //给定向量数据
//		shared_ptr<CMetricData> t(new CDoubleVector(data,5));                //将向量数据封装成数据对象
//		//范围查询的范围值
//		double radius=35;                                                     //给定范围查找的范围值
//		//范围查询对象
//		CRangeQuery *q=new CRangeQuery(radius,t);                             //创建查询对象
//	//2 从文件中读取索引作为查询时的数据库
//		//CIndex* newIndex=new CListIndex(m);
//		//newIndex->readExternal(indexFileName);
//		CMVPIndex *newIndex=new CMVPIndex();                                  //创建新的索引指针，将来要直线从文件读取的索引
//		newIndex->setFileName(indexFileName);
//		newIndex->setDataType("vector");
//		newIndex->setMetric(m);
//
//	//3 在读取的数据库中进行查找
//		vector<shared_ptr<CMetricData> > *result=0;
//		result=newIndex->search(q);                                          //调用search方法进行查询
//		cout<<"result.size:"<<result->size()<<endl;
//
//	return 0;
//}