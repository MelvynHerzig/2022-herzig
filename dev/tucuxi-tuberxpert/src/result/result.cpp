#include "result.h"

namespace Tucuxi {
namespace XpertResult {

Result::Result(const std::string& _lang) : m_language(_lang), m_generationDate(Common::DateTime::now()), m_query(nullptr)
{}

std::string Result::getLanguage()
{
    return m_language;
}

std::unique_ptr<XpertQuery::XpertQueryData>& Result::getQuery()
{
    return m_query;
}

Common::DateTime Result::getGenerationDate()
{
    return m_generationDate;
}

std::map<Query::DrugData *, DrugResult>& Result::getDrugResults()
{
    return m_drugResults;
}

} // namespace XpertResult
} // namespace Tucuxi
