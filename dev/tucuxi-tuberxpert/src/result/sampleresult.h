#ifndef SAMPLERESULT_H
#define SAMPLERESULT_H

#include <string>

#include "tucucore/drugtreatment/sample.h"

#include "abstractresult.h"

namespace Tucuxi {
namespace XpertResult {

class SampleResult : public AbstractResult<Core::Sample>
{
public:
    SampleResult(const Core::Sample* _sample, const std::string& _warning);
};

} // namespace XpertResult
} // namespace Tucuxi

#endif // SAMPLERESULT_H
