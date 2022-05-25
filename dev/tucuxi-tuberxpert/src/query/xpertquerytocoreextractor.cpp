#include "xpertquerytocoreextractor.h"

#include "../language/languagemanager.h"

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

        Tucuxi::XpertLanguage::LanguageManager& lm = Tucuxi::XpertLanguage::LanguageManager::getInstance();

        if (nbMatchingDrug == 0) {
            // TODO translation
            _errorMessage = lm.translate("absent_drug_extraction_error");
        } else {
            _errorMessage = lm.translate("multiple_drug_extraction_error");
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
