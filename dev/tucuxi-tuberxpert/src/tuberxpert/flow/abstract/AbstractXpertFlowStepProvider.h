#ifndef ABSTRACTXPERTFLOWSTEPPROVIDER_H
#define ABSTRACTXPERTFLOWSTEPPROVIDER_H

#include <memory>

#include "tuberxpert/flow/abstract/abstractxpertflowstep.h"

namespace Tucuxi {
namespace XpertFlow {

/// \brief Abstract class that must be implemented by drug xpert flow step providers.
///        A drug xpert is an object responsible to provide the execution steps
///        for a given drug. An execution step is:
///        1) Validate covariates and select drug model
///        2) Validate dosages
///        3) Validate samples
///        4) Validate targets
///        5) Create adjustment trait
///        6) Print the final report
///
///        An execution step is an implementation of the abstract xpert flow step class.
///
///        The main interest of this factory system is to be able to offer a specific
///        implementation for each drug: ImatinibXpertFlowStepProvider, RifampicinXpertFlowStepProvider...
///
/// \date 20/06/2022
/// \author Herzig Melvyn
class AbstractXpertFlowStepProvider
{

public:

    AbstractXpertFlowStepProvider() :
        m_covariateValidatorAndModelSelector(nullptr),
        m_doseValidator(nullptr),
        m_sampleValidator(nullptr),
        m_targetValidator(nullptr),
        m_adjustmentTraitCreator(nullptr),
        m_reportPrinter(nullptr){};

    /// \brief Destructor.
    virtual ~AbstractXpertFlowStepProvider() {};

    /// \brief Get the step responsible to validate the covariates and to select the drug model.
    /// \return Return the corresponding AbstractXpertFlowStep.
    virtual const std::unique_ptr<AbstractXpertFlowStep>& getCovariateValidatorAndModelSelector() const = 0;

    /// \brief Get the step responsible to validate the doses.
    /// \return Return the corresponding AbstractXpertFlowStep.
    virtual const std::unique_ptr<AbstractXpertFlowStep>& getDoseValidator() const = 0;

    /// \brief Get the step responsible to validate the samples.
    /// \return Return the corresponding AbstractXpertFlowStep.
    virtual const std::unique_ptr<AbstractXpertFlowStep>& getSampleValidator() const = 0;

    /// \brief Get the step responsible to validate the targets.
    /// \return Return the corresponding AbstractXpertFlowStep.
    virtual const std::unique_ptr<AbstractXpertFlowStep>& getTargetValidator() const = 0;

    /// \brief Get the step responsible to create the adjustment trait.
    /// \return Return the corresponding AbstractXpertFlowStep.
    virtual const std::unique_ptr<AbstractXpertFlowStep>& getAdjustmentTraitCreator() const = 0;

    /// \brief Get the step responsible to print the report.
    /// \return Return the corresponding AbstractXpertFlowStep.
    virtual const std::unique_ptr<AbstractXpertFlowStep>& getReportPrinter() const = 0;

protected:

    std::unique_ptr<AbstractXpertFlowStep> m_covariateValidatorAndModelSelector;

    std::unique_ptr<AbstractXpertFlowStep> m_doseValidator;

    std::unique_ptr<AbstractXpertFlowStep> m_sampleValidator;

    std::unique_ptr<AbstractXpertFlowStep> m_targetValidator;

    std::unique_ptr<AbstractXpertFlowStep> m_adjustmentTraitCreator;

    std::unique_ptr<AbstractXpertFlowStep> m_reportPrinter;

};

} // namespace XpertFlow
} // namespace Tucuxi

#endif // ABSTRACTXPERTFLOWSTEPPROVIDER_H
