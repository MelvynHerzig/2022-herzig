#include "ilanguage.h"
#include "enlanguage.h"

namespace Tucuxi {
namespace Xpert {


ILanguage::ILanguage(
        const std::string _name,
        const std::string _lastName,
        const std::string _institute,
        const std::string _contact,
        const std::string _drugModel,
        const std::string _drugUsed,
        const std::string _drugModelUsed,
        const std::string _constraintsForThisModel,
        const std::string _covariatesDefined,
        const std::string _compatible,
        const std::string _partiallyCompatible,
        const std::string _providedValue,
        const std::string _defaultValue,
        const std::string _usedValue,
        const std::string _multipleValues,
        const std::string _computed,
        const std::string _missing,
        const std::string _currentDosagePrediction,
        const std::string _suggestedAdjustments,
        const std::string _suggestedAdjustment,
        const std::string _dataForThisAdjustment,
        const std::string _noSuitableAdjustment,
        const std::string _targetEvaluation,
        const std::string _targetType,
        const std::string _value,
        const std::string _score,
        const std::string _globalScore,
        const std::string _dosage,
        const std::string _dose,
        const std::string _infusionTime,
        const std::string _interval,
        const std::string _nbIterations,
        const std::string _startDate,
        const std::string _endDate,
        const std::string _dates,
        const std::string _bloodConcentration,
        const std::string _predictedBloodConcentration,
        const std::string _measuredSamples) :
        NAME(_name),
        LAST_NAME(_lastName),
        INSTITUTE(_institute),
        CONTACT(_contact),
        DRUG_MODEL(_drugModel),
        DRUG_USED(_drugUsed),
        DRUG_MODEL_USED(_drugModelUsed),
        CONSTRAINTS_FOR_THIS_MODEL(_constraintsForThisModel),
        COVARIATES_DEFINED(_covariatesDefined),
        COMPATIBLE(_compatible),
        PARTIALY_COMPATIBLE(_partiallyCompatible),
        PROVIDED_VALUE(_providedValue),
        DEFAULT_VALUE(_defaultValue),
        USED_VALUE(_usedValue),
        MULTIPLE_VALUES(_multipleValues),
        COMPUTED(_computed),
        MISSING(_missing),
        CURRENT_DOSAGE_PREDICTION(_currentDosagePrediction),
        SUGGESTED_ADJUSTMENTS(_suggestedAdjustments),
        SUGGESTED_ADJUSTMENT(_suggestedAdjustment),
        DATA_FOR_THIS_ADJUSTMENT(_dataForThisAdjustment),
        NO_SUITABLE_ADJUSTMENT(_noSuitableAdjustment),
        TARGET_EVALUATION(_targetEvaluation),
        TARGET_TYPE(_targetType),
        VALUE(_value),
        SCORE(_score),
        GLOBAL_SCORE(_globalScore),
        DOSAGE(_dosage),
        DOSE(_dose),
        INFUSION_TIME(_infusionTime),
        INTERVAL(_interval),
        NB_ITERATIONS(_nbIterations),
        START_DATE(_startDate),
        END_DATE(_endDate),
        DATES(_dates),
        BLOOD_CONCENTRATION(_bloodConcentration),
        PREDICTED_BLOOD_CONCENTRATION(_predictedBloodConcentration),
        MEASURED_SAMPLES(_measuredSamples)
{ }

const std::string ILanguage::tellPredictionType(const Core::PredictionParameterType _predictionType) const
{
    return m_default;
}

const std::string ILanguage::boundWarning() const
{
    return m_default;
}

const std::string ILanguage::sampleWarning(const std::string _date, const bool _upperBound) const
{
    return m_default;
}


ILanguage* ILanguage::getLang(std::string lang)
{
    // cannot do a switch case with strings ='(
    if (lang == "fr") {
        //return ILanguage() ;
    }
    else if (lang == "en") {
        return new EnLanguage();
    }

    return new EnLanguage();


}

}
}
