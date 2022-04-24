#ifndef XPERTQUERYDATA_H
#define XPERTQUERYDATA_H

#include "tucuquery/querydata.h"
#include "tucuquery/parametersdata.h"

#include "xpertrequestdata.h"

namespace Tucuxi {
namespace XpertQuery {

/// \brief This class extends the core queryData to include administrative information and
/// the custom request "requestXpert".
/// \date 23/04/2022
/// \author Herzig Melvyn
class XpertQueryData : public Query::QueryData
{
public:
    // Constructors

    ///
    /// \brief Default constructor is not supported.
    ///
    XpertQueryData() = delete;

    /// \brief Constructor of a query.
    /// \param _queryID ID of the query.
    /// \param _clientID ID of the client.
    /// \param _pQueryDate A pointer to the date at which the query is sent.
    /// \param _language The language supported by the client.
    /// \param _pAdministrative A pointer to the administrative data.
    /// \param _pParameters A pointer to the medical data used for computation.
    /// \param _requests The requests that the server must process.
    /// \param _xpertRequests The requests for tuberXpert.
    XpertQueryData(
            const std::string& _queryID,
            const std::string& _clientID,
            const Common::DateTime& _pQueryDate,
            const std::string& _language,
            std::unique_ptr<Query::AdministrativeData> _pAdministrative,
            std::unique_ptr<Query::DrugTreatmentData> _pParameters,
            std::vector<std::unique_ptr<Query::RequestData>>& _requests,
            std::vector<std::unique_ptr<XpertRequestData>>& _xpertRequests);

    /// \brief Copy constructor is not supported.
    ///  The copy constructor is not supported because of the use of
    ///  unique_ptr wich can't be copied.
    XpertQueryData(const QueryData& _other) = delete;

//    /// \brief Gets administrative information.
//    /// \return The administrative information.
//    const Query::AdministrativeData& getpAdministrative() const;

//    /// \brief Get the custom requests for tuberXpert
//    /// \return A vector of custom requests.
//    const std::vector<std::unique_ptr<XpertRequestData>>& getXpertRequests() const;

protected:
    /// Administrative information.
    std::unique_ptr<Tucuxi::Query::AdministrativeData> m_pAdministrative;

    /// Custom requests for tuberXpert.
    std::vector<std::unique_ptr<XpertRequestData>> m_xpertRequests;
};

} // namespace XpertQuery
} // namespace Tucuxi

#endif // XPERTQUERYDATA_H
