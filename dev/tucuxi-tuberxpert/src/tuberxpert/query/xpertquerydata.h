#ifndef XPERTQUERYDATA_H
#define XPERTQUERYDATA_H

#include "tucuquery/querydata.h"
#include "tucuquery/parametersdata.h"

#include "tuberxpert/query/xpertrequestdata.h"
#include "tuberxpert/query/xpertadministrativedata.h"

namespace Tucuxi {
namespace XpertQuery {

/// \brief This class extends the core queryData to include AdministrativeData class and
/// the custom request "requestXpert" as XpertRequestData class.
/// \date 23/04/2022
/// \author Herzig Melvyn
class XpertQueryData : public Query::QueryData
{
public:

    // Constructors

    /// \brief Default constructor is not supported.
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
            std::unique_ptr<AdministrativeData> _pAdministrative,
            std::unique_ptr<Query::DrugTreatmentData> _pParameters,
            std::vector<std::unique_ptr<Query::RequestData>>& _requests,
            std::vector<std::unique_ptr<XpertRequestData>>& _xpertRequests);

    /// \brief Copy constructor is not supported.
    ///        The copy constructor is not supported because of the use of
    ///        unique_ptr wich can't be copied.
    XpertQueryData(const QueryData& _other) = delete;    

    /// \brief Gets administrative information.
    /// \return The administrative information.
    const std::unique_ptr<AdministrativeData>& getpAdministrative() const;

    /// \brief Moves the administrative data unique pointer ownership.
    /// \return Returns a right value on the administrative data unique pointer.
    std::unique_ptr<AdministrativeData>&& movepAdministrative();

    /// \brief Get the custom requests for tuberXpert
    /// \return A vector of custom requests.
    const std::vector<std::unique_ptr<XpertRequestData>>& getXpertRequests() const;

    /// \brief Moves an XpertRequest unique pointer ownership.
    /// \param _i Index of the XpertRequest to move. _i must be smaller than m_xpertRequests.size().
    /// \return Returns a right value on the XpertRequest unique pointer.
    /// \throw out_of_range If the index _i bigger or equal to m_xpertRequests.size().
    std::unique_ptr<XpertRequestData>&& moveXpertRequest(size_t _i);

protected:
    /// \brief Administrative information.
    std::unique_ptr<AdministrativeData> m_pAdministrative;

    /// \brief Custom requests for tuberXpert.
    std::vector<std::unique_ptr<XpertRequestData>> m_xpertRequests;
};

} // namespace XpertQuery
} // namespace Tucuxi

#endif // XPERTQUERYDATA_H
