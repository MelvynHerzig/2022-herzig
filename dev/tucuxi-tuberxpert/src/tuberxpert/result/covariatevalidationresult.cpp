#include "covariatevalidationresult.h"

#include "tucucommon/utils.h"

using namespace std;

namespace Tucuxi {
namespace Xpert {

CovariateValidationResult::CovariateValidationResult(const Core::CovariateDefinition* _definition,
                                 const Core::PatientCovariate* _patient,
                                 const string& _warning):
    AbstractValidationResult<Core::CovariateDefinition>(_definition, _warning), m_patient(_patient)
{}

std::string CovariateValidationResult::getValue() const
{
    if (m_patient != nullptr) {
        return m_patient->getValue();
    }

    return Common::Utils::varToString(m_source->getValue());
}

Core::DataType CovariateValidationResult::getDataType() const
{
    if (m_patient != nullptr) {
        return m_patient->getDataType();
    }

    return m_source->getDataType();
}

Common::TucuUnit CovariateValidationResult::getUnit() const
{
    if (m_patient != nullptr) {
        return m_patient->getUnit();
    }

    return m_source->getUnit();
}

CovariateType CovariateValidationResult::getType() const
{
    if (m_patient != nullptr) {
        return CovariateType::Patient;
    }

    return CovariateType::Model;
}


const Core::PatientCovariate* CovariateValidationResult::getPatient() const
{
    return m_patient;
}

} // namespace Xpert
} // namespace Tucuxi
