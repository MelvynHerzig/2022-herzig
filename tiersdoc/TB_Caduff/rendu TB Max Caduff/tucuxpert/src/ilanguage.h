#ifndef ILANGUAGE_H
#define ILANGUAGE_H

#include <string>
#include "tucucore/computingservice/computingtrait.h"


namespace Tucuxi {
namespace Xpert {

class ILanguage {
protected:
    ILanguage(
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
            const std::string _measuredSamples
    );

    const std::string m_default = "not translated.";

public:
    virtual ~ILanguage() {}

    // Admin data
    const std::string NAME;
    const std::string LAST_NAME;
    const std::string INSTITUTE;
    const std::string CONTACT;


    const std::string DRUG_MODEL;
    const std::string DRUG_USED;
    const std::string DRUG_MODEL_USED;
    const std::string CONSTRAINTS_FOR_THIS_MODEL;
    const std::string COVARIATES_DEFINED;
    const std::string COMPATIBLE;
    const std::string PARTIALY_COMPATIBLE;
    const std::string PROVIDED_VALUE;
    const std::string DEFAULT_VALUE;
    const std::string USED_VALUE;
    const std::string MULTIPLE_VALUES;
    const std::string COMPUTED;
    const std::string MISSING;

    const std::string CURRENT_DOSAGE_PREDICTION;
    const std::string SUGGESTED_ADJUSTMENTS;
    const std::string SUGGESTED_ADJUSTMENT;
    const std::string DATA_FOR_THIS_ADJUSTMENT;
    const std::string NO_SUITABLE_ADJUSTMENT;
    const std::string TARGET_EVALUATION;
    const std::string TARGET_TYPE;
    const std::string VALUE;
    const std::string SCORE;
    const std::string GLOBAL_SCORE;
    const std::string DOSAGE;
    const std::string DOSE;
    const std::string INFUSION_TIME;
    const std::string INTERVAL;
    const std::string NB_ITERATIONS;
    const std::string START_DATE;
    const std::string END_DATE;

    const std::string DATES;
    const std::string BLOOD_CONCENTRATION;
    const std::string PREDICTED_BLOOD_CONCENTRATION;
    const std::string MEASURED_SAMPLES;

    virtual const std::string tellPredictionType(const Core::PredictionParameterType _predictionType) const;
    virtual const std::string boundWarning() const;
    virtual const std::string sampleWarning(const std::string _date, const bool _upperBound ) const;


    static ILanguage* getLang (std::string lang = "en") ;


};

}
}

#endif // ILANGUAGE_H
