#ifndef GENERALXPERTFLOWSTEPPROVIDER_H
#define GENERALXPERTFLOWSTEPPROVIDER_H

#include <memory>

#include "tuberxpert/flow/abstract/abstractxpertflowstepprovider.h"

namespace Tucuxi {
namespace XpertFlow {

class GeneralXpertFlowStepProvider : public AbstractXpertFlowStepProvider
{
public:

    /// \brief Constructor. Set general steps (that are not specific to a drug).
    GeneralXpertFlowStepProvider();

    /// \brief Get the step responsible to validate the covariates and to select the drug model.
    /// \return Return the corresponding AbstractXpertFlowStep.
    const std::unique_ptr<AbstractXpertFlowStep>& getCovariateValidatorAndModelSelector() const override;

    /// \brief Get the step responsible to validate the doses.
    /// \return Return the corresponding AbstractXpertFlowStep.
    const std::unique_ptr<AbstractXpertFlowStep>& getDoseValidator() const override;

    /// \brief Get the step responsible to validate the samples.
    /// \return Return the corresponding AbstractXpertFlowStep.
    const std::unique_ptr<AbstractXpertFlowStep>& getSampleValidator() const override;

    /// \brief Get the step responsible to validate the targets.
    /// \return Return the corresponding AbstractXpertFlowStep.
    const std::unique_ptr<AbstractXpertFlowStep>& getTargetValidator() const override;

    /// \brief Get the step responsible to create the adjustment trait.
    /// \return Return the corresponding AbstractXpertFlowStep.
    const std::unique_ptr<AbstractXpertFlowStep>& getAdjustmentTraitCreator() const override;

    /// \brief Get the step responsible to print the report.
    /// \return Return the corresponding AbstractXpertFlowStep.
    const std::unique_ptr<AbstractXpertFlowStep>& getReportPrinter() const override;



};

} // namespace XpertFlow
} // namespace Tucuxi

#endif // GENERALXPERTFLOWSTEPPROVIDER_H
