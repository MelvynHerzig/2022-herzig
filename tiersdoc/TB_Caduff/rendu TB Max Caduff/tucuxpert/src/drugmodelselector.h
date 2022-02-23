#ifndef DRUGMODELSELECTOR_H
#define DRUGMODELSELECTOR_H

#include "tucucore/drugdomainconstraintsevaluator.h"
#include "tucuquery/querydata.h"

namespace Tucuxi {
namespace Xpert {

typedef struct {
    std::vector <Core::DrugDomainConstraintsEvaluator::EvaluationResult> constraintsResults;
    Core::DrugModel* drugModel = nullptr;
} DrugModelResult;

class DrugModelSelector
{
public:
    DrugModelSelector();

    int getBestDrugModel (DrugModelResult& _bestDrugModel, const std::string& _drugID, Core::DrugTreatment* _drugTreatment);
};



}
}

#endif // DRUGMODELSELECTOR_H
