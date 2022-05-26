#include "modelselector.h"

#include <vector>
#include <limits>
#include <optional>
#include <map>

#include "tucucore/drugmodelrepository.h"
#include "tucucore/drugmodel/drugmodel.h"
#include "tucucore/treatmentdrugmodelcompatibilitychecker.h"
#include "tucucore/drugdomainconstraintsevaluator.h"
#include "tucucore/drugtreatment/patientcovariate.h"
#include "tucucore/definitions.h"
#include "tucucommon/utils.h"
#include "tucucommon/unit.h"

using namespace std;

namespace Tucuxi {
namespace XpertResult {

BestDrugModelSelector::BestDrugModelSelector()
{}

void BestDrugModelSelector::getBestDrugModel(XpertRequestResult& _xpertRequestResult) const
{
    Tucuxi::Common::LoggerHelper logHelper;

    bool areFormulationAndRoutesEqual = checkPatientDosageHistoryFormulationAndRoutes(_xpertRequestResult.getTreatment()->getDosageHistory());
    if (areFormulationAndRoutesEqual == false) {
        _xpertRequestResult.setErrorMessage("All formulations and routes must be equal.");
        return;
    }

    // Targeted drug identifier
    string drugId = _xpertRequestResult.getXpertRequest()->getDrugID();

    // Get the drug model repository
    Tucuxi::Common::ComponentManager* pCmpMgr = Tucuxi::Common::ComponentManager::getInstance();
    Tucuxi::Core::IDrugModelRepository* drugModelRepository = pCmpMgr->getComponent<Tucuxi::Core::IDrugModelRepository>("DrugModelRepository");

    // Getting drug models that match the drug.
    vector<Core::DrugModel*> drugModels = drugModelRepository->getDrugModelsByDrugId(drugId);

    // Evaluate each drug model for the given drug id.
    // Remember the best.
    unsigned bestScore = numeric_limits<unsigned>::max();
    const Core::DrugModel* bestDrugModel = nullptr;
    vector<CovariateResult> bestCovariateResults;

    Core::TreatmentDrugModelCompatibilityChecker checker;

    for (const Core::DrugModel* drugModel : drugModels) {

        // Is the treatment compatible?
        if (!checker.checkCompatibility(_xpertRequestResult.getTreatment().get(), drugModel)) {
            continue;
        }

        // Are the drug model constraints respected?
        Common::DateTime start = getOldestCovariateDateTime(_xpertRequestResult.getTreatment()->getCovariates());
        Common::DateTime end = Common::DateTime::now();
        vector<Core::DrugDomainConstraintsEvaluator::EvaluationResult> results;
        Core::DrugDomainConstraintsEvaluator ddcEvaluator;
        Core::DrugDomainConstraintsEvaluator::Result result = ddcEvaluator.evaluate(*drugModel, *_xpertRequestResult.getTreatment(), start, end, results);

        if(result != Core::DrugDomainConstraintsEvaluator::Result::PartiallyCompatible &&
                result != Core::DrugDomainConstraintsEvaluator::Result::Compatible) {
            continue;
        }

        try {
          // Individually, are the covariates respecting their validation?
            vector<CovariateResult> covariateResults;
            unsigned score = computeScore(_xpertRequestResult.getTreatment()->getCovariates(), drugModel->getCovariates(), covariateResults);

            // Compare score
            if (score > bestScore || ( score == bestScore && bestDrugModel->getCovariates().size() < drugModel->getCovariates().size())) {
                bestScore = score;
                bestDrugModel = drugModel;
                bestCovariateResults = covariateResults;
            }

        }  catch (std::invalid_argument e) {
            // Unit conversion may fail, so we catch and jump to next model. Probably won't use this covariate
            logHelper.warn(e.what());
            continue;
        }
    }

    // If a compatible model is found
    if (bestScore != numeric_limits<unsigned>::max()) {
        _xpertRequestResult.setCovariateResults(move(bestCovariateResults));
        _xpertRequestResult.setDrugModel(bestDrugModel);
    } else {
        _xpertRequestResult.setErrorMessage("No valid drug model found.");
    }
}

bool BestDrugModelSelector::checkPatientDosageHistoryFormulationAndRoutes(const Core::DosageHistory &_dosageHistory) const
{
    // Check that all formulations and routes are the same.
    vector<Core::FormulationAndRoute> queryFormulationsAndRoutes = _dosageHistory.getFormulationAndRouteList();
    for (size_t j = 0; j < queryFormulationsAndRoutes.size(); ++j) {

        // Except for the first formulation and route, if it's not equal to the previous, they are not all the same
        if (j == 0) continue;
        if (queryFormulationsAndRoutes[j].getAdministrationRoute() != queryFormulationsAndRoutes[j - 1].getAdministrationRoute() ||
                queryFormulationsAndRoutes[j].getFormulation() != queryFormulationsAndRoutes[j - 1].getFormulation()) {
            return false;
        }
    }

    return true;
}

Common::DateTime BestDrugModelSelector::getOldestCovariateDateTime(const Core::PatientVariates& _patientVariates) const
{
    Common::DateTime oldestDateTime = Common::DateTime::now();
    for (const unique_ptr<Core::PatientCovariate>& patientVariate: _patientVariates) {
        if (patientVariate->getEventTime() < oldestDateTime) {
            oldestDateTime = patientVariate->getEventTime();
        }
    }

    return oldestDateTime;
}

unsigned BestDrugModelSelector::computeScore(const Core::PatientVariates& _patientVariates,
                                             const Core::CovariateDefinitions& _modelDefinitions,
                                             vector<CovariateResult>& _results) const
{
    unsigned score = 0;

    // Map covariate id to patient covariate.
    map<string, vector<const Core::PatientCovariate*>> idToPatient;

    // filtered vector of covariate definitions
    map<string, const Core::CovariateDefinition*> definitionFiltered;

    // Inserting definition in idToDefinition.
    for (const unique_ptr<Core::CovariateDefinition>& definition : _modelDefinitions){
        // Filter computed definition.
        if (definition->isComputed() == false) {
            definitionFiltered[definition->getId()] = definition.get();
        }
    }

    // Filtering patient covariate.
    for (const unique_ptr<Core::PatientCovariate>& pc : _patientVariates){
        if (definitionFiltered.count(pc->getId()) == 1) {
            idToPatient[pc->getId()].emplace_back(pc.get());
        }
    }

    // Now compute the score.
    for (auto it = definitionFiltered.begin(); it != definitionFiltered.end(); ++it) {

        string covariateId = it->first;
        const Core::CovariateDefinition* definition = it->second;

        Core::Operation* operation = definition->getValidation();
        Core::OperationInputList defInputList = operation->getInputs();


        // The patient has no covariate for this definition.
        if (idToPatient.count(definition->getId()) == 0) {
            _results.emplace_back(definition, nullptr, nullopt);
            ++score;
        // There are some covariates for this definition.
        } else {

            for (const Core::PatientCovariate* patientCovariate : idToPatient[definition->getId()]){
                Core::Value newVal = Common::Utils::stringToValue( patientCovariate->getValue(), patientCovariate->getDataType());
                newVal = Common::UnitManager::convertToUnit(newVal, patientCovariate->getUnit(), definition->getUnit());

                Core::OperationInputList inputList;
                inputList.push_back(Core::OperationInput(defInputList.front().getName(), newVal));

                double result = 0;
                // Operation evaluation failed.
                if (!operation->evaluate(inputList, result)){
                    throw invalid_argument("Evaluation failed for covariate " + covariateId);
                }

                optional<string> warning;
                // Domain not respected
                if (result == 0) {
                    // TODO get translated error message from model
                    warning = "Domain error";
                    ++score;
                // Domain is respected
                } else {
                    warning = nullopt;
                }

                 _results.emplace_back(definition, patientCovariate, warning);
            }
        }
    }
    return score;
}

} // namespace XpertResult
} // namespace Tucuxi
