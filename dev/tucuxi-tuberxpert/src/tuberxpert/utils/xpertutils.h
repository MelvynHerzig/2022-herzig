#ifndef TUCUXI_XPERTUTILS_H
#define TUCUXI_XPERTUTILS_H

#include <string>

#include "tucucommon/datetime.h"
#include "tucucommon/translatablestring.h"
#include "tucucore/drugtreatment/drugtreatment.h"
#include "tucucore/computingcomponent.h"
#include "tucucore/computingservice/computingrequest.h"
#include "tucucore/computingservice/computingtrait.h"

#include "tuberxpert/language/languagemanager.h"
#include "tuberxpert/result/xpertrequestresult.h"

namespace Tucuxi {
namespace Xpert {

/// \brief Convert a floating point variable to a string with two decimals.
/// \param _value Variable to convert to a string.
/// \return String describing the variable.
std::string varToString(const double& _value);

/// \brief Convert a covariate type to a string.
/// \param _value Covariate type to convert to a string.
/// \return String describing the covariate type.
/// \throw Invalid argument if the CovariateType is not supported.
std::string varToString(CovariateType _value);

/// \brief Computes the corresponding string out of an OutputLang.
/// \param lang OutputLang to "translate" into string.
/// \return Returns a string corresponding to OutputLang requested.
/// \throw LanguageException If the OutputLang is not supported.
std::string varToString(OutputLang _lang);

/// \brief Convert a warning level to a string.
/// \param _value Warning level to convert to a string.
/// \return String describing the warning level.
/// \throw Invalid argument if the CovariateType is not supported.
std::string varToString(WarningLevel _lang);

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

/// \brief Compute the final file name considering the path and the desired format.
///        The final file name is <drugId>_<request number>_<computation time>.<file format>
/// \param _xpertRequestResult To get the directory path, the drug id and the file format.
/// \return Return the final file name.
std::string computeFileName(const XpertRequestResult& _xpertRequestResult);

template<typename T, typename U>
void executeRequestAndGetResult(std::unique_ptr<T> _trait, const XpertRequestResult& _xpertRequestResult, std::unique_ptr<U>& _responsePointer)
{
    // Make computig request and computing response
    Core::ComputingRequest computingRequest { "", *_xpertRequestResult.getDrugModel(), *_xpertRequestResult.getTreatment(), move(_trait)};
    std::unique_ptr<Core::ComputingResponse> computingResponse = std::make_unique<Core::ComputingResponse>("");

    // Start the core computation
    Core::IComputingService* computingComponent = dynamic_cast<Core::IComputingService*>(Core::ComputingComponent::createComponent());
    Core::ComputingStatus result = computingComponent->compute(computingRequest, computingResponse);

    // If computation failed, just set to nullptr and leave
    if (result != Core::ComputingStatus::Ok) {
        _responsePointer = nullptr;
        return;
    }

    // Acquire the data and put them in the unique pointer as response
    U* dataPointer = dynamic_cast<U*>(computingResponse->getUniquePointerData().release());
    _responsePointer = std::unique_ptr<U>(dataPointer);
}


} // namespace Xpert
} // namespace Tucuxi

#endif // TUCUXI_XPERTUTILS_H
