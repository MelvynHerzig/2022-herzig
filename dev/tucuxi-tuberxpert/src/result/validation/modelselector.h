#ifndef MODELSELECTOR_H
#define MODELSELECTOR_H

#include <map>

#include "tucuquery/parametersdata.h"

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
class ModelSelector
{
public:


    /// \brief ModelSelector constructor.
    ModelSelector();

    /// \brief Gets the best drug model for each drug requested by a request xpert in the query.
    /// \param _xpertResult Xpert result containing the query to parse and the decisions.
    /// \return Returns a Status depending on the execution.
    bool getBestDrugModel(const std::unique_ptr<XpertQuery::XpertRequestData>& _xpertRequest, XpertResult& _xpertResult);

    /// \brief Gets the error message describing error that could happen during getBestModelForQueryDrug.
    /// \return Returns the error message as a string.
    std::string getErrorMessage() const;


protected:

    /// \brief Append an error message to existing error message.
    /// \param _errorMessage Error message to append.
    void setErrorMessage(const std::string& _errorMessage);

    const Query::DrugData* extractDrugData(const std::unique_ptr<XpertQuery::XpertRequestData>& _xpertRequest, const std::unique_ptr<XpertQuery::XpertQueryData>& _xpertQuery);

    bool computeDrugModelScore(const Core::DrugModel* _drugModel, const Query::DrugData* _drugData, const std::vector<std::unique_ptr<Core::PatientCovariate>>& _patientCovariates, std::map<Core::CovariateDefinition*, CovariateResult>& _covariateResults, unsigned& _score);

    bool isFormulationAndRouteSupportedByDrugModel(const Core::DrugModel* _drugModel, const Query::DrugData* _drugData);

    void createCovariateResultFromPatient(const std::unique_ptr<Core::PatientCovariate>& _patientCovariate, const Core::CovariateDefinition* _covariateDefinition, std::map<Core::CovariateDefinition*, CovariateResult>& _covariateResults, double& _score);


protected:

    /// Error message of last getBestModelForQueryDrugs execution.
    std::string m_errorMessage;
};

} // namespace XpertResult
} // namespace Tucuxi

#endif // MODELSELECTOR_H
