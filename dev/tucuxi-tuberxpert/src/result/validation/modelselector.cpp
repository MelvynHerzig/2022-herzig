#include "modelselector.h"

#include <vector>
#include <limits>
#include <optional>
#include <algorithm>

#include "tucucore/dosage.h"
#include "tucucore/drugmodelrepository.h"
#include "tucucore/drugmodel/drugmodel.h"
#include "tucucore/definitions.h"
#include "tucucommon/unit.h"

using namespace std;

namespace Tucuxi {
namespace XpertResult {

ModelSelector::ModelSelector()
{}

bool ModelSelector::getBestDrugModel(const unique_ptr<XpertQuery::XpertRequestData>& _xpertRequest, XpertResult& _xpertResult)
{
    // Drug identifier targeted.
//    string drugId = _xpertRequest->getDrugID();

//    // Get the drug data for the given request.
//    const Query::DrugData* drugData = extractDrugData(_xpertRequest, _xpertResult.getQuery());

//    if (drugData == nullptr) return false;

//    // Get the "DrugModelRepository" component from the component manager
//    Tucuxi::Common::ComponentManager* pCmpMgr = Tucuxi::Common::ComponentManager::getInstance();
//    Tucuxi::Core::IDrugModelRepository* drugModelRepository = pCmpMgr->getComponent<Tucuxi::Core::IDrugModelRepository>("DrugModelRepository");

//    // Getting drug models that match the drug.
//    vector<Core::DrugModel*> drugModels = drugModelRepository->getDrugModelsByDrugId(drugId);

//    // Get the drug model with the lowest dissimilarity score.
//    unsigned bestScore = numeric_limits<unsigned>::max();
//    XpertRequestResult requestResult;

//    for (const Core::DrugModel* drugModel : drugModels) {

//        unsigned score = 0;
//        map<Core::CovariateDefinition*, CovariateResult> covariateResults;
//        bool scoreSuccess = computeDrugModelScore(drugModel, drugData, _xpertResult.getQuery()->getpParameters().getpPatient().getCovariates(), covariateResults, score);

//        // Better, the best is replaced.
//        if (scoreSuccess && score < bestScore) {
//            bestScore = score;
//        }
//    }

//    // If a result has not been found (the initial score is still the same)
//    if (bestScore == numeric_limits<unsigned>::max()) {
//        return false;
//    }

//    _xpertResult.getXpertRequestResults().emplace(make_pair(_xpertRequest.get(), requestResult));
    return true;
}

string ModelSelector::getErrorMessage() const
{
    return m_errorMessage;
}

void ModelSelector::setErrorMessage(const string& _errorMessage)
{
    m_errorMessage = _errorMessage;
}

const Query::DrugData* ModelSelector::extractDrugData(const unique_ptr<XpertQuery::XpertRequestData>& _xpertRequest, const unique_ptr<XpertQuery::XpertQueryData>& _xpertQuery)
{
    string drugId = _xpertRequest->getDrugID();

    // Get the number of matching drugs by id.
    const Query::DrugData* selectedDrugData = nullptr;
    auto drugDataBegin = _xpertQuery->getpParameters().getDrugs().begin();
    auto drugDataEnd = _xpertQuery->getpParameters().getDrugs().end();

    // Use lambda to count and select the matching drug data.
    int nbMatchingDrug = count_if(drugDataBegin, drugDataEnd, [drugId, &selectedDrugData](const unique_ptr<Query::DrugData>& drugData) {
        if (drugData->getDrugID()  == drugId) {
            selectedDrugData = drugData.get();
            return true;
        }

        return false;
    });

    // Check that there is one drug data that matches.
    if (nbMatchingDrug != 1) {
        if (nbMatchingDrug == 0) {
            setErrorMessage("Could not fullfil request for " + drugId + ", drug not found in query");
        } else {
            setErrorMessage("Could not fullfil request for " + drugId + ", drug found multiple times in query");
        }
        return nullptr;
    }

    // Check that all formulations and routes are the same.
    vector<Core::FormulationAndRoute> queryFormulationsAndRoutes = selectedDrugData->getpTreatment().getpDosageHistory().getFormulationAndRouteList();
    for (size_t j = 0; j < queryFormulationsAndRoutes.size(); ++j) {

        // Except for the first formulation and route, if it's not equal to the previous, they are not all the same
        if (j == 0) continue;
        if (queryFormulationsAndRoutes[j].getAdministrationRoute() != queryFormulationsAndRoutes[j - 1].getAdministrationRoute() ||
                queryFormulationsAndRoutes[j].getFormulation() != queryFormulationsAndRoutes[j - 1].getFormulation()) {

            setErrorMessage("The formulations and routes are not all the same for " + drugId + ". Adjustment aborted.");
            return nullptr;
        }
    }

    return selectedDrugData;
}

bool ModelSelector::computeDrugModelScore(const Core::DrugModel* _drugModel, const Query::DrugData* _drugData, const std::vector<std::unique_ptr<Core::PatientCovariate>>& _patientCovariates, map<Core::CovariateDefinition*, CovariateResult>& _covariateResults, unsigned& _score)
{
    // Before further investigation, check if the formulation and route used is compatible.
    if (not isFormulationAndRouteSupportedByDrugModel(_drugModel, _drugData)) {
        return false;
    }

    _score = 0;

    // Check for each covariate definition, if the patient has the covariate and if it respects the bounds.
    for (const unique_ptr<Core::CovariateDefinition>& covariateDefinition : _drugModel->getCovariates()){

        string covariateId = covariateDefinition->getId();

        // Count and get pointer to the corresponding patientCovariate.
        const Core::PatientCovariate* patientCovariate = nullptr;
        int nbMatchingCovariates = count_if(_patientCovariates.begin(), _patientCovariates.end(),
                                            [covariateId, &patientCovariate](const unique_ptr<Core::PatientCovariate>& pc){
            if (pc->getId() == covariateId) {
                patientCovariate = pc.get();
                return true;
            }
            return false;
        });

        // If not present only once, use definition and put warning
        if (nbMatchingCovariates != 1) {

            optional<string> warning;
            if (nbMatchingCovariates > 0) {
                // TODO Translation
                warning = "Multiple definitions in query replaced by model definition";
            } else {
                warning = nullopt;
            }

            ++_score;
            _covariateResults.emplace(make_pair(covariateDefinition.get(), CovariateResult(covariateDefinition.get(), CovariateType::Model, warning)));

        // Otherwise
        } else {


        }
    }

    return true;
}



bool ModelSelector::isFormulationAndRouteSupportedByDrugModel(const Core::DrugModel* _drugModel, const Query::DrugData* _drugData)
{
    // Get formulations and routes of the query.
    vector<Core::FormulationAndRoute> queryFromulationAndRouteList = _drugData->getpTreatment().getpDosageHistory().getFormulationAndRouteList();
    if (!queryFromulationAndRouteList.empty()){

        // Get formulations and routes of the model.
        for (const unique_ptr<Core::FullFormulationAndRoute>& modelFullFormulationAndRoute : _drugModel->getFormulationAndRoutes().getList()){

            Core::FormulationAndRoute modelFormulationAndRoute = modelFullFormulationAndRoute->getFormulationAndRoute();

            // If the formultations and routes match.
            if (modelFormulationAndRoute.getFormulation() == queryFromulationAndRouteList.front().getFormulation() &&
                    modelFormulationAndRoute.getAdministrationRoute() == queryFromulationAndRouteList.front().getAdministrationRoute() ) {
                return true;
            }
        }
    }

    return false;
}

void ModelSelector::createCovariateResultFromPatient(const std::unique_ptr<Core::PatientCovariate>& _patientCovariate, const Core::CovariateDefinition* _covariateDefinition, map<Core::CovariateDefinition*, CovariateResult>& _covariateResults, double &_score)
{
//    // If units are different.
//    if ((_patientCovariate->getUnit() == _covariateDefinition->getUnit()) == false) {

//        // Can be converted
//        if (Common::UnitManager::isCompatible(_covariateDefinition->getUnit(), _patientCovariate->getUnit())) {
//            double convertedValue = Common::UnitManager::convertToUnit(stod(_patientCovariate->getValue()), _patientCovariate->getUnit(), _covariateDefinition->getUnit());

//            // TODO check domain
//            CovariateResult<const Core::PatientCovariate*> covR{CovariateSource::Model, patientCovariatePtr, nullopt};
//            ++_score;
//            drugResult.getCovariateResults().emplace(make_pair(covariateDefinition.get(), &covR));

//        // Incompatible unit
//        } else {

//            // TODO Translation
//            optional<string> warning = "Covariate found in query but incompatble unit with defintion.";
//            ++_score;
//            _covariateResults.emplace(make_pair(_covariateDefinition, CovariateResult(_covariateDefinition, CovariateType::Model, warning)));
//        }

//    // Units are the same
//    } else {


//        Core::Operation* operation = _covariateDefinition->getValidation();
//        Core::OperationInputList defInputList = operation->getInputs();
//        Core::OperationInputList inputList;

//        // TODO confirm that 1 input by covariate definition, but it's assumed here
//        switch (defInputList.front().getType()) {
//        case Core::InputType::BOOL: {
//            bool b;
//            istringstream(_patientCovariate->getValue()) >> boolalpha >> b;
//            inputList.push_back(Core::OperationInput(defInputList.front().getName(),b));
//        } break;
//        case Core::InputType::DOUBLE: {
//            inputList.push_back(Core::OperationInput(defInputList.front().getName(),stod(_patientCovariate->getValue())));
//        } break;
//        case Core::InputType::INTEGER: {
//            inputList.push_back(Core::OperationInput(defInputList.front().getName(),stoi(_patientCovariate->getValue())));
//        } break;
//        }

//        double result = 0;
//        if (!operation->evaluate(inputList, result)){

//            // TODO Translation
//            optional<string> warning = "Covariate found in query but could not get validated.";
//            ++_score;
//            _covariateResults.emplace(make_pair(_covariateDefinition, CovariateResult(_covariateDefinition, CovariateType::Model, warning)));

//        } else {
//            // Not respecting the domain
//            optional<string> warning;
//            if (result == 0) {

//                // TODO Translation
//                optional<string> warning = "Domain error";
//                ++_score;

//            } else {
//                warning = nullopt;
//            }

//            _covariateResults.emplace(make_pair(_covariateDefinition, CovariateResult(_patientCovariate.get(), CovariateType::Patient, warning)));
//        }
//    }
}

} // namespace XpertResult
} // namespace Tucuxi
