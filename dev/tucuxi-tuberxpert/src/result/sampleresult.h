#ifndef SAMPLERESULT_H
#define SAMPLERESULT_H

#include <string>

#include "tucucore/drugtreatment/sample.h"

#include "abstractresult.h"

namespace Tucuxi {
namespace XpertResult {

// SampleEvaluationResult
class SampleResult : public AbstractResult<Core::Sample>
{
public:
    SampleResult(const Core::Sample* _sample, unsigned _percentile);

    WarningLevel getWarningLevel() const;

    unsigned getPercentile() const;

    static std::string computeWarning(unsigned _percentile);

protected:

    unsigned m_percentile;


};

} // namespace XpertResult
} // namespace Tucuxi

#endif // SAMPLERESULT_H
