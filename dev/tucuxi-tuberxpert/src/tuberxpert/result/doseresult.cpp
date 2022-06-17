#include "doseresult.h"

using namespace std;

namespace Tucuxi {
namespace XpertResult {


DoseResult::DoseResult(const Core::SingleDose* _dose, const std::string& _warning) : AbstractResult<Core::SingleDose>(_dose, _warning)
{}

} // namespace XpertResult
} // namespace Tucuxi
