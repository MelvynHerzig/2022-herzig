#ifndef XMLREADER_H
#define XMLREADER_H

#include <string>
#include <vector>
#include "tucucommon/xmldocument.h"
#include "tucucommon/xmlnode.h"
#include "tucucommon/xmlattribute.h"
#include "tucucommon/xmliterator.h"
#include "query.h"

namespace Tucuxi {
namespace Server {

class XMLReader
{
public:
    XMLReader(const std::string& _xml);

    XMLReader(XMLReader& other) = delete;

    ~XMLReader();

    const Query& getQuery() const;

protected:
    // Methods to separate the creation of an AdministrativeData
    std::unique_ptr<AdministrativeData> createAdministrativeData() const;
    std::unique_ptr<Person> createPerson(Common::XmlNodeIterator& _personRootIterator) const;
    std::unique_ptr<PersonalContact> createPersonalContact(Common::XmlNodeIterator& _personalContactRootIterator) const;
    std::unique_ptr<InstituteContact> createInstituteContact(Common::XmlNodeIterator& _instituteContactRootIterator) const;
    std::unique_ptr<Address> createAddress(Common::XmlNodeIterator& _addressRootIterator) const;
    std::unique_ptr<Phone> createPhone(Common::XmlNodeIterator& _phoneRootIterator) const;
    std::unique_ptr<Email> createEmail(Common::XmlNodeIterator& _emailRootIterator) const;
    std::unique_ptr<ClinicalData> createClinicalData(Common::XmlNodeIterator& _clinicalDataRootIterator) const;

    // Methods to separate the creation of a ParametersData
    std::unique_ptr<ParametersData> createParametersData() const;
    std::unique_ptr<PatientData> createPatientData(Common::XmlNodeIterator& _patientDataRootIterator) const;
    std::unique_ptr<CovariateData> createCovariateData(Common::XmlNodeIterator& _covariateDataRootIterator) const;
    std::unique_ptr<DrugData> createDrugData(Common::XmlNodeIterator& _drugDataRootIterator) const;
    std::unique_ptr<TargetData> createTargetData(Common::XmlNodeIterator& _targetDataRootIterator) const;
    std::unique_ptr<SampleData> createSampleData(Common::XmlNodeIterator& _sampleDataRootIterator) const;
    std::unique_ptr<ConcentrationData> createConcentrationData(Common::XmlNodeIterator& _concentrationDataRootIterator) const;
    std::unique_ptr<Treatment> createTreatment(Common::XmlNodeIterator& _treatmentRootIterator) const;
    std::unique_ptr<Core::DosageTimeRange> createDosageTimeRange(Common::XmlNodeIterator& _dosageTimeRangeRootIterator) const;
    std::unique_ptr<Core::Dosage> createDosage(Common::XmlNodeIterator& _dosageRootIterator, Common::Duration _infustionTime) const;
    std::unique_ptr<Core::DosageBounded> createDosageBounded(Common::XmlNodeIterator& _dosageBoundedRootIterator, Common::Duration _infustionTime) const;
    std::unique_ptr<Core::FormulationAndRoute> createFormulationAndRoute(Common::XmlNodeIterator& _formulationAndRouteRootIterator) const;

    // Methods to separate the creation of the requests
    std::unique_ptr<RequestData> createRequest(Common::XmlNodeIterator& _requestRootIterator) const;
    std::unique_ptr<GraphData> createGraphData(Common::XmlNodeIterator& _graphDataRootIterator) const;
    std::unique_ptr<Backextrapolation> createBackextrapolation(Common::XmlNodeIterator& _backextrapolationRootIterator) const;

    // Utilitary methods
    const std::string getChildStringValue(Common::XmlNodeIterator _rootIterator, std::string _childName) const;
    int getChildIntValue(Common::XmlNodeIterator _rootIterator, std::string _childName) const;
    double getChildDoubleValue(Common::XmlNodeIterator _rootIterator, std::string _childName) const;
    bool getChildBoolValue(Common::XmlNodeIterator _rootIterator, std::string _childName) const;
    Common::DateTime getChildDateTimeValue(Common::XmlNodeIterator _rootIterator, std::string _childName) const;

protected:
    static const std::string m_sDATE_FORMAT;

    std::string m_xml;
    Common::XmlDocument m_xmlDocument;
    std::unique_ptr<Query> m_pQuery;
};

} // namespace Server
} // namespace Tucuxi

#endif // XMLREADER_H
