#include "xpertrequestresulthtmlexport.h"

#include <sstream>

#include "tuberxpert/utils/xpertutils.h"
#include "tuberxpert/exporter/static/filestring.h"

#include "inja/inja.hpp"

using namespace std;

namespace Tucuxi {
namespace Xpert {

XpertRequestResultHtmlExport::XpertRequestResultHtmlExport()
{}

void XpertRequestResultHtmlExport::exportToFile(XpertRequestResult& _xpertRequestResult)
{
    // Get the xml string
    string xmlString;
    //makeXmlString(_xpertRequestResult, xmlString);

    // Get the filename <drugId>_<requestNumber>_<current time to evit conflict naming>
    string fileName = computeFileName(_xpertRequestResult);

    // Opening the file
    ofstream file;
    file.open(fileName);
    if ((file.rdstate() & ostream::failbit) != 0) {
        _xpertRequestResult.setErrorMessage("The file " + fileName + " could not be opened.");
        return ;
    }

    // Write header
    file << makeHeaderString();


    // Write body
    file << makeBodyString(_xpertRequestResult);;

    file.close();

    // The html is exported.
    return;
}

string XpertRequestResultHtmlExport::makeHeaderString() const
{
    stringstream ss;

    // Making the header template
    ss << "<!doctype html>" << endl
       << "<html>" << endl
       << "<head>" << endl
       << "    <meta charset='UTF-8'>" << endl
       << "    <meta name='viewport' content='width=device-width, initial-scale=1.0'>" << endl
       << "    <!-- CSS -->" << endl
       << "    <!-- Will be injected by the template engine for 'all-in-one' file. -->" << endl
       << "    <style> {{ css.normalize }} </style>" << endl                                        // Insert css normalization
       << "    <style> {{ css.tuberxpert }} </style>" << endl                                       // Insert css made for tuberxpert
       << "    <!-- JS -->" << endl
       << "    <!-- Will be injected by the template engine for 'all-in-one' file. -->" << endl
       << "    <script> {{ js.graphing }} </script>" << endl                                        // Insert js graphing
       << "    <script> {{ js.graphdata }} </script>" << endl                                       // Insert js graphdata
       << "    <script> {{ js.tuberxpert }} </script>" << endl                                      // Insert js made for tuberxpert
       << "</head>" << endl;


    // Preparing the data to insert
    inja::json data;
    data["css"]["normalize"] = FileString::normalizeCssStr;
    data["css"]["tuberxpert"] = FileString::tuberxpertCssStr;

    data["js"]["graphing"] = FileString::graphingJsStr;
    data["js"]["graphdata"] = FileString::graphdataJsStr;
    data["js"]["tuberxpert"] = FileString::tuberxpertJsStr;

    // Rendering
    return inja::render(ss.str(), data);
}

string XpertRequestResultHtmlExport::makeBodyString(XpertRequestResult& _xpertRequestResult) const
{
    stringstream ss;

    // Making the header template
    ss << "<body class='bg-secondary'>" << endl
       << "    <div class='page-A4 bg-body'>" << endl
       << "        <!-- Header -->" << endl
       << "        <table class='header'>" << endl
       << "            <tr>" << endl
       << "                <td>" << endl
       << "                    <img alt='Tucuxi logo from asset/img/tucuxi.png' src='data:image/png;base64,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'>" << endl
       << "                </td>" << endl
       << "                <td>" << endl
       << "                    <b>TuberXpert report</b>" << endl
       << "                </td>" << endl
       << "                <td>" << endl
       << "                    <b>Computed on:&nbsp;</b> 08.04.2022" << endl
       << "                </td>" << endl
       << "            </tr>" << endl
       << "        </table>" << endl
       << "        </div>" << endl
       << "        <script>" << endl
       << "        fillImages();" << endl
       << "    </script>" << endl
       << "</body>" << endl
       << "</html>" << endl;



    return ss.str();
}

} // namespace Xpert
} // namespace Tucuxi
