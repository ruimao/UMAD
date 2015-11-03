#include "../../HeaderFiles/util/ObjectFactory.h"



 



CObjectFactory::CObjectFactory(void)

{

}





CObjectFactory::~CObjectFactory(void)

{

}



map<string,CreateFuntion> CObjectFactory::classMap;



bool CObjectFactory::objectRegister(string name, CreateFuntion constructor)

{

    classMap.insert(pair<string,CreateFuntion>(name,constructor));  

    return true;

}



void* CObjectFactory::getClassByName(string name)

{

    map<string,CreateFuntion>::const_iterator find;  

    find = classMap.find(name);  



    if(find==classMap.end())  

    {  

        return NULL;  

    } 



    else 

    {  



        return find->second();  



    }  



}