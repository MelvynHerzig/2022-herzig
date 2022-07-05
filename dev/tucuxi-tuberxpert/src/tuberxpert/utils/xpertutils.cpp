#include "xpertutils.h"

#include <cctype>
#include <sstream>
#include <iomanip>

#include "tuberxpert/language/languageexception.h"
#include "tuberxpert/result/globalresult.h"

using namespace std;

namespace Tucuxi {
namespace Xpert {

string varToString(const Core::DataType& _dataType)
{
    switch (_dataType) {
    case Core::DataType::Int: {
        return "int";
    } break;

    case Core::DataType::Double: {
        return "double";
    } break;

    case Core::DataType::Bool: {
        return "bool";
    } break;

    case Core::DataType::Date: {
       return "date";
    } break;
    }
}

string varToString(const double& _value)
{
    stringstream stream;
    stream << fixed << setprecision(2) << _value;
    return stream.str();
}

string varToString(CovariateType _value)
{
    switch (_value) {
    case CovariateType::Model : return "default";
    case CovariateType::Patient  : return "patient";
    default : throw invalid_argument("Unknown covariate type"); // If well maintained, should never be returned.
    }
}

string varToString(OutputLang _lang)
{
    switch (_lang) {
    case OutputLang::ENGLISH : return "en";
    case OutputLang::FRENCH  : return "fr";
    default : throw LanguageException("Unknown language"); // If well maintained, should never be returned.
    }
}

string varToString(WarningLevel _value)
{
    switch (_value) {
    case WarningLevel::NORMAL : return "normal";
    case WarningLevel::CRITICAL  : return "critical";
    default : throw invalid_argument("Unknown warning level"); // If well maintained, should never be returned.
    }
}

string getStringWithEnglishFallback(const Common::TranslatableString& _ts, OutputLang _lang)
{
    string target = _ts.getString(varToString(_lang));

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
    Common::DateTime dtComputation = _xpertRequestResult.getGlobalResult().getComputationTime();
    ss << _xpertRequestResult.getGlobalResult().getOutputPath() << "\\" <<
          _xpertRequestResult.getXpertRequest().getDrugID() << "_" <<
          _xpertRequestResult.getGlobalResult().getRequestIndexBeingHandled() + 1 << "_" <<
          dtComputation.day() << "-" << dtComputation.month() << "-" << dtComputation.year() << "_" <<
          dtComputation.hour() << "h" << dtComputation.minute() << "m" << dtComputation.second() << "s" <<
          "." << extension;

    return ss.str();
}

string keyToPhrase(const string& _key)
{

    stringstream ss;

    // Traverse the string
    for(size_t i=0; i < _key.length(); i++)
    {
        // If the first char is lowercase transform in uppercase.
        if (i == 0 && islower(_key[i])) {
            ss << char(toupper(_key[i]));
        }

        // Convert to lowercase if its
        // an uppercase character but not the first
        else if (i != 0 && isupper(_key[i]))
        {
            ss << ' ' << char(tolower(_key[i]));
        }

        // if lowercase character,
        // then just print
        else {
            ss << _key[i];
        }
    }

    return ss.str();
}

double getAgeIn(Core::CovariateType _ageType, const Common::DateTime& _birthDate, const Common::DateTime& _computationTime)
{
    switch (_ageType) {
    case Core::CovariateType::AgeInDays:
        return static_cast<double>(Common::Utils::dateDiffInDays(_birthDate, _computationTime));
        break;
    case Core::CovariateType::AgeInWeeks:
        return static_cast<double>(Common::Utils::dateDiffInWeeks(_birthDate,  _computationTime));
        break;
    case Core::CovariateType::AgeInMonths:
        return static_cast<double>(Common::Utils::dateDiffInMonths(_birthDate,  _computationTime));
        break;
    case Core::CovariateType::AgeInYears:
        return static_cast<double>(Common::Utils::dateDiffInYears(_birthDate,  _computationTime));
        break;
    default:
        throw invalid_argument("Invalid covariate type");
    }
}

} // namespace XpertUtils
} // namespace Tucuxi
