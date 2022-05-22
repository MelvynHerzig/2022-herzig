#include "xpertrequestresult.h"

using namespace std;

namespace Tucuxi {
namespace XpertResult {

XpertRequestResult::XpertRequestResult()
{}

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
    m_drugModel = _newDrugModel;
}

void XpertRequestResult::setCovariateResults(const std::map<Core::CovariateDefinition *, CovariateResult>& _newCovariateResults)
{
    m_covariateResults = move(_newCovariateResults);
}

} // namespace XpertResult
} // namespace Tucuxi
