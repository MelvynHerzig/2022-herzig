#ifndef MODELSELECTOR_H
#define MODELSELECTOR_H

#include <map>

#include "tucucommon/datetime.h"
#include "tucuquery/parametersdata.h"
#include "tucucore/dosage.h"

#include "../../query/xpertquerydata.h"
#include "../xpertresult.h"

namespace Tucuxi {
namespace XpertResult {

/// \brief This is a utility class responsible to get the best drug model for a given drug.
///
/// For each available model of a given drug that corresponds to the formulation and route
/// of the dosages, we calculate a dissimilarity score S based on the covariates of the model.
/// S =∑ (missing covariate) + ∑(covariates not respecting constraints)
/// The model with the lowest dissimilarity score is chosen. In case of a tie, the model with the most covariates is chosen.
/// \date 18/05/2022
/// \author Herzig Melvyn
class BestDrugModelSelector
{
public:


    /// \brief ModelSelector constructor.
    BestDrugModelSelector();

    void getBestDrugModel(XpertRequestResult& _xpertRequestResult) const;

protected: 

    bool checkPatientDosageHistoryFormulationAndRoutes(const Core::DosageHistory& _dosageHistory) const;

    Common::DateTime getOldestCovariateDateTime(const Core::PatientVariates& _patientVariates) const;

    unsigned computeScore(const Core::PatientVariates& _patientVariates,
                          const Core::CovariateDefinitions& _modelDefinitions,
                          std::vector<CovariateResult>& _results) const;

    bool checkOperation(Core::Operation* _op,
                        double _val,
                        const Core::CovariateDefinition* _definition,
                        const Core::PatientCovariate* _patient,
                        std::vector<CovariateResult>& _results) const;
};

} // namespace XpertResult
} // namespace Tucuxi

#endif // MODELSELECTOR_H
