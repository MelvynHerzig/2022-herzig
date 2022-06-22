#include "globalresult.h"

#include <optional>

#include "tuberxpert/query/xpertquerytocoreextractor.h"

using namespace std;

namespace Tucuxi {
namespace Xpert {

GlobalResult::GlobalResult(unique_ptr<XpertQueryData> _xpertQuery) :
    m_computationTime(_xpertQuery->getpQueryDate()), m_pAdministrative(_xpertQuery->moveAdmin())
{
    XpertQueryToCoreExtractor extractor;

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

Common::DateTime GlobalResult::getComputationTime() const
{
    return m_computationTime;
}

const unique_ptr<AdminData>& GlobalResult::getAdministrative() const
{
    return m_pAdministrative;
}

std::vector<XpertRequestResult>& GlobalResult::getXpertRequestResults()
{
    return m_xpertRequestResults;
}

} // namespace Xpert
} // namespace Tucuxi
