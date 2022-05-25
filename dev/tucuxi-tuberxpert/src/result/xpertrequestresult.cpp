#include "xpertrequestresult.h"

using namespace std;

namespace Tucuxi {
namespace XpertResult {

XpertRequestResult::XpertRequestResult(unique_ptr<XpertQuery::XpertRequestData> _xpertRequest,
                                       unique_ptr<Core::DrugTreatment> _dTreatment,
                                       const optional<string>& _errorMessage)
    : m_xpertRequest(move(_xpertRequest)),
      m_dTreatment(move(_dTreatment)),
      m_errorMessage(_errorMessage),
      m_drugModel(nullptr)
{}

const std::unique_ptr<XpertQuery::XpertRequestData>& XpertRequestResult::getXpertRequest() const
{
    return m_xpertRequest;
}

const  std::unique_ptr<Core::DrugTreatment>& XpertRequestResult::getTreatment() const
{
    return m_dTreatment;
}

const std::optional<std::string>& XpertRequestResult::getErrorMessage() const
{
    return m_errorMessage;
}

const Core::DrugModel* XpertRequestResult::getDrugModel() const
{
    return m_drugModel;
}

const std::map<Core::CovariateDefinition*, CovariateResult>& XpertRequestResult::getCovariateResults()
{
    return m_covariateResults;
}

void XpertRequestResult::setDrugModel(const Core::DrugModel* _newDrugModel)
{
    m_drugModel = nullptr;
}

void XpertRequestResult::setCovariateResults(const std::map<Core::CovariateDefinition *, CovariateResult>& _newCovariateResults)
{
    m_covariateResults = move(_newCovariateResults);
}


bool XpertRequestResult::shouldBeHandled() const
{
    return m_errorMessage == nullopt;
}

} // namespace XpertResult
} // namespace Tucuxi
