#ifndef ABSTRACTHTMLEXPORT_H
#define ABSTRACTHTMLEXPORT_H

#include <string>

#include "tuberxpert/result/xpertrequestresult.h"

namespace Tucuxi {
namespace Xpert {

/// \brief Class that must be implemented by any HTML exporter that
///        wants to be used by the PDF exporter.
/// \date 23/06/2022
/// \author Herzig Melvyn
class AbstractHtmlExport
{
public:

    /// \brief Destructor.
    virtual ~AbstractHtmlExport() {};

    /// \brief Export the given XpertRequestResult to an HTML file named with the given file name.
    /// \param _fileName Name of the file to use for the HTML file.
    /// \param _xpertRequestResult Result of the xpertRequest to export.
    virtual void exportToFile(const std::string& _fileName, XpertRequestResult& _xpertRequestResult) = 0;
};

} // namespace Xpert
} // namespace Tucuxi

#endif // ABSTRACTHTMLEXPORT_H
