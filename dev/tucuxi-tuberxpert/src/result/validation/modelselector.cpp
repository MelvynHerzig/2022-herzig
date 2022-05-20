#include "modelselector.h"

#include <vector>
#include <limits>
#include <optional>
#include <algorithm>

#include "tucucore/dosage.h"
#include "tucucore/drugmodelrepository.h"
#include "tucucore/drugmodel/drugmodel.h"

using namespace std;

namespace Tucuxi {
namespace XpertResult {

ModelSelector::ModelSelector(const string& _drugsFolder) : m_drugsFolder(_drugsFolder)
{}

ModelSelector::Status ModelSelector::getBestModelForQueryDrugs(XpertResult& _xpertResult)
{

    size_t nbRequestFailed = 0;

    auto patientCovariatesBegin = _xpertResult.getQuery()->getpParameters().getpPatient().getCovariates().begin();
    auto patientCovariatesEnd = _xpertResult.getQuery()->getpParameters().getpPatient().getCovariates().end();

    // Only handle the drug which are required by request xpert.
    for(const unique_ptr<XpertQuery::XpertRequestData>& xpertRequest : _xpertResult.getQuery()->getXpertRequests()) {
        string drugId = xpertRequest->getDrugID();

        //Find drug data corresponding to the request.
        auto begin = _xpertResult.getQuery()->getpParameters().getDrugs().begin();
        auto end = _xpertResult.getQuery()->getpParameters().getDrugs().end();
        int nbMatchingDrug = count_if(begin, end, [drugId](const unique_ptr<Query::DrugData>& drugData) {
            return drugData->getDrugID() == drugId;
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

        const unique_ptr<Query::DrugData>& drugData = *find_if(begin, end, [drugId](const unique_ptr<Query::DrugData>& drugData) {
            return drugData->getDrugID() == drugId;
        });

        // Check that each Formulation and route are the same
        bool areAllFormulationAndRouteSame = true;
        vector<Core::FormulationAndRoute> queryFormulationsAndRoutes = drugData->getpTreatment().getpDosageHistory().getFormulationAndRouteList();
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
        Core::DrugModelRepository drugModelRepository;
        drugModelRepository.addFolderPath(m_drugsFolder);
        vector<Core::DrugModel*> drugModels = drugModelRepository.getDrugModelsByDrugId(drugId);

        // Get the drug model with the lowest dissimilarity score.
        unsigned bestScore = numeric_limits<unsigned>::max();
        DrugResult bestDrugResult("");

        for (Core::DrugModel* drugModel : drugModels) {

            // Checking that the drug models support the formulation and route used by the query.
            if(!queryFormulationsAndRoutes.empty()){
                bool isFormulationAndRouteSupported = false;
                const vector<unique_ptr<Core::FullFormulationAndRoute>>& modelFormulationAndRoutes = drugModel->getFormulationAndRoutes().getList();
                for(size_t k = 0; k < modelFormulationAndRoutes.size(); ++k){
                    Core::FormulationAndRoute modelFormulationAndRoute = modelFormulationAndRoutes[k]->getFormulationAndRoute();
                    if(modelFormulationAndRoute.getFormulation() == queryFormulationsAndRoutes.front().getFormulation() &&
                       modelFormulationAndRoute.getAdministrationRoute() == queryFormulationsAndRoutes.front().getAdministrationRoute() ) {
                        isFormulationAndRouteSupported = true;
                        break;
                    }
                }

                if(!isFormulationAndRouteSupported){
                    continue;
                }
            }

            // Calculating score and checking covariate at the same time
            DrugResult drugResult(drugModel->getDrugModelId());
            unsigned score = 0;
            const Core::CovariateDefinitions& modelCovariates = drugModel->getCovariates();
            for (const unique_ptr<Core::CovariateDefinition>& covariateDefinition : modelCovariates){

                string covariateId = covariateDefinition->getId();
                auto it = find_if(patientCovariatesBegin, patientCovariatesEnd, [covariateId](const unique_ptr<Core::PatientCovariate>& patientCovariate){
                   return patientCovariate->getId() == covariateId;
                });

                // The covariate is missing in query
                if (it == patientCovariatesEnd) {
                    CovariateResult<const Core::CovariateDefinition&> covR{CovariateSource::Model, *covariateDefinition, nullopt};
                    ++score;
                    drugResult.getCovariateResults().emplace(make_pair(covariateDefinition.get(), &covR));
                } else {

                }

//                drugResult.getCovariateResults().emplace(make_pair(covariateDefinition))
//                Core::Operation* validation = covariateDefinition->getValidation();
//                covariateDefinition->

            }

        }
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
