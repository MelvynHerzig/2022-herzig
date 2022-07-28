#ifndef ABSTRACTXPERTFLOWSTEPPROVIDER_H
#define ABSTRACTXPERTFLOWSTEPPROVIDER_H

#include <memory>

#include "tuberxpert/flow/abstract/abstractxpertflowstep.h"

namespace Tucuxi {
namespace Xpert {

/// \brief Abstract class that must be implemented by xpert flow step providers.
///        An xpert flow step provider is an object responsible for providing
///        the execution steps for a given drug. An execution step is:
///        - 1) Validate the covariates and select drug model
///        - 2) Validate the doses
///        - 3) Validate the samples
///        - 4) Validate the targets
///        - 5) Create the adjustment trait
///        - 6) Execute the requests
///        - 7) Print the treatment adjustment report
///
///        A flow step is an implementation of the AbstractXpertFlowStep class.
///
///        When necessary, this factory system allows to be able to offer a specific
///        implementation for a drug: ImatinibXpertFlowStepProvider, RifampicinXpertFlowStepProvider...
///
/// \date 20/06/2022
/// \author Herzig Melvyn
class AbstractXpertFlowStepProvider
{

public:

    /// \brief Constructor. Simply set members to nullptr.
    AbstractXpertFlowStepProvider() :
        m_covariateValidatorAndModelSelector(nullptr),
        m_doseValidator(nullptr),
        m_sampleValidator(nullptr),
        m_targetValidator(nullptr),
        m_adjustmentTraitCreator(nullptr),
        m_requestExecutor(nullptr),
        m_reportPrinter(nullptr){};

    /// \brief Destructor.
    virtual ~AbstractXpertFlowStepProvider() {};

    /// \brief Get the step responsible for covariate validation and drug model selection.
    /// \return The corresponding AbstractXpertFlowStep.
    virtual const std::unique_ptr<AbstractXpertFlowStep>& getCovariateValidatorAndModelSelector() const = 0;

    /// \brief Get the step responsible for dose validation.
    /// \return The corresponding AbstractXpertFlowStep.
    virtual const std::unique_ptr<AbstractXpertFlowStep>& getDoseValidator() const = 0;

    /// \brief Get the step responsible for sample validation.
    /// \return The corresponding AbstractXpertFlowStep.
    virtual const std::unique_ptr<AbstractXpertFlowStep>& getSampleValidator() const = 0;

    /// \brief Get the step responsible for target validation.
    /// \return The corresponding AbstractXpertFlowStep.
    virtual const std::unique_ptr<AbstractXpertFlowStep>& getTargetValidator() const = 0;

    /// \brief Get the step responsible for adjustment trait creation.
    /// \return The corresponding AbstractXpertFlowStep.
    virtual const std::unique_ptr<AbstractXpertFlowStep>& getAdjustmentTraitCreator() const = 0;

    /// \brief Get the step responsible for request execution.
    /// \return The corresponding AbstractXpertFlowStep.
    virtual const std::unique_ptr<AbstractXpertFlowStep>& getRequestExecutor() const = 0;

    /// \brief Get the step responsible for report generation.
    /// \return The corresponding AbstractXpertFlowStep.
    virtual const std::unique_ptr<AbstractXpertFlowStep>& getReportPrinter() const = 0;

protected:

    /// \brief The step responsible for covariate validation and drug model selection.
    std::unique_ptr<AbstractXpertFlowStep> m_covariateValidatorAndModelSelector;

    /// \brief The step responsible for dose validation.
    std::unique_ptr<AbstractXpertFlowStep> m_doseValidator;

    /// \brief The step responsible for sample validation.
    std::unique_ptr<AbstractXpertFlowStep> m_sampleValidator;

    /// \brief The step responsible for target validation.
    std::unique_ptr<AbstractXpertFlowStep> m_targetValidator;

    /// \brief The step responsible for adjustment trait creation.
    std::unique_ptr<AbstractXpertFlowStep> m_adjustmentTraitCreator;

    /// \brief The step responsible for request execution.
    std::unique_ptr<AbstractXpertFlowStep> m_requestExecutor;;

    /// \brief The step responsible for report generation.
    std::unique_ptr<AbstractXpertFlowStep> m_reportPrinter;

};

} // namespace Xpert
} // namespace Tucuxi

#endif // ABSTRACTXPERTFLOWSTEPPROVIDER_H
