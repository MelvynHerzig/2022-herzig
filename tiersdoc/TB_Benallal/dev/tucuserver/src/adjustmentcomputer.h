#ifndef TUCUXI_SERVER_ADJUSTMENTCOMPUTER_H
#define TUCUXI_SERVER_ADJUSTMENTCOMPUTER_H

#include "computer.h"

#include "tucucore/computingservice/computingrequest.h"
#include "tucucore/computingservice/computingresponse.h"
#include "tucucore/computingservice/computingtrait.h"
#include "tucucore/computingcomponent.h"
#include "tucucore/drugmodelimport.h"
#include "tucucore/intakeextractor.h"

namespace Tucuxi {
namespace Server {

class AdjustmentComputer : public Computer
{
public:
    AdjustmentComputer(const Query& _query);

    void compute();

private:
    Common::DateTime computeAdjustmentTime(const Core::DrugTreatment& _drugTreatment) const;
};

} // namespace Server
} // namespace Tucuxi

#endif // TUCUXI_SERVER_ADJUSTMENTCOMPUTER_H
