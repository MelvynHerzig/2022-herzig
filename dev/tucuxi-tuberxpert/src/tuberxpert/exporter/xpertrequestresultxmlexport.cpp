#include "xpertrequestresultxmlexport.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "tucucommon/xmlattribute.h"
#include "tucucommon/xmldocument.h"
#include "tucucommon/xmlnode.h"
#include "tucucommon/utils.h"

#include "tucuquery/fullsample.h"

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
    makeXmlString(_xpertRequestResult, xmlString);

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

void XpertRequestResultXmlExport::makeXmlString(XpertRequestResult& _xpertRequestResult, string& _xmlString)
{
    // Making root
    Tucuxi::Common::XmlNode root = m_xmlDocument.createNode(Tucuxi::Common::EXmlNodeType::Element, "tuberxpertResult");
    auto attribute1 = m_xmlDocument.createAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
    root.addAttribute(attribute1);
    auto attribute2 = m_xmlDocument.createAttribute("xsi:noNamespaceSchemaLocation", "tuberxpert_computing_response.xsd");
    root.addAttribute(attribute2);

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

    // Add the adjustments
    exportAdjustmentData(_xpertRequestResult.getAdjustmentData(), root);

    // Add the parameters
    exportParameters(_xpertRequestResult, root);

    // Add statistics
    exportStatistics(_xpertRequestResult, root);

    // Add computationCovariates
    exportComputationCovariates(_xpertRequestResult, root);

    m_xmlDocument.toString(_xmlString, true);
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
    if (_fullPerson == nullptr) {
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

        //       <date>
        if (covariateValidationResult.getPatient() != nullptr) {
            addNode(covariateNode, "date", dateTimeToXmlString(covariateValidationResult.getPatient()->getEventTime()));
        }

        //       <name>
        addNode(covariateNode, "name", getStringWithEnglishFallback(covariateValidationResult.getSource()->getName().getString(), _outputLang));

        //       <value>
        addNode(covariateNode, "value", covariateValidationResult.getValue());

        //       <unit>
        addNode(covariateNode, "unit", covariateValidationResult.getUnit().toString());

        //       <datatype>
        addNode(covariateNode, "dataType", varToString(covariateValidationResult.getDataType()));

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

void XpertRequestResultXmlExport::exportDose(
        const Tucuxi::Core::SingleDose& _dosage, Tucuxi::Common::XmlNode& _parentNode)
{
    Tucuxi::Common::XmlNode doseNode = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element, "dose");
    _parentNode.addChild(doseNode);

    // <warning>
    addNode(doseNode, "value", double(_dosage.getDose()));

    // <unit>
    addNode(doseNode, "unit", _dosage.getDoseUnit().toString());

    // <infusionTimeInMinutes>
    addNode(doseNode, "infusionTimeInMinutes", _dosage.getInfusionTime().toMinutes());

    // <warning>
    auto singleDoseIt = m_xpertRequestResultInUse->getDoseResults().find(&_dosage);
    if (singleDoseIt !=  m_xpertRequestResultInUse->getDoseResults().end()){
        exportWarning(singleDoseIt->second, doseNode);
    }
}

void XpertRequestResultXmlExport::exportSampleResults(const map<const Core::Sample*, SampleValidationResult>& _sampleResults, Common::XmlNode& _rootNode)
{
    // <samples>
    Common::XmlNode samplesNode =
            m_xmlDocument.createNode(Common::EXmlNodeType::Element, "samples");
    _rootNode.addChild(samplesNode);

    // For each sample validation result
    for (const auto& sampleResultIt : _sampleResults) {
        //   <sample>
        Common::XmlNode sampleNode =
                m_xmlDocument.createNode(Common::EXmlNodeType::Element, "sample");
        samplesNode.addChild(sampleNode);

        //       <sampleId>
        const Query::FullSample* fullSample = static_cast<const Query::FullSample*>(sampleResultIt.first);
        addNode(sampleNode, "sampleId", fullSample->getSampleId());

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

bool XpertRequestResultXmlExport::exportCycleData(const Core::CycleData &_cycleData, Common::XmlNode &_cycleDatasNode)
{
    Tucuxi::Common::XmlNode cycleData = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element, "cycleData");
    _cycleDatasNode.addChild(cycleData);

    addNode(cycleData, "start", dateTimeToXmlString(_cycleData.m_start));
    addNode(cycleData, "end", dateTimeToXmlString(_cycleData.m_end));
    addNode(cycleData, "unit", _cycleData.m_unit.toString());

    // Concatenate the times
    std::string timesString;
    for (size_t i = 0; i < _cycleData.m_times[0].size(); i++) {
        timesString += std::to_string(_cycleData.m_times[0][i]);
        if (i != _cycleData.m_times[0].size() - 1) {
            timesString += ',';
        }
    }
    addNode(cycleData, "times", timesString);

    // Concatenate the points
    std::string pointsString;
    for (size_t i = 0; i < _cycleData.m_concentrations[0].size(); i++) {
        pointsString += std::to_string(_cycleData.m_concentrations[0][i]);
        if (i != _cycleData.m_concentrations[0].size() - 1) {
            pointsString += ',';
        }
    }
    addNode(cycleData, "values", pointsString);

    return true;
}

void XpertRequestResultXmlExport::exportParameters(XpertRequestResult& _xpertRequestResult, Common::XmlNode& _rootNode)
{
    // <parameters>
    Tucuxi::Common::XmlNode parametersNode =
            m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element, "parameters");
    _rootNode.addChild(parametersNode);

    // We always have typical[0] and apriori[1]. But aposteriori[2] is not sure.
    vector<string> parametersTypeNodeName{"typical", "apriori", "aposteriori"};

    // For each parameters type we have
    for (size_t i = 0; i < _xpertRequestResult.getParameters().size(); ++i){

        //   <typical> / <apriori> / <aposteriori>
        Tucuxi::Common::XmlNode parametersTypeNode =
                m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element, parametersTypeNodeName[i]);
        parametersNode.addChild(parametersTypeNode);

        // For each parameter of the given parameters set
        for (const Core::ParameterValue& parameter : _xpertRequestResult.getParameters()[i]) {

            //      <parameter>
            Tucuxi::Common::XmlNode parameterNode =
                    m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element, "parameter");
            parametersTypeNode.addChild(parameterNode);

            //                  <id>
            addNode(parameterNode, "id", parameter.m_parameterId);

            //                  <value>
            addNode(parameterNode, "value", parameter.m_value);
        }
    }
}

void XpertRequestResultXmlExport::exportStatistics(XpertRequestResult& _xpertRequestResult, Common::XmlNode& _rootNode)
{
    // <statistics>
    Tucuxi::Common::XmlNode statisticsNode =
            m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element, "statistics");
    _rootNode.addChild(statisticsNode);

    // Extract statistics
    double auc24 = -1.0;
    double peak = -1.0;
    double residual = -1.0;
    Tucuxi::Common::DateTime date;

    _xpertRequestResult.getCycleStats().getStatistic(0, Tucuxi::Core::CycleStatisticType::AUC24).getValue(date, auc24);
    _xpertRequestResult.getCycleStats().getStatistic(0, Tucuxi::Core::CycleStatisticType::Peak).getValue(date, peak);
    _xpertRequestResult.getCycleStats().getStatistic(0, Tucuxi::Core::CycleStatisticType::Residual).getValue(date, residual);

    //   <auc24>
    addNode(statisticsNode, "auc24", Common::Utils::varToString(auc24));

    //   <peak>
    addNode(statisticsNode, "peak", Common::Utils::varToString(peak));

    //   <residual>
    addNode(statisticsNode, "residual", Common::Utils::varToString(residual));
}

void XpertRequestResultXmlExport::exportComputationCovariates(XpertRequestResult& _xpertRequestResult, Common::XmlNode& _rootNode)
{
    // <computationCovariates>
    Tucuxi::Common::XmlNode computationCovariatesNode =
            m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element, "computationCovariates");
    _rootNode.addChild(computationCovariatesNode);

    // Get the covariates from the first adjsutment and its first cycle data.
    const vector<Core::CovariateValue>& computationCovariates = _xpertRequestResult.getAdjustmentData()->getAdjustments().front().getData().front().m_covariates;

    // Extract the covariates
    for(const Core::CovariateValue& covariateValue : computationCovariates) {

        //      <computationCovariate>
        Tucuxi::Common::XmlNode computationCovariateNode = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element, "computationCovariate");
        computationCovariatesNode.addChild(computationCovariateNode);

        //          <id>
        addNode(computationCovariateNode, "id", covariateValue.m_covariateId);

        //          <value>
        addNode(computationCovariateNode, "value", std::to_string(covariateValue.m_value));
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
