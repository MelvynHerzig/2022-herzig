#ifndef TEST_XPERTUTILS_H
#define TEST_XPERTUTILS_H

#include "tucucore/dosage.h"
#include "tucucore/drugmodelchecker.h"
#include "tucucore/pkmodel.h"
#include "tucucore/drugmodelimport.h"
#include "tucucore/drugmodelrepository.h"
#include "tucucore/computingservice/computingresponse.h"

#include "tuberxpert/query/xpertquerydata.h"
#include "tuberxpert/query/xpertqueryimport.h"
#include "tuberxpert/result/globalresult.h"
#include "tuberxpert/language/languageexception.h"
#include "tuberxpert/utils/xpertutils.h"

#include "fructose/fructose.h"

/// \brief Tests for utility methods of TuberXpert.
/// \date 03/06/2022
/// \author Herzig Melvyn
struct TestXpertUtils : public fructose::test_base<TestXpertUtils>
{

    /// \brief Format used to create date and time during test.
    const std::string date_format = "%Y-%m-%dT%H:%M:%S";

    /// \brief Converts output lang to string. Checks that the requested language get its corresponding
    ///        string or an exception if it is not supported.
    /// \param _testName Name of the test.
    void convertOutputLangToString(const std::string& _testName)
    {

        std::cout << _testName << std::endl;

        fructose_assert_eq(Tucuxi::Xpert::varToString(Tucuxi::Xpert::OutputLang::ENGLISH), "en");
        fructose_assert_eq(Tucuxi::Xpert::varToString(Tucuxi::Xpert::OutputLang::FRENCH), "fr");
        fructose_assert_exception(Tucuxi::Xpert::varToString(Tucuxi::Xpert::OutputLang(-1)), Tucuxi::Xpert::LanguageException);
    }


    /// \brief Converts double to string. Checks that the string contains two decimals.
    /// \param _testName Name of the test.
    void convertDoubleToString(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        fructose_assert_eq(Tucuxi::Xpert::varToString(5.411111), "5.41");
        fructose_assert_eq(Tucuxi::Xpert::varToString(6), "6.00");
    }

    /// \brief Converts covariate type to string.
    /// \param _testName Name of the test.
    void convertCovariateTypeToString(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        fructose_assert_eq(Tucuxi::Xpert::varToString(Tucuxi::Xpert::CovariateType::Model), "default");
        fructose_assert_eq(Tucuxi::Xpert::varToString(Tucuxi::Xpert::CovariateType::Patient), "patient");
        fructose_assert_exception(Tucuxi::Xpert::varToString(Tucuxi::Xpert::CovariateType(-1)), std::invalid_argument);
    }

    /// \brief Tests that the getStringWithEnglishFallback returns the targeted language or
    ///        the default language (en) or empty string.
    /// \param _testName Name of the test.
    void getStringFromTranslatableWithFallback(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        Tucuxi::Common::TranslatableString ts1("translatable string in en");
        ts1.setString("chaine de caractere traductible en fr", "fr");

        Tucuxi::Common::TranslatableString ts2("translatable string in en");


        fructose_assert_eq(Tucuxi::Xpert::getStringWithEnglishFallback(ts1, Tucuxi::Xpert::OutputLang::FRENCH), "chaine de caractere traductible en fr");
        fructose_assert_eq(Tucuxi::Xpert::getStringWithEnglishFallback(ts2, Tucuxi::Xpert::OutputLang::FRENCH), "translatable string in en");
    }

    /// \brief Converts warning level to string.
    /// \param _testName Name of the test.
    void convertWarningLevelToString(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        fructose_assert_eq(Tucuxi::Xpert::varToString(Tucuxi::Xpert::WarningLevel::CRITICAL), "critical");
        fructose_assert_eq(Tucuxi::Xpert::varToString(Tucuxi::Xpert::WarningLevel::NORMAL), "normal");
        fructose_assert_exception(Tucuxi::Xpert::varToString(Tucuxi::Xpert::WarningLevel(-1)), std::invalid_argument);
    }

    /// \brief Tests that the getOldestDosageTimeRangeStart works as expected.
    ///        This test forms a dosage history with two time ranges:
    ///         1) 2022-01-01 10h00 - 2022-01-02 13h00
    ///         2) 2022-01-03 14h00 - 2022-01-04 16h00
    ///
    ///        Once it requieres the oldest start time with a reference time of 2022-01-01 14h00
    ///        (just after the first time range) which should return 2022-01-01 10h00
    ///
    ///        Once it requieres the oldest start time with a reference time of 2022-01-01 09h00
    ///        (before both time ranges) which should return the reference time itself.
    ///
    /// \param _testName Name of the test.
    void getOldestDosageTimeRangeStartReturnsCorrectValues(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        // Common elements
        Tucuxi::Core::Unit unit{"mg"};
        Tucuxi::Core::FormulationAndRoute formulationAndRoute{Tucuxi::Core::AbsorptionModel::Extravascular};
        Tucuxi::Common::Duration duration, interval{std::chrono::hours(1)};

        // Making first time range "2022-01-01 10h00 - 2022-01-02 13h00"
        Tucuxi::Core::LastingDose lastingDose1{1, unit, formulationAndRoute, duration, interval};
        Tucuxi::Core::DosageTimeRange timeRange1{
            Tucuxi::Common::DateTime("2022-01-01T10:00:00", date_format),
            Tucuxi::Common::DateTime("2022-01-02T13:00:00", date_format),
            lastingDose1
        };

        // Making second time range "2022-01-03 14h00 - 2022-01-04 16h00"
        Tucuxi::Core::LastingDose lastingDose2{1, unit, formulationAndRoute, duration, interval};
        Tucuxi::Core::DosageTimeRange timeRange2{
            Tucuxi::Common::DateTime("2022-01-03T14:00:00", date_format),
            Tucuxi::Common::DateTime("2022-01-04T16:00:00", date_format),
            lastingDose1
        };

        // Making dsage history
        Tucuxi::Core::DosageHistory dosageHistory;
        dosageHistory.addTimeRange(timeRange1);
        dosageHistory.addTimeRange(timeRange2);

        fructose_assert_eq(Tucuxi::Xpert::getOldestDosageTimeRangeStart(dosageHistory, Tucuxi::Common::DateTime("2022-01-01T14:00:00", date_format)),
                           Tucuxi::Common::DateTime("2022-01-01T10:00:00", date_format));

        fructose_assert_eq(Tucuxi::Xpert::getOldestDosageTimeRangeStart(dosageHistory, Tucuxi::Common::DateTime("2022-01-01T09:00:00", date_format)),
                           Tucuxi::Common::DateTime("2022-01-01T09:00:00", date_format));
    }

    /// \brief Tests that the getLatestDosageTimeRangeStart works as expected.
    ///        This test forms a dosage history with two time ranges:
    ///         1) 2022-01-01 10h00 - 2022-01-02 13h00
    ///         2) 2022-01-03 14h00 - 2022-01-04 16h00
    ///
    ///        Once it requieres the latest start time with a reference time of 2022-01-07 15h00
    ///        (before both time ranges) which should return 2022-01-03 14h00
    ///
    ///        Once it requieres the oldest start time with a reference time of 2022-01-01 09h00
    ///        (before both time ranges) which should return an undefined date time.
    ///
    /// \param _testName Name of the test.
    void getLatestDosageTimeRangeStartReturnsCorrectValues(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        // Common elements
        Tucuxi::Core::Unit unit{"mg"};
        Tucuxi::Core::FormulationAndRoute formulationAndRoute{Tucuxi::Core::AbsorptionModel::Extravascular};
        Tucuxi::Common::Duration duration, interval{std::chrono::hours(1)};

        // Making first time range "2022-01-01 10h00 - 2022-01-02 13h00"
        Tucuxi::Core::LastingDose lastingDose1{1, unit, formulationAndRoute, duration, interval};
        Tucuxi::Core::DosageTimeRange timeRange1{
            Tucuxi::Common::DateTime("2022-01-01T10:00:00", date_format),
            Tucuxi::Common::DateTime("2022-01-02T13:00:00", date_format),
            lastingDose1
        };

        // Making second time range "2022-01-03 14h00 - 2022-01-04 16h00"
        Tucuxi::Core::LastingDose lastingDose2{1, unit, formulationAndRoute, duration, interval};
        Tucuxi::Core::DosageTimeRange timeRange2{
            Tucuxi::Common::DateTime("2022-01-03T14:00:00", date_format),
            Tucuxi::Common::DateTime("2022-01-04T16:00:00", date_format),
            lastingDose1
        };

        // Making dsage history
        Tucuxi::Core::DosageHistory dosageHistory;
        dosageHistory.addTimeRange(timeRange1);
        dosageHistory.addTimeRange(timeRange2);

        fructose_assert_eq(Tucuxi::Xpert::getLatestDosageTimeRangeStart(dosageHistory, Tucuxi::Common::DateTime("2022-01-07T15:00:00", date_format)),
                           Tucuxi::Common::DateTime("2022-01-03T14:00:00", date_format));

        fructose_assert_eq(Tucuxi::Xpert::getLatestDosageTimeRangeStart(dosageHistory, Tucuxi::Common::DateTime("2022-01-01T09:00:00", date_format)).isUndefined(),
                           true);
    }

    /// \brief This method tests the computeFileName method.
    ///        It computes a XpertRequestResult and check that the file name recieved is expected.
    /// \param _testName Name of the test
    void computeFileNameReturnsCorrectValues(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>imatinib_2</queryId>
                                        <clientId>124568</clientId>
                                        <date>2018-07-11T13:45:30</date> <!-- Date the xml has been sent -->
                                        <language>en</language>

                                        <drugTreatment>
                                            <!-- All the information regarding the patient -->
                                            <patient>
                                                <covariates>
                                                </covariates>
                                            </patient>
                                            <!-- List of the drugs informations we have concerning the patient -->
                                            <drugs>
                                                <!-- All the information regarding the drug -->
                                                <drug>
                                                    <drugId>imatinib</drugId>
                                                    <activePrinciple>something</activePrinciple>
                                                    <brandName>somebrand</brandName>
                                                    <atc>something</atc>
                                                    <!-- All the information regarding the treatment -->
                                                    <treatment>
                                                        <dosageHistory>
                                                            <dosageTimeRange>
                                                                <start>2018-07-06T08:00:00</start>
                                                                <end>2018-07-08T08:00:00</end>
                                                                <dosage>
                                                                    <dosageLoop>
                                                                        <lastingDosage>
                                                                            <interval>12:00:00</interval>
                                                                            <dose>
                                                                                <value>400</value>
                                                                                <unit>mg</unit>
                                                                                <infusionTimeInMinutes>60</infusionTimeInMinutes>
                                                                            </dose>
                                                                            <formulationAndRoute>
                                                                                <formulation>parenteralSolution</formulation>
                                                                                <administrationName>foo bar</administrationName>
                                                                                <administrationRoute>oral</administrationRoute>
                                                                                <absorptionModel>extravascular</absorptionModel>
                                                                            </formulationAndRoute>
                                                                        </lastingDosage>
                                                                    </dosageLoop>
                                                                </dosage>
                                                            </dosageTimeRange>
                                                        </dosageHistory>
                                                    </treatment>
                                                    <!-- Samples history -->
                                                    <samples>
                                                        <sample>
                                                            <sampleId>123456</sampleId>
                                                            <sampleDate>2018-07-07T06:00:30</sampleDate>
                                                            <concentrations>
                                                                <concentration>
                                                                    <analyteId>imatinib</analyteId>
                                                                    <value>0.7</value>
                                                                    <unit>mg/l</unit>
                                                                </concentration>
                                                            </concentrations>
                                                        </sample>
                                                        <sample>
                                                            <sampleId>123456</sampleId>
                                                            <sampleDate>2018-07-07T07:00:00</sampleDate>
                                                            <concentrations>
                                                                <concentration>
                                                                    <analyteId>imatinib</analyteId>
                                                                    <value>10.6</value>
                                                                    <unit>mg/l</unit>
                                                                </concentration>
                                                            </concentrations>
                                                        </sample>
                                                        <sample>
                                                            <sampleId>123456</sampleId>
                                                            <sampleDate>2018-07-07T13:00:00</sampleDate>
                                                            <concentrations>
                                                                <concentration>
                                                                    <analyteId>imatinib</analyteId>
                                                                    <value>0.8</value>
                                                                    <unit>mg/l</unit>
                                                                </concentration>
                                                            </concentrations>
                                                        </sample>
                                                    </samples>
                                                    <!-- Personalised targets -->
                                                    <targets>
                                                    </targets>
                                                </drug>
                                            </drugs>
                                        </drugTreatment>
                                        <!-- List of the requests we want the server to take care of -->
                                        <requests>
                                            <requestXpert>
                                                <drugId>imatinib</drugId>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <adjustmentDate>2018-07-06T08:00:00</adjustmentDate>
                                                <options>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>populationValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>allFormulationAndRoutes</formulationAndRouteSelectionOption>
                                                </options>
                                            </requestXpert>
                                        </requests>
                                    </query>)";

        std::cout << _testName << std::endl;

        // Query import
        std::unique_ptr<Tucuxi::Xpert::XpertQueryData> query = nullptr;
        Tucuxi::Xpert::XpertQueryImport importer;
        Tucuxi::Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, queryString);

        if (importResult != Tucuxi::Xpert::XpertQueryImport::Status::Ok) {
            throw std::runtime_error("Setup failed");
        }

        Tucuxi::Xpert::GlobalResult globalResult{move(query), "random\\path"};
        globalResult.incrementRequestIndexBeingHandled();
        Tucuxi::Xpert::XpertRequestResult& xrr =  globalResult.getXpertRequestResults()[0];


        fructose_assert_eq(Tucuxi::Xpert::computeFileName(xrr), "random\\path\\imatinib_1_11-7-2018_13h45m30s.xml");
    }
};

#endif // TEST_XPERTUTILS_H
