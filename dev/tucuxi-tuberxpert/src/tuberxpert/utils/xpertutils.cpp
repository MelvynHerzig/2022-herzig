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
    string i = stream.str();
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

string dateTimeToXmlString(const Tucuxi::Common::DateTime& _dateTime)
{
    if (_dateTime.isUndefined()) {
        return "";
    }

    string result;
    result = to_string(_dateTime.year()) + "." + to_string(_dateTime.month()) + "."
             + to_string(_dateTime.day()) + "T" + to_string(_dateTime.hour()) + ":"
             + to_string(_dateTime.minute()) + ":" + to_string(_dateTime.second());

    char str[20];
    snprintf(
            str,
            20,
            "%04d-%02d-%02dT%02d:%02d:%02d",
            _dateTime.year(),
            _dateTime.month(),
            _dateTime.day(),
            _dateTime.hour(),
            _dateTime.minute(),
            _dateTime.second());
    result = str;
    return result;
}

string timeToString(const Common::TimeOfDay& _timeOfDay)
{
    stringstream timeStream;

    LanguageManager& lm = LanguageManager::getInstance();

    timeStream << _timeOfDay.hour() << lm.translate("hour_acronym");

    if (_timeOfDay.minute() != 0) {
        timeStream << _timeOfDay.minute();
    }

    return timeStream.str();
}

string dateTimeToString(const Common::DateTime& _dateTime, bool _withTime)
{
    if (_dateTime.isUndefined()) {
        return "";
    }

    stringstream dateTimeStream;


    // Setting the date format
    dateTimeStream << _dateTime.day() << '.' << _dateTime.month() << '.' << _dateTime.year();

    if (!_withTime) {
        return dateTimeStream.str();
    }

    dateTimeStream << ' ' << timeToString(Common::Duration(
                                              chrono::hours(_dateTime.hour()),
                                              chrono::minutes(_dateTime.minute()),
                                              chrono::seconds(_dateTime.second()))
                                          );

    return dateTimeStream.str();
}

string beautifyString(const std::string& _value, Core::DataType _type, const std::string& _id)
{
    LanguageManager& lm = LanguageManager::getInstance();

    // Convert the value into yes/no for nice display if the data type is bool.
    if (_type == Core::DataType::Bool) {
        if (stoi(_value)) {
            return lm.translate("yes");
        } else {
            return lm.translate("no");
        }
    } else if (_type == Core::DataType::Double && _id != "sex") {
        return varToString(stod(_value));
    }

    // Convert the value into male/female/undefined for nice display.
   if (_id == "sex") {
        double sexAsDouble = stod(_value);
        if (sexAsDouble > 0.6) {
            return lm.translate("man");
        } else if (sexAsDouble < 0.4){
            return lm.translate("woman");
        } else {
            return lm.translate("undefined");
        }
    }

   return _value;
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

string computeFileName(const XpertRequestResult& _xpertRequestResult, bool _addOutputPath, bool _addExtension)
{
    stringstream ss;
    Common::DateTime dtComputation = _xpertRequestResult.getGlobalResult().getComputationTime();

    // If the output path should be prefixed.
    if (_addOutputPath) {
         ss << _xpertRequestResult.getGlobalResult().getOutputPath() << "/";
    }

    ss << _xpertRequestResult.getXpertRequest().getDrugID() << "_" <<
          _xpertRequestResult.getGlobalResult().getRequestIndexBeingHandled() + 1 << "_" <<
          dtComputation.day() << "-" << dtComputation.month() << "-" << dtComputation.year() << "_" <<
          dtComputation.hour() << "h" << dtComputation.minute() << "m" << dtComputation.second() << "s";

    // If the extension should be suffixed
    if (_addExtension) {

        string extension = "";

        switch(_xpertRequestResult.getXpertRequest().getOutputFormat()) {
        case OutputFormat::XML  : extension = "xml"; break;
        case OutputFormat::HTML : extension = "html"; break;
        case OutputFormat::PDF  : extension = "pdf"; break;
        }

        ss << "." << extension;
    }

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
