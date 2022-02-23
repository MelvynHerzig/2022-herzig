#include "apiresponsewriter.h"

namespace Tucuxi {
namespace Server {

using namespace std;
using namespace Common;

void ApiResponseWriter::addApiResponse(ApiResponse& _apiResponse)
{
    m_apiResponses.push_back(make_unique<ApiResponse>(_apiResponse));
}

std::string ApiResponseWriter::serialize()
{
    // Creating root node.
    XmlNode responsesNode = m_xmlDocument.createNode(EXmlNodeType::Element, "responses");

    // Adding children nodes to root.
    for (const unique_ptr<ApiResponse>& response : m_apiResponses) {
        XmlNode responseNode = createResponse(response);
        responsesNode.addChild(responseNode);
    }

    // Adding complete root node to the document.
    m_xmlDocument.setRoot(responsesNode);

    // Setting the xml api response in pretty print format.
    if (m_xmlDocument.toString(m_xmlApiResponse, true)) {
        return m_xmlApiResponse;
    }

    return "Error while serializing the responses!";
}

std::string ApiResponseWriter::getXmlApiResponse() const
{
    return m_xmlApiResponse;
}

const ApiResponses& ApiResponseWriter::getApiResponses() const
{
    return m_apiResponses;
}

XmlNode ApiResponseWriter::createResponse(const std::unique_ptr<ApiResponse>& _apiResponse)
{
    XmlNode responseNode = m_xmlDocument.createNode(EXmlNodeType::Element, "response");

    XmlNode queryIDNode = m_xmlDocument.createNode(EXmlNodeType::Element, "queryID", _apiResponse->getQueryID());
    responseNode.addChild(queryIDNode);

    XmlNode requestIDNode = m_xmlDocument.createNode(EXmlNodeType::Element, "requestID", _apiResponse->getRequestID());
    responseNode.addChild(requestIDNode);

    XmlNode issuesNode = createIssues(*_apiResponse);
    responseNode.addChild(issuesNode);

    XmlNode dataNode = createData(_apiResponse);
    responseNode.addChild(dataNode);

    return responseNode;
}

XmlNode ApiResponseWriter::createIssues(ApiResponse& _apiResponse)
{
    XmlNode issuesNode = m_xmlDocument.createNode(EXmlNodeType::Element, "issues");

    if (_apiResponse.getResponseIssues().getStatus() == ApiResponseStatus::SUCCESS) {
        XmlNode statusNode = m_xmlDocument.createNode(EXmlNodeType::Element, "status", "Success");
        issuesNode.addChild(statusNode);
    } else if (_apiResponse.getResponseIssues().getStatus() == ApiResponseStatus::ERROR) {
        XmlNode statusNode = m_xmlDocument.createNode(EXmlNodeType::Element, "status", "Success");
        issuesNode.addChild(statusNode);
    }

    if (_apiResponse.getResponseIssues().getErrors().size() > 0) {
        XmlNode errorsNode = createErrors(_apiResponse);
        issuesNode.addChild(errorsNode);
    }

    if (_apiResponse.getResponseIssues().getWarnings().size() > 0) {
        XmlNode warningsNode = createWarnings(_apiResponse);
        issuesNode.addChild(warningsNode);
    }

    return issuesNode;
}

XmlNode ApiResponseWriter::createErrors(ApiResponse& _apiResponse)
{
    XmlNode errorsNode = m_xmlDocument.createNode(EXmlNodeType::Element, "errors");

    for (const unique_ptr<ApiResponseMessage>& error : _apiResponse.getResponseIssues().getErrors()) {
        XmlNode errorNode = m_xmlDocument.createNode(EXmlNodeType::Element, "error");

        XmlNode idNode = m_xmlDocument.createNode(EXmlNodeType::Element, "id", error->getId());

        XmlAttribute languageAttribute = m_xmlDocument.createAttribute("lang", error->getLanguage());
        idNode.addAttribute(languageAttribute);
        errorNode.addChild(idNode);

        XmlNode messageNode = m_xmlDocument.createNode(EXmlNodeType::Element, "message", error->getContent());
        errorNode.addChild(messageNode);

        errorsNode.addChild(errorNode);
    }

    return errorsNode;
}

XmlNode ApiResponseWriter::createWarnings(ApiResponse& _apiResponse)
{
    XmlNode warningsNode = m_xmlDocument.createNode(EXmlNodeType::Element, "warnings");

    for (const unique_ptr<ApiResponseMessage>& warning : _apiResponse.getResponseIssues().getWarnings()) {
        XmlNode warningNode = m_xmlDocument.createNode(EXmlNodeType::Element, "warning");

        XmlNode idNode = m_xmlDocument.createNode(EXmlNodeType::Element, "id", warning->getId());
        idNode.setName("id");
        idNode.setValue(warning->getId());

        XmlAttribute languageAttribute = m_xmlDocument.createAttribute("lang", warning->getLanguage());
        idNode.addAttribute(languageAttribute);
        warningNode.addChild(idNode);

        XmlNode messageNode = m_xmlDocument.createNode(EXmlNodeType::Element, "message", warning->getContent());
        warningNode.addChild(messageNode);

        warningsNode.addChild(warningNode);
    }

    return warningsNode;
}

string ApiResponseWriter::dateToString(const DateTime& _datetime) const
{
    return
            boost::lexical_cast<string>(_datetime.year()) +
            "-" +
            boost::lexical_cast<string>(_datetime.month()) +
            "-" +
            boost::lexical_cast<string>(_datetime.day()) +
            "T" +
            boost::lexical_cast<string>(_datetime.hour()) +
            ":" +
            boost::lexical_cast<string>(_datetime.minute()) +
            ":" +
            boost::lexical_cast<string>(_datetime.second());
}

XmlNode ApiResponseAdjustmentWriter::createData(const std::unique_ptr<ApiResponse>& _apiResponse)
{
    /*
    XmlNode dataNode = m_xmlDocument.createNode(EXmlNodeType::Element, "data");

    try {
        const unique_ptr<ApiResponseAdjustment> adjustmentResponse(dynamic_cast<ApiResponseAdjustment*>(_apiResponse.get()));
        // I'm getting nullptr on this dynamic_cast.
        for (const Core::FullDosage& adjustment : adjustmentResponse->getAdjustments()) {
            string scoreValue = boost::lexical_cast<string>(adjustment.getGlobalScore());
            XmlNode scoreNode = m_xmlDocument.createNode(EXmlNodeType::Element, "score", scoreValue);
            dataNode.addChild(scoreNode);

            // Create dosage history ?

            for (const Core::CycleData& cycle : adjustment.m_data) {
                XmlNode cycleNode = m_xmlDocument.createNode(EXmlNodeType::Element, "cycle");
                dataNode.addChild(cycleNode);

                string startValue = dateToString(cycle.m_start);
                XmlNode startNode = m_xmlDocument.createNode(EXmlNodeType::Element, "start", startValue);
                cycleNode.addChild(startNode);

                string endValue = dateToString(cycle.m_end);
                XmlNode endNode = m_xmlDocument.createNode(EXmlNodeType::Element, "end", endValue);
                cycleNode.addChild(endNode);

                XmlNode concentrationsNode = m_xmlDocument.createNode(EXmlNodeType::Element, "concentrations");
                cycleNode.addChild(concentrationsNode);

                for (vector<Core::Concentration> analyte : cycle.m_concentrations) {
                    XmlNode analyteNode = m_xmlDocument.createNode(EXmlNodeType::Element, "analyte");
                    cycleNode.addChild(analyteNode);

                    for (Core::Concentration c : analyte) {
                        string concentrationValue = boost::lexical_cast<string>(c);
                        XmlNode concentrationNode = m_xmlDocument.createNode(EXmlNodeType::Element, "concentration", concentrationValue);
                        cycleNode.addChild(concentrationNode);
                    }
                }
            }
        }
    }
    catch (bad_cast& ex) {
        XmlNode dataNode = m_xmlDocument.createNode(EXmlNodeType::Element, "data", ex.what());
    }
    */

    XmlNode dataNode = m_xmlDocument.createNode(EXmlNodeType::Element, "data", "Placeholder");
    return dataNode;
}

} // namespace Server
} // namespace Tucuxi
