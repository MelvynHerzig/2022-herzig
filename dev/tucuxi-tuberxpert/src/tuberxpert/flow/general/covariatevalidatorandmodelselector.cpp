#include "covariatevalidatorandmodelselector.h"

#include <vector>
#include <limits>
#include <optional>
#include <map>

#include "tucucore/covariateevent.h"
#include "tucucore/covariateextractor.h"
#include "tucucore/drugmodelrepository.h"
#include "tucucore/drugmodel/drugmodel.h"
#include "tucucore/treatmentdrugmodelcompatibilitychecker.h"
#include "tucucore/drugdomainconstraintsevaluator.h"
#include "tucucore/drugtreatment/patientcovariate.h"
#include "tucucore/definitions.h"
#include "tucucommon/utils.h"
#include "tucucommon/unit.h"

#include "tuberxpert/utils/xpertutils.h"
#include "tuberxpert/result/xpertqueryresult.h"

using namespace std;

namespace Tucuxi {
namespace Xpert {

void CovariateValidatorAndModelSelector::perform(XpertRequestResult& _xpertRequestResult)
{
    // Fix the computation time.
    m_computationTime = _xpertRequestResult.getXpertQueryResult().getComputationTime();

    Tucuxi::Common::LoggerHelper logHelper;

    // Check if there is a treatment.
    if (_xpertRequestResult.getTreatment() == nullptr) {
        _xpertRequestResult.setErrorMessage("No treatment set.");
        return;
    }

    // Check the compatibility of the forumulations and routes between the doses of the patient.
    bool areFormulationsAndRoutesEqual = checkFormulationsAndRoutesCompatibility(_xpertRequestResult.getTreatment()->getDosageHistory());
    if (areFormulationsAndRoutesEqual == false) {
        _xpertRequestResult.setErrorMessage("All formulations and routes must be equal.");
        return;
    }

    // Get the drug identifier of the xpertReqest.
    string drugId = _xpertRequestResult.getXpertRequest().getDrugId();

    // Get the drug model repository.
    Tucuxi::Common::ComponentManager* pCmpMgr = Tucuxi::Common::ComponentManager::getInstance();
    Tucuxi::Core::IDrugModelRepository* drugModelRepository = pCmpMgr->getComponent<Tucuxi::Core::IDrugModelRepository>("DrugModelRepository");

    // Get the drug models that match the drug identifier.
    vector<Core::DrugModel*> drugModels = drugModelRepository->getDrugModelsByDrugId(drugId);

    if (drugModels.size() == 0) {
        _xpertRequestResult.setErrorMessage("The drug files directory does not contain a drug model for the given drug: " + drugId);
        return;
    }

    // Remember the best drug model.
    unsigned lowestKnownScore = numeric_limits<unsigned>::max();             // Score of the best model known.
    const Core::DrugModel* bestModel = nullptr;                              // Pointer on the best model known.
    vector<CovariateValidationResult> covariateValidationResultsOfBestModel; // Covariates validation result for the best model known.

    Core::TreatmentDrugModelCompatibilityChecker drugModelTreatmentCompatiblityChecker;

    // Evaluate each drug model for the given drug identifier.
    for (const Core::DrugModel* drugModel : drugModels) {

        // Does the drug model supports the formulation and route of the treatment?
        if (!drugModelTreatmentCompatiblityChecker.checkCompatibility(_xpertRequestResult.getTreatment().get(), drugModel)) {
            logHelper.warn(drugModel->getDrugModelId() + " incompatible: Formulations and routes are not matching.");
            continue;
        }

        // Are the drug model constraints respected?
        Common::DateTime start = getOldestCovariateDateTime(_xpertRequestResult.getTreatment()->getCovariates());
        Common::DateTime end = m_computationTime;
        vector<Core::DrugDomainConstraintsEvaluator::EvaluationResult> results;
        Core::DrugDomainConstraintsEvaluator constraintEvaluator;
        Core::DrugDomainConstraintsEvaluator::Result constraintsResult = constraintEvaluator.evaluate(*drugModel,
                                                                                                      *_xpertRequestResult.getTreatment(),
                                                                                                      start,
                                                                                                      end,
                                                                                                      results);

        // If the contraints are not compatible.
        if(constraintsResult == Core::DrugDomainConstraintsEvaluator::Result::Incompatible) {
            logHelper.warn(drugModel->getDrugModelId() + " incompatible: constraints not respected.");
            continue;
        }

        // If the covariates extraction failed.
        if(constraintsResult == Core::DrugDomainConstraintsEvaluator::Result::ComputationError) {
            _xpertRequestResult.setErrorMessage("Covariates extraction failed for drug model: " +
                                                drugModel->getDrugModelId() +
                                                ". It may be caused by covariates that could not be converted.");
            return;
        }

        try {
            // Compute the score for this drug model.
            vector<CovariateValidationResult> covariateResults;
            unsigned score = computeScore(
                        _xpertRequestResult.getTreatment()->getCovariates(),
                        drugModel->getCovariates(),
                        _xpertRequestResult.getXpertRequest().getOutputLang(),
                        covariateResults);

            // Compare the score with the score of the best drug model.
            // Better or same but with more covariate definitions.
            if (score < lowestKnownScore ||
                    ( score == lowestKnownScore && bestModel->getCovariates().size() < drugModel->getCovariates().size())) {

                // The current model become the best model known.
                lowestKnownScore = score;
                bestModel = drugModel;
                covariateValidationResultsOfBestModel = covariateResults;
            }

        }  catch (const invalid_argument& e) {
            // We catch operation error, then stop drug model search.
            // (Covariate type compatibility already checked when checking contraints).
            // We don't move on to the next model because as soon as the problem is fixed,
            // it can be this model that is the best.
            _xpertRequestResult.setErrorMessage("Patient covariate error found when handling model " +
                                                drugModel->getDrugModelId() +
                                                ", details: " +
                                                string(e.what()));
            return;
        }
    }

    // If a model is found.
    if (lowestKnownScore != numeric_limits<unsigned>::max()) {

        // Check the language compatibility.
        if (checkCovariateDefinitionsSupportedLanguage(bestModel->getCovariates(), _xpertRequestResult.getXpertRequest().getOutputLang())) {

            _xpertRequestResult.setCovariateResults(move(covariateValidationResultsOfBestModel));
            _xpertRequestResult.setDrugModel(bestModel);
        // The model does not support the language.
        } else {

            _xpertRequestResult.setErrorMessage("Best drug model found but covariate definitions dont't support requested language.");
        }
    // No model found.
    } else {
        _xpertRequestResult.setErrorMessage("No valid drug model found.");
    }
}

bool CovariateValidatorAndModelSelector::checkFormulationsAndRoutesCompatibility(const Core::DosageHistory &_dosageHistory) const
{
    // Check that all formulations and routes are equal.
    vector<Core::FormulationAndRoute> queryFormulationsAndRoutes = _dosageHistory.getFormulationAndRouteList();
    for (size_t i = 0; i < queryFormulationsAndRoutes.size(); ++i) {

        // With the exception of the first formulation and route, if not equal to the previous one, they are not all identical.
        if (i == 0 || queryFormulationsAndRoutes[i] == queryFormulationsAndRoutes[i-1]) {
            continue;
        }
        return false;
    }

    return true;
}

Common::DateTime CovariateValidatorAndModelSelector::getOldestCovariateDateTime(const Core::PatientVariates& _patientCovariates) const
{
    Common::DateTime oldestDateTimeKnown = m_computationTime;

    // For each patient's covariate.
    for (const unique_ptr<Core::PatientCovariate>& patientCovariate: _patientCovariates) {
        if (patientCovariate->getEventTime() < oldestDateTimeKnown) {
            oldestDateTimeKnown = patientCovariate->getEventTime();
        }
    }

    return oldestDateTimeKnown;
}

unsigned CovariateValidatorAndModelSelector::computeScore(const Core::PatientVariates& _patientVariates,
                                             const Core::CovariateDefinitions& _modelDefinitions,
                                             OutputLang _lang,
                                             vector<CovariateValidationResult>& _results) const
{
    unsigned score = 0;

    // Map covariate id to patient covariate.
    map<string, vector<const Core::PatientCovariate*>> idToPatient;

    // Map covariate id to covariate definition.
    map<string, const Core::CovariateDefinition*> idToDefinitionFiltered;

    // Inserting definition in idToDefinition.
    for (const unique_ptr<Core::CovariateDefinition>& covariateDefinition : _modelDefinitions){

        // Filter computed definition.
        if (covariateDefinition->isComputed() == false) {
            idToDefinitionFiltered[covariateDefinition->getId()] = covariateDefinition.get();
        }
    }

    // Retrieving only needed patient covariate
    for (const unique_ptr<Core::PatientCovariate>& patientCovariate : _patientVariates){

        // If a covariate definition with the same id exist
        if (idToDefinitionFiltered.count(patientCovariate->getId()) == 1 ||
                (patientCovariate->getId() == "birthdate" && idToDefinitionFiltered.count("age") == 1)) {
            idToPatient[patientCovariate->getId()].emplace_back(patientCovariate.get());
        }
    }

    // Compute the score.
    // For each covariate definition
    for (auto idToDefinitionIt = idToDefinitionFiltered.begin();
         idToDefinitionIt != idToDefinitionFiltered.end();
         ++idToDefinitionIt) {

        string covariateId = idToDefinitionIt->first;
        const Core::CovariateDefinition* definition = idToDefinitionIt->second;

        // If the covariate definition is "age".
        if(covariateId == "age") {

            // If the patient has no covariate for this definition.
            if (idToPatient.count("birthdate") == 0) {
                _results.emplace_back(definition, nullptr, "");
                ++score;
            // There are some covariates for this definition.
            } else {
                if (idToPatient["birthdate"].size() > 1){
                    throw invalid_argument("Multiple birthdate not allowed.");
                } else if (idToPatient["birthdate"][0]->getDataType() != Core::DataType::Date){
                    throw invalid_argument("Invalid data type of birthdate.");
                } else {

                    // Get the age out of patient birthdate covariate.
                    double age = getAgeIn(idToDefinitionFiltered["age"]->getType(),
                                          idToPatient["birthdate"][0]->getValueAsDate(),
                                           m_computationTime);

                    // Check the validation.
                    if (checkOperation(age, definition, idToPatient["birthdate"][0], _lang, _results) == false) {
                        ++score;
                    }
                }
            }

        // If covariate definition is not "age"
        } else {

            // If the patient has no covariate for this definition.
            if (idToPatient.count(definition->getId()) == 0) {
                _results.emplace_back(definition, nullptr, "");
                ++score;
            // There are some covariates for this definition.
            } else {
                bool alreadyScored = false;

                // Check each patient's covariate for this definition.
                for (const Core::PatientCovariate* patientCovariate : idToPatient[definition->getId()]){

                    // Get the value and convert.
                    Core::Value value = Common::Utils::stringToValue( patientCovariate->getValue(), patientCovariate->getDataType());
                    value = Common::UnitManager::convertToUnit(value, patientCovariate->getUnit(), definition->getUnit());

                    if (checkOperation(value, definition, patientCovariate, _lang, _results) == false && !alreadyScored) {
                        ++score;
                        alreadyScored = true;
                    }
                }
            }
        }
    }

    return score;
}

bool CovariateValidatorAndModelSelector::checkOperation(double _patientConvertedValue,
                                                        const Core::CovariateDefinition* _definition,
                                                        const Core::PatientCovariate* _patient,
                                                        OutputLang _lang,
                                                        vector<CovariateValidationResult>& _results) const
{
    // Get the validation operation.
    Core::Operation* _op = _definition->getValidation();
    Core::OperationInputList defInputList = _op->getInputs();
    Core::OperationInputList inputList;

    // Add the covariate value in the operation input.
    inputList.push_back(Core::OperationInput(defInputList.front().getName(), _patientConvertedValue));

    double result = 0;

    // Evaluate and check if operation could be completed.
    if (!_op->evaluate(inputList, result)){
        throw invalid_argument("Evaluation failed for covariate " + _patient->getId());
    }

    // Get the warning message if the validation is not respected.
    string warning = (result == 0 ? getStringWithEnglishFallback(_definition->getValidationErrorMessage(), _lang) : "");

    // Add the validation result in the results.
    _results.emplace_back(_definition, _patient, warning);

    // If validation operation is respected.
    return result == 1;
}

bool CovariateValidatorAndModelSelector::checkCovariateDefinitionsSupportedLanguage(
        const Core::CovariateDefinitions& _modelDefinitions,
        OutputLang _lang) const
{
    // For each covariate definition
    for (const unique_ptr<Core::CovariateDefinition>& covariateDefinition : _modelDefinitions) {

        // Checking name translation.
        if( covariateDefinition->getName().getString(outputLangToString(OutputLang::ENGLISH)) == "" &&
                covariateDefinition->getName().getString(outputLangToString(_lang)) == "") {
            return false;
        }

        // Checking description translation.
        if( covariateDefinition->getDescription().getString(outputLangToString(OutputLang::ENGLISH)) == "" &&
                covariateDefinition->getDescription().getString(outputLangToString(_lang)) == "") {
            return false;
        }

        // Checking validation error message translation.
        if( covariateDefinition->getValidationErrorMessage().getString(outputLangToString(OutputLang::ENGLISH)) == "" &&
                covariateDefinition->getValidationErrorMessage().getString(outputLangToString(_lang)) == "") {
            return false;
        }
    }

    return true;
}

} // namespace Xpert
} // namespace Tucuxi
