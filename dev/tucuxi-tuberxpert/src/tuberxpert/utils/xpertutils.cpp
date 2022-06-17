#include "xpertutils.h"

#include <sstream>
#include <iomanip>

#include "tuberxpert/language/languageexception.h"

using namespace std;

namespace Tucuxi {
namespace XpertUtils {

string varToString(const double& _value)
{
    stringstream stream;
    stream << std::fixed << setprecision(2) << _value;
    return stream.str();
}

string outputLangToString(XpertQuery::OutputLang _lang)
{
    switch (_lang) {
    case Tucuxi::XpertQuery::OutputLang::ENGLISH : return "en";
    case Tucuxi::XpertQuery::OutputLang::FRENCH  : return "fr";
    default : throw XpertLanguage::LanguageException("Unknown language"); // If well maintained, should never be returned.
    }
}

string getStringWithEnglishFallback(const Common::TranslatableString& _ts, XpertQuery::OutputLang _lang)
{
    string target = _ts.getString(outputLangToString(_lang));

    if (target != ""){
        return target;
    } else {
        return _ts.getString();
    }
}

} // namespace XpertUtils
} // namespace Tucuxi
