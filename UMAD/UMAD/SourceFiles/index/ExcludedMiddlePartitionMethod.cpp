#include "../../HeaderFiles/index/ExcludedMiddlePartitionMethod.h"







void extract(vector<shared_ptr<CMetricData> > &data,vector<double> &distance,vector<int> &offsets,int start,int target,int first,int type)

{

    shared_ptr<CMetricData> tempObject1;

    shared_ptr<CMetricData> tempObject2;



    double tempd1;

    double tempd2;



    if(type==1)

    {

        /* tempObject1 = data.at(offsets[1]);

        data.at(offsets[1]) = data.at(target+first);

        tempObject2 = data.at(offsets[2]);

        data.at(offsets[2]) = tempObject1;

        data.at(target+first) = tempObject2;*/



        swap(data.at(offsets[1]),data.at(target+first));

        swap(data.at(offsets[2]),data.at(target+first));



        /*  tempd1 = distance.at(offsets[1]-first);

        distance.at(offsets[1]-first)=distance.at(target);

        tempd2 = distance.at(offsets[2]-first);

        distance.at(offsets[2]-first)=tempd1;

        distance.at(target)=tempd2;

        */

        swap(distance.at(offsets[1]-first),distance.at(target));

        swap(distance.at(offsets[2]-first),distance.at(target));

    }

    else

    {

        /* tempObject1 = data.at(offsets[2]);

        data.at(offsets[2]) = data.at(target+first);        

        data.at(target+first) = tempObject1;*/



        swap(data.at(offsets[2]),data.at(target+first));



        /* tempd1 = distance.at(offsets[1]-first);

        distance.at(offsets[1]-first)=distance.at(target);        

        distance.at(target)=tempd1;*/



        swap(distance.at(offsets[2]-first),distance.at(target));

    }

    /*data.insert(data.begin()+start,data.at(target+first));

    data.erase(data.begin()+(target+first+1));

    distance.insert(distance.begin()+start,distance[target]);

    distance.erase(distance.begin()+(target+1));*/



}







/*==============================================================================================================================*/





CExcludedMiddlePartitionMethod::CExcludedMiddlePartitionMethod(void)

{

}





CExcludedMiddlePartitionMethod::~CExcludedMiddlePartitionMethod(void)

{

}



CPartitionResults CExcludedMiddlePartitionMethod::partition(CMetricDistance *metric, const vector<shared_ptr<CMetricData> > &pivots,vector<shared_ptr<CMetricData> > &data,int first,int size,double maxRadius,int numPartitions,int maxLeafSize,double middleProportion,double trisectionRadius,char* selectOptimalPivots,char* partitionByOnePivot)

{



    int i,j,k;



    const int numPivots = pivots.size();



    //double* distance = new double[size]; 



    vector<double> LowerBound(3);

    vector<double> upperBound(3);



    //CDoubleIndexObjectPair* tempPair;







    vector<double> distance(size);

    //distance from pivot to each dataObject



    for(int i=first; i<first+size; i++)

    {

        distance[i-first]= metric->getDistance(pivots.at(0).get(),data.at(i).get());



        //tempPair = new CDoubleIndexObjectPair(metric->getDistance(pivots.at(0),data.at(i)),data[i]);

        //wrapper.push_back(tempPair);

    }







    double midRadius = getMid(distance);

    //the 











    vector<int> offsets(4); 

    offsets.at(0)=first;    

    offsets.at(3)=first+size;



    offsets[1]= offsets[0];









    LowerBound[0]=distance.at(getMin(distance,0,distance.size()));

    upperBound[0]=LowerBound[0];



    for(int i=first;i<size+first;i++)

    { 

        if(distance[i-first]<=midRadius-maxRadius)

        {







            if(distance[i-first]<=LowerBound[0])

            {

                LowerBound[0]=distance[i-first];

            }

            if(distance[i-first]>=upperBound[0])

            {

                upperBound[0]=distance[i-first];

            }







            swap(data.at(offsets[1]),data.at(i));            

            swap(distance[i-first],distance[offsets[1]-first]);



            offsets[1]++;

        }

    }





    offsets[2]=offsets[1];

    LowerBound[1]=midRadius+maxRadius;

    upperBound[1]=LowerBound[1];







    for(int i=offsets[1];i<size+first;i++)

    { 

        if(distance[i-first]>=midRadius+maxRadius)

        {    





            if(distance[i-first]<=LowerBound[1])

            {

                LowerBound[1]=distance[i-first];

            }

            if(distance[i-first]>=upperBound[1])

            {

                upperBound[1]=distance[i-first];

            }







            swap(data.at(offsets[2]),data.at(i));            

            swap(distance[i-first],distance[offsets[2]-first]);



            offsets[2]++;



        }

    }





    //











    for(i=1;abs(offsets[3]-offsets[2])>middleProportion*size&&abs(offsets[3]-offsets[2])>1;i=-i)

    {               

        if(i>0)

        {

            while (abs(offsets[3]-offsets[2])>1)

            {



                j=getMin(distance,offsets[2]-first,offsets[3]-first);





                upperBound[0]=distance[j];

                extract(data,distance,offsets,offsets[1]-first,j,first,i);

                offsets[1]++;

                offsets[2]++;



                k=getMin(distance,j+1,offsets[3]-first);



                if(k>=0&&distance[j]==distance[k])

                {

                    j=k;

                }

                else

                {

                    break;

                }



            }







            //upperBound[0]=distance[j];



            //data.insert(data.begin()+offsets[1],data.at(j+first));

            //data.erase(data.begin()+(j+first+1));

            //distance.insert(distance.begin()+offsets[1],distance[j]);

            //distance.erase(distance.begin()+(j+1));





            //offsets[1]++;

            //offsets[2]++;





        }

        if(i<0)

        { 





            while (abs(offsets[3]-offsets[2])>1)

            {

                j=getMax(distance,offsets[2]-first,offsets[3]-first);



                LowerBound[1]=distance[j];

                extract(data,distance,offsets,offsets[2]-first,j,first,i);            

                offsets[2]++;





                k=getMax(distance,j+1,offsets[3]-first);





                if(k>=0&&distance[j]==distance[k])

                {

                    j=k;

                }

                else

                {

                    break;

                }



            }









            //j=getMax(distance,offsets[2],offsets[3]);



            //LowerBound[1]=distance[j]; 



            ///*  data.insert(data.begin()+offsets[2],data.at(j+first));

            //data.erase(data.begin()+(j+first+1));

            //distance.insert(distance.begin()+offsets[2]-first,distance[j]);

            //distance.erase(distance.begin()+(j+1));    */       





            //extract(data,distance,offsets[2],j,first);



            //offsets[2]++;

        }



    }

    //if proportion of middle set bigger than m , decrease its size;

    LowerBound[2]=distance[getMin(distance,offsets[2]-first,offsets[3]-first)];

    upperBound[2]=distance[getMax(distance,offsets[2]-first,offsets[3]-first)];



    /*data.insert(data.end(),data.begin()+offsets[3],data.begin()+offsets[4]+1);

    data.erase(data.begin()+offsets[3],data.begin()+offsets[4]);    */

    //move all ecluded datas to the end of datalist 











    vector<vector<double> > lowerBounds;	

    lowerBounds.push_back(LowerBound);



    vector<vector<double> > upperBounds;

    upperBounds.push_back(upperBound);



    /*  for(int i=0;i<offsets.size();i++)

    {

    offsets.at(i)-=first;

    }*/



    return CPartitionResults(offsets,lowerBounds,upperBounds);

}



int CExcludedMiddlePartitionMethod::getMin(vector<double> &distance,int begin,int end)

{

    if(end<begin||begin>=distance.size())

        return -1;



    int minIndex=begin;



    double min = distance[begin];







    for (int i = begin;i<end;i++)

    {

        if(distance[i]<=min)

        {

            minIndex=i;

            min = distance[i];

        }

    }



    return minIndex;

}



int CExcludedMiddlePartitionMethod::getMax(vector<double> &distance,int begin,int end)

{

    if(end<begin||begin>=distance.size())

        return -1;



    int maxIndex=begin;



    double max = distance[begin];







    for (int i = begin;i<end;i++)

    {

        if(distance[i]>=max)

        {

            maxIndex=i;

            max = distance[i];

        }

    }



    return maxIndex;

}



double CExcludedMiddlePartitionMethod::getMid(vector<double> distance)

{

    nth_element(distance.begin(),distance.begin()+distance.size()/2,distance.end());

    return distance.at(distance.size()/2);







}



CPartitionResults CExcludedMiddlePartitionMethod:: partition(CMetricDistance *metric, const vector<shared_ptr<CMetricData> > &pivots,vector<shared_ptr<CMetricData> > &data, int first, int size, int numPartitions, int maxLeafSize,double trisectionRadius,char* selectOptimalPivots,char* partitionByOnePivot)

{



    return partition(metric, pivots,data, first,size,_maxRadius,numPartitions,maxLeafSize,_middleProportion,trisectionRadius,selectOptimalPivots,partitionByOnePivot);

}



double CExcludedMiddlePartitionMethod::_middleProportion = 0.2;

double CExcludedMiddlePartitionMethod::_maxRadius = 0.003;



