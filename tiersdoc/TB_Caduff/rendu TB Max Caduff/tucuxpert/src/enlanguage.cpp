#include "enlanguage.h"

namespace Tucuxi {
namespace Xpert {


EnLanguage::EnLanguage() : ILanguage(
    "name",
    "last name",
    "institute",
    "contact",

    "drug model",
    "drug used",
    "drug model used",
    "constraints for this model",
    "covariates defined",
    "compatible",
    "partially compatible",
    "provided value",
    "default value",
    "used value",
    "multiple values",
    "computed",
    "missing",

    "current dosage prediction",
    "suggested adjustments",
    "suggested adjustment",
    "data for this adjustment",
    "no suitable adjustment found",
    "target evaluation",
    "target type",
    "value",
    "score",
    "global score",
    "dosage",
    "dose",
    "infusion time",
    "interval",
    "number of iterations",
    "start date",
    "end date",

    "dates",
    "blood concentration",
    "predicted blood concentration",
    "measured samples"
) {}

const std::string EnLanguage::tellPredictionType (const Core::PredictionParameterType _predictionType) const
{
    const std::string types[] = {"default patient", "a priori", "a posteriori"};

    return "The type of prediction used is: " + types[static_cast<int>(_predictionType)];
}

const std::string EnLanguage::boundWarning() const
{
    return "Warning: at least one value of the predicted concentration is outside the allowed range.";
}

const std::string EnLanguage::sampleWarning (const std::string _date, const bool _upperBound) const
{
    return "Warning: the sample taken at: " + _date + " is " + (_upperBound ? "above the 95th " : "below the 5th ") +
            "percentile, its value might be wrong." ;
}

}
}
