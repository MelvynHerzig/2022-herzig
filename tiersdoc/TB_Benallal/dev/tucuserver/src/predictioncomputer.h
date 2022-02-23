#ifndef TUCUXI_SERVER_PREDICTIONCOMPUTER_H
#define TUCUXI_SERVER_PREDICTIONCOMPUTER_H

#include "computer.h"

namespace Tucuxi {
namespace Server {

class PredictionComputer : public Computer
{
public:
    PredictionComputer(const Query& _query);

    void compute();
};

} // namespace Server
} // namespace Tucuxi

#endif // TUCUXI_SERVER_PREDICTIONCOMPUTER_H
