#ifndef ABSTRACTHTMLEXPORT_H
#define ABSTRACTHTMLEXPORT_H

#include <string>

#include "tuberxpert/result/xpertrequestresult.h"

namespace Tucuxi {
namespace Xpert {

/// \brief Class that must be implemented by any html exporter that
///        want to be used by the pdf exporter.
/// \date 23/06/2022
/// \author Herzig Melvyn
class AbstractHtmlExport
{
public:

    /// \brief Destructor.
    virtual ~AbstractHtmlExport() {};

    /// \brief Export the given XpertRequestResult to an html file named with the given file name.
    /// \param _fileName Name of the file to use for the html file.
    /// \param _xpertRequestResult Result of the xpertRequest to export.
    virtual void exportToFile(const std::string& _fileName, XpertRequestResult& _xpertRequestResult) = 0;
};

} // namespace Xpert
} // namespace Tucuxi

#endif // ABSTRACTHTMLEXPORT_H
