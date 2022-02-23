#ifndef TUCUXPERTHANDLER_H
#define TUCUXPERTHANDLER_H
#include <string>
#include "tucucore/computingservice/computingrequest.h"


namespace Tucuxi {
namespace Xpert {




class TucuxpertHandler
{

public:

    TucuxpertHandler();

    int handle (const std::string& _filePath, const std::string& _outputFile, const std::string& _drugPath);



};



}
}

#endif // TUCUXPERTHANDLER_H
