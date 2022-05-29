#ifndef DOSERESULT_H
#define DOSERESULT_H

#include "tucucore/dosage.h"

namespace Tucuxi {
namespace XpertResult {

class DoseResult
{
public:
    DoseResult(const Core::SingleDose* _dose, const std::string& _warning);

    const Core::SingleDose* getDose() const;

    const std::string& getWarning() const;

protected:

    const Core::SingleDose* m_dose;

    std::string m_warning;
};

} // namespace XpertResult
} // namespace Tucuxi

#endif // DOSERESULT_H
