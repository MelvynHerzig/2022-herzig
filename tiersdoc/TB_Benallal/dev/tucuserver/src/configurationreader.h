#ifndef TUCUXI_SERVER_CONFIGURATIONREADER_H
#define TUCUXI_SERVER_CONFIGURATIONREADER_H

#include "tucucommon/xmldocument.h"
#include "tucucommon/xmlnode.h"
#include "tucucommon/xmliterator.h"

#include "tucucore/definitions.h"

namespace Tucuxi {
namespace Server {

class ConfigurationReader
{
public:
    ConfigurationReader(const std::string& _filename);
    ConfigurationReader(ConfigurationReader& other) = delete;

    uint16_t getPort() const;
    Core::CycleSize getCycleSize() const;
    unsigned int getMaxDateInterval() const;
    unsigned int getMaxAdjustments() const;

protected:
    uint16_t createPort(Common::XmlNodeIterator& _netRootIterator);
    Core::CycleSize createCycleSize(Common::XmlNodeIterator& _computationRootIterator);
    unsigned int createMaxDateInterval(Common::XmlNodeIterator& _computationRootIterator);
    unsigned int createMaxAdjustments(Common::XmlNodeIterator& _computationRootIterator);

protected:
    std::string m_filename;
    Common::XmlDocument m_xmlDocument;

    uint16_t m_port;
    Core::CycleSize m_cycleSize;
    unsigned int m_maxDateInterval;
    unsigned int m_maxAdjustments;
};

} // namespace Server
} // namespace Tucuxi

#endif // TUCUXI_SERVER_CONFIGURATIONREADER_H
