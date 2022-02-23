#ifndef TUCUXI_SERVER_CONFIGURATION_H
#define TUCUXI_SERVER_CONFIGURATION_H

#include <cstdint>

#include "tucucore/definitions.h"

#include "configurationreader.h"

namespace Tucuxi {
namespace Server {

class Configuration
{
public:
    static Configuration* getInstance();

    uint16_t getPort() const;
    void setPort(const uint16_t& port);

    Core::CycleSize getCycleSize() const;
    void setCycleSize(const Core::CycleSize& cycleSize);

    unsigned int getMaxDateInterval() const;
    void setMaxDateInterval(unsigned int maxDateInterval);

    unsigned int getMaxAdjustments() const;
    void setMaxAdjustments(unsigned int maxAdjustments);

protected:
    Configuration();

protected:
    static Configuration* m_sConfiguration;

    uint16_t m_port;
    Core::CycleSize m_cycleSize;
    unsigned int m_maxDateInterval;
    unsigned int m_maxAdjustments;
};

} // namespace Server
} // namespace Tucuxi

#endif // TUCUXI_SERVER_CONFIGURATION_H
