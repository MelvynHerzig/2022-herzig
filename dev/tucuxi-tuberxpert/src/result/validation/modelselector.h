#ifndef MODELSELECTOR_H
#define MODELSELECTOR_H

#include <map>

#include "tucucommon/datetime.h"
#include "tucuquery/parametersdata.h"
#include "tucucore/dosage.h"

#include "../xpertrequestresult.h"

namespace Tucuxi {
namespace XpertResult {

/// \brief This is a utility class responsible to get the best drug model for a given drug.
///
/// For each available model of a given drug that corresponds to the formulation and route
/// of the dosages, we calculate a dissimilarity score S based on the covariates of the model.
/// S =∑ (missing covariate) + ∑(covariates not respecting constraints (once per covariate definition))
/// The model with the lowest dissimilarity score is chosen. In case of a tie, the model with the most covariates is chosen.
/// \date 18/05/2022
/// \author Herzig Melvyn
class BestDrugModelSelector
{
public:
    /// \brief Constructor.
    BestDrugModelSelector();

    /// \brief For a given XpertRequestResult gets the best drug model and sets
    ///        its CovariateResult vector.
    /// \param _xpertRequestResult XpertRequestResult containing the drugTreatment to be used
    ///                            and the CovariateResult vector to be set.
    void getBestDrugModel(XpertRequestResult& _xpertRequestResult) const;

protected: 

    /// \brief Checks that the patient formulations and routes are all equal.
    /// \param _dosageHistory Dosage history of the patient to be checked.
    /// \return True if all the routes are equal otherwise false.
    bool checkPatientDosageHistoryFormulationAndRoutes(const Core::DosageHistory& _dosageHistory) const;

    /// \brief Gets the oldest patient covariate date.
    /// \param _patientVariates List of patient covariates.
    /// \return Date and time of the oldest covariate. Now if no covariates.
    Common::DateTime getOldestCovariateDateTime(const Core::PatientVariates& _patientVariates) const;

    /// \brief For a given drug model, computes its score in regards of the patient covariates.
    /// \param _patientVariates Patient covariates.
    /// \param _modelDefinitions Drug model to evaluate.
    /// \param _results Vector to store the covariates results for this model.
    /// \return The score of the model.
    /// \throw invalid_argument When the conversion between patient covariate and model
    ///        definition fails.
    unsigned computeScore(const Core::PatientVariates& _patientVariates,
                          const Core::CovariateDefinitions& _modelDefinitions,
                          std::vector<CovariateResult>& _results) const;

    /// \brief For a given operation and a value check if valid. In case of success push the given
    ///        patient covariate and definition into the results.
    /// \param _op Operation to checks.
    /// \param _val Value to use in operation.
    /// \param _definition Definition to push in result in case of operation that returns true.
    /// \param _patient Patient covariate to push in result in case of operation that returns true.
    /// \param _results Vector of covariate results to push patient and definition in case of success.
    /// \throw invalid_argument When the evaluation could not be checked because of an error.
    bool checkOperation(Core::Operation* _op,
                        double _val,
                        const Core::CovariateDefinition* _definition,
                        const Core::PatientCovariate* _patient,
                        std::vector<CovariateResult>& _results) const;
};

} // namespace XpertResult
} // namespace Tucuxi

#endif // MODELSELECTOR_H
