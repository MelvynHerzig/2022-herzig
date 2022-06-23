#include "xpertutils.h"

#include <sstream>
#include <iomanip>

#include "tuberxpert/language/languageexception.h"
#include "tuberxpert/result/globalresult.h"

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

string computeFileName(const XpertRequestResult& _xpertRequestResult)
{
    string extension = "";

    switch(_xpertRequestResult.getXpertRequest().getOutputFormat()) {
    case OutputFormat::XML  : extension = "xml"; break;
    case OutputFormat::HTML : extension = "html"; break;
    case OutputFormat::PDF  : extension = "pdf"; break;
    }

    stringstream ss;
    Common::DateTime dtComputation = _xpertRequestResult.getGlobalResult()->getComputationTime();
    ss << _xpertRequestResult.getGlobalResult()->getOutputPath() << "\\" <<
          _xpertRequestResult.getXpertRequest().getDrugID() << "_" <<
          _xpertRequestResult.getGlobalResult()->getRequestIndexBeingHandled() + 1 << "_" <<
          dtComputation.day() << "-" << dtComputation.month() << "-" << dtComputation.year() << "_" <<
          dtComputation.hour() << "h" << dtComputation.minute() << "m" << dtComputation.second() << "s" <<
          "." << extension;

    return ss.str();
}

} // namespace XpertUtils
} // namespace Tucuxi
