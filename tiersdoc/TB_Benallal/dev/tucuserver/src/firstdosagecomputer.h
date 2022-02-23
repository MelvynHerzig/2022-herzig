#ifndef TUCUXI_SERVER_FIRSTDOSAGECOMPUTER_H
#define TUCUXI_SERVER_FIRSTDOSAGECOMPUTER_H

#include "computer.h"

#include "tucucore/computingservice/computingrequest.h"
#include "tucucore/computingservice/computingresponse.h"
#include "tucucore/computingservice/computingtrait.h"
#include "tucucore/computingcomponent.h"
#include "tucucore/drugmodelimport.h"

namespace Tucuxi {
namespace Server {

class FirstDosageComputer : public Computer
{
public:
    FirstDosageComputer(const Query& _query);

    void compute();
};

} // namespace Server
} // namespace Tucuxi

#endif // TUCUXI_SERVER_FIRSTDOSAGECOMPUTER_H
