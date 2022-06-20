#include "generalxpertflowstepprovider.h"

#include "tuberxpert/result/validation/covariatevalidatorandmodelselector.h"
#include "tuberxpert/result/validation/dosevalidator.h"
#include "tuberxpert/result/validation/samplevalidator.h"
#include "tuberxpert/result/validation/targetvalidator.h"
#include "tuberxpert/result/request/adjustmenttraitcreator.h"

using namespace std;

namespace Tucuxi {
namespace XpertFlow {

GeneralXpertFlowStepProvider::GeneralXpertFlowStepProvider()
{
    m_covariateValidatorAndModelSelector = make_unique<XpertResult::CovariateValidatorAndModelSelector>();
    m_doseValidator = make_unique<XpertResult::DoseValidator>();
    m_sampleValidator = make_unique<XpertResult::SampleValidator>();
    m_targetValidator = make_unique<XpertResult::TargetValidator>();
    m_adjustmentTraitCreator = make_unique<XpertResult::AdjustmentTraitCreator>();
}

const std::unique_ptr<AbstractXpertFlowStep>& GeneralXpertFlowStepProvider::getCovariateValidatorAndModelSelector() const
{
    return m_covariateValidatorAndModelSelector;
}

const std::unique_ptr<AbstractXpertFlowStep>& GeneralXpertFlowStepProvider::getDoseValidator() const
{
    return m_doseValidator;
}

const std::unique_ptr<AbstractXpertFlowStep>& GeneralXpertFlowStepProvider::getSampleValidator() const
{
    return m_sampleValidator;
}

const std::unique_ptr<AbstractXpertFlowStep>& GeneralXpertFlowStepProvider::getTargetValidator() const
{
    return m_targetValidator;
}

const std::unique_ptr<AbstractXpertFlowStep>& GeneralXpertFlowStepProvider::getAdjustmentTraitCreator() const
{
    return m_adjustmentTraitCreator;
}

const std::unique_ptr<AbstractXpertFlowStep>& GeneralXpertFlowStepProvider::getReportPrinter() const
{
    return m_reportPrinter;
}

} // namespace XpertFlow
} // namespace Tucuxi
