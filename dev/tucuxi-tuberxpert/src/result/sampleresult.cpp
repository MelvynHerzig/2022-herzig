#include "sampleresult.h"

#include <algorithm>

#include "../language/languagemanager.h"

using namespace std;

namespace Tucuxi {
namespace XpertResult {

SampleResult::SampleResult(const Core::Sample* _sample, unsigned _percentile)
    : AbstractResult<Core::Sample>(_sample, computeWarning(_percentile)), m_percentile(_percentile)
{}

WarningLevel SampleResult::getWarningLevel() const
{
    if (m_percentile <= 5 || m_percentile >= 95) {
        return WarningLevel::CRITICAL;
    }

    return WarningLevel::NORMAL;
}

unsigned SampleResult::getPercentile() const
{
    return m_percentile;
}

string SampleResult::computeWarning(unsigned _percentile)
{
    // if the percentile is in the warning limits.
    if (_percentile <= 10 || _percentile >= 90) {
        XpertLanguage::LanguageManager& lm = XpertLanguage::LanguageManager::getInstance();

        // Get base of message and % of population that is below or above the patient.
        string baseWarning = _percentile <= 50 ? lm.translate("population_above") : lm.translate("population_below");
        unsigned value = _percentile <= 50 ? 100 - _percentile : min(100, int(_percentile));

        return to_string(value) + "% " + baseWarning;
    }

    return "";
}

} // namespace XpertResult
} // namespace Tucuxi
