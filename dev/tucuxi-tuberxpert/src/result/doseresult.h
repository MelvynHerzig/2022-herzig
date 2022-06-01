#ifndef DOSERESULT_H
#define DOSERESULT_H

#include "tucucore/dosage.h"

namespace Tucuxi {
namespace XpertResult {

/// \brief This class stores results for a patient doses.
///
///        It contains a pointer to the concerned dose and eventually
///        a warning message.
///
///        The warning message tells if the dose is too low or too high
///        in regards of the recommanded doses from the drug model.
/// \date 01/06/2022
/// \author Herzig Melvyn
class DoseResult
{
public:

    /// \brief Constructor.
    /// \param _dose Concerned patient dose by this result.
    /// \param _warning Potential warning in regards of the dose.
    DoseResult(const Core::SingleDose* _dose, const std::string& _warning);

    /// \brief Gets the related patient dose.
    /// \return The pointer to the patient dose.
    const Core::SingleDose* getDose() const;

    /// \brief Gets the warning message.
    /// \return The warning message. May be empty string if none.
    const std::string& getWarning() const;

protected:

    /// \brief Patient's dose whose result is made for.
    const Core::SingleDose* m_dose;

    /// \brief Warning in regards of the patient's dosage.
    std::string m_warning;
};

} // namespace XpertResult
} // namespace Tucuxi

#endif // DOSERESULT_H
