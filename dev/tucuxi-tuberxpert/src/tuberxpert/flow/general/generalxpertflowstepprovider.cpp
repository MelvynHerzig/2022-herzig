#include "generalxpertflowstepprovider.h"

#include "tuberxpert/flow/general/covariatevalidatorandmodelselector.h"
#include "tuberxpert/flow/general/dosevalidator.h"
#include "tuberxpert/flow/general/samplevalidator.h"
#include "tuberxpert/flow/general/targetvalidator.h"
#include "tuberxpert/flow/general/adjustmenttraitcreator.h"
#include "tuberxpert/flow/general/reportprinter.h"

using namespace std;

namespace Tucuxi {
namespace Xpert {

GeneralXpertFlowStepProvider::GeneralXpertFlowStepProvider()
{
    m_covariateValidatorAndModelSelector = make_unique<CovariateValidatorAndModelSelector>();
    m_doseValidator = make_unique<DoseValidator>();
    m_sampleValidator = make_unique<SampleValidator>();
    m_targetValidator = make_unique<TargetValidator>();
    m_adjustmentTraitCreator = make_unique<AdjustmentTraitCreator>();
    m_reportPrinter = make_unique<ReportPrinter>();
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

} // namespace Xpert
} // namespace Tucuxi
