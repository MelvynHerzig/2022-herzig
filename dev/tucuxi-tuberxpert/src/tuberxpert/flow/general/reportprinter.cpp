#include "reportprinter.h"

#include <memory>

#include "tuberxpert/query/xpertrequestdata.h"
#include "tuberxpert/exporter/abstractxpertrequestresultexport.h"
#include "tuberxpert/exporter/xpertrequestresultxmlexport.h"
#include "tuberxpert/exporter/xpertrequestresulthtmlexport.h"
#include "tuberxpert/exporter/xpertrequestresultpdfexport.h"

using namespace std;

namespace Tucuxi {
namespace Xpert {

ReportPrinter::ReportPrinter()
{}

void ReportPrinter::perform(XpertRequestResult& _xpertRequestResult)
{
    // Extracts the request format
    OutputFormat desiredFormat = _xpertRequestResult.getXpertRequest().getOutputFormat();

    // Exporter
    unique_ptr<AbstractXpertRequestResultExport> exporter = nullptr;

    // Select the cooresponding exporter
    switch(desiredFormat) {
    case OutputFormat::XML  : exporter = make_unique<XpertRequestResultXmlExport>(); break;
    case OutputFormat::HTML : exporter = make_unique<XpertRequestResultHtmlExport>(); break;
    case OutputFormat::PDF  : exporter = make_unique<XpertRequestResultPdfExport>(make_unique<XpertRequestResultHtmlExport>()); break;
    }

    // Export
    exporter->exportToFile(_xpertRequestResult);
}

} // namespace Xpert
} // namespace Tucuxi
