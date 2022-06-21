#ifndef TARGETVALIDATOR_H
#define TARGETVALIDATOR_H

#include "tuberxpert/flow/abstract/abstractxpertflowstep.h"
#include "tuberxpert/result/xpertrequestresult.h"

namespace Tucuxi {
namespace XpertFlow {

/// \brief This class is used to check patient's target.
/// \date 10/06/2022
/// \author Herzig Melvyn
class TargetValidator : public XpertFlow::AbstractXpertFlowStep
{
public:

    /// \brief Constructor
    TargetValidator();

    /// \brief The method getTargetValidation simply checks that two targets don't overlap.
    ///        In other words, it checks that the target that have the same active moiety,
    ///        don't have the same target type. Additionally, it checks that each patient's
    ///        target is related to an active moiety that exists in the drug model.
    ///
    ///        If one of those checks fails, the XpertRequestResult gets an error and
    ///        it is considered as "to not being handled anymore".
    /// \param _xpertRequestResult XpertResultObject containing the treatment and the related drug model.
    void perform(XpertResult::XpertRequestResult& _xpertRequestResult) const;
};

} // namespace XpertFlow
} // namespace Tucuxi

#endif // TARGETVALIDATOR_H
