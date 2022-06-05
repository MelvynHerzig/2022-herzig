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
    SampleResult(const Core::Sample* _sample, unsigned _posOver100Percentile);

    WarningLevel getWarningLevel() const;

    unsigned getPosOver100Percentile() const;

    static std::string computeWarning(unsigned _posOver100Percentile);

protected:

    unsigned m_posOver100Percentile;


};

} // namespace XpertResult
} // namespace Tucuxi

#endif // SAMPLERESULT_H
