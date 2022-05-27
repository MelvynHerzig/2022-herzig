#ifndef COVARIATERESULT_H
#define COVARIATERESULT_H

#include <string>
#include <optional>

#include "tucucore/drugmodel/covariatedefinition.h"
#include "tucuquery/parametersdata.h"

namespace Tucuxi {
namespace XpertResult {

/// \brief Enum used to specify the source of a covariate
enum class CovariateType {
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
///        The generic value is expected to be a PatientCovariate or a CovariateDefinition.
///        They must implement the methods getValue() and getUnit()
///
/// \date 20/05/2022
/// \author Herzig Melvyn
class CovariateResult
{
public:


    CovariateResult(const Core::CovariateDefinition* _definition,
                    const Core::PatientCovariate* _patient,
                    const std::optional<std::string>& _warning);

    /// \brief Get the value (as string) of the covariate.
    /// \return Returns the value.
    std::string getValue() const;

    /// \brief Get the data's unit of measure.
    /// \return Data's unit of measure.
    Common::TucuUnit getUnit() const;

    CovariateType getType() const;

    /// \brief Get the optional warning message.
    /// \return Return a copy of the optional warning.
    std::optional<std::string> getWarning() const;

    const Core::CovariateDefinition* getDefinition() const;

protected:

    const Core::CovariateDefinition* m_definition;

    const Core::PatientCovariate* m_patient;

    /// Error to print when generating the covariate in report.
    std::optional<std::string> m_warning;
};

} // namespace XpertResult
} // namespace Tucuxi

#endif // COVARIATERESULT_H
