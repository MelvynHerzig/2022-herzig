#ifndef XPERTREQUESTRESULTPDFEXPORT_H
#define XPERTREQUESTRESULTPDFEXPORT_H

#include "tuberxpert/exporter/abstractxpertrequestresultexport.h"
#include "tuberxpert/exporter/abstracthtmlexport.h"

namespace Tucuxi {
namespace Xpert {

/// \brief This class exports an XpertRequestResult in pdf.
///
///        This exporter converts an html file into a pdf using
///        wkhtmltopdf C library (https://wkhtmltopdf.org/).
/// \date 23/06/2022
/// \author Herzig Melvyn
class XpertRequestResultPdfExport : public AbstractXpertRequestResultExport
{
public:

    /// \brief Constructor.
    /// \param _htmlExport Html exporter to use to create the html file.
    XpertRequestResultPdfExport(std::unique_ptr<AbstractHtmlExport> _htmlExport);

    /// \brief Export the XpertRequestResult to a pdf file. The export may fail (i.e. file creation rights).
    ///        In this case, the XpertRequestResult error message is set.
    /// \param _xpertRequestResult Result of the xpertRequest to export.
    void exportToFile(XpertRequestResult& _xpertRequestResult) override;

protected:

    /// \brief Html exporter to use to create the html file
    ///        before converting it to pdf.
    std::unique_ptr<AbstractHtmlExport> m_htmlExport;
};

} // namespace Xpert
} // namespace Tucuxi

#endif // XPERTREQUESTRESULTPDFEXPORT_H
