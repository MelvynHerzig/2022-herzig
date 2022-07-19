#include "xpertrequestresulthtmlexport.h"

#include <sstream>

#include "tucucore/dosage.h"

#include "tuberxpert/utils/xpertutils.h"
#include "tuberxpert/exporter/static/filestring.h"
#include "tuberxpert/result/xpertqueryresult.h"
#include "tuberxpert/language/languagemanager.h"

using namespace std;

namespace Tucuxi {
namespace Xpert {

void XpertRequestResultHtmlExport::exportToFile(XpertRequestResult& _xpertRequestResult)
{

    // Get the filename <drugId>_<requestNumber>_<current time to evit conflict naming>.<file extension>
    string fileName = computeFileName(_xpertRequestResult);
    exportToFile(fileName, _xpertRequestResult);
}

void XpertRequestResultHtmlExport::exportToFile(const string& _fileName, XpertRequestResult& _xpertRequestResult)
{
    // Open the file.
    ofstream fileStream;
    fileStream.open(_fileName);
    if ((fileStream.rdstate() & ostream::failbit) != 0) {
        _xpertRequestResult.setErrorMessage("The file " + _fileName + " could not be opened.");
        return ;
    }

    m_xpertRequestResultInUse = &_xpertRequestResult;

    // Maybe we didn't manage to set up a correct html template. (Just in case)
    try {

        // Write the html header.
        fileStream << makeHeaderString();

        // Write the html body.
        fileStream << makeBodyString(_xpertRequestResult);

    } catch (inja::RenderError& e) {
        _xpertRequestResult.setErrorMessage("Failed to render html: " + string(e.what()));
        return ;
    }

    fileStream.close();
}

string XpertRequestResultHtmlExport::makeHeaderString() const
{
    stringstream templateStream;

    // Making the header template.
    templateStream << "<!doctype html>" << endl
                   << "<html>" << endl
                   << "<head>" << endl
                   << "    <meta charset='UTF-8'>" << endl
                   << "    <meta name='viewport' content='width=device-width, initial-scale=1.0'>" << endl

                      // Insert css normalization
                      // Insert css made for tuberxpert

                   << "    <!-- CSS -->" << endl
                   << "    <!-- Will be injected by the template engine for 'all-in-one' file. -->" << endl
                   << "    <style> {{ css.normalize }} </style>" << endl
                   << "    <style> {{ css.tuberxpert }} </style>" << endl

                      // Insert js graphing
                      // Insert js graphdata
                      // Insert js made for tuberxpert

                   << "    <!-- JS -->" << endl
                   << "    <!-- Will be injected by the template engine for 'all-in-one' file. -->" << endl
                   << "    <script> {{ js.graphing }} </script>" << endl
                   << "    <script> {{ js.graphdata }} </script>" << endl
                   << "    <script> {{ js.tuberxpert }} </script>" << endl
                   << "</head>" << endl;

    // Preparing the data to insert.
    inja::json json;
    json["css"]["normalize"] = FileString::normalizeCssStr;
    json["css"]["tuberxpert"] = FileString::tuberxpertCssStr;

    json["js"]["graphing"] = FileString::graphingJsStr;
    json["js"]["graphdata"] = FileString::graphdataJsStr;
    json["js"]["tuberxpert"] = FileString::tuberxpertJsStr;

    // Rendering.
    return inja::render(templateStream.str(), json);
}

string XpertRequestResultHtmlExport::makeBodyString(const XpertRequestResult& _xpertRequestResult) const
{
    stringstream templateStream;

    // Making the body template.
    // On the far left of the stream we have the template instructions and the real html content 4-5 tabs on the right.
    templateStream
       <<               "<body>" << endl
       <<               "<div class='page-A4 bg-body'>" << endl
       << endl

          // Header part: Logo, document title and computation time

       <<               "    <!-- Header -->" << endl
       <<               "    <table class='header'>" << endl
       <<               "        <tr>" << endl
       <<               "            <td>" << endl
       <<               "                <img alt='Tucuxi logo from asset/img/tucuxi.png' src='data:image/png;base64,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'>" << endl
       <<               "            </td>" << endl
       <<               "            <td>" << endl
       <<               "                <b> {{ header.tuberxpert_report_translation }} </b>" << endl
       <<               "            </td>" << endl
       <<               "            <td>" << endl
       <<               "                <b> {{ header.computed_on_translation }}:&nbsp; </b> {{ header.computation_time }}" << endl
       <<               "            </td>" << endl
       <<               "        </tr>" << endl
       <<               "    </table>" << endl
       <<               "    <hr>"
       << endl

          // Drug intro part: Drug id, last dose taken, drug model id.

       <<               "    <!-- Drug intro -->" << endl
       <<               "    <h3> {{ intro.drug_translation }} </h3>" << endl
       <<               "    <table class='drug-intro bg-light-grey'>" << endl
       <<               "        <tr>" << endl
       <<               "            <th> {{ intro.id_translation }} </th>" << endl
       <<               "            <th> {{ intro.last_dose_translation }} </th>" << endl
       <<               "            <th> {{ intro.drug_model_translation }} </th>" << endl
       <<               "        </tr>" << endl
       <<               "        <tr>" << endl
       <<               "            <td>" << endl
       <<               "                {{ intro.drug_id }}" << endl
       <<               "            </td>" << endl
       <<               "            <td>" << endl
       <<               "                {{ default( intro.last_dose, \"-\") }}" << endl
       <<               "            </td>" << endl
       <<               "            <td>" << endl
       <<               "                {{ intro.drug_model }}" << endl
       <<               "            </td>" << endl
       <<               "        </tr>" << endl
       <<               "    </table>" << endl
       << endl

          // Administrative data - contacts part: Mandator & patient personal and institute contacts
          // If an information is missing, insert a '-'.

       <<               "    <!-- Administrative/contacts -->" << endl
       <<               "    <h3> {{ contacts.translation }} </h3>" << endl
       <<               "    <table class='contacts-header'>" << endl
       <<               "        <tr>" << endl
       <<               "            <th></th>" << endl
       <<               "            <th><img alt='Doctor image from asset/img/doctor_64.png'  src='data:image/png;base64,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'> {{ contacts.mandator_translation }} </th>" << endl
       <<               "            <th><img alt='Patient image from asset/img/patient_64.png' src='data:image/png;base64,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' > {{ contacts.patient_translation }} </th>" << endl
       <<               "        </tr>" << endl
       <<               "    </table>" << endl
       << endl
       <<               "    <table class='contacts bg-light-grey'>" << endl
       <<               "        <tr>" << endl
       <<               "            <td rowspan='5'><img alt='Person image from asset/img/person.png' src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAABHNCSVQICAgIfAhkiAAAAAlwSFlzAAAEngAABJ4B0Y1KzAAAABl0RVh0U29mdHdhcmUAd3d3Lmlua3NjYXBlLm9yZ5vuPBoAAAHVSURBVFiF7dY9axRRFMbx38guBg2oEC1Mo0QtgotGBUEQbPwE4kdQwS8gWKcXtLCxUfwSYjpBJFjEYOdLGXxBXF9AcVmPxc7Gcc3u3FnHjYUPHJhh7jnP/95z78xkEaGKsizbios4jVPo4iEe4FZEfKtUMCKSA4ewghgSy9hXqWYF86P4NMK8H+9xsFYAZHiUYN6Pe3UDXKhg3o/zKbW3JG6Vc6l7qmpOKsCRMQCScrKyY5hl2W68GQOgi+mI+DpqUMoKfNHraVV18hipUoCI+IwXYwA8jYjuHwPkejIGQFpO4jE8o9oR7OJY3W/COxUAbvyNV/GefFnLzJexoxYAzGF74X4KN0eYX0OzMH4ac5UBcBhL+I53OD7wvIXLuIvbuIT5gTEn8SGvcR+tJIB8w7UHZva6bCYDNQ7g7UCNjzg7EgDbsDZkeZ8NznKI+TyeD6nxqtjSjQCulGywDq5jdgPj2fxZp6TG1WLe+rcgy7KdeIld0rSGx/n1CexNzGtjf0S0oTiDxRLyOmPxtxZgdYIAq78A6J3vst7VGR1MRfz8I2qhUdK7OtXIPdcBFiZo3tfCPwUwswkAM0WASfa/r0YRoLkJAM0iwP8VWNL7j5uUurmnH1d0pzE2H0pmAAAAAElFTkSuQmCC'></td>" << endl
       <<               "            <th> {{ contacts.id_translation}} </th>" << endl
       <<               "            <td> {{ default( contacts.mandator.id, \"-\") }} </td>" << endl
       <<               "            <td> {{ default( contacts.patient.id, \"-\") }} </td>" << endl
       <<               "        </tr>" << endl
       <<               "        <tr>" << endl
       <<               "            <th> {{ contacts.name_translation}} </th>" << endl
       <<               "            <td> {{ default( contacts.mandator.name, \"-\") }} </td>" << endl
       <<               "            <td> {{ default( contacts.patient.name, \"-\") }} </td>" << endl
       <<               "        </tr>" << endl
       <<               "        <tr>" << endl
       <<               "            <th> {{ contacts.address_translation }} </th>" << endl
       <<               "            <td> {{ default( contacts.mandator.address, \"-\") }} </td>" << endl
       <<               "            <td> {{ default( contacts.patient.address, \"-\") }} </td>" << endl
       <<               "        </tr>" << endl
       <<               "        <tr>" << endl
       <<               "            <th> {{ contacts.phone_translation }} </th>" << endl
       <<               "            <td> {{ default( contacts.mandator.phone, \"-\") }} </td>" << endl
       <<               "            <td> {{ default( contacts.patient.phone, \"-\") }} </td>" << endl
       <<               "        </tr>" << endl
       <<               "        <tr>" << endl
       <<               "            <th> {{ contacts.email_translation }}</th>" << endl
       <<               "            <td> {{ default( contacts.mandator.email, \"-\") }} </td>" << endl
       <<               "            <td> {{ default( contacts.patient.email, \"-\") }} </td>" << endl
       <<               "        </tr>" << endl
       <<               "    </table>" << endl
       << endl
       <<               "    <br>" << endl
       << endl
       <<               "    <table class='contacts bg-light-grey'>" << endl
       <<               "        <tr>" << endl
       <<               "            <td rowspan='5'><img alt='Institute image from asset/img/institute.png' src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAABHNCSVQICAgIfAhkiAAAAAlwSFlzAAAA7AAAAOwBeShxvQAAABl0RVh0U29mdHdhcmUAd3d3Lmlua3NjYXBlLm9yZ5vuPBoAAAEqSURBVFiF7da/LgRRFAbw3yyJxJ+QpRBatUIhSgmvIN5ArdErPYdS6wE0FHS7hSBahUoU/mxkdxW7I2MyY7lrZ1fsl3zFnTlnvi/nnjv38EcxjV3c4BZ7KBchvIQDPKCZ4isOsfzboiVs4hiNDOEsnmILo90IT2EHl98UzeId9jH7E+GvyhzKjtsTUuZQZm7PSeDHNrAWmHuOqIQI63ml6YC3NpsBuasYj9oGGoEGusVkqU/CHxgIA3FDFI0Knvug+wdQ9/m8juTE1VJxYzlxT6m4ieTLgWjCoYGhgf9tIGtmm0ut6zm586l1LSduUevGjTFYv9+kszKOsNJjzSq2cZ98OIMLvZ0Fk7zCQtLAWYHiMauIhiNZPBFV+qB9jZf4FERajVgkHtF4B0XDzEgEu+i7AAAAAElFTkSuQmCC'></td>" << endl
       <<               "            <th> {{ contacts.id_translation}} </th>" << endl
       <<               "            <td> {{ default( contacts.mandator.institute.id, \"-\") }} </td>" << endl
       <<               "            <td> {{ default( contacts.patient.institute.id, \"-\") }} </td>" << endl
       <<               "        </tr>" << endl
       <<               "        <tr>" << endl
       <<               "            <th> {{ contacts.name_translation }} </th>" << endl
       <<               "            <td> {{ default( contacts.mandator.institute.name, \"-\") }} </td>" << endl
       <<               "            <td> {{ default( contacts.patient.institute.name, \"-\") }} </td>" << endl
       <<               "        </tr>" << endl
       <<               "        <tr>" << endl
       <<               "            <th> {{ contacts.address_translation }} </th>" << endl
       <<               "            <td> {{ default( contacts.mandator.institute.address, \"-\") }} </td>" << endl
       <<               "            <td> {{ default( contacts.patient.institute.address, \"-\") }} </td>" << endl
       <<               "        </tr>" << endl
       <<               "        <tr>" << endl
       <<               "            <th> {{ contacts.phone_translation }} </th>" << endl
       <<               "            <td> {{ default( contacts.mandator.institute.phone, \"-\") }} </td>" << endl
       <<               "            <td> {{ default( contacts.patient.institute.phone, \"-\") }} </td>" << endl
       <<               "        </tr>" << endl
       <<               "        <tr>" << endl
       <<               "            <th> {{ contacts.email_translation }} </th>" << endl
       <<               "            <td> {{ default( contacts.mandator.institute.email, \"-\") }} </td>" << endl
       <<               "            <td> {{ default( contacts.patient.institute.email, \"-\") }} </td>" << endl
       <<               "        </tr>" << endl
       <<               "    </table>" << endl
       << endl

          // Administrative data - clinical data part:
          // If there is no clinical data
          //    Insert none
          // Else
          //    For each clinical data, insert a row

       <<               "    <!-- Administrative/clinical data -->" << endl
       <<               "    <h3> {{ clinical_datas.clinical_data_translation }} </h3>" << endl
       << "{% if not exists(\"clinical_datas.clinical_datas\") %}"
       <<               "        {{ clinical_datas.none_translation }}" << endl
       << "{% else %}"
       <<               "        <table class='clinical-data bg-light-grey'>" << endl
       <<     "{% for clinical_data in clinical_datas.clinical_datas %}"
       <<               "                <tr>" << endl
       <<               "                    <th> {{ clinical_data.key }} </th>" << endl
       <<               "                    <td> {{ clinical_data.value }} </td>" << endl
       <<               "                </tr>" << endl
       <<     "{% endfor %}"
       <<               "        </table>" << endl
       << "{% endif %}"
       << endl

          // Covariates part:
          // If there is no covariates
          //    Insert none
          // Else
          //    For each covariate
          //        Insert a table with the covariate information
          //        If there is a date of measure
          //            Insert it
          //        If there is a warning
          //            Insert it
          //
          // Note: Here, the tricky part is to have a <div class='avoid-break-inside'> that wraps
          //       the title and the first covariate for a nice page break. This div needs
          //       to be closed when there are no covariates or at the end of the first one.

       <<               "   <!-- Covariates -->" << endl
       <<               "   <div class='avoid-break-inside'>" << endl
       <<               "       <h3> {{ covariates.translation }} </h3>" << endl
       << "{% if not exists(\"covariates.covariates\") %}"
       <<               "       {{ covariates.none_translation }}" << endl
       <<               "   </div>" << endl
       << "{% else %}"
       <<     "{% for covariate in covariates.covariates %}"
       <<               "   <table class='covariate bg-light-grey'>" << endl
       <<               "        <tr>" << endl
       <<               "            <td><b> {{ covariate.name }} </b></td>" << endl
       <<               "            <td><b> {{ covariates.value_translation }}:</b> {{ covariate.value }} </td>" << endl
       <<               "            <td>"
       <<         "{% if existsIn( covariate, \"date\") %} "
       <<               "               <b> {{ covariates.date_translation }}:</b> {{ covariate.date }} "
       <<         "{% endif %}"
       <<               "            </td>" << endl
       <<               "        </tr>" << endl
       <<               "        <tr>" << endl
       <<               "            <td colspan='3'> {{ covariate.desc }} </td>" << endl
       <<               "        </tr>" << endl
       <<               "        <tr>" << endl
       <<         "{% if existsIn( covariate, \"warning\") %}"
       <<               "            <td colspan='3'>" << endl
       <<               "                <table>" << endl
       <<               "                    <tr class='bg-warning-normal'>" << endl
       <<               "                        <td>" << endl
       <<               "                            <img alt='Warning icon image from asset/img/warning.png' src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAABHNCSVQICAgIfAhkiAAAAAlwSFlzAAAA3QAAAN0BcFOiBwAAABl0RVh0U29mdHdhcmUAd3d3Lmlua3NjYXBlLm9yZ5vuPBoAAASbSURBVFiFxZdtTJVlGMd/1/2cl+cgHOSgCKjTlGFEFDrUGAIBQ8DICeopRXHK8syXWetbH3RuuvVFp261am2t1Rc3+yK51cS5UEcvY9ZW0idbrJjhS04BhfN29eEArgx4Dtj6f7xf/tfvvq77fp77FlXl/5RruhM3bg/laDTccOtmX0Qi5sKlS1/emI6PJJuB9aFQimcw8plCA0D/jd8THcoXw2n2pu729gf/GUAwGPTF3KnngOq56bBqmZDlvc+ZzkGu98cALsaHBxu7uroeOvVMqgRRz6xDolQ/lS0c3m5ItQFm0/BCKvuO/8FPfbFq4007BLzl1NNxBja1tC1RtMcyeE+ELObP+Xv/3b4hmt6+TTTOiFj6zOWOjl+c+BqnpKp6DPA2rJTHggNkZKfwymoPgFejcsypryOA5q27qhGa/CkQrJhgiiW01ftJ9wkITWU1ddVPBCAYDFpGOAmwpcowy060D4fh9FdxPmi/x8ORRBk9gRT2vOgdNdaTwWDQmjFA3J26W6Fo8TyhdrmMt3dcVc5cVk5ffMDZKwOJRkt4qSyNvCyDKkV9d+7unhFAY0tLhsIRgF11gjyKz683E6s2xuJ6X2S8XdJ9vLk2kSZROVJe3pgxbQAb72Egs7RAKFwkkw19JEt4rmAWVctcAJm4w4enBdD86msFCnvdLthR6/iwJOS32V9j43GBwt7SytqCpAHEip0AXBtKhbnpycXHMsyb72PrKg+Ay2XJiaQAmrftagTqMv3QVJbk6sfkt9lW6iUrTVCoK6+qa3QEEAqF3KJyHGB7jcHrnl58LIMdsNlTmTiWKnq8pKTkMbfHAG4PRQ+A5i9bIJQ/63DjTSS/TW2hm6L5FkC+Ly1wYFKA5tbWLFQPCtBWP8PgAJYBv83rNV4EQDhYVleXNSGAibqPAulVxcLSnMkBvGP/UVVszyRj/TZP51isK3KjkG4iHP1XgKaWncUq2ubzQEvV1BuvuliYZYPHFWNdaerEA0ezEKrwkOIRFG0rq1pbPNY9fh8wcAowm8oNsyfxG1NervDhGxaqOZNnAMBvE7g/zI5SN+91ho0Ip4DK0bjQ3LJzM0hFdgAaVzuvvdfN1MFhPAvBEg8LMgxARXnN2s0AVm9vrx2OSTswe/96w8K5zgDuDcG7n8c5d+UOK/K9+LxTlM1jYQ2OkO0XLvwcBVj16ccfvW8GwqYVWPT8EmFlvvPVn7+qdPUo3/REOXt5cOoJo1lYk+di5WILYFFUPK1GVesBNq5J7tjl5YIAqnEKFnucTUpLfJR2lCbGq2q9C6HSCOQvSA5g+VLhnX0WkWgWC7Mc3m1dFhihMNfCCMSh0iD0xxV6epN/IWUHcB4cIBKDuPL9bzHiCiL0G1HtBDj7tTISmcphhhoYZjiinP4uDIDG6ZSXt+ye45bINSDL54XKIiE303k5HgxNvQEDPojcH+Fab5jz16IMhRXgplsjhaKqbNi2s8il8olC8VRm/9T40yw5/RCPaWtXZ8eP4w+TUCjkvjUQXY3oCkECTlxU1XWrvy/qNGpc+dNo/OrDgbvfdnd3R2Aaj9Mnrb8A0TtykI+7cqgAAAAASUVORK5CYII='>" << endl
       <<               "                        </td>" << endl
       <<               "                        <td>" << endl
       <<               "                            {{ covariate.warning }}" << endl
       <<               "                        </td>" << endl
       <<               "                    </tr>" << endl
       <<               "                </table>" << endl
       <<               "            </td>" << endl
       <<         "{% endif %}"
       <<               "       </tr>" << endl
       <<               "   </table>" << endl
       <<         "{% if loop.is_first %}"
       <<               "   </div>" << endl
       <<         "{% endif %}"
       <<     "{% endfor %}"
       << "{% endif %}"
       << endl

          // Patient's treatment part:
          // If there is no dosage time ranges
          //    Insert none
          // Else
          //    For each dosage time range
          //        Insert a table with the dosage time range information
          //        If there is a type
          //            Insert the dosage type (loop/steady state)
          //        Insert a table for each single dose
          //        For each single dose
          //            Insert the posology
          //            If there is a warning
          //                Insert it
          //
          // Note: Here, the tricky part is to have a <div class='avoid-break-inside'> that wraps
          //       the title and the first dosage time range table for a nice page break. This div needs
          //       to be closed when there are no dosage time ranges or at the end of the first dosage time range.

       <<                   "    <!-- Treatment -->" << endl
       <<                   "    <div class='avoid-break-inside'>" << endl
       <<                   "        <h3> {{ treatment.translation }} </h3>" << endl
       << "{% if not exists(\"treatment.dosage_time_ranges\") %}"
       <<                   "        {{ treatment.none_translation }}" << endl
       <<                   "    </div>" << endl
       << "{% else %}"
       <<     "{% for dosage_time_range in treatment.dosage_time_ranges %}"
       <<                   "    <table class='dosage-time-range bg-light-grey'>" << endl
       <<                   "        <tr>" << endl
       <<                   "            <td><b>{{ treatment.from_translation }}</b> {{ dosage_time_range.date_from }} </td>" << endl
       <<                   "            <td><b>{{ treatment.to_translation }}</b> {{ dosage_time_range.date_to }}</td>" << endl
       <<                   "        </tr>" << endl
       <<                   "        <tr>" << endl
       <<                   "            <td colspan='2'>" << endl
       <<         "{% if existsIn( dosage_time_range, \"type\") %}"
       <<                   "               <b>{{ treatment.type_translation }}:</b> {{ dosage_time_range.type }}"
       <<         "{% endif %}"
       <<                   "            </td> " << endl
       <<                   "        </tr>" << endl
       <<                   "        <tr>" << endl
       <<                   "            <td colspan='2'>" << endl
       <<                   "                <table>" << endl
       <<         "{% for single_dose in dosage_time_range.single_doses %}"
       <<                   "                    <tr>" << endl
       <<                   "                        <td>" << endl
       <<                   "                            <img alt='Dot icon image from asset/img/dot.png' src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABEAAAAPCAYAAAACsSQRAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAAANsAAADbAfBQ5pwAAACnSURBVDhPY6AlMADi2UB8Hoi/Q2kQ3wKICQIWIC4BYpDG/1jwbyBuAGKQOpwAZAA2zegYZBBWAPICLhegY5CLQOrBgAlKg0A2EHNAmAQByDvJECaqISZQmliANZCJ9QoMfwZiMEB2yQ0oTSy4A6VRDDkDpYkFWNWD/AgKdWxOR8cgdTgTHij+sWlCx91AjBOAog5kEC4XgcRBBuBNsTBAUd4hAzAwAAAOk1RgOtjufQAAAABJRU5ErkJggg=='>" << endl
       <<                   "                        </td>" << endl
       <<                   "                        <td>" << endl
       <<                   "                            <b>{{ treatment.posology_translation }}:</b> {{ single_dose.posology }}"
       <<                   "                        </td>" << endl
       <<                   "                    </tr>" << endl
       <<                   "                    <tr class='bg-warning-normal'>" << endl
       <<             "{% if existsIn( single_dose, \"warning\") %} "
       <<                   "                       <td>" << endl
       <<                   "                           <img alt='Warning icon image from asset/img/warning.png' src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAABHNCSVQICAgIfAhkiAAAAAlwSFlzAAAA3QAAAN0BcFOiBwAAABl0RVh0U29mdHdhcmUAd3d3Lmlua3NjYXBlLm9yZ5vuPBoAAASbSURBVFiFxZdtTJVlGMd/1/2cl+cgHOSgCKjTlGFEFDrUGAIBQ8DICeopRXHK8syXWetbH3RuuvVFp261am2t1Rc3+yK51cS5UEcvY9ZW0idbrJjhS04BhfN29eEArgx4Dtj6f7xf/tfvvq77fp77FlXl/5RruhM3bg/laDTccOtmX0Qi5sKlS1/emI6PJJuB9aFQimcw8plCA0D/jd8THcoXw2n2pu729gf/GUAwGPTF3KnngOq56bBqmZDlvc+ZzkGu98cALsaHBxu7uroeOvVMqgRRz6xDolQ/lS0c3m5ItQFm0/BCKvuO/8FPfbFq4007BLzl1NNxBja1tC1RtMcyeE+ELObP+Xv/3b4hmt6+TTTOiFj6zOWOjl+c+BqnpKp6DPA2rJTHggNkZKfwymoPgFejcsypryOA5q27qhGa/CkQrJhgiiW01ftJ9wkITWU1ddVPBCAYDFpGOAmwpcowy060D4fh9FdxPmi/x8ORRBk9gRT2vOgdNdaTwWDQmjFA3J26W6Fo8TyhdrmMt3dcVc5cVk5ffMDZKwOJRkt4qSyNvCyDKkV9d+7unhFAY0tLhsIRgF11gjyKz683E6s2xuJ6X2S8XdJ9vLk2kSZROVJe3pgxbQAb72Egs7RAKFwkkw19JEt4rmAWVctcAJm4w4enBdD86msFCnvdLthR6/iwJOS32V9j43GBwt7SytqCpAHEip0AXBtKhbnpycXHMsyb72PrKg+Ay2XJiaQAmrftagTqMv3QVJbk6sfkt9lW6iUrTVCoK6+qa3QEEAqF3KJyHGB7jcHrnl58LIMdsNlTmTiWKnq8pKTkMbfHAG4PRQ+A5i9bIJQ/63DjTSS/TW2hm6L5FkC+Ly1wYFKA5tbWLFQPCtBWP8PgAJYBv83rNV4EQDhYVleXNSGAibqPAulVxcLSnMkBvGP/UVVszyRj/TZP51isK3KjkG4iHP1XgKaWncUq2ubzQEvV1BuvuliYZYPHFWNdaerEA0ezEKrwkOIRFG0rq1pbPNY9fh8wcAowm8oNsyfxG1NervDhGxaqOZNnAMBvE7g/zI5SN+91ho0Ip4DK0bjQ3LJzM0hFdgAaVzuvvdfN1MFhPAvBEg8LMgxARXnN2s0AVm9vrx2OSTswe/96w8K5zgDuDcG7n8c5d+UOK/K9+LxTlM1jYQ2OkO0XLvwcBVj16ccfvW8GwqYVWPT8EmFlvvPVn7+qdPUo3/REOXt5cOoJo1lYk+di5WILYFFUPK1GVesBNq5J7tjl5YIAqnEKFnucTUpLfJR2lCbGq2q9C6HSCOQvSA5g+VLhnX0WkWgWC7Mc3m1dFhihMNfCCMSh0iD0xxV6epN/IWUHcB4cIBKDuPL9bzHiCiL0G1HtBDj7tTISmcphhhoYZjiinP4uDIDG6ZSXt+ye45bINSDL54XKIiE303k5HgxNvQEDPojcH+Fab5jz16IMhRXgplsjhaKqbNi2s8il8olC8VRm/9T40yw5/RCPaWtXZ8eP4w+TUCjkvjUQXY3oCkECTlxU1XWrvy/qNGpc+dNo/OrDgbvfdnd3R2Aaj9Mnrb8A0TtykI+7cqgAAAAASUVORK5CYII='>" << endl
       <<                   "                       </td>" << endl
       <<                   "                       <td>" << endl
       <<                   "                           {{ single_dose.warning }}"
       <<                   "                       </td>" << endl
       <<             "{% endif %}"
       <<                   "                    </tr>" << endl
       <<         "{% endfor %}"
       <<                   "               </table>" << endl
       <<                   "           </td>" << endl
       <<                   "       </tr>" << endl
       <<                   "   </table>" << endl
       <<         "{% if loop.is_first %}"
       <<                   "   </div>" << endl
       <<         "{% endif %}"
       <<     "{% endfor %}"
       << "{% endif %}"
       << endl

          // Patient's samples part:
          // If there is no samples
          //    Insert none
          // Else
          //    For each sample
          //        Insert a table with the sample information
          //        If there is a warning
          //            Insert the warning
          //
          // Note: Here, the tricky part is to have a <div class='avoid-break-inside'> that wraps
          //       the title and the first sample table for nice page break. This div needs
          //       to be closed when there are no samples or at the end of the first sample.

       <<               "    <!-- Samples -->" << endl
       <<               "    <div class='avoid-break-inside'>" << endl
       <<               "        <h3> {{samples.translation}} </h3>" << endl
       << "{% if not exists(\"samples.samples\") %}"
       <<               "        {{ samples.none_translation }}" << endl
       <<               "    </div>" << endl
       << "{% else %}"
       <<     "{% for sample in samples.samples %}"
       <<               "    <table class='sample bg-light-grey'>" << endl
       <<               "        <tr>" << endl
       <<               "            <td><b>{{ samples.date_translation }}:</b> {{ sample.date }}" << endl
       <<               "            <td><b>{{ samples.measure_translation }}:</b> {{ sample.measure }}</td>" << endl
       <<               "            <td><b>{{ samples.percentile_translation }}:</b> {{ sample.percentile }}</td>" << endl
       <<               "        </tr>" << endl
       <<               "        <tr>" << endl
       <<         "{% if existsIn( sample, \"warning\") %} "
       <<               "            <td colspan='3'>" << endl
       <<               "                <table>" << endl
       <<               "                    <tr class='bg-warning-{{ sample.warning_level }}'>" << endl
       <<               "                        <td>" << endl
       <<               "                            <img alt='Warning icon image from asset/img/warning.png' src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAABHNCSVQICAgIfAhkiAAAAAlwSFlzAAAA3QAAAN0BcFOiBwAAABl0RVh0U29mdHdhcmUAd3d3Lmlua3NjYXBlLm9yZ5vuPBoAAASbSURBVFiFxZdtTJVlGMd/1/2cl+cgHOSgCKjTlGFEFDrUGAIBQ8DICeopRXHK8syXWetbH3RuuvVFp261am2t1Rc3+yK51cS5UEcvY9ZW0idbrJjhS04BhfN29eEArgx4Dtj6f7xf/tfvvq77fp77FlXl/5RruhM3bg/laDTccOtmX0Qi5sKlS1/emI6PJJuB9aFQimcw8plCA0D/jd8THcoXw2n2pu729gf/GUAwGPTF3KnngOq56bBqmZDlvc+ZzkGu98cALsaHBxu7uroeOvVMqgRRz6xDolQ/lS0c3m5ItQFm0/BCKvuO/8FPfbFq4007BLzl1NNxBja1tC1RtMcyeE+ELObP+Xv/3b4hmt6+TTTOiFj6zOWOjl+c+BqnpKp6DPA2rJTHggNkZKfwymoPgFejcsypryOA5q27qhGa/CkQrJhgiiW01ftJ9wkITWU1ddVPBCAYDFpGOAmwpcowy060D4fh9FdxPmi/x8ORRBk9gRT2vOgdNdaTwWDQmjFA3J26W6Fo8TyhdrmMt3dcVc5cVk5ffMDZKwOJRkt4qSyNvCyDKkV9d+7unhFAY0tLhsIRgF11gjyKz683E6s2xuJ6X2S8XdJ9vLk2kSZROVJe3pgxbQAb72Egs7RAKFwkkw19JEt4rmAWVctcAJm4w4enBdD86msFCnvdLthR6/iwJOS32V9j43GBwt7SytqCpAHEip0AXBtKhbnpycXHMsyb72PrKg+Ay2XJiaQAmrftagTqMv3QVJbk6sfkt9lW6iUrTVCoK6+qa3QEEAqF3KJyHGB7jcHrnl58LIMdsNlTmTiWKnq8pKTkMbfHAG4PRQ+A5i9bIJQ/63DjTSS/TW2hm6L5FkC+Ly1wYFKA5tbWLFQPCtBWP8PgAJYBv83rNV4EQDhYVleXNSGAibqPAulVxcLSnMkBvGP/UVVszyRj/TZP51isK3KjkG4iHP1XgKaWncUq2ubzQEvV1BuvuliYZYPHFWNdaerEA0ezEKrwkOIRFG0rq1pbPNY9fh8wcAowm8oNsyfxG1NervDhGxaqOZNnAMBvE7g/zI5SN+91ho0Ip4DK0bjQ3LJzM0hFdgAaVzuvvdfN1MFhPAvBEg8LMgxARXnN2s0AVm9vrx2OSTswe/96w8K5zgDuDcG7n8c5d+UOK/K9+LxTlM1jYQ2OkO0XLvwcBVj16ccfvW8GwqYVWPT8EmFlvvPVn7+qdPUo3/REOXt5cOoJo1lYk+di5WILYFFUPK1GVesBNq5J7tjl5YIAqnEKFnucTUpLfJR2lCbGq2q9C6HSCOQvSA5g+VLhnX0WkWgWC7Mc3m1dFhihMNfCCMSh0iD0xxV6epN/IWUHcB4cIBKDuPL9bzHiCiL0G1HtBDj7tTISmcphhhoYZjiinP4uDIDG6ZSXt+ye45bINSDL54XKIiE303k5HgxNvQEDPojcH+Fab5jz16IMhRXgplsjhaKqbNi2s8il8olC8VRm/9T40yw5/RCPaWtXZ8eP4w+TUCjkvjUQXY3oCkECTlxU1XWrvy/qNGpc+dNo/OrDgbvfdnd3R2Aaj9Mnrb8A0TtykI+7cqgAAAAASUVORK5CYII='>" << endl
       <<               "                        </td>" << endl
       <<               "                        <td>" << endl
       <<               "                            {{ sample.warning }}" << endl
       <<               "                        </td>" << endl
       <<               "                    </tr>" << endl
       <<               "                </table>" << endl
       <<               "            </td>" << endl
       <<         "{% endif %}"
       <<               "        </tr>" << endl
       <<               "    </table>" << endl
       <<         "{% if loop.is_first %}"
       <<               "   </div>" << endl
       <<         "{% endif %}"
       <<     "{% endfor %}"
       << "{% endif %}"
       << endl

          // Adjustments found part:
          // If there is more than one adjustment
          //    Insert the title and the intro phrase above the canvas.
          //    For each adjustment
          //        Insert the score
          //        For each dosage time range
          //            Insert a table with the dosage time range information
          //            If there is a type
          //                Insert the dosage type (loop/steady state)
          //            Insert a table for each single dose
          //            For each single dose
          //                Insert the posology
          //                If there is a warning
          //                    Insert it
          //
          // Note: Here, the tricky part is to have a <div class='avoid-break-inside'> that wraps the title
          //       and the graph canvas to have them groupped. Additionally, we want such a div around
          //       the score of each ajustment and the first dosage time range. However, we must include
          //       "displayed adjustment" title in the first div we create. This is possible to achieve with
          //       loop.is_first and loop.parent.is_first in loops.

       <<                       "    <!-- Adjustments -->" << endl
       << "{% if length(adjustments.adjustments) > 1 %}"
       <<                       "    <div class='avoid-break-inside'>" << endl
       <<                       "       <h3> {{ adjustments.translation }} </h3>" << endl
       <<                       "       <div> {{ adjustments.intro_phrase_translation }} </div>" << endl
       <<                       "       <h4>{{ adjustments.per_interval_translation }}</h4>" << endl
       <<                       "       <div class='canvasAdjustments'>" << endl
       <<                       "           <canvas id='canAllAdj' width='716' height='474'></canvas>" << endl
       <<                       "       </div>" << endl
       <<                       "    </div>" << endl
       << endl
       <<     "{% for adjustment in adjustments.adjustments %}"
       <<         "{% for dosage_time_range in adjustment.dosage_time_ranges %}"
       <<             "{% if loop.is_first %}"
       <<                       "    <div class='avoid-break-inside'>" << endl
       <<                 "{% if loop.parent.is_first %}"
       <<                       "        <h4>{{ adjustments.displayed_adjustments_translation }}</h4>" << endl
       <<                 "{% endif %}"
       <<                       "        <div class='adjustment-score'><b>{{ adjustments.score_translation }}:</b> {{ adjustment.score }} / 1 </div>" << endl
       <<             "{% endif %}"
       <<                       "           <table class='dosage-time-range bg-light-grey'>" << endl
       <<                       "               <tr>" << endl
       <<                       "                   <td>" << endl
       <<                       "                       <b>{{ adjustments.from_translation }}:</b> {{ dosage_time_range.date_from }}" << endl
       <<                       "                   </td>" << endl
       <<                       "                   <td>" << endl
       <<                       "                       <b>{{ adjustments.to_translation }}:</b> {{ dosage_time_range.date_to }}" << endl
       <<                       "                   </td>" << endl
       <<                       "               </tr>" << endl
       <<                       "               <tr>" << endl
       <<                       "                   <td colspan='2'>" << endl
       <<                       "                       <table>" << endl
       <<             "{% for single_dose in dosage_time_range.single_doses %}"
       <<                       "                           <tr>" << endl
       <<                       "                               <td>" << endl
       <<                       "                                   <img alt='Dot icon image from asset/img/dot.png' src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABEAAAAPCAYAAAACsSQRAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAAANsAAADbAfBQ5pwAAACnSURBVDhPY6AlMADi2UB8Hoi/Q2kQ3wKICQIWIC4BYpDG/1jwbyBuAGKQOpwAZAA2zegYZBBWAPICLhegY5CLQOrBgAlKg0A2EHNAmAQByDvJECaqISZQmliANZCJ9QoMfwZiMEB2yQ0oTSy4A6VRDDkDpYkFWNWD/AgKdWxOR8cgdTgTHij+sWlCx91AjBOAog5kEC4XgcRBBuBNsTBAUd4hAzAwAAAOk1RgOtjufQAAAABJRU5ErkJggg=='>" << endl
       <<                       "                               </td>" << endl
       <<                       "                               <td>" << endl
       <<                       "                                   <b>{{ adjustments.posology_translation }}:</b> {{ single_dose.posology }}" << endl
       <<                       "                               </td>" << endl
       <<                       "                           </tr>" << endl
       <<             "{% endfor %}" << endl
       <<                       "                       </table>" << endl
       <<                       "                   </td>" << endl
       <<                       "               </tr>" << endl
       <<                       "           </table>" << endl
       <<             "{% if loop.is_first %}"
       <<                       "       </div>"
       <<             "{% endif %}"
       <<         "{% endfor %}"
       <<     "{% endfor %}"
       << "{% endif %}"


          // Adjustment suggestion part:
          // Insert the title, the phrase above the canvas
          // For the best adjustment adjustment (the first)
          //     Insert the score
          //     For each dosage time range
          //         Insert a table with the dosage time range information
          //         If there is a type
          //             Insert the dosage type (loop/steady state)
          //         Insert a table for each single dose
          //         For each single dose
          //             Insert the posology
          //             If there is a warning
          //                 Insert it
          //
          // Note: Here, the page breaking system is quiet simple. We simply add a <div class='avoid-break-inside'>
          //       around the "displayed adjustment" title, the score and the first dosage time range. This way,
          //       we avoid the title and the score to be alone in the bottom of a page.

       <<               "    <!-- Best adjustment -->" << endl
       <<               "    <div class='avoid-break-inside'>" << endl
       <<               "       <h3> {{ adjustments.adjustment_suggested_translation }} </h3>" << endl
       <<               "       <div> {{ adjustments.suggestion_phrase_translation }} </div></h4>" << endl
       <<               "       <div class='canvasAdjustments'>" << endl
       <<               "           <canvas id='canBestAdj' width='716' height='474'></canvas>" << endl
       <<               "       </div>" << endl
       <<               "    </div>" << endl
       << endl
       << "{% for dosage_time_range in adjustments.adjustments.0.dosage_time_ranges %}"
       <<     "{% if loop.is_first %}"
       <<               "   <div class='avoid-break-inside'>" << endl
       <<               "       <h4>{{ adjustments.displayed_adjustment_translation }}</h4>" << endl
       <<               "       <div class='adjustment-score'><b>{{ adjustments.score_translation }}:</b> {{ adjustments.adjustments.0.score }} / 1 </div>" << endl
       <<     "{% endif %}"
       <<               "       <table class='dosage-time-range bg-light-grey'>" << endl
       <<               "           <tr>" << endl
       <<               "               <td>" << endl
       <<               "                   <b>{{ adjustments.from_translation }}:</b> {{ dosage_time_range.date_from }}" << endl
       <<               "               </td>" << endl
       <<               "               <td>" << endl
       <<               "                   <b>{{ adjustments.to_translation }}:</b> {{ dosage_time_range.date_to }}" << endl
       <<               "               </td>" << endl
       <<               "           </tr>" << endl
       <<               "           <tr>" << endl
       <<               "               <td colspan='2'>" << endl
       <<               "                   <table>" << endl
       <<     "{% for single_dose in dosage_time_range.single_doses %}"
       <<               "                       <tr>" << endl
       <<               "                           <td>" << endl
       <<               "                               <img alt='Dot icon image from asset/img/dot.png' src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABEAAAAPCAYAAAACsSQRAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAAANsAAADbAfBQ5pwAAACnSURBVDhPY6AlMADi2UB8Hoi/Q2kQ3wKICQIWIC4BYpDG/1jwbyBuAGKQOpwAZAA2zegYZBBWAPICLhegY5CLQOrBgAlKg0A2EHNAmAQByDvJECaqISZQmliANZCJ9QoMfwZiMEB2yQ0oTSy4A6VRDDkDpYkFWNWD/AgKdWxOR8cgdTgTHij+sWlCx91AjBOAog5kEC4XgcRBBuBNsTBAUd4hAzAwAAAOk1RgOtjufQAAAABJRU5ErkJggg=='>" << endl
       <<               "                           </td>" << endl
       <<               "                           <td>" << endl
       <<               "                               <b>{{ adjustments.posology_translation }}:</b> {{ single_dose.posology }}" << endl
       <<               "                           </td>" << endl
       <<               "                       </tr>" << endl
       <<     "{% endfor %}" << endl
       <<               "                   </table>" << endl
       <<               "               </td>" << endl
       <<               "           </tr>" << endl
       <<               "       </table>" << endl
       <<     "{% if loop.is_first %}"
       <<               "   </div>"
       <<     "{% endif %}"
       << "{% endfor %}"
       << endl

          // Targets part:
          // For each target
          //    Insert its type, its values and bounds in a table
          //
          // Note: Here, the page breaking system requires to wrap the target evolution intro phrase
          //       and the table col headers in a <div class='avoid-break-inside'> with the first target.
          //       This way, we avoid to have a weird page break that leaves the phrase and the headers alone.

       <<               "    <!-- Targets -->" << endl
       << "{% for target in targets.targets %}"
       <<     "{% if loop.is_first %}"
       <<               "    <div class='avoid-break-inside'>" << endl
       <<               "        <div> {{ targets.phrase_translation }} </div>" << endl
       <<               "        <br>" << endl
       <<               "        <table class='targets-header'>" << endl
       <<               "            <tr>" << endl
       <<               "                <th></th>" << endl
       <<               "                <th>Type</th>" << endl
       <<               "                <th>Value (unit)</th>" << endl
       <<               "                <th>Score</th>" << endl
       <<               "            </tr>"
       <<               "        </table>" << endl
       <<     "{% endif %}"
       <<               "       <table class='target bg-light-grey'>" << endl
       <<               "           <tr>" << endl
       <<               "               <td>" << endl
       <<               "                   <img alt='Dot icon image from asset/img/dot.png' src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABEAAAAPCAYAAAACsSQRAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAAANsAAADbAfBQ5pwAAACnSURBVDhPY6AlMADi2UB8Hoi/Q2kQ3wKICQIWIC4BYpDG/1jwbyBuAGKQOpwAZAA2zegYZBBWAPICLhegY5CLQOrBgAlKg0A2EHNAmAQByDvJECaqISZQmliANZCJ9QoMfwZiMEB2yQ0oTSy4A6VRDDkDpYkFWNWD/AgKdWxOR8cgdTgTHij+sWlCx91AjBOAog5kEC4XgcRBBuBNsTBAUd4hAzAwAAAOk1RgOtjufQAAAABJRU5ErkJggg=='>" << endl
       <<               "               </td>" << endl
       <<               "               <td>{{ target.type }}</td>" << endl
       <<               "               <td>{{ target.value }}</td>" << endl
       <<               "               <td>{{ target.score }} / 1</td>" << endl
       <<               "           </tr>" << endl
       <<               "           <tr>" << endl
       <<               "               <td colspan='4'><div>{{ target.bounds }}</div></td>" << endl
       <<               "           </tr>" << endl
       <<               "       </table>" << endl
       <<     "{% if loop.is_first %}"
       <<               "   </div>"
       <<     "{% endif %}"
       << "{% endfor %}"
       << endl

          // Pharmacokinetic parameters:
          // For each parameter
          //    Insert its name and value
          //
          // Note: Here, the page breaking system requires to wrap the title
          //       and the table column headers in a <div class='avoid-break-inside'> with the first parameter.
          //

       <<               "        <!-- Pharmacokinetic parameters -->" << endl
       << "{% for parameter in pks.pks %}"
       <<     "{% if loop.is_first %}"
       <<               "    <div class='avoid-break-inside'>" << endl
       <<               "        <h4> {{ pks.translation }} </h4>" << endl
       <<               "        <table class='pk-header'>" << endl
       <<               "            <tr>" << endl
       <<               "                <th></th>" << endl
       <<               "                <th></th>" << endl
       <<               "                <th>{{ pks.typical_patient_translation }}</th>" << endl
       <<               "                <th>{{ pks.a_priori_translation }}</th>" << endl
       <<               "                <th>{{ pks.a_posteriori_translation }}</th>" << endl
       <<               "            </tr>" << endl
       <<               "        </table>" << endl
       <<     "{% endif %}"
       <<               "       <table class='pk bg-light-grey'>" << endl
       <<               "           <tr>" << endl
       <<               "               <td>" << endl
       <<               "                   <img alt='Dot icon image from asset/img/dot.png' src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABEAAAAPCAYAAAACsSQRAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAAANsAAADbAfBQ5pwAAACnSURBVDhPY6AlMADi2UB8Hoi/Q2kQ3wKICQIWIC4BYpDG/1jwbyBuAGKQOpwAZAA2zegYZBBWAPICLhegY5CLQOrBgAlKg0A2EHNAmAQByDvJECaqISZQmliANZCJ9QoMfwZiMEB2yQ0oTSy4A6VRDDkDpYkFWNWD/AgKdWxOR8cgdTgTHij+sWlCx91AjBOAog5kEC4XgcRBBuBNsTBAUd4hAzAwAAAOk1RgOtjufQAAAABJRU5ErkJggg=='>" << endl
       <<               "               </td>" << endl
       <<               "               <td> {{ parameter.name }} </td>" << endl
       <<               "               <td> {{ default( parameter.typical_patient, \"-\") }} </td>" << endl
       <<               "               <td> {{ default( parameter.a_priori, \"-\") }} </td>" << endl
       <<               "               <td> {{ default( parameter.a_posteriori, \"-\") }} </td>" << endl
       <<               "        </tr>" << endl
       <<               "       </table>" << endl
       <<     "{% if loop.is_first %}"
       <<               "   </div>"
       <<     "{% endif %}"
       << "{% endfor %}"
       << endl

          // Predictions part:
          // Insert the steady state auc24
          // Insert the steady state peak
          // Insert the steady state residual
          //
          // Note: Since the table has a small fixed size, just wrap everything in
          //       a <div class='avoid-break-inside'>.


       <<               "    <!-- Predictions -->" << endl
       <<               "    <div class='avoid-break-inside'>" << endl
       <<               "        <h4 class='underline'> {{ predictions.translation }}</h4>" << endl
       <<               "        <table class='predictions bg-light-grey'>" << endl
       <<               "            <tr>" << endl
       <<               "                <th>{{ predictions.extrapolated_steady_state_auc24_translation }}</th>" << endl
       <<               "                <td>{{ predictions.extrapolated_steady_state_auc24 }}</td>" << endl
       <<               "            </tr>" << endl
       <<               "            <tr>" << endl
       <<               "                <th>{{ predictions.steady_state_peak_translation }}</th>" << endl
       <<               "                <td>{{ predictions.steady_state_peak }}</td>" << endl
       <<               "            </tr>" << endl
       <<               "            <tr>" << endl
       <<               "                <th>{{ predictions.steady_state_residual_translation }}</th>" << endl
       <<               "                <td>{{ predictions.steady_state_residual }}</td>" << endl
       <<               "            </tr>" << endl
       <<               "        </table>" << endl
       <<               "    </div>" << endl
       << endl

          // Computation covariates part:
          // For each computation covariate
          //    Insert a table with its name and value
          //
          // Note: The page break simple is like the one in the targets. We wrap the title, the headers and
          //       the first computation covariate together with <div class='avoid-break-inside'>.
          //       This way, the title and the headers won't be left alone.

       <<               "    <!-- Computation covariates -->" << endl
       << "{% for covariate in computation_covariates.computation_covariates %}"
       <<     "{% if loop.is_first %}"
       <<               "    <div class='avoid-break-inside'>" << endl
       <<               "        <h4> {{ computation_covariates.translation}} </h4>" << endl
       <<               "        <table class='computation-covariate-header'>" << endl
       <<               "            <tr>" << endl
       <<               "                <th></th>" << endl
       <<               "                <th> {{ computation_covariates.covariate_translation}} </th>" << endl
       <<               "                <th> {{ computation_covariates.value_translation}} </th>" << endl
       <<               "            </tr>" << endl
       <<               "        </table>" << endl
       <<     "{% endif %}"
       <<               "       <table class='computation-covariate bg-light-grey'>" << endl
       <<               "           <tr>" << endl
       <<               "               <td>" << endl
       <<               "                   <img alt='Dot icon image from asset/img/dot.png' src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABEAAAAPCAYAAAACsSQRAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAAANsAAADbAfBQ5pwAAACnSURBVDhPY6AlMADi2UB8Hoi/Q2kQ3wKICQIWIC4BYpDG/1jwbyBuAGKQOpwAZAA2zegYZBBWAPICLhegY5CLQOrBgAlKg0A2EHNAmAQByDvJECaqISZQmliANZCJ9QoMfwZiMEB2yQ0oTSy4A6VRDDkDpYkFWNWD/AgKdWxOR8cgdTgTHij+sWlCx91AjBOAog5kEC4XgcRBBuBNsTBAUd4hAzAwAAAOk1RgOtjufQAAAABJRU5ErkJggg=='>" << endl
       <<               "               </td>" << endl
       <<               "               <td>{{ covariate.name }}</td>" << endl
       <<               "               <td>{{ covariate.value }}</td>" << endl
       <<               "           </tr>" << endl
       <<               "       </table>" << endl
       <<     "{% if loop.is_first %}"
       <<               "   </div>"
       <<     "{% endif %}"
       << "{% endfor %}"
       << endl
       <<               "    <!-- Copyright -->" << endl
       <<               "    <div class='copyright'>" << endl
       <<               "        <span>Copyright (c) HEIG-VD/CHUV - 2022 | Icons by <a href='https://www.flaticon.com/fr/auteurs/gajah-mada'> Gajah Mada - Flaticon</a> & <a href='https://www.flaticon.com/fr/auteurs/freepik'>Freepik - Flaticon</a></span>" << endl
       <<               "    </div>" << endl
       << endl
       <<               "    <!-- Ending clean / otherwise we see background on pdf printing -->" << endl
       <<               "    <div class='ending'/>" << endl
       << endl
       <<               "</div>" << endl
       <<               "<script>" << endl
       << endl

          // Data for graphs part:
          // For each adjustment
          //    If this is not the first adjustment
          //        Add a comma in front of the array
          //    For each cycle data
          //        If this is not the first cycle data
          //            Add a comma in front of the array
          //        Insert the cycle data start date
          //        Insert the cycle data time offsets
          //        Insert the cycle data concentrations
          // It gives something like: adjustments [ adjustment [ CycleData [ start, times : [...], values : [....] ], ... ], ... ]

       <<               "    var adustmentsData = [" << endl
       << "{% for adjustment in graph_data.adjustments %}"
       <<     "{% if not loop.is_first %}, {% endif %}"
       <<               "          [" << endl
       <<     "{% for cycle in adjustment.cycles %}"
       <<                   "{% if not loop.is_first %}, {% endif %}"
       <<               "              [" << endl
       <<               "              '{{ cycle.start }}'," << endl
       <<               "              [{{ cycle.times }}]," << endl
       <<               "              [{{ cycle.values }}]," << endl
       <<               "              ]" << endl
       <<     "{% endfor %}"
       <<               "          ]" << endl
       <<"{% endfor %}"
       <<               "    ];" << endl


          // Target data part:
          // For each target
          //    Add a comma if not first
          //    Insert target type, min, best and max values
          // It gives something like: targets [ target [type, min, best, max], ... ]

       <<               "    var targets = [" << endl
       << "{% for target in graph_data.targets %}"
       <<     "{% if not loop.is_first %}, {% endif %}"
       <<               "               [ '{{ target.type }}', {{ target.min }} , {{ target.best }}, {{ target.max }} ] " << endl
       << "{% endfor %}"
       <<               "    ];"
       <<               "    var obj = new GraphFullData();" << endl
       <<               "    addTargets(obj, targets);" << endl
       <<               "    displayGraphs(obj, adustmentsData, {{graph_data.adjustment_date}}, {{graph_data.end_date}});" << endl
       <<               "</script>" << endl
       <<               "</body>" << endl
       <<               "</html>" << endl;


    // Preparing the data to insert
    inja::json xpertRequestResultJson;
    getHeaderJson(_xpertRequestResult, xpertRequestResultJson["header"]);
    getDrugIntroJson(_xpertRequestResult, xpertRequestResultJson["intro"]);
    getContactsJson(_xpertRequestResult.getXpertQueryResult().getAdminData(), xpertRequestResultJson["contacts"]);
    getClinicalDatasJson(_xpertRequestResult.getXpertQueryResult().getAdminData(), xpertRequestResultJson["clinical_datas"]);
    getCovariatesJson(_xpertRequestResult.getCovariateValidationResults(), xpertRequestResultJson["covariates"]);
    getTreatmentJson(_xpertRequestResult.getTreatment()->getDosageHistory(), xpertRequestResultJson["treatment"]);
    getSamplesJson(_xpertRequestResult.getSampleValidationResults(), xpertRequestResultJson["samples"]);
    getAdjustmentsJson(_xpertRequestResult.getAdjustmentData(), xpertRequestResultJson["adjustments"]);
    getTargetsJson(_xpertRequestResult.getAdjustmentData(), xpertRequestResultJson["targets"]);
    getParametersJson(_xpertRequestResult, xpertRequestResultJson["pks"]);
    getPredictionsJson(_xpertRequestResult, xpertRequestResultJson["predictions"]);
    getComputationCovariatesJson(_xpertRequestResult, xpertRequestResultJson["computation_covariates"]);
    getGraphDataJson(_xpertRequestResult, xpertRequestResultJson["graph_data"]);

    return inja::render(templateStream.str(), xpertRequestResultJson);
}

void XpertRequestResultHtmlExport::getHeaderJson(const XpertRequestResult& _xpertRequestResult, inja::json& _headerJson) const
{
    LanguageManager& langMgr = LanguageManager::getInstance();

    // TuberXpert report translation
    _headerJson["tuberxpert_report_translation"] = langMgr.translate("tuberxpert_report");

    // Computed on translation
    _headerJson["computed_on_translation"] = langMgr.translate("computed_on");

    // Computation time
    _headerJson["computation_time"] = dateTimeToString(_xpertRequestResult.getXpertQueryResult().getComputationTime(), false);
}

void XpertRequestResultHtmlExport::getDrugIntroJson(const XpertRequestResult& _xpertRequestResult, inja::json& _introJson) const
{
    LanguageManager& langMgr = LanguageManager::getInstance();

    // Drug translation
    _introJson["drug_translation"] = langMgr.translate("drug");

    // Drug id translation
    _introJson["id_translation"] = langMgr.translate("id");

    // Last dose translation
    _introJson["last_dose_translation"] = langMgr.translate("last_dose");

    // Drug model translation
    _introJson["drug_model_translation"] = langMgr.translate("drug_model");

    // Drug id
    _introJson["drug_id"] = _xpertRequestResult.getXpertRequest().getDrugId();

    // Last dose
    if (_xpertRequestResult.getLastIntake() != nullptr) {
        _introJson["last_dose"] = doubleToString(_xpertRequestResult.getLastIntake()->getDose()) + " " + _xpertRequestResult.getLastIntake()->getUnit().toString();
    }

    // Drug model
    _introJson["drug_model"] =  _xpertRequestResult.getDrugModel()->getDrugModelId();
}

void XpertRequestResultHtmlExport::getContactsJson(const unique_ptr<AdminData>& _admin, inja::json& _contactsJson) const
{
    // Get the rows and columns header translations
    LanguageManager& langMgr = LanguageManager::getInstance();

    // Contacts translation
    _contactsJson["translation"] = langMgr.translate("contacts");

    // Mandator translation
    _contactsJson["mandator_translation"] = langMgr.translate("mandator");

    // Patient translation
    _contactsJson["patient_translation"] = langMgr.translate("patient");

    // Id translation
    _contactsJson["id_translation"] = langMgr.translate("id");

    // Name translation
    _contactsJson["name_translation"] = langMgr.translate("name");

    // Address translation
    _contactsJson["address_translation"] = langMgr.translate("address");

    // Phome translation
    _contactsJson["phone_translation"] = langMgr.translate("phone");

    // Email translation
    _contactsJson["email_translation"] = langMgr.translate("email");


    // Try to get mandator and patient available information
    if (_admin != nullptr) {
        getPersonJson(_admin->getMandator(), _contactsJson["mandator"]);
        getPersonJson(_admin->getPatient(), _contactsJson["patient"]);
    }
}

void XpertRequestResultHtmlExport::getPersonJson(const unique_ptr<FullPersonData>& _person, inja::json& _personJson) const
{
    if (_person == nullptr) {
        return;
    }

    // Id
    string id = _person->getPerson().getId();
    if (id != "") {
        _personJson["id"] = id;
    }

    // Name
    string title = _person->getPerson().getTitle();
    stringstream nameStream;
    if (title != "") {
        nameStream << title << " ";
    }
    nameStream << _person->getPerson().getFirstName() << " " << _person->getPerson().getLastName();
    _personJson["name"] = nameStream.str();

    // Address
    getAddressJson(_person->getPerson().getAddress(), _personJson["address"]);

    // Phone
    getPhoneJson(_person->getPerson().getPhone(), _personJson["phone"]);

    // Email
    getEmailJson(_person->getPerson().getEmail(), _personJson["email"]);

    // Institute
    getInstituteJson(_person->getInstitute(), _personJson["institute"]);

}

void XpertRequestResultHtmlExport::getInstituteJson(const unique_ptr<InstituteData>& _institute, inja::json& _instituteJson) const
{
    if (_institute == nullptr) {
        return;
    }

    // Id
    string id = _institute->getId();
    if (id != "") {
        _instituteJson["id"] = id;
    }

    // Name
    _instituteJson["name"] = _institute->getName();

    // Address
    getAddressJson(_institute->getAddress(), _instituteJson["address"]);

    // Phone
    getPhoneJson(_institute->getPhone(), _instituteJson["phone"]);

    // Email
    getEmailJson(_institute->getEmail(), _instituteJson["email"]);
}

void XpertRequestResultHtmlExport::getAddressJson(const unique_ptr<AddressData>& _address, inja::json& _addressJson) const
{
    if (_address == nullptr) {
        _addressJson = "-";
        return;
    }

    stringstream addressStream;

    // Street
    // Postal code
    // City
    addressStream << _address->getStreet() << "<br>"
       << Common::Utils::varToString(_address->getPostalCode()) << "<br>"
       << _address->getCity();

    // State
    string state = _address->getState();
    if (state != "") {
        addressStream << "<br>" << state;
    }

    // Country
    string country = _address->getCountry();
    if (country != "") {
        addressStream << "<br>" << country;
    }

    _addressJson = addressStream.str();
}

void XpertRequestResultHtmlExport::getPhoneJson(const unique_ptr<PhoneData>& _phone, inja::json& _phoneJson) const
{
    if (_phone == nullptr) {
        _phoneJson = "-";
        return;
    }

    LanguageManager& langMgr = LanguageManager::getInstance();
    stringstream phoneStream;

    // Number
    phoneStream << _phone->getNumber();

    // Type
    string type = _phone->getType();
    if (type != "") {
        phoneStream << " (" << langMgr.translate(type) << ")";
    }

    _phoneJson = phoneStream.str();
}

void XpertRequestResultHtmlExport::getEmailJson(const unique_ptr<EmailData>& _email, inja::json& _emailJson) const
{
    if (_email == nullptr) {
        _emailJson = "-";
        return;
    }

    LanguageManager& langMgr = LanguageManager::getInstance();
    stringstream emailStream;

    // Address
    emailStream << _email->getAddress();

    // Type
    string type = _email->getType();
    if (type != "") {
        emailStream << " (" << langMgr.translate(type) << ")";
    }

    _emailJson = emailStream.str();
}

void XpertRequestResultHtmlExport::getClinicalDatasJson(const unique_ptr<AdminData>& _admin, inja::json& _clinicalDatasJson) const
{

    LanguageManager& lm = LanguageManager::getInstance();

    // Clinical data translation
    _clinicalDatasJson["clinical_data_translation"] = lm.translate("clinical_data");

    // None translation
    _clinicalDatasJson["none_translation"] = lm.translate("none");

    // If there are clinical data
    if (_admin != nullptr &&
            _admin->getClinicalDatas() != nullptr) {

        // For each clinical data
        for (auto clinicalDataIt = _admin->getClinicalDatas()->getData().begin();
             clinicalDataIt != _admin->getClinicalDatas()->getData().end();
             ++clinicalDataIt){

            // Create json value
            inja::json clinicalDataJson;
            clinicalDataJson["key"] = keyToPhrase(clinicalDataIt->first);
            clinicalDataJson["value"] = clinicalDataIt->second;

            // Add the row to the row list
            _clinicalDatasJson["clinical_datas"].emplace_back(clinicalDataJson);
        }
    }
}

void XpertRequestResultHtmlExport::getCovariatesJson(const vector<CovariateValidationResult>& _covariateResults, inja::json& _covariatesJson) const
{
    LanguageManager& langMgr = LanguageManager::getInstance();

    // Clinical data translation
    _covariatesJson["translation"] = langMgr.translate("covariates");

    // None translation
    _covariatesJson["none_translation"] = langMgr.translate("none");

    // Value translation
    _covariatesJson["value_translation"] = langMgr.translate("value");

    // Date translation
    _covariatesJson["date_translation"] = langMgr.translate("date");

    // For each covariate result
    for (const CovariateValidationResult& cvr : _covariateResults) {

        inja::json covariateJson;

        // Get the covariate name translated or english
        covariateJson["name"] = getStringWithEnglishFallback(cvr.getSource()->getName(), m_xpertRequestResultInUse->getXpertRequest().getOutputLang());

        // Get the covariate description
        covariateJson["desc"] = getStringWithEnglishFallback(cvr.getSource()->getDescription(), m_xpertRequestResultInUse->getXpertRequest().getOutputLang());

        // Get the covariate warning
        getWarningJson(cvr, covariateJson);

        // Get the covariate value.
        string value =  beautifyString(cvr.getValue(), cvr.getDataType(), cvr.getSource()->getId());

        string unit = cvr.getUnit().toString();

        // Format the value + unit + source
        stringstream valueStream;

        if (cvr.getSource()->getId() == "age" && cvr.getPatient() != nullptr) {
            valueStream << int(getAgeIn(cvr.getSource()->getType(),
                                        cvr.getPatient()->getValueAsDate(),
                                        m_xpertRequestResultInUse->getXpertQueryResult().getComputationTime()));
        } else {
            valueStream << value;
        }

        if (unit != "-" && unit != "") {
            valueStream << " " << unit;
        }

        valueStream << " (" << langMgr.translate("source_" + covariateTypeToString(cvr.getType())) << ")";

        covariateJson["value"] = valueStream.str();

        // Get the covariate date if it is a patient covariate
        if (cvr.getPatient() != nullptr) {
            covariateJson["date"] = dateTimeToString(cvr.getPatient()->getEventTime(), false);
        }

        _covariatesJson["covariates"].emplace_back(covariateJson);
    }
}

void XpertRequestResultHtmlExport::getTreatmentJson(const Core::DosageHistory& _history, inja::json& _treatmentJson) const
{
    LanguageManager& langMgr = LanguageManager::getInstance();

    // Treatment translation
    _treatmentJson["translation"] = langMgr.translate("treatment");

    // None translation
    _treatmentJson["none_translation"] = langMgr.translate("none");

    // From translation
    _treatmentJson["from_translation"] = langMgr.translate("from");

    // To translation
    _treatmentJson["to_translation"] = langMgr.translate("to");

    // Type translation
    _treatmentJson["type_translation"] = langMgr.translate("type");

    // Posologie translation
    _treatmentJson["posology_translation"] = langMgr.translate("posology");

    // Formulation and route translation
    _treatmentJson["formulation_translation"] = langMgr.translate("formulation");

    // Export the dosage time ranges
    for (const auto& dosageTimeRange : _history.getDosageTimeRanges()) {
        inja::json timeRangeJson;
        getTimeRangeJson(dosageTimeRange, timeRangeJson);
        _treatmentJson["dosage_time_ranges"].emplace_back(timeRangeJson);
    }
}

void XpertRequestResultHtmlExport::getTimeRangeJson(const unique_ptr<Core::DosageTimeRange>& _timeRange, inja::json& _dosageTimeRangeJson) const
{
    // Set date from value
    stringstream fromDateStream;
    _dosageTimeRangeJson["date_from"] = dateTimeToString(_timeRange->getStartDate());

    // Set date to value
    stringstream toDateStream;
    _dosageTimeRangeJson["date_to"] = dateTimeToString(_timeRange->getEndDate());

    getAbstractDosageJson(*_timeRange->getDosage(), _dosageTimeRangeJson, "");
}

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define TRY_GET_JSON(Type)                                                                                                    \
    if (dynamic_cast<const Tucuxi::Core::Type*>(&_dosage)) {                                                                  \
    return getDosageJson(*dynamic_cast<const Tucuxi::Core::Type*>(&_dosage), _dosageTimeRangeJson, _posologyIndicationChain); \
}

void XpertRequestResultHtmlExport::getAbstractDosageJson(const Core::Dosage& _dosage, inja::json& _dosageTimeRangeJson, const string& _posologyIndicationChain) const
{
    // The calls order is important here.
    // First start with the subclasses, else it won't work.
    TRY_GET_JSON(WeeklyDose);
    TRY_GET_JSON(DailyDose);
    TRY_GET_JSON(LastingDose);
    TRY_GET_JSON(ParallelDosageSequence);
    TRY_GET_JSON(DosageLoop);
    TRY_GET_JSON(DosageSteadyState);
    TRY_GET_JSON(DosageRepeat);
    TRY_GET_JSON(DosageSequence);
}

void XpertRequestResultHtmlExport::getDosageJson(const Core::DosageLoop& _dosage, inja::json& _dosageTimeRangeJson, const string& _posologyIndicationChain) const
{
    // Set the type of the dosage time range
    LanguageManager& langMgr = LanguageManager::getInstance();
    _dosageTimeRangeJson["type"] = langMgr.translate("continually");

    // Keep digging into the dosage tree
    getAbstractDosageJson(*_dosage.getDosage(), _dosageTimeRangeJson, _posologyIndicationChain);
}

void XpertRequestResultHtmlExport::getDosageJson(const Core::DosageSteadyState& _dosage, inja::json& _dosageTimeRangeJson, const string& _posologyIndicationChain) const
{
    // Set the type of the dosage time range.
    LanguageManager& langMgr = LanguageManager::getInstance();

    stringstream typeStream;
    typeStream << langMgr.translate("at_steady_state") << " " << dateTimeToString(_dosage.getLastDoseTime());
    _dosageTimeRangeJson["type"] = typeStream.str();

    // Keep digging into the dosage tree.
    getAbstractDosageJson(*_dosage.getDosage(), _dosageTimeRangeJson, _posologyIndicationChain);
}

void XpertRequestResultHtmlExport::getDosageJson(const Core::DosageRepeat& _dosage, inja::json& _dosageTimeRangeJson, const string& _posologyIndicationChain) const
{
    // Add indication in the posology indication chain
    LanguageManager& langMgr = LanguageManager::getInstance();

    stringstream posologyStream;
    posologyStream << _dosage.getNbTimes() << " " << langMgr.translate("times");
    string newPosologyIndicationChain = prefixPosology(posologyStream.str(), _posologyIndicationChain);

    // Keep digging into the dosage tree.
    getAbstractDosageJson(*_dosage.getDosage(), _dosageTimeRangeJson, newPosologyIndicationChain);
}

void XpertRequestResultHtmlExport::getDosageJson(const Core::DosageSequence& _dosage, inja::json& _dosageTimeRangeJson, const string& _posologyIndicationChain) const
{
    // Iterate over the dosages.
    for (const unique_ptr<Tucuxi::Core::DosageBounded>& dosage : _dosage.getDosageList()) {
        getAbstractDosageJson(*dosage, _dosageTimeRangeJson, _posologyIndicationChain);
    }
}

void XpertRequestResultHtmlExport::getDosageJson(const Core::ParallelDosageSequence& _dosage, inja::json& _dosageTimeRangeJson, const string& _posologyIndicationChain) const
{
    LanguageManager& langMgr = LanguageManager::getInstance();

    auto timeOffsetIt = _dosage.getOffsetsList().begin();

    // For each dosage, add the offset in the posology indication chain.
    for (const unique_ptr<Tucuxi::Core::DosageBounded>& dosage : _dosage.getDosageList()) {

        stringstream offsetStream;
        offsetStream << langMgr.translate("offset") << " " << TimeOfDayToString(TimeOfDay::buildUnnormalized(*timeOffsetIt));
        string newPosologyIndicationChain = prefixPosology(offsetStream.str(), _posologyIndicationChain);

        getAbstractDosageJson(*dosage,  _dosageTimeRangeJson, newPosologyIndicationChain);
        timeOffsetIt++;
    }
}

void XpertRequestResultHtmlExport::getDosageJson(const Core::LastingDose& _dosage, inja::json& _dosageTimeRangeJson, const string& _posologyIndicationChain) const
{
    // Add the indication in the posology indication chain
    LanguageManager& langMgr = LanguageManager::getInstance();

    stringstream posologyStream;
    posologyStream << langMgr.translate("interval") << " " << TimeOfDayToString(TimeOfDay::buildUnnormalized(_dosage.getTimeStep()));
    string newPosologyIndicationChain = prefixPosology(posologyStream.str(), _posologyIndicationChain);

    // Export the single dose
    getSingleDoseJson(_dosage, _dosageTimeRangeJson, newPosologyIndicationChain);
}

void XpertRequestResultHtmlExport::getDosageJson(const Core::DailyDose& _dosage, inja::json& _dosageTimeRangeJson, const string& _posologyIndicationChain) const
{
    // Add the indication in the posology indication chain
    LanguageManager& langMgr = LanguageManager::getInstance();

    stringstream posologyStream;
    posologyStream << langMgr.translate("daily_at") << " " << TimeOfDayToString(_dosage.getTimeOfDay());
    string newPosologyIndicationChain = prefixPosology(posologyStream.str(), _posologyIndicationChain);

    // Export the single dose
    getSingleDoseJson(_dosage, _dosageTimeRangeJson, newPosologyIndicationChain);
}

void XpertRequestResultHtmlExport::getDosageJson(const Core::WeeklyDose& _dosage, inja::json& _dosageTimeRangeJson, const string& _posologyIndicationChain) const
{
    // Add the indication in the posology indication chain
    LanguageManager& langMgr = LanguageManager::getInstance();

    stringstream posologyStream;
    posologyStream << langMgr.translate("every") << " " << langMgr.translate("day_" + to_string(_dosage.getDayOfWeek().operator unsigned int()))
                   << " " <<langMgr.translate("at") << " " << TimeOfDayToString(_dosage.getTimeOfDay());
    string newPosologyIndicationChain = prefixPosology(posologyStream.str(), _posologyIndicationChain);

    // Export the single dose
    getSingleDoseJson(_dosage, _dosageTimeRangeJson, newPosologyIndicationChain);
}

void XpertRequestResultHtmlExport::getSingleDoseJson(const Core::SingleDose& _dosage, inja::json& _dosageTimeRangeJson, const string& _posologyIndicationChain) const
{
    // Get the route
    LanguageManager& langMgr = LanguageManager::getInstance();
    static map<Tucuxi::Core::AdministrationRoute, string> routes = {
        {Tucuxi::Core::AdministrationRoute::Oral, "oral"},
        {Tucuxi::Core::AdministrationRoute::Nasal, "nasal"},
        {Tucuxi::Core::AdministrationRoute::Rectal, "rectal"},
        {Tucuxi::Core::AdministrationRoute::Vaginal, "vaginal"},
        {Tucuxi::Core::AdministrationRoute::Undefined, "undefined"},
        {Tucuxi::Core::AdministrationRoute::Sublingual, "sublingual"},
        {Tucuxi::Core::AdministrationRoute::Transdermal, "transdermal"},
        {Tucuxi::Core::AdministrationRoute::Subcutaneous, "subcutaneous"},
        {Tucuxi::Core::AdministrationRoute::Intramuscular, "intramuscular"},
        {Tucuxi::Core::AdministrationRoute::IntravenousDrip, "intravenous_drip"},
        {Tucuxi::Core::AdministrationRoute::IntravenousBolus, "intravenous_bolus"}};

    auto it = routes.find(_dosage.getLastFormulationAndRoute().getAdministrationRoute());

    // Get the dosage
    stringstream posologyStream;
    posologyStream << doubleToString(_dosage.getDose()) << " " << _dosage.getDoseUnit().toString();

    // If the route is defined, add it to the dosage
    if (it != routes.end() && it->second != "undefined") {
        posologyStream << " (" << langMgr.translate(it->second) << ")";
    }

    string newPosologyIndicationChain = prefixPosology(posologyStream.str(), _posologyIndicationChain);

    // Add the posology of the single dose
    inja::json singleDoseJson;
    singleDoseJson["posology"] = newPosologyIndicationChain;

    // Add the potential warning
    auto singleDoseIt = m_xpertRequestResultInUse->getDoseValidationResults().find(&_dosage);
    if (singleDoseIt !=  m_xpertRequestResultInUse->getDoseValidationResults().end()){
        getWarningJson(singleDoseIt->second, singleDoseJson);
    }

    // Add the single dose to the single doses list of the time range.
    _dosageTimeRangeJson["single_doses"].emplace_back(singleDoseJson);
}

void XpertRequestResultHtmlExport::getSamplesJson(const vector<SampleValidationResult>& _sampleResults, inja::json& _samplesJson) const
{
    LanguageManager& langMgr = LanguageManager::getInstance();

    // Get samples translation
    _samplesJson["translation"] = langMgr.translate("samples");

    // Get none translation
    _samplesJson["none_translation"] = langMgr.translate("none");

    // Get measure translation
    _samplesJson["measure_translation"] = langMgr.translate("measure");

    // Get date translation
    _samplesJson["date_translation"] = langMgr.translate("date");

    // Get percentile translation
    _samplesJson["percentile_translation"] = langMgr.translate("percentile");

    // For each sample validation result
    for (const auto& sampleValidationResult : _sampleResults) {

        inja::json sampleJson;

        // Get the sample date
        stringstream dateStream;
        sampleJson["date"] = dateTimeToString(sampleValidationResult.getSource()->getDate());

        // Get the sample measure
        stringstream valueStream;
        valueStream << sampleValidationResult.getSource()->getValue() << " " << sampleValidationResult.getSource()->getUnit().toString();
        sampleJson["measure"] = valueStream.str();

        // Get the percentile
        sampleJson["percentile"] = to_string(sampleValidationResult.getGroupNumberOver99Percentile());

        // Get the warning
        getWarningJson(sampleValidationResult, sampleJson);

        // Get the warning level
        sampleJson["warning_level"] = warningLevelToString(sampleValidationResult.getWarningLevel());

        _samplesJson["samples"].emplace_back(sampleJson);
    }
}

void XpertRequestResultHtmlExport::getAdjustmentsJson(const unique_ptr<Core::AdjustmentData>& _adjustmentData, inja::json& _adjustmentsJson) const
{
    LanguageManager& langMgr = LanguageManager::getInstance();

    // Get adjustments translation
    _adjustmentsJson["translation"] = langMgr.translate("adjustments");

    // Get intro phrase translation
    _adjustmentsJson["intro_phrase_translation"] = langMgr.translate("intro_phrase");

    // Get per interval translation
    _adjustmentsJson["per_interval_translation"] = langMgr.translate("per_interval");

    // Get displayed adjustments translation
    _adjustmentsJson["displayed_adjustments_translation"] = langMgr.translate("displayed_adjustments");

    // Get displayed adjustments translation
    _adjustmentsJson["displayed_adjustment_translation"] = langMgr.translate("displayed_adjustment");

    // Get score translation
    _adjustmentsJson["score_translation"] = langMgr.translate("score");

    // Get from translation
    _adjustmentsJson["from_translation"] = langMgr.translate("from");

    // Get to translation
    _adjustmentsJson["to_translation"] = langMgr.translate("to");

    // Get posology translation
    _adjustmentsJson["posology_translation"] = langMgr.translate("posology");

    // Get suggestion translation
    _adjustmentsJson["adjustment_suggested_translation"] = langMgr.translate("adjustment_suggested");

    // Get suggestion phrase translation
    _adjustmentsJson["suggestion_phrase_translation"] = langMgr.translate("suggestion_phrase");

    // For each adjustment
    for (const Core::DosageAdjustment& adj : _adjustmentData->getAdjustments()) {

        inja::json adjustmentJson;

        // Get adjustment score
        adjustmentJson["score"] = doubleToString(adj.getGlobalScore());

        // Export the dosage time ranges
        for (const unique_ptr<Core::DosageTimeRange>& dosageTimeRange : adj.m_history.getDosageTimeRanges()) {
            inja::json timeRangeJson;
            getTimeRangeJson(dosageTimeRange, timeRangeJson);
            adjustmentJson["dosage_time_ranges"].emplace_back(timeRangeJson);
        }

        _adjustmentsJson["adjustments"].emplace_back(adjustmentJson);
    }
}

void XpertRequestResultHtmlExport::getTargetsJson(const unique_ptr<Core::AdjustmentData>& _adjustmentData, inja::json& _targetsJson) const
{
    LanguageManager& langMgr = LanguageManager::getInstance();

    // Targets phrase translation
    _targetsJson["phrase_translation"] = langMgr.translate("targets_phrase");

    // For each target in the best suggestion
    for(const auto& target : _adjustmentData->getAdjustments()[0].m_targetsEvaluation) {
        inja::json targetJson;

        // Get the target type translation
        static map<Core::TargetType, string> types = {
            {Core::TargetType::UnknownTarget, "unknown"},
            {Core::TargetType::Residual, "residual"},
            {Core::TargetType::Peak, "peak"},
            {Core::TargetType::Mean, "mean"},
            {Core::TargetType::Auc, "auc"},
            {Core::TargetType::Auc24, "auc24"},
            {Core::TargetType::CumulativeAuc, "cumulative_auc"},
            {Core::TargetType::AucOverMic, "auc_over_mic"},
            {Core::TargetType::Auc24OverMic, "auc24_over_mic"},
            {Core::TargetType::TimeOverMic, "time_over_mic"},
            {Core::TargetType::AucDividedByMic, "auc_divided_by_mic"},
            {Core::TargetType::Auc24DividedByMic, "auc24_divided_by_mic"},
            {Core::TargetType::PeakDividedByMic, "peak_divided_by_mic"},
            {Core::TargetType::ResidualDividedByMic, "residual_divided_by_mic"},
            {Core::TargetType::FractionTimeOverMic, "fraction_time_over_mic"}};

        targetJson["type"] = langMgr.translate(types.at(target.getTargetType()));

        // Get the value (unit)
        stringstream valueStream;
        valueStream << doubleToString(target.getValue()) << " (" << target.getUnit().toString() << ")";
        targetJson["value"] = valueStream.str();

        // Get the score
        targetJson["score"] = doubleToString(target.getScore());

        // --------------- /!\ TO ADD WHEN ADJUSTMENT DATA HAS TARGET INEFFICACY AND TOXICITY /!\ -----------

//        // Get the bounds
//        stringstream boundsStream;
//        boundsStream << langMgr.translate("inefficacy") << ": " << int(target.getTarget().getInefficacyAlarm()) << " / "
//                     << "<b>"
//                     << langMgr.translate("min") << ": " << int(target.getTarget().getValueMin()) << " / "
//                     << langMgr.translate("best") << ": " << int(target.getTarget().getValueBest()) << " / "
//                     << langMgr.translate("max") << ": " << int(target.getTarget().getValueMax()) << " / "
//                     << "</b>"
//                     << langMgr.translate("toxicity") << ": " << int(target.getTarget().getToxicityAlarm());

        // --------------- /!\                      END TO ADD                                /!\ -----------


        // --------------- /!\ TO REMOVE WHEN ADJUSTMENT DATA HAS TARGET INEFFICACY AND TOXICITY /!\ -----------

        // Get the active moieties and find the active moiety of the target
        const Core::ActiveMoieties& activeMoieties = m_xpertRequestResultInUse->getDrugModel()->getActiveMoieties();
        auto activeMoityIt = find_if(activeMoieties.begin(), activeMoieties.end(),
                                     [&target](const unique_ptr<Core::ActiveMoiety>& activeMoiety){
            return activeMoiety->getActiveMoietyId() == target.getTarget().getActiveMoietyId();
        });

        // Get the target definitions of the active moiety and search if there is a target definition that
        // matches the current target type.
        const Core::TargetDefinitions& modelTargets = (*activeMoityIt)->getTargetDefinitions();
        auto targetDefinitionIt = find_if(modelTargets.begin(), modelTargets.end(),
                                          [&target](const unique_ptr<Core::TargetDefinition>& targetDefinition) {
           return targetDefinition->getTargetType() == target.getTargetType();
        });

        // Display the values of the definition if there is one, otherwise -1.
        string inefficacyAlarm = targetDefinitionIt !=  modelTargets.end() ?
                    to_string(int((*targetDefinitionIt)->getInefficacyAlarm().getValue())) : langMgr.translate("unknown");
        string toxicityAlarm = targetDefinitionIt   !=  modelTargets.end() ?
                    to_string(int((*targetDefinitionIt)->getToxicityAlarm().getValue()))   : langMgr.translate("unknown");


        // Get the bounds
        stringstream boundsStream;
        boundsStream << langMgr.translate("inefficacy") << ": " << inefficacyAlarm << " / "
                     << "<b>"
                     << langMgr.translate("min") << ": " << int(target.getTarget().getValueMin()) << " / "
                     << langMgr.translate("best") << ": " << int(target.getTarget().getValueBest()) << " / "
                     << langMgr.translate("max") << ": " << int(target.getTarget().getValueMax()) << " / "
                     << "</b>"
                     << langMgr.translate("toxicity") << ": " << toxicityAlarm;

        // --------------- /!\                      END TO REMOVE                                 /!\ -----------

        targetJson["bounds"] = boundsStream.str();

        _targetsJson["targets"].emplace_back(targetJson);
    }
}

void XpertRequestResultHtmlExport::getParametersJson(const XpertRequestResult& _xpertRequestResult, inja::json& _pksJson) const
{
    LanguageManager& langMgr = LanguageManager::getInstance();

    // Pharmacokinetic parameters translation
    _pksJson["translation"] = langMgr.translate("pharmacokinetic_parameters");

    // Typical patient translation
    _pksJson["typical_patient_translation"] = langMgr.translate("typical_patient");

    // A priori phrase translation
    _pksJson["a_priori_translation"] = langMgr.translate("a_priori");

    // A posteriori phrase translation
    _pksJson["a_posteriori_translation"] = langMgr.translate("a_posteriori");

    // For each type series (0: typical patient, 1: a priori, 2: a posteriori)
    for (size_t i = 0; i < _xpertRequestResult.getParameters().size(); i++) {

        // For each parameter.
        // We take advantage of the fact that the parameters are always in the same order
        // in the 3 vectors. Thus, we can easily add the a priori and a posteriori values.
        for (size_t j = 0; j < _xpertRequestResult.getParameters()[i].size(); ++j ) {

            const Core::ParameterValue& parameterValue = _xpertRequestResult.getParameters()[i][j];

            // Insert the parameter value depending on the type.
            switch(i) {

            // Typical patient
            case 0: {
                inja::json parameterJson;
                parameterJson["name"] = parameterValue.m_parameterId;
                parameterJson["typical_patient"] = doubleToString(parameterValue.m_value);
                _pksJson["pks"].emplace_back(parameterJson);
                break;
            }

            // A priori
            case 1: {
                _pksJson["pks"][j]["a_priori"] = doubleToString(parameterValue.m_value);
                break;
            }

            // A posteriori
            case 2: {
                _pksJson["pks"][j]["a_posteriori"] = doubleToString(parameterValue.m_value);
                break;
            }
            }
        }
    }
}

void XpertRequestResultHtmlExport::getPredictionsJson(const XpertRequestResult& _xpertRequestResult, inja::json& _predictionsJson) const
{
    LanguageManager& langMgr = LanguageManager::getInstance();

    // Predictions translation
    _predictionsJson["translation"] = langMgr.translate("predictions");

    // Extrapolated steady state auc24 translation
    _predictionsJson["extrapolated_steady_state_auc24_translation"] = langMgr.translate("extrapolated_steady_state_auc24");

    // Steady state peak translation
    _predictionsJson["steady_state_peak_translation"] = langMgr.translate("steady_state_peak");

    // Steady state residual translation
    _predictionsJson["steady_state_residual_translation"] = langMgr.translate("steady_state_residual");

    double auc24 = -1.0;
    double peak = -1.0;
    double residual = -1.0;
    Tucuxi::Common::DateTime date;

    _xpertRequestResult.getCycleStats().getStatistic(0, Tucuxi::Core::CycleStatisticType::AUC24).getValue(date, auc24);
    _xpertRequestResult.getCycleStats().getStatistic(0, Tucuxi::Core::CycleStatisticType::Peak).getValue(date, peak);
    _xpertRequestResult.getCycleStats().getStatistic(0, Tucuxi::Core::CycleStatisticType::Residual).getValue(date, residual);

    // Extrapolated steady state auc24
    _predictionsJson["extrapolated_steady_state_auc24"] = doubleToString(auc24) + " µg*h/l";

    // Steady state peak
    _predictionsJson["steady_state_peak"] = doubleToString(peak) + " µg/l";

    // Steady state residual
    _predictionsJson["steady_state_residual"] = doubleToString(residual) + " µg/l";
}

void XpertRequestResultHtmlExport::getComputationCovariatesJson(const XpertRequestResult& _xpertRequestResult, inja::json& _computationCovariatesJson) const
{
    LanguageManager& langMgr = LanguageManager::getInstance();

    // Computation covariates translation
    _computationCovariatesJson["translation"] = langMgr.translate("computation_covariates");

    // Covariate ID translation
    _computationCovariatesJson["covariate_translation"] = langMgr.translate("covariate");

    // Steady state peak translation
    _computationCovariatesJson["value_translation"] = langMgr.translate("value");

    // Extract the covariates used during the computation
    const vector<Core::CovariateValue>& computationCovariates = _xpertRequestResult.getAdjustmentData()->getAdjustments().front().getData().front().m_covariates;
    for(const Core::CovariateValue& covariateValue : computationCovariates) {

        inja::json computationCovariateJson;

        string covariateId = covariateValue.m_covariateId;

        // Find the covariate definition in the drug model
        const Core::CovariateDefinitions& covariateDefinitions = _xpertRequestResult.getDrugModel()->getCovariates();

        auto covariteDefinitionIt = find_if(covariateDefinitions.begin(), covariateDefinitions.end(),
                                            [&covariateId](const unique_ptr<Core::CovariateDefinition>& covariateDefinition) {
            return covariateDefinition->getId() == covariateId;
        });

        // Get the name
        computationCovariateJson["name"] = getStringWithEnglishFallback((*covariteDefinitionIt)->getName(), _xpertRequestResult.getXpertRequest().getOutputLang());

        // Get the value
        computationCovariateJson["value"] = beautifyString(doubleToString(covariateValue.m_value),
                                                       (*covariteDefinitionIt)->getDataType(),
                                                       (*covariteDefinitionIt)->getId());

        _computationCovariatesJson["computation_covariates"].emplace_back(computationCovariateJson);
    }
}

void XpertRequestResultHtmlExport::getGraphDataJson(const XpertRequestResult& _xpertRequestResult, inja::json& _graphDataJson) const
{
    // Get the min x date
    _graphDataJson["adjustment_date"] = "'" + Common::Utils::varToString(_xpertRequestResult.getAdjustmentTrait()->getAdjustmentTime()) + "'";

    // Get the max x date
    _graphDataJson["end_date"] = "'" + Common::Utils::varToString(_xpertRequestResult.getAdjustmentTrait()->getEnd()) + "'";

    // For each target in the best suggestion
    for (const Core::TargetEvaluationResult& target : _xpertRequestResult.getAdjustmentData()->getAdjustments()[0].m_targetsEvaluation) {

        inja::json targetJson;

        // Add target type
        targetJson["type"] = Core::toString(target.getTargetType());

        // Add target min
        targetJson["min"] = doubleToString(target.getTarget().getValueMin());

        // Add target best
        targetJson["best"] = doubleToString(target.getTarget().getValueBest());

        // Add target max
        targetJson["max"] = doubleToString(target.getTarget().getValueMax());


        _graphDataJson["targets"].emplace_back(targetJson);
    }

    // Create the js array of adjustments
    const vector<Core::DosageAdjustment>& adjustments = _xpertRequestResult.getAdjustmentData()->getAdjustments();

    // For each adjustment
    for (size_t a = 0; a < adjustments.size(); ++a) {

        inja::json adjustmentJson;

        // For each cycle data
        for (size_t c = 0; c < adjustments[a].getData().size(); ++c) {

            const Core::CycleData& cycleData = adjustments[a].getData()[c];

             inja::json cycleDataJson;


            // Add the cycle data start time
            cycleDataJson["start"] =  Common::Utils::varToString(cycleData.m_start) ;

            // Add the cycle data offsets
            stringstream timesStream;
            for (size_t i = 0; i < cycleData.m_times[0].size(); i++) {
                timesStream << to_string(cycleData.m_times[0][i]);
                if (i != cycleData.m_times[0].size() - 1) {
                     timesStream << ',';
                }
            }
            cycleDataJson["times"] = timesStream.str();

            // Add the cycle data values
            stringstream valuesStream;
            for (size_t i = 0; i < cycleData.m_concentrations[0].size(); i++) {
                valuesStream << to_string(cycleData.m_concentrations[0][i]);
                if (i != cycleData.m_concentrations[0].size() - 1) {
                    valuesStream << ',';
                }
            }
            cycleDataJson["values"] = valuesStream.str();


            adjustmentJson["cycles"].emplace_back(cycleDataJson);
        }

        _graphDataJson["adjustments"].emplace_back(adjustmentJson);
    }
}

string XpertRequestResultHtmlExport::prefixPosology(const string& _posologyIndication, const string& _posologyIndicationChain) const
{
    stringstream newPosologyIndicationsChainStream;
    newPosologyIndicationsChainStream << _posologyIndication // Add a coma only if there is already a posology indication.
                                      << (_posologyIndicationChain.empty() ? "" : ", ") << _posologyIndicationChain;

    return newPosologyIndicationsChainStream.str();
}

} // namespace Xpert
} // namespace Tucuxi
