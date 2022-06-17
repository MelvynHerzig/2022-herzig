#ifndef DOSERESULT_H
#define DOSERESULT_H

#include "tucucore/dosage.h"

#include "tuberxpert/result/abstractresult.h"

namespace Tucuxi {
namespace XpertResult {

/// \brief This class stores results for a patient doses.
///
///        It contains a pointer to the concerned dose and eventually
///        a warning message.
///
///        The warning message tells if the dose is too low or too high
///        in regards of the recommended doses from the drug model.
/// \date 01/06/2022
/// \author Herzig Melvyn
class DoseResult : public AbstractResult<Core::SingleDose>
{
public:

    /// \brief Constructor.
    /// \param _dose Concerned patient dose by this result.
    /// \param _warning Potential warning in regards of the dose.
    DoseResult(const Core::SingleDose* _dose, const std::string& _warning);

};

} // namespace XpertResult
} // namespace Tucuxi

#endif // DOSERESULT_H
