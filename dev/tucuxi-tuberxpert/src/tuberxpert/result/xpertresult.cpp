#include "xpertresult.h"

#include <optional>

#include "tuberxpert/query/xpertquerytocoreextractor.h"

using namespace std;

namespace Tucuxi {
namespace XpertResult {

XpertGlobalResult::XpertGlobalResult(unique_ptr<XpertQuery::XpertQueryData> _xpertQuery) :
    m_computationTime(_xpertQuery->getpQueryDate()), m_pAdministrative(_xpertQuery->movepAdministrative())
{
    XpertQuery::XpertQueryToCoreExtractor extractor;

    // For each requestXpert, extract the treatment.
    for (size_t i = 0; i < _xpertQuery->getXpertRequests().size(); ++i) {
        string errorMessage;
        unique_ptr<Core::DrugTreatment> drugTreatment = extractor.extractDrugTreatment(
                    _xpertQuery->getXpertRequests()[i],
                    *_xpertQuery,
                    errorMessage);

        m_xpertRequestResults.emplace_back(this, _xpertQuery->moveXpertRequest(i), move(drugTreatment), errorMessage);
    }
}

Common::DateTime XpertGlobalResult::getComputationTime() const
{
    return m_computationTime;
}

const unique_ptr<XpertQuery::AdministrativeData>& XpertGlobalResult::getAdministrative() const
{
    return m_pAdministrative;
}

std::vector<XpertRequestResult>& XpertGlobalResult::getXpertRequestResults()
{
    return m_xpertRequestResults;
}

} // namespace XpertResult
} // namespace Tucuxi
