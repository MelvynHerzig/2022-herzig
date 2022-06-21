#include "covariateresult.h"

#include "tucucommon/utils.h"

using namespace std;

namespace Tucuxi {
namespace Xpert {

CovariateResult::CovariateResult(const Core::CovariateDefinition* _definition,
                                 const Core::PatientCovariate* _patient,
                                 const string& _warning):
    AbstractResult<Core::CovariateDefinition>(_definition, _warning), m_patient(_patient)
{}

std::string CovariateResult::getValue() const
{
    if (m_patient != nullptr) {
        return m_patient->getValue();
    }

    return Common::Utils::varToString(m_source->getValue());
}

Common::TucuUnit CovariateResult::getUnit() const
{
    if (m_patient != nullptr) {
        return m_patient->getUnit();
    }

    return m_source->getUnit();
}

CovariateType CovariateResult::getType() const
{
    if (m_patient != nullptr) {
        return CovariateType::Patient;
    }

    return CovariateType::Model;
}


const Core::PatientCovariate *CovariateResult::getPatient() const
{
    return m_patient;
}

} // namespace Xpert
} // namespace Tucuxi
