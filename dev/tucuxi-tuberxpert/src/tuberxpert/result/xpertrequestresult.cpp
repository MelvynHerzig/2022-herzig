#include "xpertrequestresult.h"

#include "tuberxpert/utils/xpertutils.h"
#include "tuberxpert/result/globalresult.h"

using namespace std;

namespace Tucuxi {
namespace Xpert {

XpertRequestResult::XpertRequestResult(
        const GlobalResult& _xpertGlobalResult,
        unique_ptr<XpertRequestData> _xpertRequest,
        unique_ptr<Core::DrugTreatment> _drugTreatment,
        const string& _errorMessage)
    : m_xpertGlobalResult(_xpertGlobalResult),
      m_xpertRequest(move(_xpertRequest)),
      m_drugTreatment(move(_drugTreatment)),
      m_errorMessage(_errorMessage),
      m_drugModel(nullptr),
      m_adjustmentTrait(nullptr),
      m_adjustmentData(nullptr),
      m_lastIntake(nullptr)
{}

const XpertRequestData& XpertRequestResult::getXpertRequest() const
{
    return *m_xpertRequest;
}

const  unique_ptr<Core::DrugTreatment>& XpertRequestResult::getTreatment() const
{
    return m_drugTreatment;
}

const string& XpertRequestResult::getErrorMessage() const
{
    return m_errorMessage;
}

const Core::DrugModel* XpertRequestResult::getDrugModel() const
{
    return m_drugModel;
}

const vector<CovariateValidationResult>& XpertRequestResult::getCovariateResults() const
{
    return m_covariateResults;
}

const map<const Core::SingleDose*, DoseValidationResult>& XpertRequestResult::getDoseResults() const
{
    return m_doseResults;
}

const vector<SampleValidationResult>& XpertRequestResult::getSampleResults() const
{
    return m_sampleResults;
}

const unique_ptr<Core::ComputingTraitAdjustment>& XpertRequestResult::getAdjustmentTrait() const
{
    return m_adjustmentTrait;
}

const std::unique_ptr<Core::AdjustmentData>& XpertRequestResult::getAdjustmentData() const
{
    return m_adjustmentData;
}

const std::unique_ptr<Core::IntakeEvent>& XpertRequestResult::getLastIntake() const
{
    return m_lastIntake;
}

const GlobalResult& XpertRequestResult::getGlobalResult() const
{
    return m_xpertGlobalResult;
}

const std::vector<std::vector<Core::ParameterValue>>& XpertRequestResult::getParameters() const
{
    return m_parameters;
}

const Core::CycleStats &XpertRequestResult::getCycleStats() const
{
    return m_cycleStats;
}

void XpertRequestResult::setErrorMessage(const string& _message)
{
    m_errorMessage = _message;
}

void XpertRequestResult::setDrugModel(const Core::DrugModel* _newDrugModel)
{
    m_drugModel = _newDrugModel;
}

void XpertRequestResult::setCovariateResults(vector<CovariateValidationResult>&& _newCovariateResults)
{
    m_covariateResults = _newCovariateResults;

    // Sort the covariate results by name and by date.
    OutputLang lang = m_xpertRequest->getOutputLang();
    sort(m_covariateResults.begin(), m_covariateResults.end(),
         [lang](const CovariateValidationResult& cvr1, const CovariateValidationResult& cvr2) {

        // Try to sort by name
        string cvr1Name = getStringWithEnglishFallback(cvr1.getSource()->getName(), lang);
        string cvr2Name = getStringWithEnglishFallback(cvr2.getSource()->getName(), lang);

        if (cvr1Name != cvr2Name) {
            return cvr1Name < cvr2Name;
        // Else try by date.
        // Normaly this if is always true. Just a fool guard.
        } else if(cvr1.getPatient() != nullptr && cvr2.getPatient() != nullptr){
            return cvr1.getPatient()->getEventTime() < cvr2.getPatient()->getEventTime();
        }

        // Should never happen
        return true;
    });
}

void XpertRequestResult::setDoseResults(map<const Core::SingleDose*, DoseValidationResult>&& _newDoseResults)
{
    m_doseResults = _newDoseResults;
}

void XpertRequestResult::setSampleResults(vector<SampleValidationResult>&& _newSampleResults)
{
    m_sampleResults = _newSampleResults;

    // Sort the sample results by date and by analyteId.
    sort(m_sampleResults.begin(), m_sampleResults.end(),
         [](const SampleValidationResult& svr1, const SampleValidationResult& svr2) {


        // Try by date
        if (svr1.getSource()->getDate() != svr2.getSource()->getDate()) {
            return svr1.getSource()->getDate() < svr2.getSource()->getDate();
        // Else try by analyteID.
        } else {
            return svr1.getSource()->getAnalyteID() < svr2.getSource()->getAnalyteID();
        }
    });
}

void XpertRequestResult::setAdjustmentTrait(const Core::ComputingTraitAdjustment& _adjustmentTrait)
{
    m_adjustmentTrait = make_unique<Core::ComputingTraitAdjustment>(_adjustmentTrait);
}

void XpertRequestResult::setAdjustmentData(unique_ptr<Core::AdjustmentData> _adjustmentData)
{
    m_adjustmentData = move(_adjustmentData);
}

void XpertRequestResult::setLastIntake(unique_ptr<Core::IntakeEvent> _lastIntake)
{
    m_lastIntake = move(_lastIntake);
}

void XpertRequestResult::addParameters(const std::vector<Core::ParameterValue>& _parameters)
{
    auto it = m_parameters.begin();
    m_parameters.emplace(it, _parameters);
}

void XpertRequestResult::setCycleStats(const Core::CycleStats _cycleStats)
{
    m_cycleStats = _cycleStats;
}

bool XpertRequestResult::shouldBeHandled() const
{
    return m_errorMessage == "";
}

} // namespace Xpert
} // namespace Tucuxi
