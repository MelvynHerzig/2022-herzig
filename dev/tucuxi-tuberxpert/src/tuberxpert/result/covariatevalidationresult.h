#ifndef COVARIATEVALIDATIONRESULT_H
#define COVARIATEVALIDATIONRESULT_H

#include <string>

#include "tucucore/drugmodel/covariatedefinition.h"
#include "tucuquery/parametersdata.h"

#include "tuberxpert/result/abstractvalidationresult.h"

namespace Tucuxi {
namespace Xpert {

/// \brief Enum used to specify the source of a covariate.
enum class CovariateType {
    Patient, /**< The covariate value comes from the query. It's the patient's value. */
    Model    /**< The covariate value comes from the drug model. This is the default value. */
};

/// \brief This class stores the validation result for a patient covariate.
///        This class is created during the "covariate validator and model selector"
///        phase. It's purpose is to tell what the computing core will receive:
///
///         - The value of the drugmodel.
///              or
///         - The patient value.
///
///         When the patient value is used, it is possible to set a
///         warning if the validation of the covariate definition is not respected.
/// \date 20/05/2022
/// \author Herzig Melvyn
class CovariateValidationResult : public AbstractValidationResult<Core::CovariateDefinition>
{
public:

    /// \brief Constructor.
    /// \param _definition Definition of the covariate in the drug model. Should never be nullptr.
    ///                    The definition must have at least the same lifetime as this object.
    /// \param _patient Patient-related covariate, if present. May be nullptr, if not defined.
    ///                 The patient-related covariate must have at least the same lifetime as this object.
    /// \param _warning Associated warning message.
    CovariateValidationResult(const Core::CovariateDefinition* _definition,
                    const Core::PatientCovariate* _patient,
                    const std::string& _warning);

    // Getters

    /// \brief Get the value of the covariate. If the patient-related covariate is defined,
    ///        it is used, otherwise it returns the value of the definition.
    /// \return The value.
    std::string getValue() const;

    /// \brief Get the data type of the covariate value.
    /// \return The data type.
    Core::DataType getDataType() const;

    /// \brief Get the unit of measurement of the value. If the patient is defined,
    ///        it uses its unit, otherwise it uses the definition.
    /// \return The measurement unit.
    Common::TucuUnit getUnit() const;

    /// \brief Get where the value, the unit and the data type come from.
    /// \return CovariateType::Patient if it comes from the patient,
    ///         otherwise CovariateType::Model
    CovariateType getType() const;


    /// \brief Get the pointer on the patient covariate that overrides the definition.
    ///        The pointer returned by this method must not to be deleted.
    /// \return The pointer on the patient's covariate if it exists, otherwise nullptr.
    const Core::PatientCovariate* getPatient() const;


protected:

    /// \brief The possible patient's covariate that overrides the definition.
    const Core::PatientCovariate* m_patient;

};

} // namespace Xpert
} // namespace Tucuxi

#endif // COVARIATEVALIDATIONRESULT_H
