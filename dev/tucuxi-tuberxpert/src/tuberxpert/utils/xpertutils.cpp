#include "xpertutils.h"

#include <sstream>
#include <iomanip>

#include "tuberxpert/language/languageexception.h"

using namespace std;

namespace Tucuxi {
namespace Xpert {

string varToString(const double& _value)
{
    stringstream stream;
    stream << std::fixed << setprecision(2) << _value;
    return stream.str();
}

string outputLangToString(OutputLang _lang)
{
    switch (_lang) {
    case OutputLang::ENGLISH : return "en";
    case OutputLang::FRENCH  : return "fr";
    default : throw LanguageException("Unknown language"); // If well maintained, should never be returned.
    }
}

string getStringWithEnglishFallback(const Common::TranslatableString& _ts, OutputLang _lang)
{
    string target = _ts.getString(outputLangToString(_lang));

    if (target != ""){
        return target;
    } else {
        return _ts.getString();
    }
}

Common::DateTime getOldestDosageTimeRangeStart(const Core::DosageHistory &_dosageHistory, const Common::DateTime& _referenceTime)
{
    // In case the dosage history is empty. The olest treatment start date is reference time.
    Common::DateTime oldestDateKnown = _referenceTime;

    // Iterate on the time ranges and find the one that is the oldest.
    for (const unique_ptr<Core::DosageTimeRange>& timeRange : _dosageHistory.getDosageTimeRanges()) {
        if (timeRange->getStartDate() < oldestDateKnown){
            oldestDateKnown = timeRange->getStartDate();
        }
    }

    return oldestDateKnown;
}

Common::DateTime getLatestDosageTimeRangeStart(const Core::DosageHistory &_dosageHistory, const Common::DateTime& _referenceTime)
{
    // In case the dosage history is empty. The latest start time is undefined.
    Common::DateTime latestDateKnown = Common::DateTime::undefinedDateTime();

    // Iterate on the time ranges and find the one that is the latest.
    for (const unique_ptr<Core::DosageTimeRange>& timeRange : _dosageHistory.getDosageTimeRanges()) {
        if (timeRange->getStartDate() < _referenceTime && (latestDateKnown.isUndefined() || timeRange->getStartDate() > latestDateKnown)){
            latestDateKnown = timeRange->getStartDate();
        }
    }

    return latestDateKnown;
}

} // namespace XpertUtils
} // namespace Tucuxi
