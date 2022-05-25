#include "xpertquerydata.h"

using namespace std;

namespace Tucuxi {
namespace XpertQuery {

XpertQueryData::XpertQueryData(
        const string& _queryID,
        const string& _clientID,
        const Common::DateTime& _pQueryDate,
        const string& _language,
        unique_ptr<AdministrativeData> _pAdministrative,
        unique_ptr<Query::DrugTreatmentData> _pParameters,
        vector<unique_ptr<Query::RequestData>>& _requests,
        vector<unique_ptr<XpertRequestData>>& _xpertRequests) :
        Query::QueryData(_queryID, _clientID, _pQueryDate, _language, move(_pParameters), _requests),
        m_pAdministrative(move(_pAdministrative)), m_xpertRequests(move(_xpertRequests))
{}

std::optional<std::reference_wrapper<const AdministrativeData> > XpertQueryData::getpAdministrative() const
{
    if (m_pAdministrative != nullptr) {
        return optional<reference_wrapper<AdministrativeData>>{*m_pAdministrative};
    }

    return nullopt;
}

unique_ptr<AdministrativeData>&& XpertQueryData::movepAdministrative()
{
    return move(m_pAdministrative);
}

const std::vector<std::unique_ptr<XpertRequestData>>& XpertQueryData::getXpertRequests() const
{
    return m_xpertRequests;
}

std::unique_ptr<XpertRequestData>&& XpertQueryData::moveXpertRequest(size_t i)
{
    if (i < m_xpertRequests.size()) {
        return move(m_xpertRequests[i]);
    }

    throw out_of_range("Cannot move XpertRequestData. Out of range index.");
}


} // namespace XpertQuery
} // namespace Tucuxi
