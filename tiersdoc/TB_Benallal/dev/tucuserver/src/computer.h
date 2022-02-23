#ifndef TUCUXI_SERVER_COMPUTER_H
#define TUCUXI_SERVER_COMPUTER_H

#include "communication/query.h"
#include "communication/apiresponse.h"
#include "configuration.h"

#include "tucucore/drugtreatment/patientcovariate.h"
#include "tucucore/drugtreatment/target.h"
#include "tucucore/drugtreatment/sample.h"

namespace Tucuxi {
namespace Server {

class Computer
{
public:
    virtual ~Computer() {}

    virtual void compute() = 0;
    virtual ApiResponse& getResult();

protected:
    Computer(const Query& _query);

    Core::PatientVariates extractPatientVariates() const;
    // TODO inefficacyAlarm and toxicityAlarm are not supported yet
    Core::Targets extractTargets(size_t _drugPosition) const;
    Core::Samples extractSamples(size_t _drugPosition) const;

protected:
    const static Tucuxi::Core::CycleSize m_sCycleSize;

    const Query& m_query;
    std::unique_ptr<ApiResponse> m_result;
};

} // namespace Server
} // namespace Tucuxi

#endif // TUCUXI_SERVER_COMPUTER_H
