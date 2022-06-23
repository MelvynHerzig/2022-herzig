#include "xpertrequestresultxmlexport.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "tucucommon/xmlattribute.h"
#include "tucucommon/xmldocument.h"
#include "tucucommon/xmlnode.h"
#include "tucucommon/utils.h"

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

    // Add the admin
    exportAdminData(_xpertRequestResult.getGlobalResult()->getAdminData(), root);

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
                m_xmlDocument.createNode(Common::EXmlNodeType::Element, "value", varToString(_xpertRequestResult.getLastIntake()->getDose()));
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

void XpertRequestResultXmlExport::exportAdminData(const std::unique_ptr<AdminData>& _admin, Common::XmlNode& _rootNode)
{
    // We export the admin if contains at least one of his elements.
    if (_admin->getMandator() == nullptr && _admin->getPatient() == nullptr && _admin->getClinicalData() == nullptr) {
        return;
    }

    // <admin>
    Common::XmlNode adminNode =
            m_xmlDocument.createNode(Common::EXmlNodeType::Element, "admin", "");
    _rootNode.addChild(adminNode);

    //   <mandator>
    exportFullPersonData(_admin->getMandator(), adminNode, "mandator");

    //   <patient>
    exportFullPersonData(_admin->getPatient(), adminNode, "patient");

    //   <clinicalData>
    exportClinicalData(_admin->getClinicalData(), adminNode);
}

void XpertRequestResultXmlExport::exportFullPersonData(const std::unique_ptr<FullPersonData>& _fullPerson, Common::XmlNode& _adminNode, const std::string& _nodeName)
{
    // If the requiered person to export is not present, just leave
    if ( (_nodeName == "mandator" && _fullPerson == nullptr) ||
         (_nodeName == "patient" && _fullPerson == nullptr)) {
        return;
    }

    // <mandator> or <patient>
    Common::XmlNode patientMandatorNode =
            m_xmlDocument.createNode(Common::EXmlNodeType::Element, _nodeName, "");
    _adminNode.addChild(patientMandatorNode);

    //   <person>
    exportPersonData(_fullPerson->getPerson(), patientMandatorNode);

    //   <institute>
    exportInstituteData(_fullPerson->getInstitute(), patientMandatorNode);

}

void XpertRequestResultXmlExport::exportPersonData(const PersonData& _person, Common::XmlNode& _patientMandatorNode)
{
    // <person>
    Common::XmlNode personNode =
            m_xmlDocument.createNode(Common::EXmlNodeType::Element, "person", "");
    _patientMandatorNode.addChild(personNode);

    //   <id>
    if (_person.getId() != "") {

        Common::XmlNode idNode =
                m_xmlDocument.createNode(Common::EXmlNodeType::Element, "id", _person.getId());
        personNode.addChild(idNode);

    }

    //   <title>
    if (_person.getTitle() != "") {

        Common::XmlNode titleNode =
                m_xmlDocument.createNode(Common::EXmlNodeType::Element, "title", _person.getTitle());
        personNode.addChild(titleNode);

    }

    //   <firstName>
    Common::XmlNode firstNameNode =
            m_xmlDocument.createNode(Common::EXmlNodeType::Element, "firstName", _person.getFirstName());
    personNode.addChild(firstNameNode);

    //   <lastName>
    Common::XmlNode lastNameNode =
            m_xmlDocument.createNode(Common::EXmlNodeType::Element, "lastName", _person.getLastName());
    personNode.addChild(lastNameNode);

    //   <address>
    exportAddressData(_person.getAddress(), personNode);

    //   <phone>
    exportPhoneData(_person.getPhone(), personNode);

    //   <email>
    exportEmailData(_person.getEmail(), personNode);
}

void XpertRequestResultXmlExport::exportInstituteData(const std::unique_ptr<InstituteData>& _institute, Common::XmlNode& _patientMandatorNode)
{
    // If no institute, just leave
    if (_institute == nullptr) {
        return;
    }

    // <institute>
    Common::XmlNode instituteNode =
            m_xmlDocument.createNode(Common::EXmlNodeType::Element, "institute", "");
    _patientMandatorNode.addChild(instituteNode);


    //   <id>
    if (_institute->getId() != "") {

        Common::XmlNode idNode =
                m_xmlDocument.createNode(Common::EXmlNodeType::Element, "id", _institute->getId());
        instituteNode.addChild(idNode);

    }

    //   <name>
    Common::XmlNode nameNode =
            m_xmlDocument.createNode(Common::EXmlNodeType::Element, "name", _institute->getName());
    instituteNode.addChild(nameNode);

    //   <address>
    exportAddressData(_institute->getAddress(), instituteNode);

    //   <phone>
    exportPhoneData(_institute->getPhone(), instituteNode);

    //   <email>
    exportEmailData(_institute->getEmail(), instituteNode);
}

void XpertRequestResultXmlExport::exportAddressData(const std::unique_ptr<AddressData>& _address, Common::XmlNode& _personInstitutNode)
{
    if (_address == nullptr) {
        return;
    }

    // <address>
    Common::XmlNode addressNode =
            m_xmlDocument.createNode(Common::EXmlNodeType::Element, "address", "");
    _personInstitutNode.addChild(addressNode);

    //   <street>
    Common::XmlNode streetNode =
            m_xmlDocument.createNode(Common::EXmlNodeType::Element, "street", _address->getStreet());
    addressNode.addChild(streetNode);

    //   <postCode>
    Common::XmlNode postCodeNode =
            m_xmlDocument.createNode(Common::EXmlNodeType::Element, "postCode", Common::Utils::varToString(_address->getPostCode()));
    addressNode.addChild(postCodeNode);

    //   <city>
    Common::XmlNode cityNode =
            m_xmlDocument.createNode(Common::EXmlNodeType::Element, "city", _address->getCity());
    addressNode.addChild(cityNode);

    //   <state>
    if (_address->getState() != "") {

        Common::XmlNode stateNode =
                m_xmlDocument.createNode(Common::EXmlNodeType::Element, "state", _address->getState());
        addressNode.addChild(stateNode);

    }

    //   <country>
    if (_address->getCountry() != "") {

        Common::XmlNode countryNode =
                m_xmlDocument.createNode(Common::EXmlNodeType::Element, "country", _address->getCountry());
        addressNode.addChild(countryNode);

    }
}

void XpertRequestResultXmlExport::exportPhoneData(const std::unique_ptr<PhoneData>& _phone, Common::XmlNode& _personInstitutNode)
{
    if (_phone == nullptr) {
        return;
    }

    // <phone>
    Common::XmlNode phoneNode =
            m_xmlDocument.createNode(Common::EXmlNodeType::Element, "phone", "");
    _personInstitutNode.addChild(phoneNode);

    //   <number>
    Common::XmlNode numberNode =
            m_xmlDocument.createNode(Common::EXmlNodeType::Element, "number", _phone->getNumber());
    phoneNode.addChild(numberNode);

    //   <type>
    if (_phone->getType() != "") {

        Common::XmlNode typeNode =
                m_xmlDocument.createNode(Common::EXmlNodeType::Element, "type", _phone->getType());
        phoneNode.addChild(typeNode);

    }
}

void XpertRequestResultXmlExport::exportEmailData(const std::unique_ptr<EmailData>& _email, Common::XmlNode& _personInstitutNode)
{
    if (_email == nullptr) {
        return;
    }

    // <phone>
    Common::XmlNode emailNode =
            m_xmlDocument.createNode(Common::EXmlNodeType::Element, "email", "");
    _personInstitutNode.addChild(emailNode);

    //   <number>
    Common::XmlNode addressNode =
            m_xmlDocument.createNode(Common::EXmlNodeType::Element, "address", _email->getAddress());
    emailNode.addChild(addressNode);

    //   <type>
    if (_email->getType() != "") {

        Common::XmlNode typeNode =
                m_xmlDocument.createNode(Common::EXmlNodeType::Element, "type", _email->getType());
        emailNode.addChild(typeNode);

    }
}

void XpertRequestResultXmlExport::exportClinicalData(const std::unique_ptr<ClinicalData>& _clinicalData, Common::XmlNode& _adminNode)
{
    if (_clinicalData == nullptr) {
        return;
    }

    // <clinicalData>
    Common::XmlNode clinicalDataNode =
            m_xmlDocument.createNode(Common::EXmlNodeType::Element, "clinicalData", "");
    _adminNode.addChild(clinicalDataNode);

    for (auto entryIt = _clinicalData->getData().begin(); entryIt != _clinicalData->getData().end(); ++entryIt){
        //   <clinicalDataEntry key="...">
        Common::XmlNode clinicalDataEntryNode =
                m_xmlDocument.createNode(Common::EXmlNodeType::Element, "clinicalDataEntry", entryIt->second);
        auto keyAttribute = m_xmlDocument.createAttribute("key", entryIt->first);
        clinicalDataEntryNode.addAttribute(keyAttribute);
        clinicalDataNode.addChild(clinicalDataEntryNode);
    }
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
