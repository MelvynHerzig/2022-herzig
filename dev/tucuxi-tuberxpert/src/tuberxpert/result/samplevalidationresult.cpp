#include "samplevalidationresult.h"

#include "tuberxpert/language/languagemanager.h"

using namespace std;

namespace Tucuxi {
namespace Xpert {

SampleValidationResult::SampleValidationResult(const Core::Sample* _sample,
                                               unsigned _groupNumberOver99Percentile) :
    AbstractValidationResult<Core::Sample>(_sample, computeWarning(_groupNumberOver99Percentile)),
    m_groupNumberOver99Percentile(_groupNumberOver99Percentile)
{}

WarningLevel SampleValidationResult::getWarningLevel() const
{
    // If below or equal to the 5th percentile or above the 95th percentile.
    if (m_groupNumberOver99Percentile <= 5 || m_groupNumberOver99Percentile > 95) {
        return WarningLevel::CRITICAL;
    }

    return WarningLevel::NORMAL;
}

unsigned SampleValidationResult::getGroupNumberOver99Percentile() const
{
    return m_groupNumberOver99Percentile;
}

string SampleValidationResult::computeWarning(unsigned _groupNumberOver99Percentile)
{
    // If the percentile is within the warning limits.
    if (_groupNumberOver99Percentile <= 10 || _groupNumberOver99Percentile > 90) {

        LanguageManager& langMgr = LanguageManager::getInstance();

        // Get the base of the message and the perentage of the population that is below or above the patient.
        string baseWarning = _groupNumberOver99Percentile <= 50 ?
                    langMgr.translate("population_above") :
                    langMgr.translate("population_below");

        unsigned populationPercentage = _groupNumberOver99Percentile <= 50 ?
                    100 - _groupNumberOver99Percentile :
                    _groupNumberOver99Percentile - 1;

        return to_string(populationPercentage) + baseWarning;
    }

    return "";
}

} // namespace Xpert
} // namespace Tucuxi
