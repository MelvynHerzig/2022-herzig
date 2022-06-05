#ifndef SAMPLEVALIDATOR_H
#define SAMPLEVALIDATOR_H

#include "tucucommon/datetime.h"
#include "tucucore/drugtreatment/sample.h"
#include "tucucore/computingservice/computingresponse.h"

#include "../xpertrequestresult.h"

namespace Tucuxi {
namespace XpertResult {

class SampleValidator
{
public:
    SampleValidator(Common::DateTime _computationDate = Common::DateTime::now());

    void getSampleValidations(XpertRequestResult& _xpertRequestResult) const;

    // from 1 to 100
    // throw invalid_argument if conversion failed
    // only works with time[0] and concentrations[0]
    unsigned findPosOver100Percentile(const Core::PercentilesData* _percentilesData, const std::unique_ptr<Core::Sample>& _sample) const;

protected:


    Common::DateTime m_computationDate;
};

} // namespace XpertResult
} // namespace Tucuxi

#endif // SAMPLEVALIDATOR_H
