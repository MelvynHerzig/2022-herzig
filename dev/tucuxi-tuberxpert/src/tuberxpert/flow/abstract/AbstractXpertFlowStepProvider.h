#ifndef ABSTRACTXPERTFLOWSTEPPROVIDER_H
#define ABSTRACTXPERTFLOWSTEPPROVIDER_H

#include <memory>

#include "tuberxpert/flow/abstract/abstractxpertflowstep.h"

namespace Tucuxi {
namespace Xpert {

/// \brief Abstract class that must be implemented by xpert flow step providers.
///        A drug xpert is an object responsible to provide the execution steps
///        for a given drug. An execution step is:
///        1) Validate covariates and select drug model
///        2) Validate dosages
///        3) Validate samples
///        4) Validate targets
///        5) Create adjustment trait
///        6) Execute the requests
///        7) Print the final report
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

    /// \brief Constructor. Simply set attributes to nullptr.
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

    /// \brief Get the step responsible to execute the requests.
    /// \return Return the corresponding AbstractXpertFlowStep.
    virtual const std::unique_ptr<AbstractXpertFlowStep>& getRequestExecutor() const = 0;

    /// \brief Get the step responsible to print the report.
    /// \return Return the corresponding AbstractXpertFlowStep.
    virtual const std::unique_ptr<AbstractXpertFlowStep>& getReportPrinter() const = 0;

protected:

    /// \brief Step responsible to validate covariates and select the best drug model.
    std::unique_ptr<AbstractXpertFlowStep> m_covariateValidatorAndModelSelector;

    /// \brief Step responsible to validate the doses.
    std::unique_ptr<AbstractXpertFlowStep> m_doseValidator;

    /// \brief Step responsible to validate the sampels.
    std::unique_ptr<AbstractXpertFlowStep> m_sampleValidator;

    /// \brief Step responsible to validate the targets.
    std::unique_ptr<AbstractXpertFlowStep> m_targetValidator;

    /// \brief Step responsible to create the adjustment trait to submit.
    std::unique_ptr<AbstractXpertFlowStep> m_adjustmentTraitCreator;

    /// \brief Step responsible to submit the requests to the core.
    std::unique_ptr<AbstractXpertFlowStep> m_requestExecutor;;

    /// \brief Step responsible to print the final report.
    std::unique_ptr<AbstractXpertFlowStep> m_reportPrinter;

};

} // namespace Xpert
} // namespace Tucuxi

#endif // ABSTRACTXPERTFLOWSTEPPROVIDER_H
