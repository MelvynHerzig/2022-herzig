#ifndef XPERTREQUESTRESULTXMLEXPORT_H
#define XPERTREQUESTRESULTXMLEXPORT_H

#include "tucucommon/xmlattribute.h"
#include "tucucommon/xmldocument.h"
#include "tucucommon/xmlnode.h"

#include "tuberxpert/exporter/abstractxpertrequestresultexport.h"

namespace Tucuxi {
namespace Xpert {

/// \brief This class exports an XpertRequestResult into xml.
///        It can be in a file or a string.
/// \date 23/06/2022
/// \author Herzig Melvyn
class XpertRequestResultXmlExport : public AbstractXpertRequestResultExport
{
public:

    /// \brief Constructor.
    XpertRequestResultXmlExport();

    /// \brief Export the xpert request result to file.
    /// \param _xpertRequestResult Xpert request result to export.
    void exportToFile(XpertRequestResult& _xpertRequestResult) override;

protected:

    bool makeXmlString(XpertRequestResult& _xpertRequestResult, std::string& _xmlString);

    void exportDrugIntro(XpertRequestResult& _xpertRequestResult, Common::XmlNode& _rootNode);

    std::string dateTimeToXmlString(const Common::DateTime& _dateTime) const;

protected:

    Common::XmlDocument m_xmlDocument;
};

} // namespace Xpert
} // namespace Tucuxi

#endif // XPERTREQUESTRESULTXMLEXPORT_H
