#include "sampleresult.h"

#include "../language/languagemanager.h"

using namespace std;

namespace Tucuxi {
namespace XpertResult {

SampleResult::SampleResult(const Core::Sample* _sample, unsigned _posOver100PercentileGroups)
    : AbstractResult<Core::Sample>(_sample, computeWarning(_posOver100PercentileGroups)), m_posOver100Percentile(_posOver100PercentileGroups)
{}

WarningLevel SampleResult::getWarningLevel() const
{
    if (m_posOver100Percentile <= 5 || m_posOver100Percentile > 95) {
        return WarningLevel::CRITICAL;
    }

    return WarningLevel::NORMAL;
}

unsigned SampleResult::getPosOver100Percentile() const
{
    return m_posOver100Percentile;
}

string SampleResult::computeWarning(unsigned _posOver100PercentileGroups)
{
    // if the percentile is in the warning limits.
    if (_posOver100PercentileGroups <= 10 || _posOver100PercentileGroups > 90) {
        XpertLanguage::LanguageManager& lm = XpertLanguage::LanguageManager::getInstance();

        // Get base of message and % of population that is below or above the patient.
        string baseWarning = _posOver100PercentileGroups <= 50 ? lm.translate("population_above") : lm.translate("population_below");
        unsigned value = _posOver100PercentileGroups <= 50 ? 100 - _posOver100PercentileGroups : _posOver100PercentileGroups - 1;

        return to_string(value) + baseWarning;
    }

    return "";
}

} // namespace XpertResult
} // namespace Tucuxi
