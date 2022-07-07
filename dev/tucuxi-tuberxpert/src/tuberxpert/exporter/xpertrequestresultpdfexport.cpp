#include "xpertrequestresultpdfexport.h"

#include "tuberxpert/utils/xpertutils.h"

#include "wkhtmltox/pdf.h"

using namespace std;

namespace Tucuxi {
namespace Xpert {

XpertRequestResultPdfExport::XpertRequestResultPdfExport()
{}

void XpertRequestResultPdfExport::exportToFile(XpertRequestResult& _xpertRequestResult)
{
    if( remove( "C:/Users/melvy/Desktop/test.pdf" ) != 0 )
        cout <<  "Error deleting file" ;


    wkhtmltopdf_global_settings* gs;
    wkhtmltopdf_object_settings* os;
    wkhtmltopdf_converter * c;

    /* Init wkhtmltopdf in graphics less mode */
    wkhtmltopdf_init(false);

    /*
     * Create a global settings object used to store options that are not
     * related to input objects, note that control of this object is parsed to
     * the converter later, which is then responsible for freeing it
     */
    gs = wkhtmltopdf_create_global_settings();
    /* We want the result to be storred in the file called test.pdf */
    string fileName = "C:\\Users\\melvy\\Desktop\\test.pdf";
    wkhtmltopdf_set_global_setting(gs, "out", fileName.c_str());
    wkhtmltopdf_set_global_setting(gs, "margin.top", "8");
    wkhtmltopdf_set_global_setting(gs, "margin.bottom", "8");
    wkhtmltopdf_set_global_setting(gs, "margin.left", "0");
    wkhtmltopdf_set_global_setting(gs, "margin.right", "0");

    /*
     * Create a input object settings object that is used to store settings
     * related to a input object, note again that control of this object is parsed to
     * the converter later, which is then responsible for freeing it
     */
    os = wkhtmltopdf_create_object_settings();
    /* We want to convert to convert the qstring documentation page */
    wkhtmltopdf_set_object_setting(os, "page", "C:\\Users\\melvy\\Desktop\\imatinib_1_24-6-2022_13h45m30s.html");
    wkhtmltopdf_set_object_setting(os, "load.blockLocalFileAccess", "false");
    wkhtmltopdf_set_object_setting(os, "web.enableIntelligentShrinking", "false");

    /* Create the actual converter object used to convert the pages */
    c = wkhtmltopdf_create_converter(gs);

    /*
     * Add the the settings object describing the qstring documentation page
     * to the list of pages to convert. Objects are converted in the order in which
     * they are added
     */
    wkhtmltopdf_add_object(c, os, NULL);

    /* Perform the actual conversion */
    if (!wkhtmltopdf_convert(c))
        cout << "Conversion failed!";

    /* Output possible http error code encountered */
    cout << "httpErrorCode: " << wkhtmltopdf_http_error_code(c) << endl;

    /* Destroy the converter object since we are done with it */
    wkhtmltopdf_destroy_converter(c);

    /* We will no longer be needing wkhtmltopdf funcionality */
    wkhtmltopdf_deinit();
}

} // namespace Xpert
} // namespace Tucuxi
