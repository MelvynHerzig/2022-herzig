#ifndef TUCUXI_XPERTUTILS_H
#define TUCUXI_XPERTUTILS_H

#include <string>

#include "tucucommon/translatablestring.h"

#include "../language/languagemanager.h"

namespace Tucuxi {
namespace XpertUtils {

/// \brief Convert a floating point variable to a string with two decimals.
/// \param _value Variable to convert to a string.
/// \return String describing the variable.
std::string varToString(const double& _value);


/// \brief Computes the corresponding string out of an OutputLang.
/// \param lang OutputLang to "translate" into string.
/// \return Returns a string corresponding to OutputLang requested.
/// \throw LanguageException If the OutputLang is not supported.
std::string outputLangToString(XpertQuery::OutputLang _lang);


/// \brief Extracts a string from a translatable string in regard of a given language. If
///        the language is not extractable, it tries to fallback with english.
/// \param _ts Translatable string.
/// \param _lang Lang to extract.
/// \return The translation if present, else the english translation else empty string.
std::string getStringWithEnglishFallback(const Common::TranslatableString& _ts, XpertQuery::OutputLang _lang);


} // namespace XpertUtils
} // namespace Tucuxi

#endif // TUCUXI_XPERTUTILS_H
