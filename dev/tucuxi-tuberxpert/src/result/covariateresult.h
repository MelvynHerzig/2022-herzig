#ifndef COVARIATERESULT_H
#define COVARIATERESULT_H

#include <string>

namespace Tucuxi {
namespace XpertResult {


/// \brief Enum used to specify the source of a covariate
enum class CovariateSource {
    Patient,
    Model
};

/// \brief This is a wrapper class that stores the information about the
///        covariates of drug considering a drug model.
///
///        Basically this class is used in a DrugResult which is associated to a drug model.
///        A DrugResult contains a list of covariate that corresponds to the definitions located
///        in the associated drug model. Consequently, it contains a map [CovariateDefinition, CovariateResult]
///        that tells for each definitions what is the value used, the unit and the source that are stored in
///        this class.
///
///        A patient coavriate might be invalid (not respecting the domain of the definition), so it could
///        contain an optional error message.
///
/// \date 20/05/2022
/// \author Herzig Melvyn
class CovariateResult
{
public:

    /// \brief Create a covariate result for covariate definition.
    /// \param value Value used for computation
    /// \param unit Unit used.
    /// \param source Source of the covariate.
    /// \param error Optional error message
    CovariateResult(const std::string& value, const std::string& unit, CovariateSource source, const std::string& error = "");
};

} // namespace XpertResult
} // namespace Tucuxi

#endif // COVARIATERESULT_H
