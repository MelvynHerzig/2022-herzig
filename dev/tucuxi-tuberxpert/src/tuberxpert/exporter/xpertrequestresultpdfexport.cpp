#include "xpertrequestresultpdfexport.h"

#include "tuberxpert/utils/xpertutils.h"

#include "wkhtmltox/pdf.h"

using namespace std;

namespace Tucuxi {
namespace Xpert {

XpertRequestResultPdfExport::XpertRequestResultPdfExport(unique_ptr<AbstractHtmlExport> _htmlExport) : m_htmlExport(move(_htmlExport))
{}

void XpertRequestResultPdfExport::exportToFile(XpertRequestResult& _xpertRequestResult)
{
    // Get the output file name
    string outputFileName = computeFileName(_xpertRequestResult);

    // Try to remove the output file if it already exists.
    // Wkhtmltox does not replace it automatically.
    ifstream f(outputFileName.c_str());
    if (f.good() && remove(outputFileName.c_str()) != 0) {
        _xpertRequestResult.setErrorMessage("Output pdf file: " + outputFileName + " already exists and could not be replaced.");
        return;
    }

    // Make the filename of the temporary html file.
    string tempHtmlFileName = computeFileName(_xpertRequestResult, true, false) + "_temp.html";

    // Generate the temp html file.
    m_htmlExport->exportToFile(tempHtmlFileName, _xpertRequestResult);
    if( !_xpertRequestResult.shouldBeHandled() ){
        _xpertRequestResult.setErrorMessage("Error during the generation of the html for pdf exportation.");
        remove(tempHtmlFileName.c_str()); // Just in case, try to delete the temp file.
        return;
    }

    // Preparing the wkhtmltox export objects.
    wkhtmltopdf_global_settings* gs;
    wkhtmltopdf_object_settings* os;
    wkhtmltopdf_converter* c;

    // Init wkhtmltopdf in graphics less mode
    wkhtmltopdf_init(false);


    // Create a global settings object used to store options that are not
    // related to input objects,
    gs = wkhtmltopdf_create_global_settings();

    wkhtmltopdf_set_global_setting(gs, "out", outputFileName.c_str());
    wkhtmltopdf_set_global_setting(gs, "margin.top", "8");
    wkhtmltopdf_set_global_setting(gs, "margin.bottom", "8");
    wkhtmltopdf_set_global_setting(gs, "margin.left", "0");
    wkhtmltopdf_set_global_setting(gs, "margin.right", "0");

    // Create a input object settings object that is used to store settings
    // related to a input object, note again that control of this object is parsed to
    // the converter later, which is then responsible for freeing it
    os = wkhtmltopdf_create_object_settings();

    // Convert the temp html file
    wkhtmltopdf_set_object_setting(os, "page", tempHtmlFileName.c_str());
    wkhtmltopdf_set_object_setting(os, "load.blockLocalFileAccess", "false");
    wkhtmltopdf_set_object_setting(os, "web.enableIntelligentShrinking", "false");

    // Create the actual converter object used to convert the pages
    c = wkhtmltopdf_create_converter(gs);

    // Add the the settings object.
    // to the list of pages to convert. Objects are converted in the order in which
    // they are added
    wkhtmltopdf_add_object(c, os, NULL);

    // Perform the actual conversion
    if (!wkhtmltopdf_convert(c)) {
        _xpertRequestResult.setErrorMessage("Error during the exportation of the html into pdf. Http error code: " +
                                            to_string(wkhtmltopdf_http_error_code(c)));
    }

    // Destroy the converter object since we are done with it
    wkhtmltopdf_destroy_converter(c);

    // We will no longer be needing wkhtmltopdf funcionality
    wkhtmltopdf_deinit();

    // Remove the temp file
    remove(tempHtmlFileName.c_str());
}

} // namespace Xpert
} // namespace Tucuxi
