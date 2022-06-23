#include "globalresult.h"

#include <optional>

#include "tuberxpert/query/xpertquerytocoreextractor.h"

using namespace std;

namespace Tucuxi {
namespace Xpert {

GlobalResult::GlobalResult(unique_ptr<XpertQueryData> _xpertQuery, const string& _outputPath) :
    m_computationTime(_xpertQuery->getpQueryDate()),
    m_adminData(_xpertQuery->moveAdmin()),
    m_outputPath(_outputPath),
    m_requestIndexBeingHandled(-1)

{
    XpertQueryToCoreExtractor extractor;

    // For each requestXpert, extract the treatment.
    for (size_t i = 0; i < _xpertQuery->getXpertRequests().size(); ++i) {
        string errorMessage;
        unique_ptr<Core::DrugTreatment> drugTreatment = extractor.extractDrugTreatment(
                    _xpertQuery->getXpertRequests()[i],
                    *_xpertQuery,
                    errorMessage);

        m_xpertRequestResults.emplace_back(this, _xpertQuery->moveXpertRequest(i), move(drugTreatment), errorMessage);
    }
}

Common::DateTime GlobalResult::getComputationTime() const
{
    return m_computationTime;
}

const unique_ptr<AdminData>& GlobalResult::getAdminData() const
{
    return m_adminData;
}

std::vector<XpertRequestResult>& GlobalResult::getXpertRequestResults()
{
    return m_xpertRequestResults;
}

string GlobalResult::getOutputPath() const
{
    return m_outputPath;
}

int GlobalResult::getRequestIndexBeingHandled() const
{
    return m_requestIndexBeingHandled;
}

int GlobalResult::incrementRequestIndexBeingHandled()
{
    return ++m_requestIndexBeingHandled;
}

} // namespace Xpert
} // namespace Tucuxi
