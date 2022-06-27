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
/// \date 23/06/2022
/// \author Herzig Melvyn
class XpertRequestResultXmlExport : public AbstractXpertRequestResultExport, protected Query::ComputingQueryResponseXmlExport
{
public:

    /// \brief Constructor.
    XpertRequestResultXmlExport();

    /// \brief Export the xpert request result to file. The export may fail. In that
    ///        case, the error message of the XpertRequestResult gets set.
    /// \param _xpertRequestResult Xpert request result to export.
    void exportToFile(XpertRequestResult& _xpertRequestResult) override;

protected:

    /// \brief Transform the given XpertRequestResult into an XML string.
    /// \param _xpertRequestResult XpertRequestResult to transform.
    /// \param _xmlString Resulting string.
    void makeXmlString(XpertRequestResult& _xpertRequestResult, std::string& _xmlString);

    /// \brief Create and append the "introduction" xml nodes: computation time, drugID, last dose and drug model id.
    /// \param _xpertRequestResult XpertRquestResult to retrieve the information.
    /// \param _rootNode Root node where to append the created nodes.
    void exportDrugIntro(XpertRequestResult& _xpertRequestResult, Common::XmlNode& _rootNode);

    ///
    /// \brief Create and append the admin data.
    /// \param _admin AdminData to export.
    /// \param _rootNode Root node where to append the creates nodes.
    void exportAdminData(const std::unique_ptr<AdminData>& _admin, Common::XmlNode& _rootNode);

    /// \brief Create and append a patient/mandator node to the admin node.
    /// \param _fullPerson Information to export.
    /// \param _adminNode Admin node where to append.
    /// \param _nodeName Name of the node (expect "patient" or "mandator").
    void exportFullPersonData(const std::unique_ptr<FullPersonData>& _fullPerson, Common::XmlNode& _adminNode, const std::string& _nodeName);

    /// \brief Create and append a person node to a patient/mandator node.
    /// \param _person Person data to export.
    /// \param _patientMandatorNode Patient/mandator node where to append.
    void exportPersonData(const PersonData& _person, Common::XmlNode& _patientMandatorNode);

    /// \brief Create and append the institude node of a patient/mandator.
    /// \param _institute Institute data to export.
    /// \param _patientMandatorNode Patient/mandator node where to append.
    void exportInstituteData(const std::unique_ptr<InstituteData>& _institute, Common::XmlNode& _patientMandatorNode);

    /// \brief Create and append an address node to a person/institute node.
    /// \param _address Address data to export.
    /// \param _personInstitutNode Person/institute node where to append.
    void exportAddressData(const std::unique_ptr<AddressData>& _address, Common::XmlNode& _personInstitutNode);

    /// \brief Create and append an phone node to a person/institute node.
    /// \param _phone Phone data to export.
    /// \param _personInstitutNode Person/institute node where to append.
    void exportPhoneData(const std::unique_ptr<PhoneData>& _phone, Common::XmlNode& _personInstitutNode);

    /// \brief Create and append an Email node to a person/institute node.
    /// \param _email Email data to export.
    /// \param _personInstitutNode Person/institute node where to append.
    void exportEmailData(const std::unique_ptr<EmailData>& _email, Common::XmlNode& _personInstitutNode);

    /// \brief Create and append the clinicalData node to the admin node.
    /// \param _clinicalData ClinicalData to export.
    /// \param _adminNode Admin node where to append.
    void exportClinicalData(const std::unique_ptr<ClinicalData>& _clinicalData, Common::XmlNode& _adminNode);

    /// \brief Create and append the covariates validation results.
    /// \param _covariateResults Covariate validation results to export.
    /// \param _rootNode Root node where to append.
    /// \param _outputLang Language to use for the description and the name of the covariates.
    void exportCovariateResults(const std::vector<CovariateValidationResult>& _covariateResults, Common::XmlNode& _rootNode, OutputLang _outputLang);

    /// \brief Create and append the treatment node to the root node. Mainly use the inherithed methods.
    /// \param _treatment Treatment to export.
    /// \param _rootNode Root node where to append.
    void exportTreatment(const std::unique_ptr<Core::DrugTreatment>& _treatment, Tucuxi::Common::XmlNode& _rootNode);

    /// \brief Override of the inherited method in order to include the warning related to a single dose.
    ///        Create and append a dose to a parent node.
    /// \param _dosage Single dose to export.
    /// \param _parentNode Node where to append the single dose.
    void exportDose(const Tucuxi::Core::SingleDose& _dosage, Tucuxi::Common::XmlNode& _parentNode) override;

    /// \brief Create and append the sample results to the root node.
    /// \param _sampleResults Sample results to export.
    /// \param _rootNode Root node where to append.
    void exportSampleResults(const std::map<const Core::Sample*, SampleValidationResult>& _sampleResults, Common::XmlNode& _rootNode);

    /// \brief Create and append the adjustment data node to the root node.
    /// \param _adjustmentData Adjustment data to export.
    /// \param _rootNode Root node where to append.
    void exportAdjustmentData(const std::unique_ptr<Core::AdjustmentData>& _adjustmentData, Tucuxi::Common::XmlNode& _rootNode);

    /// \brief Create and append a cycle data node to the cycleDatas node of an adjustment.
    ///        Override the inherited method to only export the needed information and not
    ///        the covariate, the statistics nor the parameters.
    /// \param _cycleData Cycle data to export
    /// \param _cycleDatasNode CycleDatas node where to append.
    /// \return True.
    bool exportCycleData(const Tucuxi::Core::CycleData& _cycleData, Tucuxi::Common::XmlNode& _cycleDatasNode) override;

    /// \brief Create and append the paramters values to the root node.
    ///        It creates the parameters for typical and apriori types. If possible, it makes aposteriori.
    /// \param _xpertRequestResult XpertRequestResult containing the parameters to export.
    /// \param _rootNode Root node where to append.
    void exportParameters(XpertRequestResult& _xpertRequestResult, Common::XmlNode& _rootNode);

    /// \brief Create and append the statistics at steady state to the root node.
    /// \param _xpertRequestResult XpertRequestResult containing the statistics at steady state.
    /// \param _rootNode Root node where to append.
    void exportStatistics(XpertRequestResult& _xpertRequestResult, Common::XmlNode& _rootNode);

    /// \brief Create and append the covariate used for the computation nodes to the root node.
    /// \param _xpertRequestResult XpertRequestResult containing the covariates used during computation.
    /// \param _rootNode Root node where to append.
    void exportComputationCovariates(XpertRequestResult& _xpertRequestResult, Common::XmlNode& _rootNode);

    /// \brief For a given validation result object, create and append its node to a parent node.
    /// \param _validationResult Validation result value to export.
    /// \param _parentNode Parent node where to append.
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

    /// \brief Converts a date time into a string usable as an xml value.
    /// \param _dateTime Date time to export.
    /// \return Resulting string value.
    std::string dateTimeToXmlString(const Common::DateTime& _dateTime) const;

protected:

    /// \brief We keep a reference on the xml document variable in order to create xml nodes
    ///        in any method without needing to pass it as argument everywhere.
    Common::XmlDocument m_xmlDocument;

    /// \brief We need to keep a reference on the xpert request result object in order
    ///        to retrieve the dose validation results map in the exportSingleDose method.
    XpertRequestResult* m_xpertRequestResultInUse;
};

} // namespace Xpert
} // namespace Tucuxi

#endif // XPERTREQUESTRESULTXMLEXPORT_H
