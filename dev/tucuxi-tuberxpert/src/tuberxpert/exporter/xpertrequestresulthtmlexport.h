#ifndef XPERTREQUESTRESULTHTMLEXPORT_H
#define XPERTREQUESTRESULTHTMLEXPORT_H

#include "tuberxpert/exporter/abstractxpertrequestresultexport.h"

namespace Tucuxi {
namespace Xpert {

/// \brief This class exports an XpertRequestResult into html.
///         It can be in a file or a string.
/// \date 23/06/2022
/// \author Herzig Melvyn
class XpertRequestResultHtmlExport : public AbstractXpertRequestResultExport
{
public:

    /// \brief Constructor.
    XpertRequestResultHtmlExport();

    /// \brief Export the xpert request result to file.
    /// \param _xpertRequestResult Xpert request result to export.
    void exportToFile(XpertRequestResult& _xpertRequestResult) override;
};

} // namespace Xpert
} // namespace Tucuxi

#endif // XPERTREQUESTRESULTHTMLEXPORT_H
