#include "drugmodelselector.h"
#include "tucucommon/componentmanager.h"
#include "tucucore/drugmodelrepository.h"

namespace Tucuxi {
namespace Xpert {

typedef Tucuxi::Core::DrugDomainConstraintsEvaluator Evaluator;


DrugModelSelector::DrugModelSelector()
{

}



int DrugModelSelector::getBestDrugModel(DrugModelResult& _bestDrugModel, const std::string& _drugID, Core::DrugTreatment* _drugTreatment)
{

    // retrieving all appropriate DrugModels
    Common::ComponentManager* pCmpMgr = Common::ComponentManager::getInstance();
    Core::IDrugModelRepository* drugModelRepository = pCmpMgr->getComponent<Core::IDrugModelRepository>("DrugModelRepository");
    std::vector<Core::DrugModel*> drugModels = drugModelRepository->getDrugModelsByDrugId (_drugID);
    int nbDrugModels = drugModels.size();
    if (nbDrugModels == 0) {
        return EXIT_FAILURE;
    }

    // checking constraints for each model
    Evaluator drugDomainConstraintsEvaluator;
    std::vector <Evaluator::Result> results;
    results.reserve (nbDrugModels);
    std::vector <std::vector <Evaluator::EvaluationResult>> allConstraintsResults (nbDrugModels);
    DateTime now = DateTime();
    DateTime end;
    end.addDays(1);

    for (int i = 0; i < nbDrugModels; i++) {
        results.push_back (drugDomainConstraintsEvaluator.evaluate (
                               *drugModels[i], *_drugTreatment, now, end, allConstraintsResults[i]));
    }

    // selecting the model with the most fully compatible constraints,
    // using partially compatible constraints to settle in case of equality.
    int bestPos = -1;
    int bestNbCompat = -1;
    int bestNbPartCompat = -1;
    for (int i = 0; i < nbDrugModels; i++) {
        if (results[i] == Evaluator::Result::Compatible) {
            int size = allConstraintsResults[i].size();
            if (size > bestNbCompat) {
                bestNbCompat = size;
                bestNbPartCompat = 0;
                bestPos = i;
            }
        }
        else if (results[i] == Evaluator::Result::PartiallyCompatible) {

            int currNbCompat = 0;
            int currNbPartCompat = 0;
            for (size_t j = 0; j < allConstraintsResults[i].size(); j++) {
                if (allConstraintsResults[i][j].m_result == Evaluator::Result::Compatible ) {
                    currNbCompat ++;
                }
                else if (allConstraintsResults[i][j].m_result == Evaluator::Result::PartiallyCompatible) {
                    currNbPartCompat ++;
                }
                else { // computation error or incompatible, abort
                    currNbCompat = -1;
                    break;
                }
            }
            if ( currNbCompat > bestNbCompat ||
               (currNbCompat == bestNbCompat && currNbPartCompat > bestNbPartCompat)) {
                bestNbCompat = currNbCompat;
                bestNbPartCompat = currNbPartCompat;
                bestPos = i;
            }
        } // else discard this model
    }
    // fail if no model fits
    if (bestPos == -1) {
        return EXIT_FAILURE;
    }

    _bestDrugModel.drugModel = drugModels[bestPos];
    _bestDrugModel.constraintsResults = allConstraintsResults [bestPos];

    return EXIT_SUCCESS;
}




}
}


