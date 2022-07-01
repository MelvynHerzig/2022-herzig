#ifndef XPERTREQUESTRESULTHTMLEXPORT_H
#define XPERTREQUESTRESULTHTMLEXPORT_H

#include "tuberxpert/exporter/abstractxpertrequestresultexport.h"

namespace Tucuxi {
namespace Xpert {

/// \brief This class exports an XpertRequestResult into html.
/// \date 23/06/2022
/// \author Herzig Melvyn
class XpertRequestResultHtmlExport : public AbstractXpertRequestResultExport
{
public:

    /// \brief Constructor.
    XpertRequestResultHtmlExport();

    /// \brief Export the xpert request result to file. The export may fail. In that
    ///        case, the error message of the XpertRequestResult gets set.
    /// \param _xpertRequestResult Xpert request result to export.
    void exportToFile(XpertRequestResult& _xpertRequestResult) override;

protected:

    std::string makeHeaderString() const;

    std::string makeBodyString(XpertRequestResult& _xpertRequestResult) const;

    std::string exportHeader(XpertRequestResult& _xpertRequestResult) const;

    std::string exportDrugIntro(XpertRequestResult& _xpertRequestResult) const;

    std::string exportAdminContactData(XpertRequestResult& _xpertRequestResult) const;


};

} // namespace Xpert
} // namespace Tucuxi

#endif // XPERTREQUESTRESULTHTMLEXPORT_H
