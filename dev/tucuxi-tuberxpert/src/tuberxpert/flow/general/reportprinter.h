#ifndef REPORTPRINTER_H
#define REPORTPRINTER_H

#include "tuberxpert/flow/abstract/abstractxpertflowstep.h"
#include "tuberxpert/result/xpertrequestresult.h"

namespace Tucuxi {
namespace Xpert {

/// \brief This step chooses the exporter that correspond to the
///        XpertRequest output desired format.
///        In other words, it selects between the xml, html or pdf exported
///        and passes the xpertRequestResult to be exported.
/// \date 23/06/2022
/// \author Herzig Melvyn
class ReportPrinter : public AbstractXpertFlowStep
{
public:

    /// \brief Constructor.
    ReportPrinter();

    /// \brief Select the corresponding exporter and passes the xpertRequestResult to it.
    /// \param _xpertRequestResult XpertRequestResult to export.
    void perform(XpertRequestResult& _xpertRequestResult) override;

};

} // namespace Xpert
} // namespace Tucuxi

#endif // REPORTPRINTER_H
