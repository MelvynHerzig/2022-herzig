#ifndef TEST_XPERTUTILS_H
#define TEST_XPERTUTILS_H

#include "testutils.h"

#include "tuberxpert/language/languageexception.h"
#include "tuberxpert/utils/xpertutils.h"
#include "tuberxpert/result/xpertrequestresult.h"
#include "tuberxpert/result/xpertqueryresult.h"

#include "fructose/fructose.h"

/// \brief Tests for the methods in tuberxpert/utils/xpertutils.h.
/// \date 03/06/2022
/// \author Herzig Melvyn
struct TestXpertUtils : public fructose::test_base<TestXpertUtils>
{

    /// \brief Convert all the DataType to string. Check that the string contains the correct value.
    ///        Check that the value are expected.
    ///        - Bool must be "bool"
    ///        - Int must be "int"
    ///        - Double must be "doule"
    ///        - Date must be "date".
    /// \param _testName Name of the test.
    void dataTypeToString_behavesCorrectly_withAllDataType(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        fructose_assert_eq(Tucuxi::Xpert::dataTypeToString(Tucuxi::Core::DataType::Bool), "bool");
        fructose_assert_eq(Tucuxi::Xpert::dataTypeToString(Tucuxi::Core::DataType::Int), "int");
        fructose_assert_eq(Tucuxi::Xpert::dataTypeToString(Tucuxi::Core::DataType::Double), "double");
        fructose_assert_eq(Tucuxi::Xpert::dataTypeToString(Tucuxi::Core::DataType::Date), "date");
    }

    /// \brief Convert somes doubles to string. Check that the strings contain two decimals.
    ///        Check that the value are expected.
    ///        - 5.411111 must be 5.41
    ///        - 6 must be 6.00
    /// \param _testName Name of the test.
    void doubleToString_behavesCorrectly(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        fructose_assert_eq(Tucuxi::Xpert::doubleToString(5.411111), "5.41");
        fructose_assert_eq(Tucuxi::Xpert::doubleToString(6), "6.00");
    }

    /// \brief Convert all the CovariateType to string. Check that the values are expected.
    ///        - Model must be "default"
    ///        - Patient must be "patient"
    /// \param _testName Name of the test.
    void covariateTypeToString_behavesCorrectly_withAllTheCovariateTypes(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        fructose_assert_eq(Tucuxi::Xpert::covariateTypeToString(Tucuxi::Xpert::CovariateType::Model), "default");
        fructose_assert_eq(Tucuxi::Xpert::covariateTypeToString(Tucuxi::Xpert::CovariateType::Patient), "patient");
        fructose_assert_exception(Tucuxi::Xpert::covariateTypeToString(Tucuxi::Xpert::CovariateType(-1)), std::invalid_argument);
    }

    /// \brief Convert the OutputLang to string. Check that the values are expected or a LanguageException.
    ///        At the moment the only supported languages are: English and French.
    ///        - ENGNLISH must be "en"
    ///        - FRENCH must be "fr"
    ///        - -1 -> LanguageException
    /// \param _testName Name of the test.
    void outputLangToString_behavesCorrectly_withAllOutputLangAndUnsupportedValues(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        fructose_assert_eq(Tucuxi::Xpert::outputLangToString(Tucuxi::Xpert::OutputLang::ENGLISH), "en");
        fructose_assert_eq(Tucuxi::Xpert::outputLangToString(Tucuxi::Xpert::OutputLang::FRENCH), "fr");
        fructose_assert_exception(Tucuxi::Xpert::outputLangToString(Tucuxi::Xpert::OutputLang(-1)), Tucuxi::Xpert::LanguageException);
    }

    /// \brief Convert the WarningLevel to string. Check that the values are expected or a invalid_argument.
    ///        - CRITICAL must be "critical"
    ///        - NORMAL must be "normal"
    /// \param _testName Name of the test.
    void warningLevelToString_behavesCorrectly_withAllWarningLevelAndUnsupportedValues(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        fructose_assert_eq(Tucuxi::Xpert::warningLevelToString(Tucuxi::Xpert::WarningLevel::CRITICAL), "critical");
        fructose_assert_eq(Tucuxi::Xpert::warningLevelToString(Tucuxi::Xpert::WarningLevel::NORMAL), "normal");
        fructose_assert_exception(Tucuxi::Xpert::warningLevelToString(Tucuxi::Xpert::WarningLevel(-1)), std::invalid_argument);
    }

    /// \brief Convert a dateTime to a formatted date time string for xml. Check that the string
    ///        contains the correct value.
    ///        The 1.1.2022 at 10h must be "2022-01-01T10:00:00"
    /// \param _testName Name of the test.
    void dateTimeToXmlString_behavesCorrectly(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        std::string formatedDateTimeString = "2022-01-01T10:00:00";

        Tucuxi::Common::DateTime formatedDateTime{formatedDateTimeString, TestUtils::date_format};

        fructose_assert_eq(Tucuxi::Xpert::dateTimeToXmlString(formatedDateTime), formatedDateTimeString);
    }

    /// \brief Convert some TimeOfDay to string. Checks that the valuea are expected.
    ///        - 8h 0m 0s must be "8h"
    ///        - 8h 30m 1s must be "8h30"
    /// \param _testName Name of the test.
    void timeToString_behavesCorrectly(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        Tucuxi::Xpert::LanguageManager& langMgr = Tucuxi::Xpert::LanguageManager::getInstance();
        langMgr.loadTranslations(TestUtils::englishTranslationFile);

        // Prepare the timeOfDay to convert
        Tucuxi::Common::TimeOfDay timeOfDayWithoutMinute{Tucuxi::Common::Duration(
                        std::chrono::hours(8),
                        std::chrono::minutes(0),
                        std::chrono::seconds(1))};

        Tucuxi::Common::TimeOfDay timeOfDayWithMinute{Tucuxi::Common::Duration(
                        std::chrono::hours(8),
                        std::chrono::minutes(30),
                        std::chrono::seconds(1))};

        // Check
        fructose_assert_eq(Tucuxi::Xpert::TimeOfDayToString(timeOfDayWithoutMinute), "8h");
        fructose_assert_eq(Tucuxi::Xpert::TimeOfDayToString(timeOfDayWithMinute), "8h30");
    }

    /// \brief Convert some dateTime to a date time strings for html/pdf. Check that the strings
    ///        contain the correct value.
    ///        - the 1.1.2022 at 10h (with time indication) must be "1.1.2022 10h"
    ///        - the 1.1.2022 at 10h (without time indication) must be "1.1.2022"
    /// \param _testName Name of the test.
    void dateTimeToString_behavesCorrectly(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        Tucuxi::Xpert::LanguageManager& langMgr = Tucuxi::Xpert::LanguageManager::getInstance();
        langMgr.loadTranslations(TestUtils::englishTranslationFile);

        std::string formatedDateTimeString = "2022-01-01T10:00:00";

        Tucuxi::Common::DateTime formatedDateTime{formatedDateTimeString, TestUtils::date_format};

        fructose_assert_eq(Tucuxi::Xpert::dateTimeToString(formatedDateTime), "1.1.2022 10h");
        fructose_assert_eq(Tucuxi::Xpert::dateTimeToString(formatedDateTime, false), "1.1.2022");
    }

    /// \brief Convert some covariate string values. Check that
    ///        resulting string are the expected values.
    ///        - "1.0" Bool (no id) must be "Yes"
    ///        - "0.0" Bool (no id) must be "No"
    ///        - "1.0" Sex (no id) must be "Man"
    ///        - "0.0" Sex (no id) must be "Woman"
    ///        - "0.5" Sex (no id) must be "Undefined"
    ///        - "42" Int age must be "42"
    ///        - "72.652222" Double bodyweight must be "72.65"
    /// \param _testName Name of the test.
    void beautifyString_behavesCorrectly(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        Tucuxi::Xpert::LanguageManager& langMgr = Tucuxi::Xpert::LanguageManager::getInstance();
        langMgr.loadTranslations(TestUtils::englishTranslationFile);

        fructose_assert_eq(Tucuxi::Xpert::beautifyString("1.0", Tucuxi::Core::DataType::Bool, ""), "Yes");
        fructose_assert_eq(Tucuxi::Xpert::beautifyString("0.0", Tucuxi::Core::DataType::Bool, ""), "No");

        fructose_assert_eq(Tucuxi::Xpert::beautifyString("1.0", Tucuxi::Core::DataType::Double, "sex"), "Man");
        fructose_assert_eq(Tucuxi::Xpert::beautifyString("0.0", Tucuxi::Core::DataType::Double, "sex"), "Woman");
        fructose_assert_eq(Tucuxi::Xpert::beautifyString("0.5", Tucuxi::Core::DataType::Double, "sex"), "Undefined");

        fructose_assert_eq(Tucuxi::Xpert::beautifyString("42", Tucuxi::Core::DataType::Int, "age"), "42");

        fructose_assert_eq(Tucuxi::Xpert::beautifyString("72.652222", Tucuxi::Core::DataType::Double, "bodyweight"), "72.65");
    }

    /// \brief Check that the method getStringWithEnglishFallback returns the correct value.
    ///        - A translatableString with the required language must returns the targeted translation.
    ///        - A translatableString without the required language but English returns the English translation.
    ///        - A translatableString without the required language and without English returns an empty string.
    /// \param _testName Name of the test.
    void getStringWithEnglishFallback_behavesCorrectly(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        Tucuxi::Common::TranslatableString translatableStringWithRequiredLang("translatable string in en");
        translatableStringWithRequiredLang.setString("chaine de caracteres traductible en fr", "fr");

        Tucuxi::Common::TranslatableString translatableStringWithoutRequiredLangs("translatable string in en");

        Tucuxi::Common::TranslatableString translatableStringWithoutRequiredLangAndWithoutEnglish;

        fructose_assert_eq(Tucuxi::Xpert::getStringWithEnglishFallback(translatableStringWithRequiredLang, Tucuxi::Xpert::OutputLang::FRENCH), "chaine de caracteres traductible en fr");
        fructose_assert_eq(Tucuxi::Xpert::getStringWithEnglishFallback(translatableStringWithoutRequiredLangs, Tucuxi::Xpert::OutputLang::FRENCH), "translatable string in en");
        fructose_assert_eq(Tucuxi::Xpert::getStringWithEnglishFallback(translatableStringWithoutRequiredLangAndWithoutEnglish, Tucuxi::Xpert::OutputLang::FRENCH), "");
    }

    /// \brief Test that the getOldestDosageTimeRangeStart works as expected.
    ///        This test forms a dosage history with two time ranges:
    ///         1) 2022-01-01 10h00 - 2022-01-02 13h00
    ///         2) 2022-01-03 14h00 - 2022-01-04 16h00
    ///
    ///        Once it requires the oldest start time with a computation time of 2022-01-01 14h00
    ///        (just after the first time range) which should return 2022-01-01 10h00
    ///
    ///        Once it requires the oldest start time with a computation time of 2022-01-01 09h00
    ///        (before both time ranges) which should return the reference time itself.
    /// \param _testName Name of the test.
    void getOldestDosageTimeRangeStart_behavesCorrectly(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        // Common elements
        Tucuxi::Core::Unit unit{"mg"};
        Tucuxi::Core::FormulationAndRoute formulationAndRoute{Tucuxi::Core::AbsorptionModel::Extravascular};
        Tucuxi::Common::Duration duration, interval{std::chrono::hours(1)};

        // Making first time range "2022-01-01 10h00 - 2022-01-02 13h00"
        Tucuxi::Core::LastingDose lastingDose1{1, unit, formulationAndRoute, duration, interval};
        Tucuxi::Core::DosageTimeRange timeRange1{
            Tucuxi::Common::DateTime("2022-01-01T10:00:00", TestUtils::date_format),
            Tucuxi::Common::DateTime("2022-01-02T13:00:00", TestUtils::date_format),
            lastingDose1
        };

        // Making second time range "2022-01-03 14h00 - 2022-01-04 16h00"
        Tucuxi::Core::LastingDose lastingDose2{1, unit, formulationAndRoute, duration, interval};
        Tucuxi::Core::DosageTimeRange timeRange2{
            Tucuxi::Common::DateTime("2022-01-03T14:00:00", TestUtils::date_format),
            Tucuxi::Common::DateTime("2022-01-04T16:00:00", TestUtils::date_format),
            lastingDose1
        };

        // Making dsage history
        Tucuxi::Core::DosageHistory dosageHistory;
        dosageHistory.addTimeRange(timeRange1);
        dosageHistory.addTimeRange(timeRange2);

        fructose_assert_eq(Tucuxi::Xpert::getOldestDosageTimeRangeStart(dosageHistory, Tucuxi::Common::DateTime("2022-01-01T14:00:00", TestUtils::date_format)),
                           Tucuxi::Common::DateTime("2022-01-01T10:00:00", TestUtils::date_format));

        fructose_assert_eq(Tucuxi::Xpert::getOldestDosageTimeRangeStart(dosageHistory, Tucuxi::Common::DateTime("2022-01-01T09:00:00", TestUtils::date_format)),
                           Tucuxi::Common::DateTime("2022-01-01T09:00:00", TestUtils::date_format));
    }

    /// \brief Tests that the getLatestDosageTimeRangeStart works as expected.
    ///        This test forms a dosage history with two time ranges:
    ///         1) 2022-01-01 10h00 - 2022-01-02 13h00
    ///         2) 2022-01-03 14h00 - 2022-01-04 16h00
    ///
    ///        Once it requires the latest start time with a computation time of 2022-01-07 15h00
    ///        (after both time ranges) which should return 2022-01-03 14h00
    ///
    ///        Once it requires the oldest start time with a computation time of 2022-01-01 09h00
    ///        (before both time ranges) which should return an undefined date time.
    ///
    /// \param _testName Name of the test.
    void getLatestDosageTimeRangeStart_behavesCorrectly(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        // Common elements
        Tucuxi::Core::Unit unit{"mg"};
        Tucuxi::Core::FormulationAndRoute formulationAndRoute{Tucuxi::Core::AbsorptionModel::Extravascular};
        Tucuxi::Common::Duration duration, interval{std::chrono::hours(1)};

        // Making first time range "2022-01-01 10h00 - 2022-01-02 13h00"
        Tucuxi::Core::LastingDose lastingDose1{1, unit, formulationAndRoute, duration, interval};
        Tucuxi::Core::DosageTimeRange timeRange1{
            Tucuxi::Common::DateTime("2022-01-01T10:00:00", TestUtils::date_format),
            Tucuxi::Common::DateTime("2022-01-02T13:00:00", TestUtils::date_format),
            lastingDose1
        };

        // Making second time range "2022-01-03 14h00 - 2022-01-04 16h00"
        Tucuxi::Core::LastingDose lastingDose2{1, unit, formulationAndRoute, duration, interval};
        Tucuxi::Core::DosageTimeRange timeRange2{
            Tucuxi::Common::DateTime("2022-01-03T14:00:00", TestUtils::date_format),
            Tucuxi::Common::DateTime("2022-01-04T16:00:00", TestUtils::date_format),
            lastingDose1
        };

        // Making dosage history
        Tucuxi::Core::DosageHistory dosageHistory;
        dosageHistory.addTimeRange(timeRange1);
        dosageHistory.addTimeRange(timeRange2);

        fructose_assert_eq(Tucuxi::Xpert::getLatestDosageTimeRangeStart(dosageHistory, Tucuxi::Common::DateTime("2022-01-07T15:00:00", TestUtils::date_format)),
                           Tucuxi::Common::DateTime("2022-01-03T14:00:00", TestUtils::date_format));

        fructose_assert_eq(Tucuxi::Xpert::getLatestDosageTimeRangeStart(dosageHistory, Tucuxi::Common::DateTime("2022-01-01T09:00:00", TestUtils::date_format)).isUndefined(),
                           true);
    }

    /// \brief This method tests the computeFileName method.
    ///        It computes a XpertRequestResult based on classic imatinib request.
    ///        The names returned should be:
    ///        - "random/path/imatinib_1_11-7-2018_13h45m30s.xml" with the path and the extension.
    ///        - "imatinib_1_11-7-2018_13h45m30s.xml" without the path.
    ///        - "imatinib_1_11-7-2018_13h45m30s" without the path and without the exension.
    /// \param _testName Name of the test
    void computeFileName_behavesCorrectly(const std::string& _testName)
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
                                            <xpertRequest>
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
                                            </xpertRequest>
                                        </requests>
                                    </query>)";

        std::cout << _testName << std::endl;

        // Preparation - importation
        std::unique_ptr<Tucuxi::Xpert::XpertQueryResult> _xpertQueryResult = nullptr;

        TestUtils::setupEnv(queryString, TestUtils::originalImatinibModelString, _xpertQueryResult);
        Tucuxi::Xpert::XpertRequestResult& xpertRequestresult =  _xpertQueryResult->getXpertRequestResults()[0];

        // Verification
        fructose_assert_eq(Tucuxi::Xpert::computeFileName(xpertRequestresult), "random/path/imatinib_1_11-7-2018_13h45m30s.xml");
        fructose_assert_eq(Tucuxi::Xpert::computeFileName(xpertRequestresult, false), "imatinib_1_11-7-2018_13h45m30s.xml");
        fructose_assert_eq(Tucuxi::Xpert::computeFileName(xpertRequestresult, false, false), "imatinib_1_11-7-2018_13h45m30s");
    }

    /// \brief Check that the method executeRequestAndGetResult sets the result to nullptr when
    ///        the execution fails or that the resulting pointer is correctly retrieved. The test
    ///        prepares two percentile traits, one that must fail and one that must succeed.
    ///        One trait is prepared with a too large period which will fail.
    ///        The other is a valid trait like the ones used in samplevalidator.cpp.
    ///        - The first response must be nullptr.
    ///        - The second response must not be nullptr and must contain 3 percentiles.
    /// \param _testName Name of the test
    void executeRequestAndGetResult_behavesCorrectly(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>imatinib</queryId>
                                        <clientId>124568</clientId>
                                        <date>2018-07-11T13:45:30</date> <!-- Date the xml has been sent -->
                                        <language>en</language>

                                        <drugTreatment>
                                            <!-- All the information regarding the patient -->
                                            <patient>
                                                <covariates>
                                                    <covariate>
                                                        <covariateId>birthdate</covariateId>
                                                        <date>2018-07-11T10:45:30</date>
                                                        <value>1990-01-01T00:00:00</value>
                                                        <unit></unit>
                                                          <dataType>date</dataType>
                                                        <nature>discrete</nature>
                                                    </covariate>
                                                    <covariate>
                                                        <covariateId>bodyweight</covariateId>
                                                        <date>2017-07-06T08:00:00</date>
                                                        <value>70</value>
                                                        <unit>kg</unit>
                                                          <dataType>double</dataType>
                                                        <nature>discrete</nature>
                                                    </covariate>
                                                    <covariate>
                                                        <covariateId>bodyweight</covariateId>
                                                        <date>2018-07-06T08:00:00</date>
                                                        <value>150000</value>
                                                        <unit>g</unit>
                                                          <dataType>double</dataType>
                                                        <nature>discrete</nature>
                                                    </covariate>
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
                                            <xpertRequest>
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
                                            </xpertRequest>
                                        </requests>
                                    </query>)";

        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::Xpert::XpertQueryResult> xpertQueryResult = nullptr;

        TestUtils::setupEnv(queryString, TestUtils::originalImatinibModelString, xpertQueryResult);

        Tucuxi::Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

        // Preparing common values for the traits
        std::string responseId = "";
        Tucuxi::Core::PercentileRanks ranks = {1.0, 2.0, 3.0};
        double nbPointsPerHour = 1;
        Tucuxi::Core::ComputingOption computingOption{Tucuxi::Core::PredictionParameterType::Apriori, Tucuxi::Core::CompartmentsOption::AllActiveMoieties};
        Tucuxi::Common::DateTime start = Tucuxi::Common::DateTime("2018-07-07T13:00:00", TestUtils::date_format);

        // Normal end
        Tucuxi::Common::DateTime normalEnd = start + std::chrono::hours(1);

        // Too long start/end (+ 1'000'000 hours)
        Tucuxi::Common::DateTime tooFarEnd = start + std::chrono::hours(1000000);

        // Making traits and response pointers
        std::unique_ptr<Tucuxi::Core::ComputingTraitPercentiles> goodTrait =
                std::make_unique<Tucuxi::Core::ComputingTraitPercentiles>(responseId, start, normalEnd, ranks, nbPointsPerHour, computingOption);

        std::unique_ptr<Tucuxi::Core::PercentilesData> goodResult = nullptr;

        std::unique_ptr<Tucuxi::Core::ComputingTraitPercentiles> failTrait =
                std::make_unique<Tucuxi::Core::ComputingTraitPercentiles>(responseId, start, tooFarEnd, ranks, nbPointsPerHour, computingOption);

        std::unique_ptr<Tucuxi::Core::PercentilesData> failResult = nullptr;

        // Execution
        Tucuxi::Xpert::executeRequestAndGetResult(std::move(goodTrait), xpertRequestResult, goodResult);
        Tucuxi::Xpert::executeRequestAndGetResult(std::move(failTrait), xpertRequestResult, failResult);

        fructose_assert_eq(failResult.get(), nullptr);

        fructose_assert_ne(goodResult.get(), nullptr);
        fructose_assert_eq(goodResult->getNbRanks(), 3);
    }

    /// \brief Test that a camel case key is correctly converted into a phrase.
    ///        - "" must be ""
    ///        - "ab c" must be "Ab c"
    ///        - "camelCase" must be "camelCase"
    /// \param _testName Name of the test
    void keyToPhrase_behavesCorrectly(const std::string& _testName)
    {

        std::cout << _testName << std::endl;

        fructose_assert_eq(Tucuxi::Xpert::keyToPhrase(""), "");
        fructose_assert_eq(Tucuxi::Xpert::keyToPhrase("ab c"), "Ab c");
        fructose_assert_eq(Tucuxi::Xpert::keyToPhrase("camelCase"), "Camel case");
    }

    /// \brief Test the age calculator getAgeIn method.
    ///        The birthday is set "2000-01-01T12:00:00" and the computation time to "2010-01-01T12:00:00"
    ///        - The age in days must be 3653
    ///        - The age in weeks must be 521
    ///        - The age in months must be 119
    ///        - The age in years must be 10
    ///        - The age an invalid CovariateType throws an invalid_argument
    /// \param _testName Name of the test
    void getAgeIn_behavesCorrectly(const std::string& _testName)
    {

        std::cout << _testName << std::endl;

        Tucuxi::Common::DateTime birthDate = Tucuxi::Common::DateTime("2000-01-01T12:00:00", TestUtils::date_format);
        Tucuxi::Common::DateTime computationTime = Tucuxi::Common::DateTime("2010-01-01T12:00:00", TestUtils::date_format);

        fructose_assert_eq(int(Tucuxi::Xpert::getAgeIn(Tucuxi::Core::CovariateType::AgeInDays, birthDate, computationTime)), 3653);
        fructose_assert_eq(int(Tucuxi::Xpert::getAgeIn(Tucuxi::Core::CovariateType::AgeInWeeks, birthDate, computationTime)), 521);
        fructose_assert_eq(int(Tucuxi::Xpert::getAgeIn(Tucuxi::Core::CovariateType::AgeInMonths, birthDate, computationTime)), 119);
        fructose_assert_eq(int(Tucuxi::Xpert::getAgeIn(Tucuxi::Core::CovariateType::AgeInYears, birthDate, computationTime)), 10);
        fructose_assert_exception(Tucuxi::Xpert::getAgeIn(Tucuxi::Core::CovariateType(-1), birthDate, computationTime), std::invalid_argument);
    }
};

#endif // TEST_XPERTUTILS_H
