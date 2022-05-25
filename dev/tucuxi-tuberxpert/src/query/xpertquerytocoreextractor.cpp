#include "xpertquerytocoreextractor.h"

using namespace std;

namespace Tucuxi {
namespace XpertQuery {

unique_ptr<Core::DrugTreatment> XpertQuery::XpertQueryToCoreExtractor::extractDrugTreatment(
        const unique_ptr<XpertRequestData>& _xpertRequest,
        const XpertQueryData& _xpertQuery,
        optional<string>& _errorMessage) const
{
    string drugId = _xpertRequest->getDrugID();

    // Get the number of matching drugs by id.
    auto drugDataBegin = _xpertQuery.getpParameters().getDrugs().begin();
    auto drugDataEnd = _xpertQuery.getpParameters().getDrugs().end();

    // Use lambda to count the matching drug data.
    int nbMatchingDrug = count_if(drugDataBegin, drugDataEnd, [drugId](const unique_ptr<Query::DrugData>& drugData) {
        if (drugData->getDrugID()  == drugId) {
            return true;
        }
        return false;
    });

    // If there is none or multiple drugs matchins
    if (nbMatchingDrug != 1) {
        if (nbMatchingDrug == 0) {
            // TODO translation
            _errorMessage = "Could not fullfil request for " + drugId + ", drug not found in query";
        } else {
            _errorMessage = "Could not fullfil request for " + drugId + ", drug found multiple times in query";
        }
        return nullptr;
    }

    // There is only one drug matching, extract the tratment
    _errorMessage = nullopt;
    string requestId = "";
    string drugModelId = "";
    Query::RequestData rd {requestId, drugId, drugModelId, nullptr};
    return QueryToCoreExtractor::extractDrugTreatment(_xpertQuery, rd);
}

} // namespace XpertQuery
} // namespace Tucuxi
