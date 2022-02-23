#ifndef QUERYCREATOR_H
#define QUERYCREATOR_H

#include "tucuquery/queryimport.h"
#include "tucuquery/computingquery.h"
#include "drugmodelselector.h"
#include "xpertdata.h"

#include "tucucore/computingservice/computingrequest.h"


namespace Tucuxi {
namespace Xpert {




class QueryCreator : public Query::QueryImport
{

public:

    QueryCreator();

    // those 2 function's content are copy-pasted from queryimport except the last line, it needs refactoring.
    QueryImport::Status importFromFile(XpertData& _xpertData, const std::string& _filePath);
    QueryImport::Status importFromString(XpertData& _xpertData, const std::string& _xmlStr);


protected:

    Status createQuery (XpertData& _xpertData, Common::XmlDocument& _document);

};




}
}

#endif // QUERYCREATOR_H

