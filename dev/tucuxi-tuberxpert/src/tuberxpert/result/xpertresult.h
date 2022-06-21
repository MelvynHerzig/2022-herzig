#ifndef XPERTRESULT_H
#define XPERTRESULT_H

#include <memory>
#include <map>

#include "tucucommon/datetime.h"
#include "tucucore/drugtreatment/drugtreatment.h"
#include "tuberxpert/query/xpertquerydata.h"
#include "tuberxpert/result/xpertrequestresult.h"

namespace Tucuxi {
namespace XpertResult {

/// \brief This is a wrapper class that contains the analysis for TuberXpert request.
///
///        The class is constructed with an XpertQueryDataObject. It acquires the administrative data.
///        Then for each XpertRequest found, it creates an XpertRequestResult that is stored in an
///        internal vector.
/// \date 20/05/2022
/// \author Herzig Melvyn
class XpertResult
{
public:

    /// \brief Constructor. Moves the AdministrativeData unique pointer
    ///        from _xpertQuery and moves each XpertRequest unique pointer
    ///        into XpertRequestResult.
    XpertResult(std::unique_ptr<XpertQuery::XpertQueryData> _xpertQuery);

    /// \brief Copy constructor is not supported.
    ///        The copy constructor is not supported because of the use of
    ///        unique_ptr wich can't be copied.
    XpertResult(const XpertResult& _other) = delete;


    /// \brief Gets the generation date of the result. In other words, the creation date
    ///        of this object.
    /// \return Returns the generation date.
    Common::DateTime getGenerationDate();

    /// \brief Gets a reference on the administrative data.
    /// \return A reference on the administrative data.
    const std::unique_ptr<XpertQuery::AdministrativeData>& getAdministrative() const;

    /// \brief Gets a reference on the vector of XpertRequestResult objects.
    ///        This is non const because the objects must be editable by the
    ///        TuberXpert "pipeline".
    /// \return A reference one the vector of XpertRequestResult objects.
    std::vector<XpertRequestResult>& getXpertRequestResults();

protected:

    /// \brief Date of generation.
    Common::DateTime m_generationDate;

    /// \brief Administrative information moved from query.
    std::unique_ptr<XpertQuery::AdministrativeData> m_pAdministrative;

    /// \brief Vector of XpertRequestResults edited along the TuberXpert "pipeline".
    std::vector<XpertRequestResult> m_xpertRequestResults;
};

} // namespace XpertResult
} // namespace Tucuxi

#endif // XPERTRESULT_H
