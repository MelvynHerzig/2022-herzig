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
    // Saving the reference on the xpertRequestResult to be able to retrieve
    // it anywhere anytime. For example, when exporting the
    // single doses and being able to retrieve the associated validation result.
    m_xpertRequestResultInUse = &_xpertRequestResult;

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
    addNode(root, "computationTime", dateTimeToXmlString(_xpertRequestResult.getGlobalResult()->getComputationTime()));

    // Add the intro drugId, modelId, lastDose
    exportDrugIntro(_xpertRequestResult, root);

    // Add the admin
    exportAdminData(_xpertRequestResult.getGlobalResult()->getAdminData(), root);

    // Add the covariates
    exportCovariateResults(_xpertRequestResult.getCovariateResults(), root, _xpertRequestResult.getXpertRequest().getOutputLang());

    // Add the dosage history (treatment)
    exportTreatment(_xpertRequestResult.getTreatment(), root);

    // Add the samples
    exportSampleResults(_xpertRequestResult.getSampleResults(), root);

    exportAdjustmentData(_xpertRequestResult.getAdjustmentData(), root);

    m_xmlDocument.toString(_xmlString, true);
    return true;
}

void XpertRequestResultXmlExport::exportDrugIntro(XpertRequestResult& _xpertRequestResult, Common::XmlNode& _rootNode)
{
    // <drug>
    Common::XmlNode drugNode =
            m_xmlDocument.createNode(Common::EXmlNodeType::Element, "drug");
    _rootNode.addChild(drugNode);

    //  <drugId>
    addNode(drugNode, "drugId", _xpertRequestResult.getXpertRequest().getDrugID());

    //      <lastDose>
    Common::XmlNode lastDoseNode =
            m_xmlDocument.createNode(Common::EXmlNodeType::Element, "lastDose");
    drugNode.addChild(lastDoseNode);

    if (_xpertRequestResult.getLastIntake() != nullptr) {
        //              <value>
        addNode(lastDoseNode, "value", varToString(_xpertRequestResult.getLastIntake()->getDose()));

        //              <unit>
        addNode(lastDoseNode, "unit", _xpertRequestResult.getLastIntake()->getUnit().toString());
    }

    //  <drugModelId>
    addNode(drugNode, "drugModelId", _xpertRequestResult.getDrugModel()->getDrugModelId());
}

void XpertRequestResultXmlExport::exportAdminData(const unique_ptr<AdminData>& _admin, Common::XmlNode& _rootNode)
{
    // We export the admin if contains at least one of his elements.
    if (_admin->getMandator() == nullptr && _admin->getPatient() == nullptr && _admin->getClinicalData() == nullptr) {
        return;
    }

    // <admin>
    Common::XmlNode adminNode =
            m_xmlDocument.createNode(Common::EXmlNodeType::Element, "admin");
    _rootNode.addChild(adminNode);

    //   <mandator>
    exportFullPersonData(_admin->getMandator(), adminNode, "mandator");

    //   <patient>
    exportFullPersonData(_admin->getPatient(), adminNode, "patient");

    //   <clinicalData>
    exportClinicalData(_admin->getClinicalData(), adminNode);
}

void XpertRequestResultXmlExport::exportFullPersonData(const unique_ptr<FullPersonData>& _fullPerson, Common::XmlNode& _adminNode, const string& _nodeName)
{
    // If the requiered person to export is not present, just leave
    if ( (_nodeName == "mandator" && _fullPerson == nullptr) ||
         (_nodeName == "patient" && _fullPerson == nullptr)) {
        return;
    }

    // <mandator> or <patient>
    Common::XmlNode patientMandatorNode =
            m_xmlDocument.createNode(Common::EXmlNodeType::Element, _nodeName);
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
            m_xmlDocument.createNode(Common::EXmlNodeType::Element, "person");
    _patientMandatorNode.addChild(personNode);

    //   <id>
    if (_person.getId() != "") {
        addNode(personNode, "id", _person.getId());
    }

    //   <title>
    if (_person.getTitle() != "") {
        addNode(personNode, "title", _person.getTitle());
    }

    //   <firstName>
    addNode(personNode, "firstName", _person.getFirstName());

    //   <lastName>
    addNode(personNode, "lastName", _person.getLastName());

    //   <address>
    exportAddressData(_person.getAddress(), personNode);

    //   <phone>
    exportPhoneData(_person.getPhone(), personNode);

    //   <email>
    exportEmailData(_person.getEmail(), personNode);
}

void XpertRequestResultXmlExport::exportInstituteData(const unique_ptr<InstituteData>& _institute, Common::XmlNode& _patientMandatorNode)
{
    // If no institute, just leave
    if (_institute == nullptr) {
        return;
    }

    // <institute>
    Common::XmlNode instituteNode =
            m_xmlDocument.createNode(Common::EXmlNodeType::Element, "institute");
    _patientMandatorNode.addChild(instituteNode);


    //   <id>
    if (_institute->getId() != "") {
        addNode(instituteNode, "id",  _institute->getId());
    }

    //   <name>
    addNode(instituteNode, "name", _institute->getName());

    //   <address>
    exportAddressData(_institute->getAddress(), instituteNode);

    //   <phone>
    exportPhoneData(_institute->getPhone(), instituteNode);

    //   <email>
    exportEmailData(_institute->getEmail(), instituteNode);
}

void XpertRequestResultXmlExport::exportAddressData(const unique_ptr<AddressData>& _address, Common::XmlNode& _personInstitutNode)
{
    if (_address == nullptr) {
        return;
    }

    // <address>
    Common::XmlNode addressNode =
            m_xmlDocument.createNode(Common::EXmlNodeType::Element, "address");
    _personInstitutNode.addChild(addressNode);

    //   <street>
    addNode(addressNode, "street", _address->getStreet());

    //   <postCode>
    addNode(addressNode, "postCode", Common::Utils::varToString(_address->getPostCode()));

    //   <city>
    addNode(addressNode, "city", _address->getCity());

    //   <state>
    if (_address->getState() != "") {
        addNode(addressNode, "state", _address->getState());
    }

    //   <country>
    if (_address->getCountry() != "") {
        addNode(addressNode, "country", _address->getCountry());
    }
}

void XpertRequestResultXmlExport::exportPhoneData(const unique_ptr<PhoneData>& _phone, Common::XmlNode& _personInstitutNode)
{
    if (_phone == nullptr) {
        return;
    }

    // <phone>
    Common::XmlNode phoneNode =
            m_xmlDocument.createNode(Common::EXmlNodeType::Element, "phone");
    _personInstitutNode.addChild(phoneNode);

    //   <number>
    addNode(phoneNode, "number", _phone->getNumber());

    //   <type>
    if (_phone->getType() != "") {
         addNode(phoneNode, "type", _phone->getType());
    }
}

void XpertRequestResultXmlExport::exportEmailData(const unique_ptr<EmailData>& _email, Common::XmlNode& _personInstitutNode)
{
    if (_email == nullptr) {
        return;
    }

    // <phone>
    Common::XmlNode emailNode =
            m_xmlDocument.createNode(Common::EXmlNodeType::Element, "email");
    _personInstitutNode.addChild(emailNode);

    //   <number>
    addNode(emailNode, "address", _email->getAddress());

    //   <type>
    if (_email->getType() != "") {
        addNode(emailNode, "type", _email->getType());
    }
}

void XpertRequestResultXmlExport::exportClinicalData(const unique_ptr<ClinicalData>& _clinicalData, Common::XmlNode& _adminNode)
{
    if (_clinicalData == nullptr) {
        return;
    }

    // <clinicalData>
    Common::XmlNode clinicalDataNode =
            m_xmlDocument.createNode(Common::EXmlNodeType::Element, "clinicalData");
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

void XpertRequestResultXmlExport::exportCovariateResults(const vector<CovariateValidationResult>& _covariateResults, Common::XmlNode& _rootNode, OutputLang _outputLang)
{
    // <covariates>
    Common::XmlNode covariatesNode =
            m_xmlDocument.createNode(Common::EXmlNodeType::Element, "covariates");
    _rootNode.addChild(covariatesNode);

    // For each covariate validation result
    for (const CovariateValidationResult& covariateValidationResult : _covariateResults) {
        //   <covariate>
        Common::XmlNode covariateNode =
                m_xmlDocument.createNode(Common::EXmlNodeType::Element, "covariate");
        covariatesNode.addChild(covariateNode);

        //       <covariateId>
        addNode(covariateNode, "covariateId", covariateValidationResult.getSource()->getId());


        //       <name>
        addNode(covariateNode, "name", getStringWithEnglishFallback(covariateValidationResult.getSource()->getName().getString(), _outputLang));

        //       <value>
        addNode(covariateNode, "value", covariateValidationResult.getValue());

        //       <unit>
        addNode(covariateNode, "unit", covariateValidationResult.getUnit().toString());

        //       <desc>
        addNode(covariateNode, "desc", getStringWithEnglishFallback(covariateValidationResult.getSource()->getDescription(), _outputLang));

        //       <source>
        addNode(covariateNode, "source", varToString(covariateValidationResult.getType()));

        //       <warning>
        exportWarning(covariateValidationResult, covariateNode);
    }
}

void XpertRequestResultXmlExport::exportTreatment(const std::unique_ptr<Core::DrugTreatment>& _treatment, Common::XmlNode& _rootNode)
{
    if (_treatment == nullptr) {
        return;
    }

    // <treatment>
    Common::XmlNode treatmentNode =
            m_xmlDocument.createNode(Common::EXmlNodeType::Element, "treatment");
    _rootNode.addChild(treatmentNode);

    //   <dosageHistory>
    exportDosageHistory(_treatment->getDosageHistory(), treatmentNode);
}

void XpertRequestResultXmlExport::exportSingleDose(const Core::SingleDose& _dosage, Common::XmlNode& _rootNode)
{
    // <dose>
    exportDose(_dosage, _rootNode);

    // <warning>
    auto singleDoseIt = m_xpertRequestResultInUse->getDoseResults().find(&_dosage);
    if (singleDoseIt !=  m_xpertRequestResultInUse->getDoseResults().end()){
        exportWarning(singleDoseIt->second, _rootNode);
    }

    // <formulationAndRoute>
    exportFormulationAndRoute(_dosage, _rootNode);
}

void XpertRequestResultXmlExport::exportSampleResults(const map<const Core::Sample*, SampleValidationResult>& _sampleResults, Common::XmlNode& _rootNode)
{
    // <samples>
    Common::XmlNode samplesNode =
            m_xmlDocument.createNode(Common::EXmlNodeType::Element, "samples");
    _rootNode.addChild(samplesNode);

    // For each sample validation result
    for (auto sampleResultIt : _sampleResults) {
        //   <sample>
        Common::XmlNode sampleNode =
                m_xmlDocument.createNode(Common::EXmlNodeType::Element, "sample");
        samplesNode.addChild(sampleNode);

        //       <sampleId>
        // addNode(sampleNode, "sampleId", sampleValidationResult.getSource().getSampleId());


        //       <sampleDate>
        addNode(sampleNode, "sampleDate", dateTimeToXmlString(sampleResultIt.first->getDate()));

        //       <concentrations>
        Common::XmlNode concentrationsNode =
                m_xmlDocument.createNode(Common::EXmlNodeType::Element, "concentrations");
        sampleNode.addChild(concentrationsNode);

        // One day when the samples will contain multiple concentrations, use a for loop on them.

        //          <concentration>
        Common::XmlNode concentrationNode =
                m_xmlDocument.createNode(Common::EXmlNodeType::Element, "concentration");
        concentrationsNode.addChild(concentrationNode);

        //              <analyteId>
        addNode(concentrationNode, "analyteId", sampleResultIt.first->getAnalyteID().toString());

        //              <percentile>
        addNode(concentrationNode, "percentile", sampleResultIt.second.getGroupNumberOver99Percentile());

        //              <value>
        addNode(concentrationNode, "value", sampleResultIt.first->getValue());

        //              <unit>
        addNode(concentrationNode, "unit", sampleResultIt.first->getUnit().toString());

        //              <warning>
        exportWarning(sampleResultIt.second, concentrationNode);
    }
}

void XpertRequestResultXmlExport::exportAdjustmentData(const unique_ptr<Core::AdjustmentData>& _adjustmentData, Common::XmlNode& _rootNode)
{
    // <dataAdjustment>
    Tucuxi::Common::XmlNode dataAdjustmentNode =
            m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element, "dataAdjustment");
    _rootNode.addChild(dataAdjustmentNode);

    //   <analyteIds>
    Tucuxi::Common::XmlNode analyteIdsNode = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element, "analyteIds");
    dataAdjustmentNode.addChild(analyteIdsNode);
    for (const auto& comp : _adjustmentData->getCompartmentInfos()) {

        //      <analyteId>
        addNode(analyteIdsNode, "analyteId", comp.getId());
    }

    //   <adjustments>
    Tucuxi::Common::XmlNode adjustmentsNode = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element, "adjustments");
    dataAdjustmentNode.addChild(adjustmentsNode);

    // For each adjustment
    for (const auto& adj : _adjustmentData->getAdjustments()) {

        //      <adjustment>
        Tucuxi::Common::XmlNode adjustmentNode = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element, "adjustment");
        adjustmentsNode.addChild(adjustmentNode);

        //          <score>
        addNode(adjustmentNode, "score", adj.getGlobalScore());

        //          <targetEvaluations>
        Tucuxi::Common::XmlNode targetEvaluationsNode =
                m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element, "targetEvaluations");
        adjustmentNode.addChild(targetEvaluationsNode);

        // For each target evaluation
        for (const auto& target : adj.m_targetsEvaluation) {

            //              <targetEvaluation>
            Tucuxi::Common::XmlNode targetEvaluationNode =
                    m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element, "targetEvaluation");
            targetEvaluationsNode.addChild(targetEvaluationNode);

            //                  <targetType>
            addNode(targetEvaluationNode, "targetType", toString(target.getTargetType()));

            //                  <unit>
            addNode(targetEvaluationNode, "unit", target.getUnit().toString());

            //                  <value>
            addNode(targetEvaluationNode, "value", target.getValue());

            //                  <score>
            addNode(targetEvaluationNode, "score", target.getScore());

            //                  <min>
            addNode(targetEvaluationNode, "min", target.getTarget().getValueMin());

            //                  <best>
            addNode(targetEvaluationNode, "best", target.getTarget().getValueBest());

            //                  <max>
            addNode(targetEvaluationNode, "max", target.getTarget().getValueMax());

            //                  <inefficacyAlarm>
            addNode(targetEvaluationNode, "inefficacyAlarm", target.getTarget().getInefficacyAlarm());

            //                  <toxicityAlarm>
            addNode(targetEvaluationNode, "toxicityAlarm", target.getTarget().getToxicityAlarm());
        }

        //          <dosageHistory>
        exportDosageHistory(adj.m_history, adjustmentNode);

        //          <cycleDatas>
        exportCycleDatas(adj.getData(), adjustmentNode);
    }
}


string XpertRequestResultXmlExport::dateTimeToXmlString(const Tucuxi::Common::DateTime& _dateTime) const
{
    if (_dateTime.isUndefined()) {
        return "";
    }

    string result;
    result = to_string(_dateTime.year()) + "." + to_string(_dateTime.month()) + "."
             + to_string(_dateTime.day()) + "T" + to_string(_dateTime.hour()) + ":"
             + to_string(_dateTime.minute()) + ":" + to_string(_dateTime.second());

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
