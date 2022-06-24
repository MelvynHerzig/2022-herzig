#ifndef XPERTREQUESTRESULTXMLEXPORT_H
#define XPERTREQUESTRESULTXMLEXPORT_H

#include "tucucommon/xmlattribute.h"
#include "tucucommon/xmldocument.h"
#include "tucucommon/xmlnode.h"
#include "tucuquery/computingqueryresponsexmlexport.h"

#include "tuberxpert/query/admindata.h"
#include "tuberxpert/exporter/abstractxpertrequestresultexport.h"

namespace Tucuxi {
namespace Xpert {

/// \brief This class exports an XpertRequestResult into xml.
///        It can be in a file or a string.
/// \date 23/06/2022
/// \author Herzig Melvyn
class XpertRequestResultXmlExport : public AbstractXpertRequestResultExport, protected Query::ComputingQueryResponseXmlExport
{
public:

    /// \brief Constructor.
    XpertRequestResultXmlExport();

    /// \brief Export the xpert request result to file.
    /// \param _xpertRequestResult Xpert request result to export.
    void exportToFile(XpertRequestResult& _xpertRequestResult) override;

protected:

    bool makeXmlString(XpertRequestResult& _xpertRequestResult, std::string& _xmlString);

    void exportDrugIntro(XpertRequestResult& _xpertRequestResult, Common::XmlNode& _rootNode);

    void exportAdminData(const std::unique_ptr<AdminData>& _admin, Common::XmlNode& _rootNode);

    void exportFullPersonData(const std::unique_ptr<FullPersonData>& _fullPerson, Common::XmlNode& _adminNode, const std::string& _nodeName);

    void exportPersonData(const PersonData& _person, Common::XmlNode& _patientMandatorNode);

    void exportInstituteData(const std::unique_ptr<InstituteData>& _institute, Common::XmlNode& _patientMandatorNode);

    void exportAddressData(const std::unique_ptr<AddressData>& _address, Common::XmlNode& _personInstitutNode);

    void exportPhoneData(const std::unique_ptr<PhoneData>& _phone, Common::XmlNode& _personInstitutNode);

    void exportEmailData(const std::unique_ptr<EmailData>& _email, Common::XmlNode& _personInstitutNode);

    void exportClinicalData(const std::unique_ptr<ClinicalData>& _clinicalData, Common::XmlNode& _adminNode);

    void exportCovariateResults(const std::vector<CovariateValidationResult>& _covariateResults, Common::XmlNode& _rootNodem, OutputLang _outputLang);

    void exportTreatment(const std::unique_ptr<Core::DrugTreatment>& _treatment, Tucuxi::Common::XmlNode& _rootNode);

    void exportSingleDose(const Tucuxi::Core::SingleDose& _dosage, Tucuxi::Common::XmlNode& _rootNode) override;

    template<typename T>
    void exportWarning(const AbstractValidationResult<T>& _validationResult, Tucuxi::Common::XmlNode& _parentNode) {

        if (!_validationResult.getWarning().empty()) {
            Common::XmlNode warningNode =
                    m_xmlDocument.createNode(Common::EXmlNodeType::Element, "warning", _validationResult.getWarning());
            auto levelAttribute = m_xmlDocument.createAttribute("level", varToString(_validationResult.getWarningLevel()));
            warningNode.addAttribute(levelAttribute);
            _parentNode.addChild(warningNode);
        }
    }

    std::string dateTimeToXmlString(const Common::DateTime& _dateTime) const;

protected:

    Common::XmlDocument m_xmlDocument;

    XpertRequestResult* m_xpertRequestResultInUse;
};

} // namespace Xpert
} // namespace Tucuxi

#endif // XPERTREQUESTRESULTXMLEXPORT_H
