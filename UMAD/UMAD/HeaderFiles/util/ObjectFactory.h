#pragma once



#include <map>

#include <string>



#include "../../HeaderFiles/metricdata/MetricData.h"



using namespace std;



typedef void* (*CreateFuntion)(void);









class CObjectFactory

{

public:

    CObjectFactory(void);

    ~CObjectFactory(void);

    



    static bool objectRegister(string objectName,CreateFuntion );

    static void*  getClassByName(string name);  



private:

    static map<string,CreateFuntion> classMap;



};



