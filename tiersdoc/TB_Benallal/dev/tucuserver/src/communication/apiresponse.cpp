#include "apiresponse.h"

namespace Tucuxi {
namespace Server {

using namespace std;

ApiResponseIssues::ApiResponseIssues(ApiResponseIssues& _other)
    : m_status(_other.getStatus())
{
    for (const unique_ptr<ApiResponseMessage>& error : _other.getErrors()) {
        addError(error->getId(), error->getLanguage(), error->getContent());
    }

    for (const unique_ptr<ApiResponseMessage>& warning : _other.getWarnings()) {
        addWarning(warning->getId(), warning->getLanguage(), warning->getContent());
    }
}

void ApiResponseIssues::addError(const std::string& _id, const std::string& _language, const std::string& _content)
{
    if (m_status == ApiResponseStatus::SUCCESS) {
        m_status = ApiResponseStatus::ERROR;
    }

    m_errors.push_back( make_unique<ApiResponseMessage>(_id, _language, _content) );
}

void ApiResponseIssues::addWarning(const std::string& _id, const std::string& _language, const std::string& _content)
{
    m_warnings.push_back( make_unique<ApiResponseMessage>(_id, _language, _content) );
}

ApiResponse::ApiResponse(const std::string& _queryID, const std::string& _requestID)
    : m_queryID(_queryID), m_requestID(_requestID) {}

const ApiResponseIssues& ApiResponse::getResponseIssues() const
{
    return m_responseIssues;
}

std::string ApiResponse::getRequestID() const
{
    return m_requestID;
}

void ApiResponse::setRequestID(const std::string& _requestID)
{
    m_requestID = _requestID;
}

std::string ApiResponse::getQueryID() const
{
    return m_queryID;
}

void ApiResponse::setQueryID(const std::string& _queryID)
{
    m_queryID = _queryID;
}

void ApiResponse::addError(const std::string& _id, const std::string& _language, const std::string& _content)
{
    m_responseIssues.addError(_id, _language, _content);
}

void ApiResponse::addWarning(const std::string& _id, const std::string& _language, const std::string& _content)
{
    m_responseIssues.addWarning(_id, _language, _content);
}

std::vector<Core::FullDosage> ApiResponseAdjustment::getAdjustments() const
{
    return m_adjustments;
}

} // namespace Server
} // namespace Tucuxi
