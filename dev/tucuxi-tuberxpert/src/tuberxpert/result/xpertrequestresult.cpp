#include "xpertrequestresult.h"

#include "tuberxpert/utils/xpertutils.h"
#include "tuberxpert/result/xpertqueryresult.h"

using namespace std;

namespace Tucuxi {
namespace Xpert {

XpertRequestResult::XpertRequestResult(
        const XpertQueryResult& _xpertQueryResult,
        unique_ptr<XpertRequestData> _xpertRequest,
        unique_ptr<Core::DrugTreatment> _drugTreatment,
        const string& _errorMessage):
    m_xpertQueryResult(_xpertQueryResult),
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

string XpertRequestResult::getErrorMessage() const
{
    return m_errorMessage;
}

const Core::DrugModel* XpertRequestResult::getDrugModel() const
{
    return m_drugModel;
}

const vector<CovariateValidationResult>& XpertRequestResult::getCovariateValidationResults() const
{
    return m_covariateValidationResults;
}

const map<const Core::SingleDose*, DoseValidationResult>& XpertRequestResult::getDoseValidationResults() const
{
    return m_doseValidationResults;
}

const vector<SampleValidationResult>& XpertRequestResult::getSampleValidationResults() const
{
    return m_sampleValidationResults;
}

const unique_ptr<Core::ComputingTraitAdjustment>& XpertRequestResult::getAdjustmentTrait() const
{
    return m_adjustmentTrait;
}

const unique_ptr<Core::AdjustmentData>& XpertRequestResult::getAdjustmentData() const
{
    return m_adjustmentData;
}

const unique_ptr<Core::IntakeEvent>& XpertRequestResult::getLastIntake() const
{
    return m_lastIntake;
}

const XpertQueryResult& XpertRequestResult::getXpertQueryResult() const
{
    return m_xpertQueryResult;
}

const vector<vector<Core::ParameterValue>>& XpertRequestResult::getParameters() const
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

void XpertRequestResult::setDrugModel(const Core::DrugModel* _drugModel)
{
    m_drugModel = _drugModel;
}

void XpertRequestResult::setCovariateResults(vector<CovariateValidationResult>&& _covariateValidationResults)
{
    m_covariateValidationResults = _covariateValidationResults;

    // Sort the covariate validation results by name and date.
    OutputLang lang = m_xpertRequest->getOutputLang();
    sort(m_covariateValidationResults.begin(), m_covariateValidationResults.end(),
         [lang](const CovariateValidationResult& covValRes_1, const CovariateValidationResult& covValRes_2) {

        // Try to sort by name.
        string covaritateName_1 = getStringWithEnglishFallback(covValRes_1.getSource()->getName(), lang);
        string covariateName_2 = getStringWithEnglishFallback(covValRes_2.getSource()->getName(), lang);

        if (covaritateName_1 != covariateName_2) {
            return covaritateName_1 < covariateName_2;
        // Otherwhise try by date.
        // Normally this if is always true. The only time there is the
        // same name is when the patient has multiple measures.
        // So, this is just a fool guard.
        } else if(covValRes_1.getPatient() != nullptr && covValRes_2.getPatient() != nullptr){
            return covValRes_1.getPatient()->getEventTime() < covValRes_2.getPatient()->getEventTime();
        }

        // Should never happen.
        return true;
    });
}

void XpertRequestResult::setDoseResults(map<const Core::SingleDose*, DoseValidationResult>&& _doseValidationResults)
{
    m_doseValidationResults = _doseValidationResults;
}

void XpertRequestResult::setSampleResults(vector<SampleValidationResult>&& _sampleValidationResults)
{
    m_sampleValidationResults = _sampleValidationResults;

    // Sort the sample results by date and by analyteId.
    sort(m_sampleValidationResults.begin(), m_sampleValidationResults.end(),
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

void XpertRequestResult::addParameters(const vector<Core::ParameterValue>& _parameters)
{
    auto it = m_parameters.begin();
    m_parameters.emplace(it, _parameters);
}

void XpertRequestResult::setCycleStats(const Core::CycleStats _cycleStats)
{
    m_cycleStats = _cycleStats;
}

bool XpertRequestResult::shouldContinueProcessing() const
{
    return m_errorMessage == "";
}

} // namespace Xpert
} // namespace Tucuxi
