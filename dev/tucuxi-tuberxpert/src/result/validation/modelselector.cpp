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

ModelSelector::Status ModelSelector::getBestModelForQueryDrugs(XpertResult& _xpertResult)
{
    size_t nbRequestFailed = 0;

    Common::UnitManager unitManager;

    auto patientCovariatesBegin = _xpertResult.getQuery()->getpParameters().getpPatient().getCovariates().begin();
    auto patientCovariatesEnd = _xpertResult.getQuery()->getpParameters().getpPatient().getCovariates().end();

    // Only handle the drug which are required by request xpert.
    for (const unique_ptr<XpertQuery::XpertRequestData>& xpertRequest : _xpertResult.getQuery()->getXpertRequests()) {
        string drugId = xpertRequest->getDrugID();

        //Find drug data corresponding to the request.
        auto begin = _xpertResult.getQuery()->getpParameters().getDrugs().begin();
        auto end = _xpertResult.getQuery()->getpParameters().getDrugs().end();
        const Query::DrugData* drugDataPtr = nullptr;
        int nbMatchingDrug = count_if(begin, end, [drugId, &drugDataPtr](const unique_ptr<Query::DrugData>& drugData) {

            if (drugData->getDrugID()  == drugId) {
                drugDataPtr = drugData.get();
                return true;
            }

            return false;
        });

        if (nbMatchingDrug != 1) {
            if (nbMatchingDrug == 0) {
                appendErrorMessage("Could not fullfil request for " + drugId + ", drug not found in query");
            } else {
                appendErrorMessage("Could not fullfil request for " + drugId + ", drug found multiple times in query");
            }
            ++nbRequestFailed;
            continue;
        }

        // Check that each Formulation and route are the same
        bool areAllFormulationAndRouteSame = true;
        vector<Core::FormulationAndRoute> queryFormulationsAndRoutes = drugDataPtr->getpTreatment().getpDosageHistory().getFormulationAndRouteList();
        for (size_t j = 0; j < queryFormulationsAndRoutes.size(); ++j) {

            // Except for the first formulation and route, if it's not equal to the previous, they are not all the same
            if (j == 0) continue;
            if (queryFormulationsAndRoutes[j].getAdministrationRoute() != queryFormulationsAndRoutes[j - 1].getAdministrationRoute() ||
                    queryFormulationsAndRoutes[j].getFormulation() != queryFormulationsAndRoutes[j - 1].getFormulation()) {
                areAllFormulationAndRouteSame = false;
                break;
            }

        }

        if (! areAllFormulationAndRouteSame) {
            appendErrorMessage("The formulations and routes are not all the same for " + drugId + ". Adjustment aborted.");
            ++nbRequestFailed;
            continue;
        }

        // Getting drug models corresponsding to the current drug id
        Tucuxi::Core::IDrugModelRepository* drugModelRepository;

        // Get the "DrugModelRepository" component from the component manager
        Tucuxi::Common::ComponentManager* pCmpMgr = Tucuxi::Common::ComponentManager::getInstance();
        drugModelRepository = pCmpMgr->getComponent<Tucuxi::Core::IDrugModelRepository>("DrugModelRepository");

        vector<Core::DrugModel*> drugModels = drugModelRepository->getDrugModelsByDrugId(drugId);

        // Get the drug model with the lowest dissimilarity score.
        unsigned bestScore = numeric_limits<unsigned>::max();
        DrugResult bestDrugResult(nullptr);

        for (Core::DrugModel* drugModel : drugModels) {

            // Checking that the drug models support the formulation and route used by the query.
            if (!queryFormulationsAndRoutes.empty()){
                bool isFormulationAndRouteSupported = false;
                const vector<unique_ptr<Core::FullFormulationAndRoute>>& modelFormulationAndRoutes = drugModel->getFormulationAndRoutes().getList();
                for (size_t k = 0; k < modelFormulationAndRoutes.size(); ++k){
                    Core::FormulationAndRoute modelFormulationAndRoute = modelFormulationAndRoutes[k]->getFormulationAndRoute();
                    if (modelFormulationAndRoute.getFormulation() == queryFormulationsAndRoutes.front().getFormulation() &&
                            modelFormulationAndRoute.getAdministrationRoute() == queryFormulationsAndRoutes.front().getAdministrationRoute() ) {
                        isFormulationAndRouteSupported = true;
                        break;
                    }
                }

                if (! isFormulationAndRouteSupported){
                    continue;
                }
            }

            // Calculating score and checking covariate at the same time
            DrugResult drugResult(drugModel);
            unsigned score = 0;
            const Core::CovariateDefinitions& modelCovariates = drugModel->getCovariates();
            for (const unique_ptr<Core::CovariateDefinition>& covariateDefinition : modelCovariates){

                string covariateId = covariateDefinition->getId();

                const Core::PatientCovariate* patientCovariatePtr = nullptr;
                int nbMatchingCovariates = count_if(patientCovariatesBegin, patientCovariatesEnd,
                                                    [covariateId, &patientCovariatePtr](const unique_ptr<Core::PatientCovariate>& patientCovariate){
                    if (patientCovariate->getId() == covariateId) {
                        patientCovariatePtr = patientCovariate.get();
                        return true;
                    }
                    return false;
                });

                // The covariate is missing in query or present multiple time
                if (nbMatchingCovariates != 1) {

                    optional<string> warning;
                    if (nbMatchingCovariates > 0) {
                        // TODO Translation
                        warning = "Multiple definitions in query replaced by model definition";
                    } else {
                        warning = nullopt;
                    }

                    CovariateResult<const Core::CovariateDefinition&> covR{CovariateSource::Model, *covariateDefinition, warning};
                    ++score;
                    drugResult.getCovariateResults().emplace(make_pair(covariateDefinition.get(), &covR));

                    // The covariate is present once in query
                } else {


                    // Same unit
                    if (patientCovariatePtr->getUnit() == covariateDefinition->getUnit()) {
                        Core::Operation* operation = covariateDefinition->getValidation();
                        Core::OperationInputList defInputList = operation->getInputs();
                        Core::OperationInputList inputList;

                        // TODO confirm that 1 input by covariate definition, but it's assumed here
                        switch (defInputList.front().getType()) {
                        case Core::InputType::BOOL: {
                            bool b;
                            istringstream(patientCovariatePtr->getValue()) >> std::boolalpha >> b;
                            inputList.push_back(Core::OperationInput(defInputList.front().getName(),b));
                        } break;
                        case Core::InputType::DOUBLE: {
                            inputList.push_back(Core::OperationInput(defInputList.front().getName(),stod(patientCovariatePtr->getValue())));
                        } break;
                        case Core::InputType::INTEGER: {
                            inputList.push_back(Core::OperationInput(defInputList.front().getName(),stoi(patientCovariatePtr->getValue())));
                        } break;
                        }

                        double result = 0;
                        if (!operation->evaluate(inputList, result)){
                            // TODO error
                        }

                        // Not respecting the domain
                        if (result == 0) {
                            // TODO Translation
                            CovariateResult<const Core::PatientCovariate*> covR{CovariateSource::Model, patientCovariatePtr, "Present in query but wrong domain"};
                            ++score;
                            drugResult.getCovariateResults().emplace(make_pair(covariateDefinition.get(), &covR));
                        }
                    } else {
                        if (unitManager.isCompatible(covariateDefinition->getUnit(), patientCovariatePtr->getUnit())) {
                            double convertedValue = unitManager.convertToUnit(stod(patientCovariatePtr->getValue()), patientCovariatePtr->getUnit(), covariateDefinition->getUnit());

                            // TODO check domain

                            CovariateResult<const Core::PatientCovariate*> covR{CovariateSource::Model, patientCovariatePtr, nullopt};
                            ++score;
                            drugResult.getCovariateResults().emplace(make_pair(covariateDefinition.get(), &covR));

                        } else {
                            // TODO Translation
                            CovariateResult<const Core::CovariateDefinition&> covR{CovariateSource::Model, *covariateDefinition, "Present in query but invalid unit"};
                            ++score;
                            drugResult.getCovariateResults().emplace(make_pair(covariateDefinition.get(), &covR));
                        }
                    }


                    // Checks if unit are compatible
                    // unitManager.isCompatible(patientCovariatePtr->getUnit(), )

                    // Validate the domain of the definition
                    // Core::Operation* operation = covariateDefinition->getValidation();
                    // Core::OperationInputList opInputList = operation->getInputs();

                }

                //                drugResult.getCovariateResults().emplace(make_pair(covariateDefinition))
                //                Core::Operation* validation = covariateDefinition->getValidation();
                //                covariateDefinition->

            }

            if (score < bestScore) {
                bestScore = score;
                bestDrugResult = drugResult;
            }

        }

        _xpertResult.getDrugResults().emplace(make_pair(xpertRequest.get(), bestDrugResult));
    }

    return Status::Ok;
}

std::string ModelSelector::getErrorMessage() const
{
    return m_errorMessage;
}

void ModelSelector::appendErrorMessage(const std::string& _errorMessage)
{
    m_errorMessage += _errorMessage + "\n";
}

} // namespace XpertResult
} // namespace Tucuxi
