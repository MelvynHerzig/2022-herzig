#include "modelselector.h"

namespace Tucuxi {
namespace XpertValidation {

ModelSelector::ModelSelector()
{}

ModelSelector::Status ModelSelector::getBestModelForQueryDrugs(const XpertQuery::XpertQueryData &_query, std::map<Query::DrugData *, std::string> &_modelIdPerDrug)
{
    return Status::Ok;
}

std::string ModelSelector::getErrorMessage() const
{
    return m_errorMessage;
}

void ModelSelector::setStatus(Status _status, const std::string& _errorMessage)
{
    m_status = _status;
    m_errorMessage += _errorMessage + "\n";
}

} // namespace XpertValidation
} // namespace Tucuxi
