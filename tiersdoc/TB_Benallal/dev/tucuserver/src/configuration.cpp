#include "configuration.h"

namespace Tucuxi {
namespace Server {

Configuration* Configuration::m_sConfiguration = nullptr;

Configuration::Configuration()
    : m_port(9090), m_cycleSize(250), m_maxDateInterval(31)
{}

unsigned int Configuration::getMaxAdjustments() const
{
    return m_maxAdjustments;
}

void Configuration::setMaxAdjustments(unsigned int maxAdjustments)
{
    m_maxAdjustments = maxAdjustments;
}

void Configuration::setMaxDateInterval(unsigned int maxDateInterval)
{
    m_maxDateInterval = maxDateInterval;
}

void Configuration::setCycleSize(const Core::CycleSize &cycleSize)
{
    m_cycleSize = cycleSize;
}

void Configuration::setPort(const uint16_t &port)
{
    m_port = port;
}

Configuration* Configuration::getInstance()
{
    if (m_sConfiguration == nullptr) {
        ConfigurationReader configurationReader("../tucuserver/tucuserver.cfg");
        m_sConfiguration = new Configuration();
        m_sConfiguration->setPort(configurationReader.getPort());
        m_sConfiguration->setCycleSize(configurationReader.getCycleSize());
        m_sConfiguration->setMaxDateInterval(configurationReader.getMaxDateInterval());
        m_sConfiguration->setMaxAdjustments(configurationReader.getMaxAdjustments());
    }

    return m_sConfiguration;
}

uint16_t Configuration::getPort() const
{
    return m_port;
}

Core::CycleSize Configuration::getCycleSize() const
{
    return m_cycleSize;
}

unsigned int Configuration::getMaxDateInterval() const
{
    return m_maxDateInterval;
}

} // namespace Server
} // namespace Tucuxi
