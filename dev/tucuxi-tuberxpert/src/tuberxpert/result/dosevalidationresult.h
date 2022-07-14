#ifndef DOSEVALIDATIONRESULT_H
#define DOSEVALIDATIONRESULT_H

#include "tucucore/dosage.h"

#include "tuberxpert/result/abstractvalidationresult.h"

namespace Tucuxi {
namespace Xpert {

/// \brief This class stores the validation result for a dose of a patient.
///        It contains a pointer to the single dose and possibly
///        a warning message.
///
///        The warning message indicates if the dose is too low or too high
///        compared to the doses recommended by the drug model.
/// \date 01/06/2022
/// \author Herzig Melvyn
class DoseValidationResult : public AbstractValidationResult<Core::SingleDose>
{
public:

    /// \brief Constructor.
    /// \param _dose Patient dose concerned by this validation result.
    /// \param _warning Associated warning message.
    DoseValidationResult(const Core::SingleDose* _dose, const std::string& _warning);

};

} // namespace Xpert
} // namespace Tucuxi

#endif // DOSEVALIDATIONRESULT_H
