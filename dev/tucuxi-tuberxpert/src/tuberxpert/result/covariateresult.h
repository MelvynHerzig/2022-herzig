#ifndef COVARIATERESULT_H
#define COVARIATERESULT_H

#include <string>

#include "tucucore/drugmodel/covariatedefinition.h"
#include "tucuquery/parametersdata.h"

#include "tuberxpert/result/abstractresult.h"

namespace Tucuxi {
namespace XpertResult {

/// \brief Enum used to specify the source of a covariate
enum class CovariateType {
    Patient,
    Model
};

/// \brief This class stores results for patients covariates.
///
///        This class is created during the ModelSelector phase. It aims to
///        tell what the computing core will receive:
///
///         - The value from the drugmodel
///              or
///         - The value from the patient + the drug model definition associated.
///
///         When the value from the patient is used, it is possible to set a
///         warning if the covariateDefinition validation is not respected.
/// \date 20/05/2022
/// \author Herzig Melvyn
class CovariateResult : public AbstractResult<Core::CovariateDefinition>
{
public:

    /// \brief Constructor. Used in ModelSelector.
    /// \param _definition. Associated drug model definition.
    /// \param _patient Related patient covariate if present.
    /// \param _warning Warning noticed.
    CovariateResult(const Core::CovariateDefinition* _definition,
                    const Core::PatientCovariate* _patient,
                    const std::string& _warning);

    /// \brief Gets the value (as string) of the covariate.
    ///        If the patient is set, use it otherwise returns
    ///        the value from the definition.
    /// \return The value.
    std::string getValue() const;

    /// \brief Gets the data's unit of measure.
    ///        If the patient is set, use it otherwise returns
    ///        the unit from the definition.
    /// \return Data's unit of measure.
    Common::TucuUnit getUnit() const;

    /// \brief Gets information about the source of getUnit and getValue
    /// \return Return CovariateType::Patient if patient is not nullptr
    ///         otherwise CovariateType::Model
    CovariateType getType() const;


    /// \brief Gets the possible patientCovariate that override the definition.
    /// \return The patient covariate. May be nullptr.
    const Core::PatientCovariate* getPatient() const;


protected:

    /// \brief The possible patientCovariate that override the definition.
    const Core::PatientCovariate* m_patient;

};

} // namespace XpertResult
} // namespace Tucuxi

#endif // COVARIATERESULT_H
