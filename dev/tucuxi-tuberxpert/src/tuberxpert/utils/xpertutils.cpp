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

} // namespace XpertUtils
} // namespace Tucuxi
