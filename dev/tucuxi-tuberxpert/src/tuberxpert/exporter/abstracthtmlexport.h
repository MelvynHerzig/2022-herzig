#ifndef ABSTRACTHTMLEXPORT_H
#define ABSTRACTHTMLEXPORT_H

#include <string>

#include "tuberxpert/result/xpertrequestresult.h"

namespace Tucuxi {
namespace Xpert {

/// \date 23/06/2022
/// \author Herzig Melvyn
class AbstractHtmlExport
{
public:

    /// \brief Destructor.
    virtual ~AbstractHtmlExport() {};

    virtual void exportToFile(const std::string& _fileName, XpertRequestResult& _xpertRequestResult) = 0;
};

} // namespace Xpert
} // namespace Tucuxi

#endif // ABSTRACTHTMLEXPORT_H
