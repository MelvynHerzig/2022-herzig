#include "doseresult.h"

using namespace std;

namespace Tucuxi {
namespace XpertResult {


DoseResult::DoseResult(const Core::SingleDose* _dose, const std::string& _warning) :
    m_dose(_dose), m_warning(_warning)
{}

const Core::SingleDose *DoseResult::getDose() const
{
    return m_dose;
}

const std::string& DoseResult::getWarning() const
{
    return m_warning;
}

} // namespace XpertResult
} // namespace Tucuxi
