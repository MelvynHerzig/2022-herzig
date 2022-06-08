#ifndef TARGETVALIDATOR_H
#define TARGETVALIDATOR_H

#include "../../result/xpertrequestresult.h"

namespace Tucuxi {
namespace XpertResult {

class TargetValidator
{
public:
    TargetValidator();

    void getTargetValidations(XpertRequestResult& _xpertRequestResult) const;
};

} // namespace XpertResult
} // namespace Tucuxi

#endif // TARGETVALIDATOR_H
