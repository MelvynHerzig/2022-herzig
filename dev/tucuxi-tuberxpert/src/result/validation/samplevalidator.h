#ifndef SAMPLEVALIDATOR_H
#define SAMPLEVALIDATOR_H

#include "tucucommon/datetime.h"
#include "tucucore/drugtreatment/sample.h"

#include "../xpertrequestresult.h"

namespace Tucuxi {
namespace XpertResult {

class SampleValidator
{
public:
    SampleValidator(Common::DateTime _computationDate = Common::DateTime::now());;

    void getSampleValidations(XpertRequestResult& _xpertRequestResult) const;

protected:

    Common::DateTime getOldestSampleDateTime(const Core::Samples& _samples) const;

    Common::DateTime m_computationDate;
};

} // namespace XpertResult
} // namespace Tucuxi

#endif // SAMPLEVALIDATOR_H
