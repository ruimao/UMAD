#include "../../HeaderFiles/util/logTool.h"

namespace logspace
{
	Logger logObj=Logger::getRoot();

	int _flag_=0;
}

Logger logspace::logToFile()
{
	PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("file.log.properties"));

	return logObj;
}

Logger logspace::logToConsole()
{
	PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("console.log.properties"));
	return logObj;
}