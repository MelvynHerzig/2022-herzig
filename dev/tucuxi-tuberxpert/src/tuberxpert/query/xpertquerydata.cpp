#include "xpertquerydata.h"

using namespace std;

namespace Tucuxi {
namespace Xpert {

XpertQueryData::XpertQueryData(
        const string& _queryId,
        const string& _clientId,
        const Common::DateTime& _queryDate,
        const string& _language,
        unique_ptr<AdminData> _admin,
        unique_ptr<Query::DrugTreatmentData> _parameters,
        vector<unique_ptr<Query::RequestData>>& _requests,
        vector<unique_ptr<XpertRequestData>>& _xpertRequests) :
    Query::QueryData(_queryId, _clientId, _queryDate, _language, move(_parameters), _requests),
    m_admin(move(_admin)),
    m_xpertRequests(move(_xpertRequests))
{}

const unique_ptr<AdminData>& XpertQueryData::getAdminData() const
{
    return m_admin;
}

unique_ptr<AdminData>&& XpertQueryData::moveAdminData()
{
    return move(m_admin);
}

const vector<unique_ptr<XpertRequestData>>& XpertQueryData::getXpertRequests() const
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


} // namespace Xpert
} // namespace Tucuxi
