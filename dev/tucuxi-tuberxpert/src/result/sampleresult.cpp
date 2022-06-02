#include "sampleresult.h"

namespace Tucuxi {
namespace XpertResult {

SampleResult::SampleResult(const Core::Sample* _sample, const std::string& _warning) : AbstractResult<Core::Sample>(_sample, _warning)
{

}

} // namespace XpertResult
} // namespace Tucuxi
