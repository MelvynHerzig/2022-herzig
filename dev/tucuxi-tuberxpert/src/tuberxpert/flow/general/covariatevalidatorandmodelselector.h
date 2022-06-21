#ifndef COVARIATEVALIDATORANDMODELSELECTOR_H
#define COVARIATEVALIDATORANDMODELSELECTOR_H

#include <map>

#include "tucucommon/datetime.h"
#include "tucuquery/parametersdata.h"
#include "tucucore/dosage.h"

#include "tuberxpert/flow/abstract/abstractxpertflowstep.h"
#include "tuberxpert/query/xpertrequestdata.h"
#include "tuberxpert/result/xpertrequestresult.h"

namespace Tucuxi {
namespace XpertFlow {

/// \brief This is a utility class responsible to get the best drug model for a given drug.
///
/// For each available model of a given drug that corresponds to the formulation and route
/// of the dosages, we calculate a dissimilarity score S based on the covariates of the model.
/// S =∑ (missing covariate) + ∑(covariates not respecting constraints (once per covariate definition))
/// The model with the lowest dissimilarity score is chosen. In case of a tie, the model with the most covariates is chosen.
///
/// The selected drug model must support the request language. If it doesn't, the XpertRequestResult
/// gets its error message set and is not handled anymore.
///
/// \date 18/05/2022
/// \author Herzig Melvyn
class CovariateValidatorAndModelSelector : public XpertFlow::AbstractXpertFlowStep
{
public:
    /// \brief Constructor.
    /// \param _computationDate This attribute is used for testing purpose. It specifies "when is
    ///        the selector executed". For example, it allows to get the same age when executed
    ///        at different times.
    CovariateValidatorAndModelSelector(Common::DateTime _computationDate = Common::DateTime::now());

    /// \brief For a given XpertRequestResult gets the best drug model and sets
    ///        its CovariateResult vector.
    /// \param _xpertRequestResult XpertRequestResult containing the drugTreatment to be used
    ///                            and the CovariateResult vector to be set.
    void perform(XpertResult::XpertRequestResult& _xpertRequestResult) const;

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
    /// \param _lang Request's language to get the good translation of the error message from the definition
    ///              when populating _results.
    /// \param _results Vector to store the covariates results for this model.
    /// \return The score of the model.
    /// \throw invalid_argument When the conversion between patient covariate and model
    ///        definition fails or if a definition does not support english nor the requested language.
    unsigned computeScore(const Core::PatientVariates& _patientVariates,
                          const Core::CovariateDefinitions& _modelDefinitions,
                          XpertQuery::OutputLang _lang,
                          std::vector<XpertResult::CovariateResult>& _results) const;

    /// \brief For a given operation and a value check if valid. In case of success push the given
    ///        patient covariate and definition into the results.
    /// \param _op Operation to checks.
    /// \param _val Value to use in operation.
    /// \param _definition Definition to push in result in case of operation that returns true.
    /// \param _patient Patient covariate to push in result in case of operation that returns true.
    /// \param _lang Request's language to get the good translation of the error message from the definition.
    /// \param _results Vector of covariate results to push patient and definition in case of success.
    /// \throw invalid_argument When the evaluation could not be checked because of an error.
    bool checkOperation(Core::Operation* _op,
                        double _val,
                        const Core::CovariateDefinition* _definition,
                        const Core::PatientCovariate* _patient,
                        XpertQuery::OutputLang _lang,
                        std::vector<XpertResult::CovariateResult>& _results) const;

    /// \brief For a given series of covariate definitions. This method checks that they
    ///        all support the requested output lang or at least english.
    /// \param _modelDefinitions List of covariate definition to check.
    /// \param _lang Output lang desired.
    /// \return Return true if english or _lang is supported otherwise false.
    bool checkCovariateDefinitionsLanguage(const Core::CovariateDefinitions& _modelDefinitions, XpertQuery::OutputLang _lang) const;

protected:

    /// \brief Fixes the computation date to get the same values when testing
    ///        at different times.
    Common::DateTime m_computationDate;
};

} // namespace XpertFlow
} // namespace Tucuxi

#endif // COVARIATEVALIDATORANDMODELSELECTOR_H
