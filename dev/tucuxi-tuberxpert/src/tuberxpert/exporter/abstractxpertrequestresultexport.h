#ifndef ABSTRACTXPERTREQUESTRESULTEXPORT_H
#define ABSTRACTXPERTREQUESTRESULTEXPORT_H

#include "tuberxpert/result/xpertrequestresult.h"

namespace Tucuxi {
namespace Xpert {

/// \brief This class is an abstract class that represents the set of
///        exporters of XpertRequestResult.
/// \date 23/06/2022
/// \author Herzig Melvyn
class AbstractXpertRequestResultExport
{
public:

    /// \brief Destructor.
    virtual ~AbstractXpertRequestResultExport() {};

    /// \brief Export the XpertRequestResult to a file.
    /// \param _xpertRequestResult Result of the xpertRequest to export.
    virtual void exportToFile(XpertRequestResult& _xpertRequestResult) = 0;
};

} // namespace Xpert
} // namespace Tucuxi

#endif // ABSTRACTXPERTREQUESTRESULTEXPORT_H
