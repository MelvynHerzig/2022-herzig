#include "xpertquerydata.h"

using namespace std;

namespace Tucuxi {
namespace XpertQuery {

XpertQueryData::XpertQueryData(
        const string& _queryID,
        const string& _clientID,
        const Common::DateTime& _pQueryDate,
        const string& _language,
        unique_ptr<Query::AdministrativeData> _pAdministrative,
        unique_ptr<Query::DrugTreatmentData> _pParameters,
        vector<unique_ptr<Query::RequestData>>& _requests,
        vector<unique_ptr<XpertRequestData>>& _xpertRequests) :
        Query::QueryData(_queryID, _clientID, _pQueryDate, _language, move(_pParameters), _requests),
        m_pAdministrative(move(_pAdministrative)), m_xpertRequests(move(_xpertRequests))
{}

optional<reference_wrapper<const Query::AdministrativeData>> XpertQueryData::getpAdministrative() const
{
    // Admin is optional info in XML.
    if(m_pAdministrative != nullptr) {
        return optional<reference_wrapper<Query::AdministrativeData>>{*m_pAdministrative};
    }

    return nullopt;
}

const std::vector<std::unique_ptr<XpertRequestData> > &XpertQueryData::getXpertRequests() const
{
    return m_xpertRequests;
}


} // namespace XpertQuery
} // namespace Tucuxi
