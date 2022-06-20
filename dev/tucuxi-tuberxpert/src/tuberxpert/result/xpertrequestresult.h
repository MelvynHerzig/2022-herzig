#ifndef XPERTREQUESTRESULT_H
#define XPERTREQUESTRESULT_H

#include <string>
#include <vector>
#include <optional>

#include "tucucore/drugmodel/covariatedefinition.h"
#include "tucucore/drugmodel/drugmodel.h"
#include "tucucore/drugtreatment/drugtreatment.h"
#include "tucucore/dosage.h"
#include "tucucore/computingservice/computingrequest.h"
#include "tucucore/computingservice/computingtrait.h"

#include "tuberxpert/query/xpertrequestdata.h"
#include "tuberxpert/result/covariateresult.h"
#include "tuberxpert/result/doseresult.h"
#include "tuberxpert/result/sampleresult.h"

namespace Tucuxi {
namespace XpertResult {

/// \brief This is object stores the results of tuberXpert in regards of
///        a given requestXpert. This object is filled along the tuberXpert "pipeline".
/// \date 20/05/2022
/// \author Herzig Melvyn
class XpertRequestResult
{
public:

    /// \brief Constructor. Used in XpertResult construction.
    /// \param _xpertRequest Related requestXpert.
    /// \param _dTreatment Associated treatment if extraction was successfull.
    /// \param _errorMessage If the treatment extraction was not successfull, the related error message or empty string.
    XpertRequestResult(
            std::unique_ptr<XpertQuery::XpertRequestData> _xpertRequest,
            std::unique_ptr<Core::DrugTreatment> _dTreatment,
            const std::string& _errorMessage);

    /// \brief Gets the related XpertRequestData.
    /// \return A reference on the XpertRequestData.
    const XpertQuery::XpertRequestData& getXpertRequest() const;

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
    const std::vector<CovariateResult>& getCovariateResults();

    /// \brief Gets the doses results.
    /// \return The vector containing each DoseResult for each dose found in the treatment.
    ///         This may be empty if there is no dosage or if the doses validation failed.
    const std::map<const Core::SingleDose*, DoseResult>& getDoseResults();

    /// \brief Gets the samples results.
    /// \return The vector containing each SampleResult for each sample found in the treatment.
    ///         This may be empty if there is no sample or if the samples validation failed.
    const std::map<const Core::Sample*, SampleResult>& getSampleResults();

    /// \brief Get a unique pointer on the adjustment trait used to make the computing request.
    ///        May be nullptr.
    /// \return A constant unique pointer on the adjustment trait.
    const std::unique_ptr<Core::ComputingTraitAdjustment>& getAdjustmentTrait();

    /// \brief Sets a new error message.
    /// \param _message New message to set.
    void setErrorMessage(const std::string& _message);

    /// \brief Sets a new drug model.
    /// \param _newDrugModel New drug model pointer.
    void setDrugModel(const Core::DrugModel* _newDrugModel);

    /// \brief Sets a new CovariateResult vector.
    /// \param _newCovariateResults CovariateResult vector to retrieve.
    void setCovariateResults(std::vector<CovariateResult>&& _newCovariateResults);

    /// \brief Sets a new DoseRsult map.
    /// \param _newDoseResults DoseResult map to retrieve.
    void setDoseResults(std::map<const Core::SingleDose*, DoseResult>&& _newDoseResults);

    /// \brief Sets a new SampleRsult map.
    /// \param _newSampleResults SampleResult map to retrieve.
    void setSampleResults(std::map<const Core::Sample*, SampleResult>&& _newSampleResults);

    /// \brief Set the adjustment trait to use in order to create a computing request for the core.
    /// \param _adjustmentTrait Computing adjustment trait to retrieve.
    void setAdjustmentTrait(const Core::ComputingTraitAdjustment& _adjustmentTrait);

    /// \brief Checks if the XpertRequestResult should go to next pipeline step.
    /// \return True if no problem was detected until the call otherwise false.
    bool shouldBeHandled() const;

protected:

    /// \brief Unique pointer to the related request this object stores results for.
    std::unique_ptr<XpertQuery::XpertRequestData> m_xpertRequest;

    /// \brief Treatment related to the request of this object.
    std::unique_ptr<Core::DrugTreatment> m_dTreatment;

    /// \brief Error message possibly set during a step of the pipeline.
    std::string m_errorMessage;

    /// \brief Drug model chosen during ModelSelector phase.
    const Core::DrugModel* m_drugModel;

    /// \brief Result for each covariate made during ModelSelector phase.
    ///        One entry per covariate present and per definition missing in regards
    ///        of the selected drug model.
    std::vector<CovariateResult> m_covariateResults;

    /// \brief Result for each dose made during DoseValidator phase.
    ///        One entry per dose found. The map keys are the same pointers
    ///        stored in each DoseResult.
    std::map<const Core::SingleDose*, DoseResult> m_doseResults;

    /// \brief Result for each sample made during SampleValidator phase.
    ///        One entry per sample found. The map keys are the same pointers
    ///        stored in each SampleResult.
    std::map<const Core::Sample*, SampleResult> m_sampleResults;

    /// \brief m_adjustmentTrait Adjustment trait used to make the computing request.
    std::unique_ptr<Core::ComputingTraitAdjustment> m_adjustmentTrait;

};

} // namespace XpertResult
} // namespace Tucuxi

#endif // XPERTREQUESTRESULT_H
