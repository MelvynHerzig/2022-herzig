#ifndef XPERTREQUESTRESULTPDFEXPORT_H
#define XPERTREQUESTRESULTPDFEXPORT_H

#include <memory>

#include "tuberxpert/exporter/abstractxpertrequestresultexport.h"
#include "tuberxpert/exporter/abstracthtmlexport.h"

namespace Tucuxi {
namespace Xpert {

/// \brief This class exports an XpertRequestResult into pdf.
/// \date 23/06/2022
/// \author Herzig Melvyn
class XpertRequestResultPdfExport : public AbstractXpertRequestResultExport
{
public:

    /// \brief Constructor.
    XpertRequestResultPdfExport(std::unique_ptr<AbstractHtmlExport> _htmlExport);

    /// \brief Export the xpert request result to a file.
    /// \param _xpertRequestResult Xpert request result to export.
    void exportToFile(XpertRequestResult& _xpertRequestResult) override;

protected:

    std::unique_ptr<AbstractHtmlExport> m_htmlExport;
};

} // namespace Xpert
} // namespace Tucuxi

#endif // XPERTREQUESTRESULTPDFEXPORT_H
