#include "covariateresult.h"

#include "tucucommon/utils.h"

namespace Tucuxi {
namespace XpertResult {

CovariateResult::CovariateResult(const Core::CovariateDefinition* _definition,
                                 const Core::PatientCovariate* _patient,
                                 const std::optional<std::string>& _warning):
    m_definition(_definition), m_patient(_patient), m_warning(_warning)
{}

std::string CovariateResult::getValue() const
{
    if (m_patient != nullptr) {
        return m_patient->getValue();
    }

    return Common::Utils::varToString(m_definition->getValue());
}

Common::TucuUnit CovariateResult::getUnit() const
{
    if (m_patient != nullptr) {
        return m_patient->getUnit();
    }

    return m_definition->getUnit();
}

CovariateType CovariateResult::getType() const
{
    if (m_patient != nullptr) {
        return CovariateType::Patient;
    }

    return CovariateType::Model;
}

std::optional<std::string> CovariateResult::getWarning() const
{
    return m_warning;
}

} // namespace XpertResult
} // namespace Tucuxi
