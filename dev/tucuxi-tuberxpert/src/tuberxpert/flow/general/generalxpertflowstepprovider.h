#ifndef GENERALXPERTFLOWSTEPPROVIDER_H
#define GENERALXPERTFLOWSTEPPROVIDER_H

#include <memory>

#include "tuberxpert/flow/abstract/abstractxpertflowstepprovider.h"

namespace Tucuxi {
namespace Xpert {

/// \brief General provider of xpert flow step (generically for the drugs).
///        It provides instances of:
///        - 1) CovariateValidatorAndModelSelector
///        - 2) DosageValidator
///        - 3) SampleValidator
///        - 4) TargetValidator
///        - 5) AdjustmentTraitCreator
///        - 6) Execute the requests
///        - 7) Print the final report
///
/// \date 20/06/2022
/// \author Herzig Melvyn
class GeneralXpertFlowStepProvider : public AbstractXpertFlowStepProvider
{
public:

    /// \brief Constructor. Set the general flow steps (mainly generic/non-drug specific).
    GeneralXpertFlowStepProvider();

    /// \brief Get the step responsible for covariate validation and drug model selection.
    /// \return An instance of CovariateValidatorAndModelSelector.
    const std::unique_ptr<AbstractXpertFlowStep>& getCovariateValidatorAndModelSelector() const override;

    /// \brief Get the step responsible for doses validation.
    /// \return An instance of DoseValidator.
    const std::unique_ptr<AbstractXpertFlowStep>& getDoseValidator() const override;

    /// \brief Get the step responsible for sample validation.
    /// \return An instance of SampleValidator.
    const std::unique_ptr<AbstractXpertFlowStep>& getSampleValidator() const override;

    /// \brief Get the step responsible for target validation.
    /// \return An instance of TargetValidator.
    const std::unique_ptr<AbstractXpertFlowStep>& getTargetValidator() const override;

    /// \brief Get the step responsible for adjustment trait creation.
    /// \return An instance of AdjustmentTraitCreator.
    const std::unique_ptr<AbstractXpertFlowStep>& getAdjustmentTraitCreator() const override;

    /// \brief Get the step responsible for request execution.
    /// \return An instance of RequestExecutor.
    const std::unique_ptr<AbstractXpertFlowStep>& getRequestExecutor() const override;

    /// \brief Get the step responsible for report generation.
    /// \return An instance of ReportPrinter.
    const std::unique_ptr<AbstractXpertFlowStep>& getReportPrinter() const override;

};

} // namespace Xpert
} // namespace Tucuxi

#endif // GENERALXPERTFLOWSTEPPROVIDER_H
