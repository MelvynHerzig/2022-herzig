#include "drugresult.h"

namespace Tucuxi {
namespace XpertResult {

DrugResult::DrugResult(const Core::DrugModel* _drugModel) : m_drugModel(_drugModel)
{}

const Core::DrugModel* DrugResult::getDrugModel() const
{
    return m_drugModel;
}

std::map<Core::CovariateDefinition*, AbsractCovariateResult*>& DrugResult::getCovariateResults()
{
    return m_covariateResults;
}

} // namespace XpertResult
} // namespace Tucuxi
