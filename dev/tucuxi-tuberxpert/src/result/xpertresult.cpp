#include "xpertresult.h"

namespace Tucuxi {
namespace XpertResult {

XpertResult::XpertResult() : m_generationDate(Common::DateTime::now()), m_query(nullptr)
{}

Common::DateTime XpertResult::getGenerationDate()
{
    return m_generationDate;
}

std::unique_ptr<XpertQuery::XpertQueryData>& XpertResult::getQuery()
{
    return m_query;
}

std::map<XpertQuery::XpertRequestData*, XpertRequestResult>& XpertResult::getXpertRequestResults()
{
    return m_xpertRequestResults;
}

} // namespace XpertResult
} // namespace Tucuxi
