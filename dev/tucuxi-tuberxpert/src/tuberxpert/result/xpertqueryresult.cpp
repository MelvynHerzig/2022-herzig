#include "xpertqueryresult.h"

#include <optional>

#include "tuberxpert/query/xpertquerytocoreextractor.h"

using namespace std;

namespace Tucuxi {
namespace Xpert {

XpertQueryResult::XpertQueryResult(unique_ptr<XpertQueryData> _xpertQuery, const string& _outputPath) :
    m_computationTime(_xpertQuery->getpQueryDate()),
    m_adminData(_xpertQuery->moveAdminData()),
    m_outputPath(_outputPath),
    m_requestIndexBeingProcessed(-1)

{
    XpertQueryToCoreExtractor extractor;

    // For each xpertRequest, extract its treatment.
    for (size_t i = 0; i < _xpertQuery->getXpertRequests().size(); ++i) {
        string errorMessage;
        unique_ptr<Core::DrugTreatment> drugTreatment = extractor.extractDrugTreatment(
                    _xpertQuery->getXpertRequests()[i],
                    *_xpertQuery,
                    errorMessage);

        m_xpertRequestResults.emplace_back(*this,
                                           _xpertQuery->moveXpertRequest(i),
                                           move(drugTreatment),
                                           errorMessage);
    }
}

Common::DateTime XpertQueryResult::getComputationTime() const
{
    return m_computationTime;
}

const unique_ptr<AdminData>& XpertQueryResult::getAdminData() const
{
    return m_adminData;
}

vector<XpertRequestResult>& XpertQueryResult::getXpertRequestResults()
{
    return m_xpertRequestResults;
}

string XpertQueryResult::getOutputPath() const
{
    return m_outputPath;
}

int XpertQueryResult::getRequestIndexBeingProcessed() const
{
    return m_requestIndexBeingProcessed;
}

int XpertQueryResult::incrementRequestIndexBeingProcessed()
{
    return ++m_requestIndexBeingProcessed;
}

} // namespace Xpert
} // namespace Tucuxi
