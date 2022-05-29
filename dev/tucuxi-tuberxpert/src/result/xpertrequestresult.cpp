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

void XpertRequestResult::setDosageResults(std::map<const Core::SingleDose*, DoseResult> &&_newDoseResults)
{
    m_doseResults = _newDoseResults;
}

bool XpertRequestResult::shouldBeHandled() const
{
    return m_errorMessage == "";
}

} // namespace XpertResult
} // namespace Tucuxi
