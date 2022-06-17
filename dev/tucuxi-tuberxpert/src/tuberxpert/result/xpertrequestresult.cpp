#include "xpertrequestresult.h"

using namespace std;

namespace Tucuxi {
namespace XpertResult {

XpertRequestResult::XpertRequestResult(unique_ptr<XpertQuery::XpertRequestData> _xpertRequest,
                                       unique_ptr<Core::DrugTreatment> _dTreatment,
                                       const string& _errorMessage)
    : m_xpertRequest(move(_xpertRequest)),
      m_dTreatment(move(_dTreatment)),
      m_errorMessage(_errorMessage),
      m_drugModel(nullptr)
{}

const XpertQuery::XpertRequestData& XpertRequestResult::getXpertRequest() const
{
    return *m_xpertRequest;
}

const  std::unique_ptr<Core::DrugTreatment>& XpertRequestResult::getTreatment() const
{
    return m_dTreatment;
}

const std::string& XpertRequestResult::getErrorMessage() const
{
    return m_errorMessage;
}

const Core::DrugModel* XpertRequestResult::getDrugModel() const
{
    return m_drugModel;
}

const vector<CovariateResult>& XpertRequestResult::getCovariateResults()
{
    return m_covariateResults;
}

const std::map<const Core::SingleDose*, DoseResult>& XpertRequestResult::getDoseResults()
{
    return m_doseResults;
}

const std::map<const Core::Sample *, SampleResult>& XpertRequestResult::getSampleResults()
{
    return m_sampleResults;
}

const std::unique_ptr<Core::ComputingTraitAdjustment>& XpertRequestResult::getAdjustmentTrait()
{
    return m_adjustmentTrait;
}

void XpertRequestResult::setErrorMessage(const std::string& _message)
{
    m_errorMessage = _message;
}

void XpertRequestResult::setDrugModel(const Core::DrugModel* _newDrugModel)
{
    m_drugModel = _newDrugModel;
}

void XpertRequestResult::setCovariateResults(std::vector<CovariateResult>&& _newCovariateResults)
{
    m_covariateResults = _newCovariateResults;
}

void XpertRequestResult::setDoseResults(std::map<const Core::SingleDose*, DoseResult>&& _newDoseResults)
{
    m_doseResults = _newDoseResults;
}

void XpertRequestResult::setSampleResults(std::map<const Core::Sample*, SampleResult>&& _newSampleResults)
{
    m_sampleResults = _newSampleResults;
}

void XpertRequestResult::setAdjustmentTrait(std::unique_ptr<Core::ComputingTraitAdjustment> _adjustmentTrait)
{
    m_adjustmentTrait = move(_adjustmentTrait);
}

bool XpertRequestResult::shouldBeHandled() const
{
    return m_errorMessage == "";
}

} // namespace XpertResult
} // namespace Tucuxi
