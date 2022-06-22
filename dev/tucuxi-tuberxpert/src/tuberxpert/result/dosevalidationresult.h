#ifndef DOSEVALIDATIONRESULT_H
#define DOSEVALIDATIONRESULT_H

#include "tucucore/dosage.h"

#include "tuberxpert/result/abstractvalidationresult.h"

namespace Tucuxi {
namespace Xpert {

/// \brief This class stores results for a patient doses.
///
///        It contains a pointer to the concerned dose and eventually
///        a warning message.
///
///        The warning message tells if the dose is too low or too high
///        in regards of the recommended doses from the drug model.
/// \date 01/06/2022
/// \author Herzig Melvyn
class DoseValidationResult : public AbstractValidationResult<Core::SingleDose>
{
public:

    /// \brief Constructor.
    /// \param _dose Concerned patient dose by this result.
    /// \param _warning Potential warning in regards of the dose.
    DoseValidationResult(const Core::SingleDose* _dose, const std::string& _warning);

};

} // namespace Xpert
} // namespace Tucuxi

#endif // DOSEVALIDATIONRESULT_H
