#ifndef ABSTRACTXPERTFLOWSTEP_H
#define ABSTRACTXPERTFLOWSTEP_H

#include "tuberxpert/result/xpertrequestresult.h"

namespace Tucuxi {
namespace Xpert {

/// \brief Abstract class that must be implemented by objects that will
///        be returned by a concrete AbstractXpertFlowStepProvider.
///
/// \date 20/06/2022
/// \author Herzig Melvyn
class AbstractXpertFlowStep
{

public:

    /// \brief Destructor.
    virtual ~AbstractXpertFlowStep(){};

    /// \brief Method to call in order to perform the step.
    /// \param _xpertRequestResult XpertRequestResult object which will contain the
    ///        result of this flow step and which contains the information
    ///        necessary for the execution of this flow step.
    virtual void perform(XpertRequestResult& _xpertRequestResult) = 0;

};

} // namespace Xpert
} // namespace Tucuxi

#endif // ABSTRACTXPERTFLOWSTEP_H
