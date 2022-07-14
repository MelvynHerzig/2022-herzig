#ifndef TARGETVALIDATOR_H
#define TARGETVALIDATOR_H

#include "tuberxpert/flow/abstract/abstractxpertflowstep.h"
#include "tuberxpert/result/xpertrequestresult.h"

namespace Tucuxi {
namespace Xpert {

/// \brief This class is used to check the consistency of the patient's targets.
///
///        - Two patient targets cannot have the same target type for the same active moitety.
///        - Each patient target active moiety must exist in the drug model.
/// \date 10/06/2022
/// \author Herzig Melvyn
class TargetValidator : public AbstractXpertFlowStep
{
public:

    /// \brief The getTargetValidation method simply checks that two targets don't overlap.
    ///        In other words, it checks that the targets that have the same active moiety
    ///        don't have the same target type. In addition, it checks that each patient's
    ///        target is related to an active moiety that exists in the drug model.
    ///
    ///        If any of these checks fail, the XpertRequestResult gets an error and
    ///        should not be processed further.
    /// \param _xpertRequestResult XpertResult object containing the targets and the associated drug model.
    void perform(XpertRequestResult& _xpertRequestResult);
};

} // namespace Xpert
} // namespace Tucuxi

#endif // TARGETVALIDATOR_H
