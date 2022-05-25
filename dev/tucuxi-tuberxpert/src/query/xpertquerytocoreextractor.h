#ifndef XPERTQUERYTOCOREEXTRACTOR_H
#define XPERTQUERYTOCOREEXTRACTOR_H

#include <optional>
#include <string>

#include "tucuquery/querytocoreextractor.h"
#include "tucucore/drugtreatment/drugtreatment.h"

#include "xpertrequestdata.h"
#include "xpertquerydata.h"

namespace Tucuxi {
namespace XpertQuery {

class XpertQueryToCoreExtractor : Query::QueryToCoreExtractor
{
public:
    std::unique_ptr<Core::DrugTreatment> extractDrugTreatment(
            const std::unique_ptr<XpertRequestData>& _xpertRequest,
            const XpertQueryData& _xpertQuery,
            std::optional<std::string>& _errorMessage) const;
};

} // namespace XpertQuery
} // namespace Tucuxi

#endif // XPERTQUERYTOCOREEXTRACTOR_H
