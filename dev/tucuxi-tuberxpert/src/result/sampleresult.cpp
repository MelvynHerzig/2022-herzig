#include "sampleresult.h"

#include "../language/languagemanager.h"

using namespace std;

namespace Tucuxi {
namespace XpertResult {

SampleResult::SampleResult(const Core::Sample* _sample, unsigned _groupNumberOver99Percentile)
    : AbstractResult<Core::Sample>(_sample, computeWarning(_groupNumberOver99Percentile)), m_groupNumberOver99Percentile(_groupNumberOver99Percentile)
{}

WarningLevel SampleResult::getWarningLevel() const
{
    if (m_groupNumberOver99Percentile <= 5 || m_groupNumberOver99Percentile > 95) {
        return WarningLevel::CRITICAL;
    }

    return WarningLevel::NORMAL;
}

unsigned SampleResult::getGroupNumberOver99Percentile() const
{
    return m_groupNumberOver99Percentile;
}

string SampleResult::computeWarning(unsigned _groupNumberOver99Percentile)
{
    // if the percentile is in the warning limits.
    if (_groupNumberOver99Percentile <= 10 || _groupNumberOver99Percentile > 90) {
        XpertLanguage::LanguageManager& lm = XpertLanguage::LanguageManager::getInstance();

        // Get base of message and % of population that is below or above the patient.
        string baseWarning = _groupNumberOver99Percentile <= 50 ? lm.translate("population_above") : lm.translate("population_below");
        unsigned value = _groupNumberOver99Percentile <= 50 ? 100 - _groupNumberOver99Percentile : _groupNumberOver99Percentile - 1;

        return to_string(value) + baseWarning;
    }

    return "";
}

} // namespace XpertResult
} // namespace Tucuxi
