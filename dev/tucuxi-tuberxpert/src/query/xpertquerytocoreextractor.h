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

/// \brief This class extends the tucuquery QueryToCoreExtractor.
///        The main point of this class is to provide a method
///        to extract a drugTreatment for a given XpertRequestData.
/// \date 23/04/2022
/// \author Herzig Melvyn
class XpertQueryToCoreExtractor : Query::QueryToCoreExtractor
{
public:

    /// \brief Extracts a drugTreatment for a given XpertRequestData in regards
    ///        of a XpertQueryData. This method just add a few verification steps
    ///        before calling the QueryToCoreExtractor::extractDrugTreatment method.
    /// \param _xpertRequest Request to extract related drug treatment.
    /// \param _xpertQuery Query where to look for information.
    /// \param _errorMessage Error message so store information if something goes wrong.
    ///                      Set to "" if everything is fine.
    /// \return Retrusna unique pointer to the generaterd DrugTreatment.
    ///         May be a unique pointer on nullptr if it goes wrong.
    std::unique_ptr<Core::DrugTreatment> extractDrugTreatment(
            const std::unique_ptr<XpertRequestData>& _xpertRequest,
            const XpertQueryData& _xpertQuery,
            std::string& _errorMessage) const;
};

} // namespace XpertQuery
} // namespace Tucuxi

#endif // XPERTQUERYTOCOREEXTRACTOR_H
