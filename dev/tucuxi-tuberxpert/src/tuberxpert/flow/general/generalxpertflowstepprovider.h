#ifndef GENERALXPERTFLOWSTEPPROVIDER_H
#define GENERALXPERTFLOWSTEPPROVIDER_H

#include <memory>

#include "tuberxpert/flow/abstract/abstractxpertflowstepprovider.h"

namespace Tucuxi {
namespace Xpert {

/// \brief General xpert flow step providers (in a generic manner for the drug)
///        It provides instances of:
///        1) CovariateValidatorAndModelSelector
///        2) DosageValidator
///        3) SampleValidator
///        4) TargetValidator
///        5) AdjustmentTraitCreator
///        6) ReportPrinter
///
/// \date 20/06/2022
/// \author Herzig Melvyn
class GeneralXpertFlowStepProvider : public AbstractXpertFlowStepProvider
{
public:

    /// \brief Constructor. Set general steps (that are not specific to a drug).
    /// \param _computationTime This attribute is used for testing purpose. It specifies "when is
    ///        the class executed". It allows to get the same timings when executed
    ///        at different times.
    GeneralXpertFlowStepProvider(Common::DateTime _computationTime = Common::DateTime::now());

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

} // namespace Xpert
} // namespace Tucuxi

#endif // GENERALXPERTFLOWSTEPPROVIDER_H
