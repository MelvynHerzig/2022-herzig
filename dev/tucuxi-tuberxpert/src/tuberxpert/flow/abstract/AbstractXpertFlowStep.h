#ifndef ABSTRACTXPERTFLOWSTEP_H
#define ABSTRACTXPERTFLOWSTEP_H

#include "tuberxpert/result/xpertrequestresult.h"

namespace Tucuxi {
namespace Xpert {

/// \brief Abstract class that must be implemented by objects that will
///        be returned by a concrete xpert flow step provider.
///
/// \date 20/06/2022
/// \author Herzig Melvyn
class AbstractXpertFlowStep
{

public:

    /// \brief Destructor.
    virtual ~AbstractXpertFlowStep(){};

    /// \brief Method to call in order to perform the step.
    /// \param _xpertRequestResult XpertRequestResult object that will hold the step result and
    ///        that hold the information needed for the step be performed.
    virtual void perform(XpertRequestResult& _xpertRequestResult) = 0;

};

} // namespace Xpert
} // namespace Tucuxi

#endif // ABSTRACTXPERTFLOWSTEP_H
