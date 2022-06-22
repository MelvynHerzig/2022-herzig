#ifndef XPERTREQUESTRESULT_H
#define XPERTREQUESTRESULT_H

#include <string>
#include <vector>

#include "tucucore/drugmodel/covariatedefinition.h"
#include "tucucore/drugmodel/drugmodel.h"
#include "tucucore/drugtreatment/drugtreatment.h"
#include "tucucore/dosage.h"
#include "tucucore/computingservice/computingrequest.h"
#include "tucucore/computingservice/computingtrait.h"
#include "tucucore/computingservice/computingresponse.h"
#include "tucucore/intakeevent.h"

#include "tuberxpert/query/xpertrequestdata.h"
#include "tuberxpert/result/covariatevalidationresult.h"
#include "tuberxpert/result/dosevalidationresult.h"
#include "tuberxpert/result/samplevalidationresult.h"

namespace Tucuxi {
namespace Xpert {

class GlobalResult;

/// \brief This is object stores the results of tuberXpert in regards of
///        a given requestXpert. This object is filled along the tuberXpert execution flow.
/// \date 20/05/2022
/// \author Herzig Melvyn
class XpertRequestResult
{
public:

    /// \brief Constructor. Used in XpertResult construction.
    /// \param _xpertGlobalResult Where to retrieve the computation time and the administrative data.
    ///                           Must survive as long as this object is alive (reference stored).
    /// \param _xpertRequest Related requestXpert.
    /// \param _drugTreatment Associated treatment if extraction was successfull.
    /// \param _errorMessage If the treatment extraction was not successfull, the related error message or empty string.
    XpertRequestResult(
            const GlobalResult* _xpertGlobalResult,
            std::unique_ptr<XpertRequestData> _xpertRequest,
            std::unique_ptr<Core::DrugTreatment> _drugTreatment,
            const std::string& _errorMessage);

    /// \brief Gets the related XpertRequestData.
    /// \return A reference on the XpertRequestData.
    const XpertRequestData& getXpertRequest() const;

    /// \brief Gets the related treatment.
    /// \return The related treatment. May be nullptr if extraction failed.
    const std::unique_ptr<Core::DrugTreatment>& getTreatment() const;

    /// \brief Gets the error message that might be set during the "pipeline".
    /// \return The error message. Empty string if everything is fine.
    const std::string& getErrorMessage() const;

    /// \brief Gets the drug model chosen during ModelSelector step.
    /// \return The drug model selected or nullptr if none.
    const Core::DrugModel* getDrugModel() const;

    /// \brief Gets the covariates results.
    /// \return The vector containing each CovariateResult for each covariates. May be empty if
    ///         model selection failed.
    const std::vector<CovariateValidationResult>& getCovariateResults();

    /// \brief Gets the doses results.
    /// \return The vector containing each DoseResult for each dose found in the treatment.
    ///         This may be empty if there is no dosage or if the doses validation failed.
    const std::map<const Core::SingleDose*, DoseValidationResult>& getDoseResults();

    /// \brief Gets the samples results.
    /// \return The vector containing each SampleResult for each sample found in the treatment.
    ///         This may be empty if there is no sample or if the samples validation failed.
    const std::map<const Core::Sample*, SampleValidationResult>& getSampleResults();

    /// \brief Get a unique pointer on the adjustment trait used to make the computing request.
    ///        May be nullptr.
    /// \return A constant unique pointer on the adjustment trait.
    const std::unique_ptr<Core::ComputingTraitAdjustment>& getAdjustmentTrait();

    /// \brief Get a unique pointer on the adjustment data retrieved by the computation of the adjustment trait.
    ///        May be nullptr.
    /// \return A constant unique pointer on the adjustment data.
    const std::unique_ptr<Core::AdjustmentData>& getAdjustmentData();

    /// \brief Get a unique pointer on the last intake of the patient.
    ///        May be nullptr since it is possible to not have any last intake.
    /// \return A constant unique pointer on the intake event.
    const std::unique_ptr<Core::IntakeEvent>& getLastIntake();

    /// \brief Get a constant pointer on the GlobalResult object that contains common information
    ///        for all the XpertRequestResult as well as all the XpertRequestResult. This pointer
    ///        is not to be deleted.
    /// \return Return a constant pointer on the XpertResult held by this object.
    ///         May be nullptr.
    const GlobalResult* getGlobalResult() const;

    /// \brief Sets a new error message.
    /// \param _message New message to set.
    void setErrorMessage(const std::string& _message);

    /// \brief Sets a new drug model.
    /// \param _newDrugModel New drug model pointer.
    void setDrugModel(const Core::DrugModel* _newDrugModel);

    /// \brief Sets a new CovariateResult vector.
    /// \param _newCovariateResults CovariateResult vector to retrieve.
    void setCovariateResults(std::vector<CovariateValidationResult>&& _newCovariateResults);

    /// \brief Sets a new DoseRsult map.
    /// \param _newDoseResults DoseResult map to retrieve.
    void setDoseResults(std::map<const Core::SingleDose*, DoseValidationResult>&& _newDoseResults);

    /// \brief Sets a new SampleRsult map.
    /// \param _newSampleResults SampleResult map to retrieve.
    void setSampleResults(std::map<const Core::Sample*, SampleValidationResult>&& _newSampleResults);

    /// \brief Set the adjustment trait to use in order to create a computing request for the core.
    /// \param _adjustmentTrait Computing adjustment trait to retrieve.
    void setAdjustmentTrait(const Core::ComputingTraitAdjustment& _adjustmentTrait);

    /// \brief Set the adjustment data that has been retrieved after the execution of the adjustment trait.
    /// \param _adjustmentTrait Computing adjustment trait to retrieve.
    void setAdjustmentData(std::unique_ptr<Core::AdjustmentData> _adjustmentData);

    /// \brief Set the last intake of the patient.
    /// \param _lastIntake Last intake to save.
    void setLastIntake(std::unique_ptr<Core::IntakeEvent> _lastIntake);

    /// \brief Checks if the XpertRequestResult should go to next pipeline step.
    /// \return True if no problem was detected until the call otherwise false.
    bool shouldBeHandled() const;

protected:

    /// \brief Where to retrieve the computation time and the administrative data
    ///        /!\ No need to free
    const GlobalResult* m_xpertGlobalResult;

    /// \brief Unique pointer to the related request this object stores results for.
    std::unique_ptr<XpertRequestData> m_xpertRequest;

    /// \brief Treatment related to the request of this object.
    std::unique_ptr<Core::DrugTreatment> m_drugTreatment;

    /// \brief Error message possibly set during a step of the flow.
    std::string m_errorMessage;

    /// \brief Drug model chosen during ModelSelector phase.
    const Core::DrugModel* m_drugModel;

    /// \brief Result for each covariate made during ModelSelector phase.
    ///        One entry per covariate present and per definition missing in regards
    ///        of the selected drug model.
    std::vector<CovariateValidationResult> m_covariateResults;

    /// \brief Result for each dose made during DoseValidator phase.
    ///        One entry per dose found. The map keys are the same pointers
    ///        stored in each DoseResult.
    std::map<const Core::SingleDose*, DoseValidationResult> m_doseResults;

    /// \brief Result for each sample made during SampleValidator phase.
    ///        One entry per sample found. The map keys are the same pointers
    ///        stored in each SampleResult.
    std::map<const Core::Sample*, SampleValidationResult> m_sampleResults;

    /// \brief Adjustment trait used to make the computing request.
    std::unique_ptr<Core::ComputingTraitAdjustment> m_adjustmentTrait;

    /// \brief Adjustment data retrieved after submitting the adjustment trait to the core.
    std::unique_ptr<Core::AdjustmentData> m_adjustmentData;

    /// \brief Pointer on the last intake of the patient.
    std::unique_ptr<Core::IntakeEvent> m_lastIntake;
};

} // namespace Xpert
} // namespace Tucuxi

#endif // XPERTREQUESTRESULT_H
