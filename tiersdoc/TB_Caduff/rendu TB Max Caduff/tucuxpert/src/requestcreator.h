#ifndef REQUESTCREATOR_H
#define REQUESTCREATOR_H

#include "tucuquery/querydata.h"
#include "tucuquery/computingquery.h"
#include "drugmodelselector.h"
#include "xpertdata.h"



namespace Tucuxi {
namespace Xpert {


class RequestCreator
{

public:

    RequestCreator();

    int createRequests (XpertData& _xpertData, Query::QueryData*& _pAvailableData );

};




}
}


#endif // REQUESTCREATOR_H
