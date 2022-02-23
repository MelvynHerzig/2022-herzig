#include "predictioncomputer.h"

namespace Tucuxi {
namespace Server {

PredictionComputer::PredictionComputer(const Query& _query)
    : Computer(_query)
{}

void PredictionComputer::compute()
{
    std::string requestType = m_query.getRequests()
                                    .at(0)
                                    ->getRequestType();

    std::string errorContent = "Request type is: " + requestType + ". (PredictionComputer)";
    m_result->addError("DefaultError", m_query.getLanguage(), errorContent);
}

} // namespace Server
} // namespace Tucuxi
