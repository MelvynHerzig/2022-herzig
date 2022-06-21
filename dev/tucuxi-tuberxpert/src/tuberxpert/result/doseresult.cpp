#include "doseresult.h"

using namespace std;

namespace Tucuxi {
namespace Xpert {


DoseResult::DoseResult(const Core::SingleDose* _dose, const std::string& _warning) : AbstractResult<Core::SingleDose>(_dose, _warning)
{}

} // namespace Xpert
} // namespace Tucuxi
