#ifndef TUCUXI_SERVER_RESPONSE_H
#define TUCUXI_SERVER_RESPONSE_H

#include "tucucore/computingservice/computingresponse.h"

namespace Tucuxi {
namespace Server {

enum class ApiResponseStatus
{
    SUCCESS,
    ERROR
};

class ApiResponseMessage
{
public:
    ApiResponseMessage() : m_id("unknown"), m_language("en"), m_content("") {}
    ApiResponseMessage(const std::string& _id, const std::string& _language, const std::string& _content)
        : m_id(_id), m_language(_language), m_content(_content) {}
    ApiResponseMessage(ApiResponseMessage& _other)
        : m_id(_other.m_id), m_language(_other.m_language), m_content(_other.m_content) {}

    std::string getId() const { return m_id; }
    std::string getLanguage() const { return m_language; }
    std::string getContent() const { return m_content; }
protected:
    std::string m_id;
    std::string m_language;
    std::string m_content;
};

class ApiResponseIssues
{
public:
    ApiResponseIssues() : m_status(ApiResponseStatus::SUCCESS) {}
    ApiResponseIssues(ApiResponseIssues& _other);

    void addError(const std::string& _id, const std::string& _language, const std::string& _content);
    void addWarning(const std::string& _id, const std::string& _language, const std::string& _content);

    ApiResponseStatus getStatus() const { return m_status; }
    void setStatus(const ApiResponseStatus& _status) { m_status = _status; }

    const std::vector< std::unique_ptr<ApiResponseMessage> >& getErrors() const { return m_errors; }
    const std::vector< std::unique_ptr<ApiResponseMessage> >& getWarnings() const { return m_warnings; }

protected:
    ApiResponseStatus m_status;
    std::vector< std::unique_ptr<ApiResponseMessage> > m_errors;
    std::vector< std::unique_ptr<ApiResponseMessage> > m_warnings;
};

class ApiResponse
{
public:
    virtual ~ApiResponse() = default;

    void addError(const std::string& _id, const std::string& _language, const std::string& _content);
    void addWarning(const std::string& _id, const std::string& _language, const std::string& _content);

    std::string getQueryID() const;
    void setQueryID(const std::string& _queryID);

    std::string getRequestID() const;
    void setRequestID(const std::string& _requestID);

    const ApiResponseIssues& getResponseIssues() const;

protected:
    ApiResponse(const std::string& _queryID, const std::string& _requestID);

    std::string m_queryID;
    std::string m_requestID;
    ApiResponseIssues m_responseIssues;
};

typedef std::vector< std::unique_ptr<ApiResponse> > ApiResponses;

class ApiResponseAdjustment : public ApiResponse
{
public:
    ApiResponseAdjustment(const std::string& _queryID, const std::string& _requestID)
        : ApiResponse(_queryID, _requestID) {}

    void addAdjustment(const Core::FullDosage& _adjustment)
    {m_adjustments.push_back(_adjustment);}

    std::vector<Core::FullDosage> getAdjustments() const;

protected:
    std::vector<Core::FullDosage> m_adjustments;

};

} // namespace Server
} // namespace Tucuxi

#endif // TUCUXI_SERVER_RESPONSE_H
