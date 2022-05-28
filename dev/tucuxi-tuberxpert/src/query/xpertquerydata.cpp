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

const unique_ptr<AdministrativeData>& XpertQueryData::getpAdministrative() const
{
    return m_pAdministrative;
}

unique_ptr<AdministrativeData>&& XpertQueryData::movepAdministrative()
{
    return move(m_pAdministrative);
}

const std::vector<std::unique_ptr<XpertRequestData>>& XpertQueryData::getXpertRequests() const
{
    return m_xpertRequests;
}

unique_ptr<XpertRequestData>&& XpertQueryData::moveXpertRequest(size_t _i)
{
    if (_i < m_xpertRequests.size()) {
        return move(m_xpertRequests[_i]);
    }

    throw out_of_range("Cannot move XpertRequestData. Out of range index.");
}


} // namespace XpertQuery
} // namespace Tucuxi
