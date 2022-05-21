#include "xpertresult.h"

namespace Tucuxi {
namespace XpertResult {

XpertResult::XpertResult() : m_generationDate(Common::DateTime::now()), m_query(nullptr)
{}

std::unique_ptr<XpertQuery::XpertQueryData>& XpertResult::getQuery()
{
    return m_query;
}

Common::DateTime XpertResult::getGenerationDate()
{
    return m_generationDate;
}

std::map<XpertQuery::XpertRequestData*, DrugResult>& XpertResult::getDrugResults()
{
    return m_drugResults;
}

} // namespace XpertResult
} // namespace Tucuxi
