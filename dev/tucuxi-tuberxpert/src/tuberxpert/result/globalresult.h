#ifndef GLOBALRESULT_H
#define GLOBALRESULT_H

#include <memory>
#include <map>

#include "tucucommon/datetime.h"
#include "tucucore/drugtreatment/drugtreatment.h"
#include "tuberxpert/query/xpertquerydata.h"
#include "tuberxpert/result/xpertrequestresult.h"

namespace Tucuxi {
namespace Xpert {

/// \brief This is a wrapper class that contains the analysis for TuberXpert request.
///
///        The class is constructed with an XpertQueryDataObject. It acquires the administrative data.
///        Then for each XpertRequest found, it creates an XpertRequestResult that is stored in an
///        internal vector.
///
///         Its main purpose is to store the common elements during the lifetime of the XpertRequestResult.
/// \date 20/05/2022
/// \author Herzig Melvyn
class GlobalResult
{
public:

    /// \brief Constructor. Moves the AdministrativeData unique pointer
    ///        from _xpertQuery and moves each XpertRequest unique pointer
    ///        into XpertRequestResult.
    GlobalResult(std::unique_ptr<XpertQueryData> _xpertQuery);

    /// \brief Copy constructor is not supported.
    ///        The copy constructor is not supported because of the use of
    ///        unique_ptr wich can't be copied.
    GlobalResult(const GlobalResult& _other) = delete;


    /// \brief Gets the computation time, in other words the date on top of the query.
    /// \return Returns the generation date.
    Common::DateTime getComputationTime() const;

    /// \brief Gets a reference on the administrative data.
    /// \return A reference on the administrative data.
    const std::unique_ptr<AdminData>& getAdministrative() const;

    /// \brief Gets a reference on the vector of XpertRequestResult objects.
    ///        This is non const because the objects must be editable by the
    ///        TuberXpert flow steps.
    /// \return A reference one the vector of XpertRequestResult objects.
    std::vector<XpertRequestResult>& getXpertRequestResults();

protected:

    /// \brief Time of the computation. This field saves the value in first date field of the query.
    ///        It is usefull to obtain the same result (as long as the field is not modified) when
    ///        the requests are executed multiple times but not the day/year/month.
    Common::DateTime m_computationTime;

    /// \brief Administrative information moved from query.
    std::unique_ptr<AdminData> m_pAdministrative;

    /// \brief Vector of XpertRequestResults edited along the TuberXpert "pipeline".
    std::vector<XpertRequestResult> m_xpertRequestResults;
};

} // namespace Xpert
} // namespace Tucuxi

#endif // GLOBALRESULT_H
