#include "covariateresult.h"

namespace Tucuxi {
namespace XpertResult {

CovariateResult::CovariateResult(const Core::CovariateDefinition*_source, CovariateType _type, std::optional<std::string>& _warning) :
m_sourceFromModel(_source), m_sourceFromPatient(nullptr), m_type(_type), m_warning(_warning)
{}

CovariateResult::CovariateResult(const Core::PatientCovariate*_source, CovariateType _type, std::optional<std::string>& _warning) :
 m_sourceFromModel(nullptr), m_sourceFromPatient(_source), m_type(_type), m_warning(_warning)
{}

std::string CovariateResult::getValue() const
{
    if (m_sourceFromModel == nullptr) {
        return m_sourceFromPatient->getValue();
    } else {
        return Common::Utils::varToString(m_sourceFromModel->getValue());
    }
}

Common::TucuUnit CovariateResult::getUnit() const
{
    if (m_sourceFromModel == nullptr) {
        return m_sourceFromPatient->getUnit();
    } else {
        return m_sourceFromModel->getUnit();
    }
}

CovariateType CovariateResult::getType() const
{
    return m_type;
}

std::optional<std::string> CovariateResult::getWarning() const
{
    return m_warning;
}






} // namespace XpertResult
} // namespace Tucuxi
