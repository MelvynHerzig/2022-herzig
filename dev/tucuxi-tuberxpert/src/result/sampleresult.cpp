#include "sampleresult.h"

#include <algorithm>

#include "../language/languagemanager.h"

using namespace std;

namespace Tucuxi {
namespace XpertResult {

SampleResult::SampleResult(const Core::Sample* _sample, unsigned _percentile, const std::string& _warning)
    : AbstractResult<Core::Sample>(_sample, _warning), m_percentile(_percentile)
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

bool SampleResult::computeWarning(unsigned _percentile)
{
    if (_percentile <= 10 || _percentile >= 90) {
        XpertLanguage::LanguageManager& lm = XpertLanguage::LanguageManager::getInstance();
        string baseWarning = _percentile <= 50 ? lm.translate("population_above") : lm.translate("population_below");
        unsigned value = _percentile <= 50 ? max(0, 100 - int(_percentile)) : min (100, int(_percentile));

        //return velue + " "
    }
}

} // namespace XpertResult
} // namespace Tucuxi
