#include "modelselector.h"

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

#include "../../utils/xpertutils.h"

using namespace std;

namespace Tucuxi {
namespace XpertResult {

BestDrugModelSelector::BestDrugModelSelector(Common::DateTime _computationDate) : m_computationDate(_computationDate)
{}

void BestDrugModelSelector::getBestDrugModel(XpertRequestResult& _xpertRequestResult) const
{
    Tucuxi::Common::LoggerHelper logHelper;

    // Checks patient forumulations and routes compatibility.
    bool areFormulationAndRoutesEqual = checkPatientDosageHistoryFormulationAndRoutes(_xpertRequestResult.getTreatment()->getDosageHistory());
    if (areFormulationAndRoutesEqual == false) {
        _xpertRequestResult.setErrorMessage("All formulations and routes must be equal.");
        return;
    }

    // Targeted drug identifier.
    string drugId = _xpertRequestResult.getXpertRequest().getDrugID();

    // Gets the drug model repository.
    Tucuxi::Common::ComponentManager* pCmpMgr = Tucuxi::Common::ComponentManager::getInstance();
    Tucuxi::Core::IDrugModelRepository* drugModelRepository = pCmpMgr->getComponent<Tucuxi::Core::IDrugModelRepository>("DrugModelRepository");

    // Getting drug models that matches the drug..
    vector<Core::DrugModel*> drugModels = drugModelRepository->getDrugModelsByDrugId(drugId);

    if (drugModels.size() == 0) {
        _xpertRequestResult.setErrorMessage("Directory does not contain drug model for the given drug.");
        return;
    }

    // Evaluates each drug model for the given drug id.
    // Remembers the best.
    unsigned bestScore = numeric_limits<unsigned>::max();
    const Core::DrugModel* bestDrugModel = nullptr;
    vector<CovariateResult> bestCovariateResults;

    Core::TreatmentDrugModelCompatibilityChecker checker;

    for (const Core::DrugModel* drugModel : drugModels) {

        // Is the treatment compatible?
        if (!checker.checkCompatibility(_xpertRequestResult.getTreatment().get(), drugModel)) {
            logHelper.warn(drugModel->getDrugModelId() + " incompatible: Formulations and routes are not matching.");
            continue;
        }

        // Are the drug model constraints respected?
        if (drugModel->getDomain().getConstraints().size() > 0) {
            Common::DateTime start = getOldestCovariateDateTime(_xpertRequestResult.getTreatment()->getCovariates());
            Common::DateTime end = m_computationDate;
            vector<Core::DrugDomainConstraintsEvaluator::EvaluationResult> results;
            Core::DrugDomainConstraintsEvaluator ddcEvaluator;
            Core::DrugDomainConstraintsEvaluator::Result result = ddcEvaluator.evaluate(*drugModel, *_xpertRequestResult.getTreatment(), start, end, results);

            if(result != Core::DrugDomainConstraintsEvaluator::Result::PartiallyCompatible &&
                    result != Core::DrugDomainConstraintsEvaluator::Result::Compatible) {
                logHelper.warn(drugModel->getDrugModelId() + " incompatible: constraints not respected.");
                continue;
            }
        }

        try {
            // Individually, are the covariates respecting their validation?
            vector<CovariateResult> covariateResults;
            unsigned score = computeScore(
                        _xpertRequestResult.getTreatment()->getCovariates(),
                        drugModel->getCovariates(),
                        _xpertRequestResult.getXpertRequest().getOutputLang(),
                        covariateResults);

            // Compare score
            if (score < bestScore || ( score == bestScore && bestDrugModel->getCovariates().size() < drugModel->getCovariates().size())) {
                bestScore = score;
                bestDrugModel = drugModel;
                bestCovariateResults = covariateResults;
            }

        }  catch (const invalid_argument& e) {
            // We catch unit conversion error and operation error, then stop drug model search.
            _xpertRequestResult.setErrorMessage("Patient covariate error found when handling model " +
                                                drugModel->getDrugModelId() +
                                                ", details: " +
                                                string(e.what()));
            return;
        }
    }

    // If a compatible model is found:
    if (bestScore != numeric_limits<unsigned>::max()) {

        //check language compatibility
        if (checkCovariateDefinitionsLanguage(bestDrugModel->getCovariates(), _xpertRequestResult.getXpertRequest().getOutputLang())) {
            _xpertRequestResult.setCovariateResults(move(bestCovariateResults));
            _xpertRequestResult.setDrugModel(bestDrugModel);
        } else {
            _xpertRequestResult.setErrorMessage("Best drug model found but covariate definitions dont't support requested language.");
        }

    } else {
        _xpertRequestResult.setErrorMessage("No valid drug model found.");
    }
}

bool BestDrugModelSelector::checkPatientDosageHistoryFormulationAndRoutes(const Core::DosageHistory &_dosageHistory) const
{
    // Checks that all formulations and routes are the same.
    vector<Core::FormulationAndRoute> queryFormulationsAndRoutes = _dosageHistory.getFormulationAndRouteList();
    for (size_t j = 0; j < queryFormulationsAndRoutes.size(); ++j) {

        // Except for the first formulation and route, if it's not equal to the previous, they are not all the same
        if (j == 0 || queryFormulationsAndRoutes[j] == queryFormulationsAndRoutes[j-1]) continue;
        return false;
    }

    return true;
}

Common::DateTime BestDrugModelSelector::getOldestCovariateDateTime(const Core::PatientVariates& _patientVariates) const
{
    Common::DateTime oldestDateTime = m_computationDate;
    for (const unique_ptr<Core::PatientCovariate>& patientVariate: _patientVariates) {
        if (patientVariate->getEventTime() < oldestDateTime) {
            oldestDateTime = patientVariate->getEventTime();
        }
    }

    return oldestDateTime;
}

unsigned BestDrugModelSelector::computeScore(const Core::PatientVariates& _patientVariates,
                                             const Core::CovariateDefinitions& _modelDefinitions,
                                             XpertQuery::OutputLang _lang,
                                             vector<CovariateResult>& _results) const
{
    unsigned score = 0;

    // Map covariate id to patient covariate.
    map<string, vector<const Core::PatientCovariate*>> idToPatient;

    // filtered vector of covariate definitions
    map<string, const Core::CovariateDefinition*> idToDefinitionFiltered;

    // Inserting definition in idToDefinition.
    for (const unique_ptr<Core::CovariateDefinition>& definition : _modelDefinitions){
        // Filter computed definition.
        if (definition->isComputed() == false) {
            idToDefinitionFiltered[definition->getId()] = definition.get();
        }
    }

    // Filtering patient covariate.
    for (const unique_ptr<Core::PatientCovariate>& pc : _patientVariates){
        if (idToDefinitionFiltered.count(pc->getId()) == 1 ||
                (pc->getId() == "birthdate" && idToDefinitionFiltered.count("age") == 1)) {
            idToPatient[pc->getId()].emplace_back(pc.get());
        }
    }

    // Now computes the score.
    for (auto it = idToDefinitionFiltered.begin(); it != idToDefinitionFiltered.end(); ++it) {

        string covariateId = it->first;
        const Core::CovariateDefinition* definition = it->second;

        Core::Operation* operation = definition->getValidation();

        // If the covariate definition is "age".
        if(covariateId == "age") {
            // The patient has set his birthday
            if (idToPatient.count("birthdate") == 0) {
                _results.emplace_back(definition, nullptr, "");
                ++score;
                // There are some covariates for this definition.
            } else {
                if (idToPatient["birthdate"].size() > 1){
                    throw invalid_argument("Multiple birthdate not allowed.");
                } else if (idToPatient["birthdate"][0]->getDataType() != Core::DataType::Date){
                    throw invalid_argument("Invalid data type for birthdate.");
                } else {
                    Common::DateTime birthdate = idToPatient["birthdate"][0]->getValueAsDate();
                    Core::Value newVal = 0;
                    switch (idToDefinitionFiltered["age"]->getType()) {
                    case Core::CovariateType::AgeInDays:
                        newVal = static_cast<double>(Common::Utils::dateDiffInDays(birthdate, m_computationDate));
                        break;
                    case Core::CovariateType::AgeInWeeks:
                        newVal = static_cast<double>(Common::Utils::dateDiffInWeeks(birthdate,  m_computationDate));
                        break;
                    case Core::CovariateType::AgeInMonths:
                        newVal = static_cast<double>(Common::Utils::dateDiffInMonths(birthdate,  m_computationDate));
                        break;
                    case Core::CovariateType::AgeInYears:
                        newVal = static_cast<double>(Common::Utils::dateDiffInYears(birthdate,  m_computationDate));
                        break;
                    default: break; // Unreachable
                    }

                    if (checkOperation(operation, newVal, definition, idToPatient["birthdate"][0], _lang, _results) == false) {
                        ++score;
                    }
                }
            }
            // If covariate definition is not "age"
        } else {
            // The patient has no covariate for this definition.
            if (idToPatient.count(definition->getId()) == 0) {
                _results.emplace_back(definition, nullptr, "");
                ++score;
                // There are some covariates for this definition.
            } else {
                bool alreadyScored = false;

                for (const Core::PatientCovariate* patientCovariate : idToPatient[definition->getId()]){
                    Core::Value newVal = Common::Utils::stringToValue( patientCovariate->getValue(), patientCovariate->getDataType());

                    newVal = Common::UnitManager::convertToUnit(newVal, patientCovariate->getUnit(), definition->getUnit());

                    if (checkOperation(operation, newVal, definition, patientCovariate, _lang, _results) == false && !alreadyScored) {
                        ++score;
                        alreadyScored = true;
                    }
                }
            }
        }
    }
    return score;
}

bool BestDrugModelSelector::checkOperation(Core::Operation* _op,
                                           double _val,
                                           const Core::CovariateDefinition* _definition,
                                           const Core::PatientCovariate* _patient,
                                           XpertQuery::OutputLang _lang,
                                           std::vector<CovariateResult>& _results) const
{
    Core::OperationInputList defInputList = _op->getInputs();
    Core::OperationInputList inputList;
    inputList.push_back(Core::OperationInput(defInputList.front().getName(), _val));

    double result = 0;
    // Operation evaluation failed.
    if (!_op->evaluate(inputList, result)){
        throw invalid_argument("Evaluation failed for covariate " + _patient->getId());
    }

    string warning = (result == 0 ? XpertUtils::getStringWithEnglishFallback(_definition->getValidationErrorMessage(), _lang) : "");

    _results.emplace_back(_definition, _patient, warning);

    // If operation checks success.
    return result == 1;
}

bool BestDrugModelSelector::checkCovariateDefinitionsLanguage(
        const Core::CovariateDefinitions& _modelDefinitions,
        XpertQuery::OutputLang _lang) const
{
    for (const unique_ptr<Core::CovariateDefinition>& cd : _modelDefinitions) {

        // Checking name translation.
        if( cd->getName().getString(XpertUtils::outputLangToString(XpertQuery::OutputLang::ENGLISH)) == "" &&
                cd->getName().getString(XpertUtils::outputLangToString(_lang)) == "") {
            return false;
        }

        // Checking description translation.
        if( cd->getDescription().getString(XpertUtils::outputLangToString(XpertQuery::OutputLang::ENGLISH)) == "" &&
                cd->getDescription().getString(XpertUtils::outputLangToString(_lang)) == "") {
            return false;
        }

        // Checking validation error message translation.
        if( cd->getValidationErrorMessage().getString(XpertUtils::outputLangToString(XpertQuery::OutputLang::ENGLISH)) == "" &&
                cd->getValidationErrorMessage().getString(XpertUtils::outputLangToString(_lang)) == "") {
            return false;
        }
    }

    return true;
}

} // namespace XpertResult
} // namespace Tucuxi
