#ifndef XPERTREQUESTRESULTPDFEXPORT_H
#define XPERTREQUESTRESULTPDFEXPORT_H

#include "tuberxpert/exporter/abstractxpertrequestresultexport.h"

namespace Tucuxi {
namespace Xpert {

/// \brief This class exports an XpertRequestResult into pdf.
///        It can only be in a file. The inherited exportToString
///        does nothing.
/// \date 23/06/2022
/// \author Herzig Melvyn
class XpertRequestResultPdfExport : public AbstractXpertRequestResultExport
{
public:

    /// \brief Constructor.
    XpertRequestResultPdfExport();

    /// \brief Export the xpert request result to a file.
    /// \param _xpertRequestResult Xpert request result to export.
    void exportToFile(XpertRequestResult& _xpertRequestResult) override;
};

} // namespace Xpert
} // namespace Tucuxi

#endif // XPERTREQUESTRESULTPDFEXPORT_H
