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

    /// \brief Possible return value of the drug model selection
    /// Ok: All good.
    /// PartiallyMissingModels: Some drugs could not get attributed a model.
    /// TotallyMissingModels: All the drugs could not get attributed a model.
    enum class Status {
        Ok = 0,
        PartiallyMissingModels,
        TotallyMissingModels
    };

    /// \brief ModelSelector constructor.
    ModelSelector();

    /// \brief Gets the best drug model for each drug requested by a request xpert in the query.
    /// \param _xpertResult Xpert result containing the query to parse and the decisions.
    /// \return Returns a Status depending on the execution.
    Status getBestModelForQueryDrugs(XpertResult& _xpertResult);

    /// \brief Gets the error message describing error that could happen during getBestModelForQueryDrug.
    /// \return Returns the error message as a string.
    std::string getErrorMessage() const;

protected:

    /// \brief Append an error message to existing error message.
    /// \param _errorMessage Error message to append.
    void appendErrorMessage(const std::string& _errorMessage);

protected:

    /// Execution status.
    Status m_status{Status::Ok};

    /// Error message of last getBestModelForQueryDrugs execution.
    std::string m_errorMessage;
};

} // namespace XpertResult
} // namespace Tucuxi

#endif // MODELSELECTOR_H
