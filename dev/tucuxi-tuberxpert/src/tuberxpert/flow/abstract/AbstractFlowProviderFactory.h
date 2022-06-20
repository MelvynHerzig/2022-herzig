#ifndef ABSTRACTFLOWPROVIDERFACTORY_H
#define ABSTRACTFLOWPROVIDERFACTORY_H

namespace Tucuxi {
namespace XpertFlow {

/// \brief Abstract class that must be implemented by drug xpert flow providers.
///        A drug xpert is an object responsible to provide the execution steps
///        for a given drug. An execution step is:
///        - Validate covariates and select drug model
///        - Validate dosages
///        - Validate samples
///        - Validate targets
///        - Create adjustment trait
///        -
///
/// \date 20/06/2022
/// \author Herzig Melvyn
class AbstractDrugXpertFlowProviderFactory
{

public:

};

} // namespace XpertFlow
} // namespace Tucuxi

#endif // ABSTRACTFLOWPROVIDERFACTORY_H
