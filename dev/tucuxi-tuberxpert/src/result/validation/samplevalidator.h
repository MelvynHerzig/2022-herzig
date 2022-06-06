#ifndef SAMPLEVALIDATOR_H
#define SAMPLEVALIDATOR_H

#include "tucucommon/datetime.h"
#include "tucucore/drugtreatment/sample.h"
#include "tucucore/computingservice/computingresponse.h"

#include "../xpertrequestresult.h"

struct TestSampleValidator;

namespace Tucuxi {
namespace XpertResult {

class SampleValidator
{
public:
    SampleValidator(Common::DateTime _computationDate = Common::DateTime::now());

    void getSampleValidations(XpertRequestResult& _xpertRequestResult) const;

    // from 1 to 100
    // throw invalid_argument if conversion failed or no valid cycle data
    // only works with time[0] and concentrations[0]
    unsigned findPosOver100PercentileGroups(const Core::PercentilesData* _percentilesData, const std::unique_ptr<Core::Sample>& _sample) const;

    friend TestSampleValidator;

protected:

    Common::DateTime m_computationDate;
};

} // namespace XpertResult
} // namespace Tucuxi

#endif // SAMPLEVALIDATOR_H
