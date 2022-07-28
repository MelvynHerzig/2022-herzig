#ifndef XPERTREQUESTRESULTHTMLEXPORT_H
#define XPERTREQUESTRESULTHTMLEXPORT_H

#include "tuberxpert/exporter/abstractxpertrequestresultexport.h"
#include "tuberxpert/exporter/abstracthtmlexport.h"
#include "tuberxpert/query/admindata.h"

#include "inja/inja.hpp"

namespace Tucuxi {
namespace Xpert {

/// \brief This class exports an XpertRequestResult in HTML.
///        It creates an all-in-one html file that contains
///        the necessary css and js to be autonomous.
///
///        This class has setup a tamplate string and fills it using
///        the inja library (https://github.com/pantor/inja) v3.3.0.
///
///        An example result (not as an all-in-one file) is available
///        in: /dev/tucuxi-tuberxpert/html/src/index.html.
///        This class tries to reproduce the layout of this example.
///
///        There is a few tricks to know about this template.
///
///        This template is made to work with wkhtmltopdf in order
///        to have a really simple PDF exporter. However, wkhtmltopdf
///        is not really recent. As a result, it does not support modern CSS
///        (such as flex box) and it does not support JS above EcmaScript5.
///
///        The solutions to those problems are as follows:
///             - The layout of the page is made with tables.
///             - The JS needs to be transpiled with Babel (see: /dev/tucuxi-tuberxpert/html/readme.md)
///
///        The width of the HTML content corresponds to an A4 page without margin.
///        The elements have been organized so as not to be spread over two pages.
///        The CSS avoid splitting tables, rows and div with the "avoid-break-inside" class
///        which is really useful to group things together and avoid page breaks inside.
///
///        Keep it in mind, the template places each data in its own table. With this approach it is
///        really easy to avoid elements being left alone.
///
///        Let's say we have the title "Covariates" and a series of two covariates, but we want to avoid
///        that the title is alone before a page break.
///        We can organize the HTML as follows:
///
///        \<div class="avoid-page-break\>                                                      <br>
///        &emsp; \<h1\> Covariates \</h1\>                                                     <br>
///        &emsp; \<table\>  The first covariate  \</table\>                                    <br>
///        \</div\>                                                                             <br>
///        \<table\> The second covariate  \</table\>                                           <br>
///
///         With this technique, we have the guarantee that the title will always be with the first covariate
///         without preventing a page break between the covariates which would not have been possible if all
///         covariates were represented by a set of rows in the same table. This approach is often used in
///         the HTML template of this class when we do not know where a group of data is going to start.
///
/// \date 23/06/2022
/// \author Herzig Melvyn
class XpertRequestResultHtmlExport : public AbstractXpertRequestResultExport, public AbstractHtmlExport
{
public:

    /// \brief Export the result of the xpertRequest to a file. The export may fail. In this
    ///        case, the XpertRequestResult error message is set.
    /// \param _xpertRequestResult Result of the xpertRequest to export.
    void exportToFile(XpertRequestResult& _xpertRequestResult) override;

    /// \brief Export the result of the xpertRequest to a file with the given file name.
    ///        The export may fail. In this case, the XpertRequestResult error message is set.
    /// \param _fileName Name to give to the html file.
    /// \param _xpertRequestResult Result of the xpertRequest to export.
    void exportToFile(const std::string& _fileName, XpertRequestResult& _xpertRequestResult) override;

protected:

    /// \brief Prepare the first part of the HTML document from <!doctype html> to </head> as a string.
    ///        Insert the meta elements and the CSS and JS minified strings from FileString.
    /// \return The resulting string.
    std::string makeHeaderString() const;

    /// \brief Prepare the second part of the HTML document from <body> to </html> as a string.
    ///        Insert the XpertRequestResult content as the page content.
    /// \param _xpertRequestResult Result of the xpertRequest to export.
    /// \return The resulting string.
    std::string makeBodyString(const XpertRequestResult& _xpertRequestResult) const;

    /// \brief Prepare and put the title of the report and the date of computation in the json.
    /// \param _xpertRequestResult XpertRequestResult containing the date of computation.
    /// \param _headerJson Json object where to put the collected data.
    void getHeaderJson(const XpertRequestResult& _xpertRequestResult, inja::json& _headerJson) const;

    /// \brief Prepare and put the drug id, the last dose and the drug model id in the json.
    /// \param _xpertRequestResult XpertRequestResult containing the data to put in the json.
    /// \param _introJson Json object where to put the collected data.
    void getDrugIntroJson(const XpertRequestResult& _xpertRequestResult, inja::json& _introJson) const;

    /// \brief Prepare and put the contact information of the mandator and the patient in the json.
    ///        It prepares the personal and institute contact information.
    /// \param _admin AdminData containing the data to put in the json.
    /// \param _contactsJson Json object where to put the collected data.
    void getContactsJson(const std::unique_ptr<AdminData>& _admin, inja::json& _contactsJson) const;

    /// \brief Prepare and put the contact information of a person in the json.
    /// \param _person FullPersonData containing the data to put in the json.
    /// \param _personJson Json object where to put the collected data.
    void getPersonJson(const std::unique_ptr<FullPersonData>& _person, inja::json& _personJson) const;

    /// \brief Prepare and put the contact information of an institute in the json.
    /// \param _institute InstituteData containing the data to put in the json.
    /// \param _instituteJson Json object where to put the collected data.
    void getInstituteJson(const std::unique_ptr<InstituteData>& _institute, inja::json& _instituteJson) const;

    /// \brief Prepare an address in a string and put it in the json.
    /// \param _address AddressData to prepare.
    /// \param _addressJson Json object where to put the address string.
    void getAddressJson(const std::unique_ptr<AddressData>& _address, inja::json& _addressJson) const;

    /// \brief Prepare a phone number in a string and put it in the json.
    /// \param _phone PhoneData to prepare.
    /// \param _addressJson Json object where to put the phone number string.
    void getPhoneJson(const std::unique_ptr<PhoneData>& _phone, inja::json& _phoneJson) const;

    /// \brief Prepare an email in a string and put it in the json.
    /// \param _email EmailData to prepare.
    /// \param _addressJson Json object where to put the email string.
    void getEmailJson(const std::unique_ptr<EmailData>& _email, inja::json& _emailJson) const;

    /// \brief Prepare and put the clinical data in the json.
    /// \param _admin AdminData containing the clinical data to put in the json.
    /// \param _clinicalDatasJson Json object where to put the collected data.
    void getClinicalDatasJson(const std::unique_ptr<AdminData>& _admin, inja::json& _clinicalDatasJson) const;

    /// \brief Prepare and put the covariate validation results in the json.
    /// \param _covariateResults The vector of the covariate validation results to put in the json.
    /// \param _covariatesJson Json object where to put the collected data.
    void getCovariatesJson(const std::vector<CovariateValidationResult>& _covariateResults, inja::json& _covariatesJson) const;

    /// \brief Prepare and put the dosage history in the json.
    /// \param _history The DosageHistory to put in the json.
    /// \param _treatmentJson Json object where to put the collected data.
    void getTreatmentJson(const Core::DosageHistory& _history, inja::json& _treatmentJson) const;

    /// \brief Prepare and put the dosage time range in the json.
    /// \param _timeRange The DosageTimeRange to put in the json.
    /// \param _treatmentRowsJson Json object where to put the collected data.
    void getTimeRangeJson(const std::unique_ptr<Core::DosageTimeRange>& _timeRange, inja::json& _treatmentRowsJson) const;

    /// \brief Get the actual type of a dosage and call the correct getDosageJson.
    /// \param _dosage Dosage to get the actual type.
    /// \param _dosageTimeRangeJson Json object where to put the collected data when a single dose is reached.
    /// \param _posologyIndicationChain Posology indications chain updated in the method getDosageJson.
    void getAbstractDosageJson(const Core::Dosage& _dosage, inja::json& _dosageTimeRangeJson, const std::string& _posologyIndicationChain) const;

    /// \brief Get the dosage in a dosage loop and call getAbstractDosage.
    ///        Set the dosage type of the json object but don't update the posology indication chain.
    /// \param _dosage DosageLoop to extract the dosage.
    /// \param _dosageTimeRangeJson Json object where to put the collected data when a single dose is reached.
    /// \param _posologyIndicationChain Posology indications chain updated in the method getDosageJson.
    void getDosageJson(const Core::DosageLoop& _dosage, inja::json& _dosageTimeRangeJson, const std::string& _posologyIndicationChain) const;

    /// \brief Get the dosage in a dosage at steady state and call getAbstractDosage.
    ///        Set the dosage type of the json object but don't update the posology indication chain.
    /// \param _dosage DosageSteadyState to extract the dosage.
    /// \param _dosageTimeRangeJson Json object where to put the collected data when a single dose is reached.
    /// \param _posologyIndicationChain Posology indications chain updated in the method getDosageJson.
    void getDosageJson(const Core::DosageSteadyState& _dosage, inja::json& _dosageTimeRangeJson, const std::string& _posologyIndicationChain) const;

    /// \brief Get the dosage in a dosage repeat and call getAbstractDosage.
    ///        Update the posology indications chain with the number of repetitions.
    /// \param _dosage DosageRepeat to extract the dosage.
    /// \param _dosageTimeRangeJson Json object where to put the collected data when a single dose is reached.
    /// \param _posologyIndicationChain Posology indications chain updated in the method getDosageJson.
    void getDosageJson(const Core::DosageRepeat& _dosage, inja::json& _dosageTimeRangeJson, const std::string& _posologyIndicationChain) const;

    /// \brief Get the dosages in a dosage sequence and call getAbstractDosage.
    ///        Don't update the posology indication chain.
    /// \param _dosage DosageSequence to extract the dosages.
    /// \param _dosageTimeRangeJson
    /// \param _posologyIndicationChain Posology indications chain updated in the method getDosageJson.
    void getDosageJson(const Core::DosageSequence& _dosage, inja::json& _dosageTimeRangeJson, const std::string& _posologyIndicationChain) const;

    /// \brief Get the dosages in a parallel dosage sequence and call getAbstractDosage.
    ///        Don't update the posology indication chain.
    /// \param _dosage ParallelDosageSequence to extract the dosages.
    /// \param _dosageTimeRangeJson Json object where to put the collected data when a single dose is reached.
    /// \param _posologyIndicationChain Posology indications chain updated in the method getDosageJson.
    void getDosageJson(const Core::ParallelDosageSequence& _dosage, inja::json& _dosageTimeRangeJson, const std::string& _posologyIndicationChain) const;

    /// \brief Get the dosage in a lasting dose and call getAbstractDosage.
    ///        Update the posology indications chain with the interval.
    /// \param _dosage LastingDose to extract the dosage.
    /// \param _dosageTimeRangeJson Json object where to put the collected data when a single dose is reached.
    /// \param _posologyIndicationChain Posology indications chain updated in the method getDosageJson.
    void getDosageJson(const Core::LastingDose& _dosage, inja::json& _dosageTimeRangeJson, const std::string& _posologyIndicationChain) const;

    /// \brief  Get the dosage in a daily dose and call getAbstractDosage.
    ///         Update the posology indications chain with the daily time.
    /// \param _dosage DailyDose to extract the dosage.
    /// \param _dosageTimeRangeJson Json object where to put the collected data when a single dose is reached.
    /// \param _posologyIndicationChain Posology indications chain updated in the method getDosageJson.
    void getDosageJson(const Core::DailyDose& _dosage, inja::json& _dosageTimeRangeJson, const std::string& _posologyIndicationChain) const;

    /// \brief Get the dosage in a weekly dose and call getAbstractDosage.
    ///        Update the posology indications chain with the week day and time.
    /// \param _dosage WeeklyDose to extract the dosage.
    /// \param _dosageTimeRangeJson Json object where to put the collected data when a single dose is reached.
    /// \param _posologyIndicationChain Posology indications chain updated in the method getDosageJson.
    void getDosageJson(const Core::WeeklyDose& _dosage, inja::json& _dosageTimeRangeJson, const std::string& _posologyIndicationChain) const;

    /// \brief Prepare and put the single dose and its posology in the json.
    /// \param _dosage The SingleDose to put in the json.
    /// \param _dosageTimeRangeJson Json object where to put the collected data when a single dose is reached.
    /// \param _posologyIndicationChain Posology indications chain updated in the method getDosageJson.
    void getSingleDoseJson(const Core::SingleDose& _dosage, inja::json& _dosageTimeRangeJson, const std::string& _posologyIndicationChain) const;

    /// \brief Prepare and put the samples validation results in the json.
    /// \param _sampleResults The vector of the sample validation results to put in the json.
    /// \param _samplesJson Json object where to put the collected data.
    void getSamplesJson(const std::vector<SampleValidationResult>& _sampleResults, inja::json& _samplesJson) const;

    /// \brief Prepare and put the adjustments in the json.
    /// \param _adjustmentData Object containing the adjustments.
    /// \param _adjustmentsJson Json object where to put the collected data.
    void getAdjustmentsJson(const std::unique_ptr<Core::AdjustmentData>& _adjustmentData, inja::json& _adjustmentsJson) const;

    /// \brief Prepare and put the targets information of the best adjustment in the json.
    /// \param _adjustmentData Object containing the best adjustment.
    /// \param _targetsJson Json object where to put the collected data.
    void getTargetsJson(const std::unique_ptr<Core::AdjustmentData>& _adjustmentData, inja::json& _targetsJson) const;

    /// \brief Prepare and put the parameters in the json.
    /// \param _xpertRequestResult XpertRequestResult containing the parameters.
    /// \param _pksJson Json object where to put the collected data.
    void getParametersJson(const XpertRequestResult& _xpertRequestResult, inja::json& _pksJson) const;

    /// \brief Prepare and put the predictions at steady state in the json.
    /// \param _xpertRequestResult XpertRequestResult containing the predictions at steady state.
    /// \param _predictionsJson Json object where to put the collected data.
    void getPredictionsJson(const XpertRequestResult& _xpertRequestResult, inja::json& _predictionsJson) const;

    /// \brief Prepare and put the covariate used during the computation in the json.
    /// \param _xpertRequestResult XpertRequestResult containing the computation covariates.
    /// \param _computationCovariatesJson Json object where to put the collected data.
    void getComputationCovariatesJson(const XpertRequestResult& _xpertRequestResult, inja::json& _computationCovariatesJson) const;

    /// \brief Prepare and put the adjustment data (times and values) in the json for graphing.
    /// \param _xpertRequestResult XpertRequestResult containing the adjustment trait and the adjustment data.
    /// \param _graphDataJson Json object where to put the collected data.
    void getGraphDataJson(const XpertRequestResult& _xpertRequestResult, inja::json& _graphDataJson) const;

    /// \brief Prefix the given posology to the given posology indication chain.
    /// \param _posologyIndication Posology to prefix.
    /// \param _posologyIndicationChain Existing posology indication chain.
    /// \return The resulting indication chain with the new posology prefixed.
    std::string prefixPosology(const std::string& _posologyIndication, const std::string& _posologyIndicationChain) const;

    /// \brief For an abstract validation result, get its warning and put it in the json.
    /// \param _validationResult Validation result to get the warning.
    /// \param _json Json object where to put the collected data.
    template<typename T>
    void getWarningJson(const AbstractValidationResult<T>& _validationResult, inja::json& _json) const {

        if (!_validationResult.getWarning().empty()) {
            _json["warning"] = _validationResult.getWarning();
        }
    }

protected:

    /// \brief We need to keep a reference on the xpert request result in order
    ///        to retreive the dose validation results map, the computation time
    ///        and the output language in each function without needing to pass it as
    ///        an argument each time.
    const XpertRequestResult* m_xpertRequestResultInUse;

};

} // namespace Xpert
} // namespace Tucuxi

#endif // XPERTREQUESTRESULTHTMLEXPORT_H
