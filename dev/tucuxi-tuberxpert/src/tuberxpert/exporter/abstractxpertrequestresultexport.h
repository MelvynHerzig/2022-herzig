#ifndef ABSTRACTXPERTREQUESTRESULTEXPORT_H
#define ABSTRACTXPERTREQUESTRESULTEXPORT_H

#include "tuberxpert/result/xpertrequestresult.h"

namespace Tucuxi {
namespace Xpert {

/// \brief This class is an abstract class that represents the set of
///        xpert request result exporters.
/// \date 23/06/2022
/// \author Herzig Melvyn
class AbstractXpertRequestResultExport
{
public:

    /// \brief Destructor.
    virtual ~AbstractXpertRequestResultExport() {};

    /// \brief Export the xpert request result to a file.
    /// \param _xpertRequestResult Xpert request result to export.
    virtual void exportToFile(XpertRequestResult& _xpertRequestResult) = 0;
};

} // namespace Xpert
} // namespace Tucuxi

#endif // ABSTRACTXPERTREQUESTRESULTEXPORT_H
