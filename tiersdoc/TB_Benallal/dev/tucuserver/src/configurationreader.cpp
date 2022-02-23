#include "configurationreader.h"

using namespace std;

namespace Tucuxi {
namespace Server {

ConfigurationReader::ConfigurationReader(const std::string& _filename)
    : m_filename(_filename), m_port(9090), m_cycleSize(250), m_maxDateInterval(31)
{

    if(m_xmlDocument.open(m_filename)) {

        const string NET_NODE_NAME = "net";
        const string COMPUTATION_NODE_NAME = "computation";

        Common::XmlNode root = m_xmlDocument.getRoot();

        Common::XmlNodeIterator netRootIterator = root.getChildren(NET_NODE_NAME);
        Common::XmlNodeIterator computationRootIterator = root.getChildren(COMPUTATION_NODE_NAME);

        m_port = createPort(netRootIterator);
        m_cycleSize = createCycleSize(computationRootIterator);
        m_maxDateInterval = createMaxDateInterval(computationRootIterator);
        m_maxAdjustments = createMaxAdjustments(computationRootIterator);
    }
}

uint16_t ConfigurationReader::createPort(Common::XmlNodeIterator& _netRootIterator)
{
    const string PORT_NODE_NAME = "port";

    uint16_t port = 9090;
    if (_netRootIterator != _netRootIterator.none()) {
        string portValue = _netRootIterator->getChildren(PORT_NODE_NAME)->getValue();
        port = stoul(portValue);
    }

    return port;
}

Core::CycleSize ConfigurationReader::createCycleSize(Common::XmlNodeIterator& _computationRootIterator)
{
    const string CYCLE_SIZE_NODE_NAME = "cycleSize";

    Core::CycleSize cycleSize = 250;
    if (_computationRootIterator != _computationRootIterator.none()) {
        string cycleSizeValue = _computationRootIterator->getChildren(CYCLE_SIZE_NODE_NAME)->getValue();
        cycleSize = stoul(cycleSizeValue);
    }

    return cycleSize;
}

unsigned int ConfigurationReader::createMaxDateInterval(Common::XmlNodeIterator& _computationRootIterator)
{
    const string MAX_DATE_INTERVAL_NODE_NAME = "maxDateInterval";

    unsigned int maxDateInterval = 31;
    if (_computationRootIterator != _computationRootIterator.none()) {
        string maxDataIntervalValue = _computationRootIterator->getChildren(MAX_DATE_INTERVAL_NODE_NAME)->getValue();
        maxDateInterval = stoul(maxDataIntervalValue);
    }

    return maxDateInterval;
}

unsigned int ConfigurationReader::createMaxAdjustments(Common::XmlNodeIterator &_computationRootIterator)
{
    const string MAX_ADJUSTMENTS_NAME = "maxAdjustments";

    unsigned int maxAdjustments = 20;
    if (_computationRootIterator != _computationRootIterator.none()) {
        string maxAdjustmentsValue = _computationRootIterator->getChildren(MAX_ADJUSTMENTS_NAME)->getValue();
        maxAdjustments = stoul(maxAdjustmentsValue);
    }

    return maxAdjustments;
}

unsigned int ConfigurationReader::getMaxAdjustments() const
{
    return m_maxAdjustments;
}

unsigned int ConfigurationReader::getMaxDateInterval() const
{
    return m_maxDateInterval;
}

Core::CycleSize ConfigurationReader::getCycleSize() const
{
    return m_cycleSize;
}

uint16_t ConfigurationReader::getPort() const
{
    return m_port;
}

} // namespace Server
} // namespace Tucuxi
