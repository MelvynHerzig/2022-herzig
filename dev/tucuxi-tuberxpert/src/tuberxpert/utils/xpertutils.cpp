#include "xpertutils.h"

#include <cctype>
#include <sstream>
#include <iomanip>

#include "tuberxpert/language/languageexception.h"
#include "tuberxpert/result/globalresult.h"

using namespace std;

namespace Tucuxi {
namespace Xpert {

string dataTypeToString(const Core::DataType& _dataType)
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

string doubleToString(const double& _double)
{
    stringstream stream;
    stream << fixed << setprecision(2) << _double;
    string i = stream.str();
    return stream.str();
}

string covariateTypeToString(CovariateType _covariateType)
{
    switch (_covariateType) {
    case CovariateType::Model : return "default";
    case CovariateType::Patient  : return "patient";
    default : throw invalid_argument("Unknown covariate type"); // If properly  maintained, should never be used.
    }
}

string outputLangToString(OutputLang _lang)
{
    switch (_lang) {
    case OutputLang::ENGLISH : return "en";
    case OutputLang::FRENCH  : return "fr";
    default : throw LanguageException("Unknown language"); // If properly  maintained, should never be used.
    }
}

string warningLevelToString(WarningLevel _level)
{
    switch (_level) {
    case WarningLevel::NORMAL : return "normal";
    case WarningLevel::CRITICAL  : return "critical";
    default : throw invalid_argument("Unknown warning level"); // If properly  maintained, should never be used.
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

string TimeOfDayToString(const Common::TimeOfDay& _timeOfDay)
{
    stringstream timeStream;

    LanguageManager& langMgr = LanguageManager::getInstance();

    timeStream << _timeOfDay.hour() << langMgr.translate("hour_acronym");

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


    // Setting the date part.
    dateTimeStream << _dateTime.day() << '.' << _dateTime.month() << '.' << _dateTime.year();

    if (!_withTime) {
        return dateTimeStream.str();
    }

    // Setting the time part.
    dateTimeStream << ' ' << TimeOfDayToString(Common::Duration(
                                                   chrono::hours(_dateTime.hour()),
                                                   chrono::minutes(_dateTime.minute()),
                                                   chrono::seconds(_dateTime.second()))
                                               );

    return dateTimeStream.str();
}

string beautifyString(const string& _value, Core::DataType _type, const string& _id)
{
    LanguageManager& langMgr = LanguageManager::getInstance();

    // Convert the value to yes/no for nice display if the data type is bool.
    if (_type == Core::DataType::Bool) {
        if (stoi(_value)) {
            return langMgr.translate("yes");
        } else {
            return langMgr.translate("no");
        }
    // Only print two decimals if it is a double.
    } else if (_type == Core::DataType::Double && _id != "sex") {
        return doubleToString(stod(_value));
    }

    // Convert the value to man/woman/undefined for nice display.
    if (_id == "sex") {
        double sexAsDouble = stod(_value);
        if (sexAsDouble > 0.6) {
            return langMgr.translate("man");
        } else if (sexAsDouble < 0.4){
            return langMgr.translate("woman");
        } else {
            return langMgr.translate("undefined");
        }
    }

    return _value;
}

string getStringWithEnglishFallback(const Common::TranslatableString& _translatableString, OutputLang _lang)
{
    string translatedString = _translatableString.getString(outputLangToString(_lang));

    // Required translation.
    if (translatedString != ""){
        return translatedString;

    // English or empty string.
    } else {
        return _translatableString.getString();
    }
}

Common::DateTime getOldestDosageTimeRangeStart(const Core::DosageHistory& _dosageHistory,
                                               const Common::DateTime& _referenceTime)
{
    // In case the dosage history is empty. The olest dosage time range
    // start date is the reference time.
    Common::DateTime oldestDateKnown = _referenceTime;

    // Iterate over the time ranges and find the one that is the oldest.
    for (const unique_ptr<Core::DosageTimeRange>& timeRange : _dosageHistory.getDosageTimeRanges()) {
        if (timeRange->getStartDate() < oldestDateKnown){
            oldestDateKnown = timeRange->getStartDate();
        }
    }

    return oldestDateKnown;
}

Common::DateTime getLatestDosageTimeRangeStart(const Core::DosageHistory& _dosageHistory,
                                               const Common::DateTime& _referenceTime)
{
    // In case the dosage history is empty. The latest dosage time range start date is undefined.
    Common::DateTime latestDateKnown = Common::DateTime::undefinedDateTime();

    // Iterate over the time ranges and find the one that is the latest.
    for (const unique_ptr<Core::DosageTimeRange>& timeRange : _dosageHistory.getDosageTimeRanges()) {
        if (timeRange->getStartDate() < _referenceTime &&
                (latestDateKnown.isUndefined() || timeRange->getStartDate() > latestDateKnown)){
            latestDateKnown = timeRange->getStartDate();
        }
    }

    return latestDateKnown;
}

string computeFileName(const XpertRequestResult& _xpertRequestResult,
                       bool _addOutputPath,
                       bool _addExtension)
{
    stringstream fileNameStream;
    Common::DateTime computationTime = _xpertRequestResult.getGlobalResult().getComputationTime();

    // If the output path should be prefixed.
    if (_addOutputPath) {
        fileNameStream << _xpertRequestResult.getGlobalResult().getOutputPath() << "/";
    }

    fileNameStream << _xpertRequestResult.getXpertRequest().getDrugID() << "_" <<
          _xpertRequestResult.getGlobalResult().getRequestIndexBeingHandled() + 1 << "_" <<
          computationTime.day() << "-" << computationTime.month() << "-" << computationTime.year() << "_" <<
          computationTime.hour() << "h" << computationTime.minute() << "m" << computationTime.second() << "s";

    // If the extension should be suffixed.
    if (_addExtension) {

        string extension = "";

        switch(_xpertRequestResult.getXpertRequest().getOutputFormat()) {
        case OutputFormat::XML  : extension = "xml"; break;
        case OutputFormat::HTML : extension = "html"; break;
        case OutputFormat::PDF  : extension = "pdf"; break;
        }

        fileNameStream << "." << extension;
    }

    return fileNameStream.str();
}

string keyToPhrase(const string& _key)
{

    stringstream phraseStream;

    // Traverse the key.
    for(size_t i=0; i < _key.length(); i++)
    {
        // If the first char is lowercase, change it to upper case.
        if (i == 0 && islower(_key[i])) {
            phraseStream << char(toupper(_key[i]));
        }

        // Convert to lower case if it's
        // an upper case character but not the first.
        else if (isupper(_key[i]) && i != 0)
        {
            phraseStream << ' ' << char(tolower(_key[i]));
        }

        // Finally, if it's a lower case character,
        // then just add it to the phrase.
        else {
            phraseStream << _key[i];
        }
    }

    return phraseStream.str();
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
        throw invalid_argument("Invalid covariate type.");
    }
}

} // namespace XpertUtils
} // namespace Tucuxi
