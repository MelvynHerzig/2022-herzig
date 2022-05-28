#include "xpertresult.h"

#include <optional>

#include "../query/xpertquerytocoreextractor.h"

using namespace std;

namespace Tucuxi {
namespace XpertResult {

XpertResult::XpertResult(unique_ptr<XpertQuery::XpertQueryData> _xpertQuery) :
    m_generationDate(Common::DateTime::now()), m_pAdministrative(_xpertQuery->movepAdministrative())
{
    XpertQuery::XpertQueryToCoreExtractor extractor;

    // For each requestXpert, extract the treatment.
    for (size_t i = 0; i < _xpertQuery->getXpertRequests().size(); ++i) {
        string errorMessage;
        unique_ptr<Core::DrugTreatment> drugTreatment = extractor.extractDrugTreatment(
                    _xpertQuery->getXpertRequests()[i],
                    *_xpertQuery,
                    errorMessage);

        m_xpertRequestResults.emplace_back(_xpertQuery->moveXpertRequest(i), move(drugTreatment), errorMessage);
    }
}

Common::DateTime XpertResult::getGenerationDate()
{
    return m_generationDate;
}

const unique_ptr<XpertQuery::AdministrativeData>& XpertResult::getAdministrative() const
{
    return m_pAdministrative;
}

std::vector<XpertRequestResult>& XpertResult::getXpertRequestResults()
{
    return m_xpertRequestResults;
}

} // namespace XpertResult
} // namespace Tucuxi
