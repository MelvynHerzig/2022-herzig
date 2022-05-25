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

    for (size_t i = 0; i < _xpertQuery->getXpertRequests().size(); ++i) {
        optional<string> errorMessage;
        /*unique_ptr<Core::DrugTreatment> drugTreatment = extractor.extractDrugTreatment(
                    _xpertQuery->getXpertRequests()[i],
                    *_xpertQuery,
                    errorMessage);*/

        //m_xpertRequestResults.emplace_back(_xpertQuery->moveXpertRequest(i), move(nullptr), errorMessage);
    }
}

Common::DateTime XpertResult::getGenerationDate()
{
    return m_generationDate;
}

} // namespace XpertResult
} // namespace Tucuxi
