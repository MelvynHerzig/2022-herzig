#ifndef SAMPLEVALIDATIONRESULT_H
#define SAMPLEVALIDATIONRESULT_H

#include <string>

#include "tucucore/drugtreatment/sample.h"

#include "tuberxpert/result/abstractvalidationresult.h"

namespace Tucuxi {
namespace Xpert {

/// \brief This class stores results for a patient samples.
///
///        It contains a pointer to the concerned sample and the group number
///        in which the sample is evaluated.
///
///        The group is a number from 1 to 100 that corresponds to the 100
///        separations produced by de 99 percentiles.
///
///        If the group number is 1, the sample is located before the first percentile,
///        if the group number is 2, the sample is located before the second percentile but after the first
///        and so on...
///
///        The 100th group is the one that is after the 99th percentile.
///
///        The warning message tells if the sample is abnormally too low/high in terms
///        of percentile.
///
///        The percentiles described in the following comments are the percentile
///        retrieved by an "a priori" percentile computation.
/// \date 08/06/2022
/// \author Herzig Melvyn
class SampleValidationResult : public AbstractValidationResult<Core::Sample>
{

public:

    /// \brief Constructor of SampleResult.
    /// \param _sample Sample targeted by this result.
    /// \param _posOver100Percentile Group position over the
    SampleValidationResult(const Core::Sample* _sample, unsigned _groupNumberOver99Percentile);

    /// \brief Gets the warning level for the current sample. This
    ///        doesn't consider if the warning is set or not.
    /// \return Returns "critical" if the sample is <=5 or >95 else "normal".
    ///         This function does not mean that a warning is set.
    WarningLevel getWarningLevel() const override;

    /// \brief Gets the group number of the current sample in which it is located.
    ///        The group is between 1 and 100. These group are the group formed by
    ///        99 percentiles.
    /// \return Returns the group number.
    unsigned getGroupNumberOver99Percentile() const;

protected:

    /// \brief Computes a warning message out of a group position number.
    ///        This uses the LanguageManager and assumes that the correct language is
    ///        loaded.
    /// \param _groupNumberOver99Percentile Group position number of a sample.
    /// \return Returns the warning message for the group position number. May be emptry string.
    static std::string computeWarning(unsigned _groupNumberOver99Percentile);

    /// \brief Group position number formed by 99 percentiles for the linked sample.
    unsigned m_groupNumberOver99Percentile;
};

} // namespace Xpert
} // namespace Tucuxi

#endif // SAMPLEVALIDATIONRESULT_H
