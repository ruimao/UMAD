#ifndef _LOGSPACE_
#define _LOGSPACE_


#include "../../SourceFiles/util/logapi/include/log4cplus/logger.h"
#include "../../SourceFiles/util/logapi/include/log4cplus/configurator.h"
#include "../../SourceFiles/util/logapi/include/log4cplus/helpers/loglog.h"
#include "../../SourceFiles/util/logapi/include/log4cplus/helpers/stringhelper.h"
#include "../../SourceFiles/util/logapi/include/log4cplus/loggingmacros.h"
#include "../../SourceFiles/util/logapi/include/log4cplus/fileappender.h"

using namespace log4cplus;
using namespace log4cplus::helpers;

namespace logspace
{
	
	extern int _flag_;

	Logger logToFile();
	Logger logToConsole();

}

#endif
