#include "dosevalidationresult.h"

using namespace std;

namespace Tucuxi {
namespace Xpert {


DoseValidationResult::DoseValidationResult(const Core::SingleDose* _dose, const std::string& _warning) : AbstractValidationResult<Core::SingleDose>(_dose, _warning)
{}

} // namespace Xpert
} // namespace Tucuxi
