#ifndef ABSTRACTXPERTFLOWSTEP_H
#define ABSTRACTXPERTFLOWSTEP_H

#include "tuberxpert/result/xpertrequestresult.h"

namespace Tucuxi {
namespace XpertFlow {

/// \brief Abstract class that must be implemented by objects that will
///        be returned by concrete xpert flow step provider.
///
/// \date 20/06/2022
/// \author Herzig Melvyn
class AbstractXpertFlowStep
{

public:

    /// \brief Destructor.
    virtual ~AbstractXpertFlowStep(){};

    /// \brief Method to be called in order to perform the step.
    /// \param _xpertRequestResult XpertRequestResult object that will hold the step result and
    ///        that hold the information needed for the step.
    virtual void perform(XpertResult::XpertRequestResult& _xpertRequestResult) const = 0;

};

} // namespace XpertFlow
} // namespace Tucuxi

#endif // ABSTRACTXPERTFLOWSTEP_H
