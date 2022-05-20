#include "modelselector.h"

#include <vector>
#include <limits>

#include "tucucore/dosage.h"
#include "tucucore/drugmodelrepository.h"
#include "tucucore/drugmodel/drugmodel.h"

using namespace std;

namespace Tucuxi {
namespace XpertResult {

ModelSelector::ModelSelector(const string& _drugsFolder) : m_drugsFolder(_drugsFolder)
{}

ModelSelector::Status ModelSelector::getBestModelForQueryDrugs(const XpertQuery::XpertQueryData& _query, map<Query::DrugData*, string>& _modelIdPerDrug)
{
    size_t nbDrugFailed = 0;

    // For each drug.
    const vector<unique_ptr<Query::DrugData>>& queryDrugs = _query.getpParameters().getDrugs();
    for(size_t i = 0; i < queryDrugs.size(); ++i) {

        // Check that each Formulation and route are the same
        bool areAllFormulationAndRouteSame = true;
        vector<Core::FormulationAndRoute> queryFormulationsAndRoutes = queryDrugs[i]->getpTreatment().getpDosageHistory().getFormulationAndRouteList();
        for(size_t j = 0; j < queryFormulationsAndRoutes.size(); ++j) {

            // Except for the first formulation and route, if it's not equal to the previous, they are not all the same
            if(j == 0) continue;
            if (queryFormulationsAndRoutes[j].getAdministrationRoute() != queryFormulationsAndRoutes[j - 1].getAdministrationRoute() ||
                queryFormulationsAndRoutes[j].getFormulation() != queryFormulationsAndRoutes[j - 1].getFormulation()) {
                areAllFormulationAndRouteSame = false;
                break;
            }

        }

        if(!areAllFormulationAndRouteSame) {
            appendErrorMessage("The formulations and routes are not all the same for " + queryDrugs[i]->getDrugID() + ". Adjustment aborted.");
            ++nbDrugFailed;
            break;
        }

        // Getting drug models corresponsding to the current drug id
        Core::DrugModelRepository drugModelRepository;
        drugModelRepository.addFolderPath(m_drugsFolder);
        vector<Core::DrugModel*> drugModels = drugModelRepository.getDrugModelsByDrugId(queryDrugs[i]->getDrugID());

        // Get the drug model with the lowest dissimilarity score.
        unsigned bestScore = numeric_limits<unsigned>::max();
        string bestDrugModelId = "";
        for(size_t j = 0; j < drugModels.size(); ++j) {

            //Checking that the drug models support the formulation and route used by the query.
            if(!queryFormulationsAndRoutes.empty()){
                bool isFormulationAndRouteSupported = false;
                const vector<unique_ptr<Core::FullFormulationAndRoute>>& modelFormulationAndRoutes = drugModels[i]->getFormulationAndRoutes().getList();
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

            // Calculate score.
            unsigned drugModelScore = 0;

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
