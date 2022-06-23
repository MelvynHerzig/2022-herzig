#include "xpertrequestresultxmlexport.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "tucucommon/xmlattribute.h"
#include "tucucommon/xmldocument.h"
#include "tucucommon/xmlnode.h"

#include "tuberxpert/result/globalresult.h"
#include "tuberxpert/utils/xpertutils.h"

using namespace std;

namespace Tucuxi {
namespace Xpert {

XpertRequestResultXmlExport::XpertRequestResultXmlExport()
{}

void XpertRequestResultXmlExport::exportToFile(XpertRequestResult& _xpertRequestResult)
{
    // Get the xml string
    string xmlString;
    if (!makeXmlString(_xpertRequestResult, xmlString)) {
        _xpertRequestResult.setErrorMessage("Failed to create the XML content.");
    }

    // Get the filename <drugId>_<requestNumber>_<current time to evit conflict naming>
    string fileName = computeFileName(_xpertRequestResult);

    // Opening the file
    ofstream file;
    file.open(fileName);
    if ((file.rdstate() & ostream::failbit) != 0) {
        _xpertRequestResult.setErrorMessage("The file " + fileName + " could not be opened.");
        return ;
    }

    // Write & close
    file << xmlString;
    file.close();

    // The xml is exported.
    return;
}

bool XpertRequestResultXmlExport::makeXmlString(XpertRequestResult& _xpertRequestResult, string& _xmlString)
{
    // Making root
    Tucuxi::Common::XmlNode root = m_xmlDocument.createNode(Tucuxi::Common::EXmlNodeType::Element, "tuberxpertResult");
    auto attribute1 = m_xmlDocument.createAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
    root.addAttribute(attribute1);
    //auto attribute2 = m_xmlDocument.createAttribute("xsi:noNamespaceSchemaLocation", "computing_response.xsd");
    //root.addAttribute(attribute2);

    m_xmlDocument.setRoot(root);

    // Date of generation
    Tucuxi::Common::XmlNode generationDate =
            m_xmlDocument.createNode(Tucuxi::Common::EXmlNodeType::Element, "computationTime",
                                     dateTimeToXmlString(_xpertRequestResult.getGlobalResult()->getComputationTime()));
    root.addChild(generationDate);

    // Add the intro drugId, modelId, lastDose
    exportDrugIntro(_xpertRequestResult, root);

    m_xmlDocument.toString(_xmlString, true);
    return true;
}

void XpertRequestResultXmlExport::exportDrugIntro(XpertRequestResult& _xpertRequestResult, Common::XmlNode& _rootNode)
{
    // <drug>
    Common::XmlNode drugNode =
            m_xmlDocument.createNode(Common::EXmlNodeType::Element, "drug", "");
    _rootNode.addChild(drugNode);

    //  <drugId>
    Common::XmlNode drugIdNode =
            m_xmlDocument.createNode(Common::EXmlNodeType::Element, "drugId", _xpertRequestResult.getXpertRequest().getDrugID());
    drugNode.addChild(drugIdNode);

    //      <lastDose>
    Common::XmlNode lastDoseNode =
            m_xmlDocument.createNode(Common::EXmlNodeType::Element, "lastDose", "");
    drugNode.addChild(lastDoseNode);

    if (_xpertRequestResult.getLastIntake() != nullptr) {
        //              <value>
        Common::XmlNode valueNode =
                m_xmlDocument.createNode(Common::EXmlNodeType::Element, "value", to_string(_xpertRequestResult.getLastIntake()->getDose()));
        lastDoseNode.addChild(valueNode);

        //              <unit>
        Common::XmlNode unitNode =
                m_xmlDocument.createNode(Common::EXmlNodeType::Element, "unit", _xpertRequestResult.getLastIntake()->getUnit().toString());
        lastDoseNode.addChild(unitNode);
    }

    //  <drugModelId>
    Common::XmlNode drugModelIdNode =
            m_xmlDocument.createNode(Common::EXmlNodeType::Element, "drugModelId", _xpertRequestResult.getDrugModel()->getDrugModelId());
    drugNode.addChild(drugModelIdNode);
}

std::string XpertRequestResultXmlExport::dateTimeToXmlString(const Tucuxi::Common::DateTime& _dateTime) const
{
    if (_dateTime.isUndefined()) {
        return "";
    }

    std::string result;
    result = std::to_string(_dateTime.year()) + "." + std::to_string(_dateTime.month()) + "."
             + std::to_string(_dateTime.day()) + "T" + std::to_string(_dateTime.hour()) + ":"
             + std::to_string(_dateTime.minute()) + ":" + std::to_string(_dateTime.second());

    char str[20];
    snprintf(
            str,
            20,
            "%04d-%02d-%02dT%02d:%02d:%02d",
            _dateTime.year(),
            _dateTime.month(),
            _dateTime.day(),
            _dateTime.hour(),
            _dateTime.minute(),
            _dateTime.second());
    result = str;
    return result;
}

} // namespace Xpert
} // namespace Tucuxi
