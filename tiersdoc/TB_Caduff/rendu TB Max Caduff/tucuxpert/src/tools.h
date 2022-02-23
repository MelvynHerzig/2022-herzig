#ifndef TOOLS_H
#define TOOLS_H
#include <string>
#include <vector>
#include "tucuquery/computingqueryresponse.h"
#include "tucucommon/datetime.h"


namespace Tucuxi {
namespace Xpert {


class Tools
{
    Tools();
public:

    static const std::string printDate (const Common::DateTime& _date);

    static const std::string printDateTime (const Common::DateTime& _date);

    static const std::string getStrWithPrecedingZero(const int _dateElement);

    template <typename T>
    static const T* getComputedDataWithId (const std::vector<Query::SingleResponseData>& _responses, const std::string& _id) {
        for (const Query::SingleResponseData& resp : _responses ) {
            if (resp.m_computingResponse->getId() == _id) {
                return dynamic_cast<const T*>(resp.m_computingResponse->getData() );
            }
        }
        return nullptr;
    }

};


}
}

#endif // TOOLS_H
