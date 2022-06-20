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
      m_drugModel(nullptr),
      m_adjustmentTrait(nullptr)
{}

const XpertQuery::XpertRequestData& XpertRequestResult::getXpertRequest() const
{
    return *m_xpertRequest;
}

const  unique_ptr<Core::DrugTreatment>& XpertRequestResult::getTreatment() const
{
    return m_dTreatment;
}

const string& XpertRequestResult::getErrorMessage() const
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

const map<const Core::SingleDose*, DoseResult>& XpertRequestResult::getDoseResults()
{
    return m_doseResults;
}

const map<const Core::Sample*, SampleResult>& XpertRequestResult::getSampleResults()
{
    return m_sampleResults;
}

const unique_ptr<Core::ComputingTraitAdjustment>& XpertRequestResult::getAdjustmentTrait()
{
    return m_adjustmentTrait;
}

void XpertRequestResult::setErrorMessage(const string& _message)
{
    m_errorMessage = _message;
}

void XpertRequestResult::setDrugModel(const Core::DrugModel* _newDrugModel)
{
    m_drugModel = _newDrugModel;
}

void XpertRequestResult::setCovariateResults(vector<CovariateResult>&& _newCovariateResults)
{
    m_covariateResults = _newCovariateResults;
}

void XpertRequestResult::setDoseResults(map<const Core::SingleDose*, DoseResult>&& _newDoseResults)
{
    m_doseResults = _newDoseResults;
}

void XpertRequestResult::setSampleResults(map<const Core::Sample*, SampleResult>&& _newSampleResults)
{
    m_sampleResults = _newSampleResults;
}

void XpertRequestResult::setAdjustmentTrait(const Core::ComputingTraitAdjustment& _adjustmentTrait)
{
    m_adjustmentTrait = make_unique<Core::ComputingTraitAdjustment>(_adjustmentTrait);
}

bool XpertRequestResult::shouldBeHandled() const
{
    return m_errorMessage == "";
}

} // namespace XpertResult
} // namespace Tucuxi
