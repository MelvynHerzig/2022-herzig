#ifndef TUCUXI_SERVER_APIRESPONSEWRITER_H
#define TUCUXI_SERVER_APIRESPONSEWRITER_H

#include "apiresponse.h"
#include "tucucommon/xmldocument.h"
#include "tucucommon/xmlnode.h"
#include "tucucommon/xmlattribute.h"
#include "tucucommon/xmliterator.h"

#include "boost/lexical_cast.hpp"

namespace Tucuxi {
namespace Server {

class ApiResponseWriter
{
public:
    virtual ~ApiResponseWriter() {}

    void addApiResponse(ApiResponse& _apiResponse);
    std::string serialize();

    std::string getXmlApiResponse() const;
    const ApiResponses& getApiResponses() const;

protected:
    virtual Common::XmlNode createResponse(const std::unique_ptr<ApiResponse>& _apiResponse) ;
    Common::XmlNode createIssues(ApiResponse& _apiResponse) ;
    Common::XmlNode createErrors(ApiResponse& _apiResponse) ;
    Common::XmlNode createWarnings(ApiResponse& _apiResponse) ;
    virtual Common::XmlNode createData(const std::unique_ptr<ApiResponse>& _apiResponse) = 0 ;

    std::string dateToString(const Common::DateTime& _datetime) const ;

protected:
    std::string m_xmlApiResponse;
    ApiResponses m_apiResponses;
    Common::XmlDocument m_xmlDocument;
};

class ApiResponseAdjustmentWriter : public ApiResponseWriter
{
protected:
    Common::XmlNode createData(const std::unique_ptr<ApiResponse>& _apiResponse) override ;
};

} // namespace Server
} // namespace Tucuxi

#endif // TUCUXI_SERVER_APIRESPONSEWRITER_H
