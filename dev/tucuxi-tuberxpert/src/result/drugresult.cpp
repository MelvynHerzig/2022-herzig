#include "drugresult.h"

namespace Tucuxi {
namespace XpertResult {

DrugResult::DrugResult(const std::string& _drugModelId) : m_drugModelId(_drugModelId)
{}

const std::string& DrugResult::drugModelId() const
{
    return m_drugModelId;
}

std::map<Core::CovariateDefinition*, AbsractCovariateResult*>& DrugResult::getCovariateResults()
{
    return m_covariateResults;
}

} // namespace XpertResult
} // namespace Tucuxi
