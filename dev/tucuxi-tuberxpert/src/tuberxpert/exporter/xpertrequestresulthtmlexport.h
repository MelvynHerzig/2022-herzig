#ifndef XPERTREQUESTRESULTHTMLEXPORT_H
#define XPERTREQUESTRESULTHTMLEXPORT_H

#include "tuberxpert/exporter/abstractxpertrequestresultexport.h"
#include "tuberxpert/query/admindata.h"

#include "inja/inja.hpp"

namespace Tucuxi {
namespace Xpert {

/// \brief This class exports an XpertRequestResult into html.
/// \date 23/06/2022
/// \author Herzig Melvyn
class XpertRequestResultHtmlExport : public AbstractXpertRequestResultExport
{
public:

    /// \brief Constructor.
    XpertRequestResultHtmlExport();

    /// \brief Export the xpert request result to file. The export may fail. In that
    ///        case, the error message of the XpertRequestResult gets set.
    /// \param _xpertRequestResult Xpert request result to export.
    void exportToFile(XpertRequestResult& _xpertRequestResult) override;

protected:

    std::string makeHeaderString() const;

    std::string makeBodyString(const XpertRequestResult& _xpertRequestResult) const;

    void getHeaderJson(const XpertRequestResult& _xpertRequestResult, inja::json& _headerJson) const;

    void getDrugIntroJson(const XpertRequestResult& _xpertRequestResult, inja::json& _introJson) const;

    void getContactsJson(const std::unique_ptr<AdminData>& _admin, inja::json& _contactsJson) const;

    void getPersonJson(const std::unique_ptr<FullPersonData>& _person, inja::json& _personJson) const;

    void getInstituteJson(const std::unique_ptr<InstituteData>& _institute, inja::json& _instituteJson) const;

    void getAddressJson(const std::unique_ptr<AddressData>& _address, inja::json& _addressJson) const;

    void getPhoneJson(const std::unique_ptr<PhoneData>& _phone, inja::json& _phoneJson) const;

    void getEmailJson(const std::unique_ptr<EmailData>& _email, inja::json& _emailJson) const;

    void getClinicalDataJson(const std::unique_ptr<AdminData>& _admin, inja::json& _clinicalDataJson) const;

    void getCovariatesJson(const std::vector<CovariateValidationResult>& _covariateResults, inja::json& _covariatesJson) const;

    void getTreatmentJson(const Core::DosageHistory& _history, inja::json& _treatmentJson) const;

    void getTimeRangeJson(const std::unique_ptr<Core::DosageTimeRange>& _timeRange, inja::json& _treatmentRowsJson) const;

    void getAbstractDosageJson(const Core::Dosage& _dosage, inja::json& _dosageTimeRangeJson, const std::string& _posologyIndicationChain) const;

    void getDosageJson(const Core::DosageLoop& _dosage, inja::json& _dosageTimeRangeJson, const std::string& _posologyIndicationChain) const;

    void getDosageJson(const Core::DosageSteadyState& _dosage, inja::json& _dosageTimeRangeJson, const std::string& _posologyIndicationChain) const;

    void getDosageJson(const Core::DosageRepeat& _dosage, inja::json& _dosageTimeRangeJson, const std::string& _posologyIndicationChain) const;

    void getDosageJson(const Core::DosageSequence& _dosage, inja::json& _dosageTimeRangeJson, const std::string& _posologyIndicationChain) const;

    void getDosageJson(const Core::ParallelDosageSequence& _dosage, inja::json& _dosageTimeRangeJson, const std::string& _posologyIndicationChain) const;

    void getDosageJson(const Core::LastingDose& _dosage, inja::json& _dosageTimeRangeJson, const std::string& _posologyIndicationChain) const;

    void getDosageJson(const Core::DailyDose& _dosage, inja::json& _dosageTimeRangeJson, const std::string& _posologyIndicationChain) const;

    void getDosageJson(const Core::WeeklyDose& _dosage, inja::json& _dosageTimeRangeJson, const std::string& _posologyIndicationChain) const;

    void getSingleDoseJson(const Core::SingleDose& _dosage, inja::json& _dosageTimeRangeJson, const std::string& _posologyIndicationChain) const;

    std::string concatenatePosology(const std::string& _posologyIndication, const std::string& _posologyIndicationChain) const;

    std::string timeToString(const Common::TimeOfDay& _timeOfDay) const;


    template<typename T>
    void getWarningJson(const AbstractValidationResult<T>& _validationResult, inja::json& _json) const {

        if (!_validationResult.getWarning().empty()) {
            _json["warning"] = _validationResult.getWarning();
        }
    }

    /// \brief We need to keep a reference on the xpert request result in order
    ///        to retreive the dose validation results map, to retreive
    ///        the computation time and to retreive the output language.
    const XpertRequestResult* m_xpertRequestResultInUse;

};

} // namespace Xpert
} // namespace Tucuxi

#endif // XPERTREQUESTRESULTHTMLEXPORT_H
