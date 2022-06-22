#ifndef TUCUXI_XPERTUTILS_H
#define TUCUXI_XPERTUTILS_H

#include <string>

#include "tucucommon/datetime.h"
#include "tucucommon/translatablestring.h"
#include "tucucore/drugtreatment/drugtreatment.h"

#include "tuberxpert/language/languagemanager.h"

namespace Tucuxi {
namespace Xpert {

/// \brief Convert a floating point variable to a string with two decimals.
/// \param _value Variable to convert to a string.
/// \return String describing the variable.
std::string varToString(const double& _value);


/// \brief Computes the corresponding string out of an OutputLang.
/// \param lang OutputLang to "translate" into string.
/// \return Returns a string corresponding to OutputLang requested.
/// \throw LanguageException If the OutputLang is not supported.
std::string outputLangToString(OutputLang _lang);


/// \brief Extracts a string from a translatable string in regard of a given language. If
///        the language is not extractable, it tries to fallback with english.
/// \param _ts Translatable string.
/// \param _lang Lang to extract.
/// \return The translation if present, else the english translation else empty string.
std::string getStringWithEnglishFallback(const Common::TranslatableString& _ts, OutputLang _lang);

/// \brief Extract the dosage time range starting time that is the oldest of the dosage history and before
///        the reference time.
/// \param _dosageHistory Dosage history to extract the oldest time range starting time.
/// \param _referenceTime Reference time. The resulting time must be before this time.
/// \return Return the oldest starting time found or the computation date if the dosage history is empty.
Common::DateTime getOldestDosageTimeRangeStart(const Core::DosageHistory& _dosageHistory, const Common::DateTime& _referenceTime);

/// \brief Extract the latest (the last in the past / youngest) dosage time range start that is the closest to
///        the time reference time.
/// \param _dosageHistory Dosage history to extract the latest time range starting time.
/// \param _referenceTime Reference time. The resulting time must be before this time.
/// \return Return the latest starting time found or an undefined date if the dosage history is empty or in the future.
Common::DateTime getLatestDosageTimeRangeStart(const Core::DosageHistory& _dosageHistory, const Common::DateTime& _referenceTime);


} // namespace Xpert
} // namespace Tucuxi

#endif // TUCUXI_XPERTUTILS_H
