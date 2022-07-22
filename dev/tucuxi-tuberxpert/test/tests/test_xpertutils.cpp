#include "test_xpertutils.h"

using namespace std;
using namespace Tucuxi;

void TestXpertUtils::dataTypeToString_behavesCorrectly_withAllDataType(const string& _testName)
{
    cout << _testName << endl;

    fructose_assert_eq(Xpert::dataTypeToString(Core::DataType::Bool), "bool");
    fructose_assert_eq(Xpert::dataTypeToString(Core::DataType::Int), "int");
    fructose_assert_eq(Xpert::dataTypeToString(Core::DataType::Double), "double");
    fructose_assert_eq(Xpert::dataTypeToString(Core::DataType::Date), "date");
}

void TestXpertUtils::doubleToString_behavesCorrectly(const string& _testName)
{
    cout << _testName << endl;

    fructose_assert_eq(Xpert::doubleToString(5.411111), "5.41");
    fructose_assert_eq(Xpert::doubleToString(6), "6.00");
}

void TestXpertUtils::covariateTypeToString_behavesCorrectly_withAllTheCovariateTypesAndUnsupportedValues(const string& _testName)
{
    cout << _testName << endl;

    fructose_assert_eq(Xpert::covariateTypeToString(Xpert::CovariateType::MODEL), "default");
    fructose_assert_eq(Xpert::covariateTypeToString(Xpert::CovariateType::PATIENT), "patient");
    fructose_assert_exception(Xpert::covariateTypeToString(Xpert::CovariateType(-1)), invalid_argument);
}

void TestXpertUtils::outputLangToString_behavesCorrectly_withAllOutputLangAndUnsupportedValues(const string& _testName)
{
    cout << _testName << endl;

    fructose_assert_eq(Xpert::outputLangToString(Xpert::OutputLang::ENGLISH), "en");
    fructose_assert_eq(Xpert::outputLangToString(Xpert::OutputLang::FRENCH), "fr");
    fructose_assert_exception(Xpert::outputLangToString(Xpert::OutputLang(-1)), Xpert::LanguageException);
}

void TestXpertUtils::warningLevelToString_behavesCorrectly_withAllWarningLevelAndUnsupportedValues(const string& _testName)
{
    cout << _testName << endl;

    fructose_assert_eq(Xpert::warningLevelToString(Xpert::WarningLevel::CRITICAL), "critical");
    fructose_assert_eq(Xpert::warningLevelToString(Xpert::WarningLevel::NORMAL), "normal");
    fructose_assert_exception(Xpert::warningLevelToString(Xpert::WarningLevel(-1)), invalid_argument);
}

void TestXpertUtils::dateTimeToXmlString_behavesCorrectly(const string& _testName)
{
    cout << _testName << endl;

    string formatedDateTimeString = "2022-01-01T10:00:00";

    Common::DateTime formatedDateTime{formatedDateTimeString, TestUtils::date_format};

    fructose_assert_eq(Xpert::dateTimeToXmlString(formatedDateTime), formatedDateTimeString);
}

void TestXpertUtils::timeToString_behavesCorrectly(const string& _testName)
{
    cout << _testName << endl;

    Xpert::LanguageManager& langMgr = Xpert::LanguageManager::getInstance();
    langMgr.loadTranslations(TestUtils::englishTranslationFile);

    // Prepare the timeOfDay to convert
    Common::TimeOfDay timeOfDayWithoutMinute{Common::Duration(
                    chrono::hours(8),
                    chrono::minutes(0),
                    chrono::seconds(1))};

    Common::TimeOfDay timeOfDayWithMinute{Common::Duration(
                    chrono::hours(8),
                    chrono::minutes(30),
                    chrono::seconds(1))};

    // Check
    fructose_assert_eq(Xpert::TimeOfDayToString(timeOfDayWithoutMinute), "8h");
    fructose_assert_eq(Xpert::TimeOfDayToString(timeOfDayWithMinute), "8h30");
}

void TestXpertUtils::dateTimeToString_behavesCorrectly(const string& _testName)
{
    cout << _testName << endl;

    Xpert::LanguageManager& langMgr = Xpert::LanguageManager::getInstance();
    langMgr.loadTranslations(TestUtils::englishTranslationFile);

    string formatedDateTimeString = "2022-01-01T10:00:00";

    Common::DateTime formatedDateTime{formatedDateTimeString, TestUtils::date_format};

    fructose_assert_eq(Xpert::dateTimeToString(formatedDateTime), "1.1.2022 10h");
    fructose_assert_eq(Xpert::dateTimeToString(formatedDateTime, false), "1.1.2022");
}

void TestXpertUtils::beautifyString_behavesCorrectly(const string& _testName)
{
    cout << _testName << endl;

    Xpert::LanguageManager& langMgr = Xpert::LanguageManager::getInstance();
    langMgr.loadTranslations(TestUtils::englishTranslationFile);

    fructose_assert_eq(Xpert::beautifyString("1.0", Core::DataType::Bool, ""), "Yes");
    fructose_assert_eq(Xpert::beautifyString("0.0", Core::DataType::Bool, ""), "No");

    fructose_assert_eq(Xpert::beautifyString("1.0", Core::DataType::Double, "sex"), "Man");
    fructose_assert_eq(Xpert::beautifyString("0.0", Core::DataType::Double, "sex"), "Woman");
    fructose_assert_eq(Xpert::beautifyString("0.5", Core::DataType::Double, "sex"), "Undefined");

    fructose_assert_eq(Xpert::beautifyString("42.00", Core::DataType::Int, "age"), "42");

    fructose_assert_eq(Xpert::beautifyString("72.652222", Core::DataType::Double, "bodyweight"), "72.65");
}

void TestXpertUtils::getStringWithEnglishFallback_behavesCorrectly(const string& _testName)
{
    cout << _testName << endl;

    Common::TranslatableString translatableStringWithRequiredLang("translatable string in en");
    translatableStringWithRequiredLang.setString("chaine de caracteres traductible en fr", "fr");

    Common::TranslatableString translatableStringWithoutRequiredLangs("translatable string in en");

    Common::TranslatableString translatableStringWithoutRequiredLangAndWithoutEnglish;

    fructose_assert_eq(Xpert::getStringWithEnglishFallback(translatableStringWithRequiredLang, Xpert::OutputLang::FRENCH), "chaine de caracteres traductible en fr");
    fructose_assert_eq(Xpert::getStringWithEnglishFallback(translatableStringWithoutRequiredLangs, Xpert::OutputLang::FRENCH), "translatable string in en");
    fructose_assert_eq(Xpert::getStringWithEnglishFallback(translatableStringWithoutRequiredLangAndWithoutEnglish, Xpert::OutputLang::FRENCH), "");
}

void TestXpertUtils::getOldestDosageTimeRangeStart_behavesCorrectly(const string& _testName)
{
    cout << _testName << endl;

    // Common elements
    Core::Unit unit{"mg"};
    Core::FormulationAndRoute formulationAndRoute{Core::AbsorptionModel::Extravascular};
    Common::Duration duration, interval{chrono::hours(1)};

    // Making first time range "2022-01-01 10h00 - 2022-01-02 13h00"
    Core::LastingDose lastingDose1{1, unit, formulationAndRoute, duration, interval};
    Core::DosageTimeRange timeRange1{
        Common::DateTime("2022-01-01T10:00:00", TestUtils::date_format),
                Common::DateTime("2022-01-02T13:00:00", TestUtils::date_format),
                lastingDose1
    };

    // Making second time range "2022-01-03 14h00 - 2022-01-04 16h00"
    Core::LastingDose lastingDose2{1, unit, formulationAndRoute, duration, interval};
    Core::DosageTimeRange timeRange2{
        Common::DateTime("2022-01-03T14:00:00", TestUtils::date_format),
                Common::DateTime("2022-01-04T16:00:00", TestUtils::date_format),
                lastingDose1
    };

    // Making dsage history
    Core::DosageHistory dosageHistory;
    dosageHistory.addTimeRange(timeRange1);
    dosageHistory.addTimeRange(timeRange2);

    fructose_assert_eq(Xpert::getOldestDosageTimeRangeStart(dosageHistory, Common::DateTime("2022-01-01T14:00:00", TestUtils::date_format)),
                       Common::DateTime("2022-01-01T10:00:00", TestUtils::date_format));

    fructose_assert_eq(Xpert::getOldestDosageTimeRangeStart(dosageHistory, Common::DateTime("2022-01-01T09:00:00", TestUtils::date_format)),
                       Common::DateTime("2022-01-01T09:00:00", TestUtils::date_format));
}

void TestXpertUtils::getLatestDosageTimeRangeStart_behavesCorrectly(const string& _testName)
{
    cout << _testName << endl;

    // Common elements
    Core::Unit unit{"mg"};
    Core::FormulationAndRoute formulationAndRoute{Core::AbsorptionModel::Extravascular};
    Common::Duration duration, interval{chrono::hours(1)};

    // Making first time range "2022-01-01 10h00 - 2022-01-02 13h00"
    Core::LastingDose lastingDose1{1, unit, formulationAndRoute, duration, interval};
    Core::DosageTimeRange timeRange1{
        Common::DateTime("2022-01-01T10:00:00", TestUtils::date_format),
                Common::DateTime("2022-01-02T13:00:00", TestUtils::date_format),
                lastingDose1
    };

    // Making second time range "2022-01-03 14h00 - 2022-01-04 16h00"
    Core::LastingDose lastingDose2{1, unit, formulationAndRoute, duration, interval};
    Core::DosageTimeRange timeRange2{
        Common::DateTime("2022-01-03T14:00:00", TestUtils::date_format),
                Common::DateTime("2022-01-04T16:00:00", TestUtils::date_format),
                lastingDose1
    };

    // Making dosage history
    Core::DosageHistory dosageHistory;
    dosageHistory.addTimeRange(timeRange1);
    dosageHistory.addTimeRange(timeRange2);

    fructose_assert_eq(Xpert::getLatestDosageTimeRangeStart(dosageHistory, Common::DateTime("2022-01-07T15:00:00", TestUtils::date_format)),
                       Common::DateTime("2022-01-03T14:00:00", TestUtils::date_format));

    fructose_assert_eq(Xpert::getLatestDosageTimeRangeStart(dosageHistory, Common::DateTime("2022-01-01T09:00:00", TestUtils::date_format)).isUndefined(),
                       true);
}

void TestXpertUtils::computeFileName_behavesCorrectly(const string& _testName)
{
    string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">

                                        <date>2018-07-11T13:45:30</date> <!-- Date the xml has been sent -->

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

    cout << _testName << endl;

    // Preparation - importation
    unique_ptr<Xpert::XpertQueryResult> _xpertQueryResult = nullptr;

    TestUtils::setupEnv(queryString, TestUtils::originalImatinibModelString, _xpertQueryResult);
    Xpert::XpertRequestResult& xpertRequestresult =  _xpertQueryResult->getXpertRequestResults()[0];

    // Verification
    fructose_assert_eq(Xpert::computeFileName(xpertRequestresult), "random/path/imatinib_1_11-7-2018_13h45m30s.xml");
    fructose_assert_eq(Xpert::computeFileName(xpertRequestresult, false), "imatinib_1_11-7-2018_13h45m30s.xml");
    fructose_assert_eq(Xpert::computeFileName(xpertRequestresult, false, false), "imatinib_1_11-7-2018_13h45m30s");
}

void TestXpertUtils::executeRequestAndGetResult_behavesCorrectly(const string& _testName)
{

    string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">

                                        <date>2018-07-11T13:45:30</date> <!-- Date the xml has been sent -->

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

    cout << _testName << endl;

    unique_ptr<Xpert::XpertQueryResult> xpertQueryResult = nullptr;

    TestUtils::setupEnv(queryString, TestUtils::originalImatinibModelString, xpertQueryResult);

    Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

    // Preparing common values for the traits
    string responseId = "";
    Core::PercentileRanks ranks = {1.0, 2.0, 3.0};
    double nbPointsPerHour = 1;
    Core::ComputingOption computingOption{Core::PredictionParameterType::Apriori, Core::CompartmentsOption::AllActiveMoieties};
    Common::DateTime start = Common::DateTime("2018-07-07T13:00:00", TestUtils::date_format);

    // Normal end
    Common::DateTime normalEnd = start + chrono::hours(1);

    // Too long start/end (+ 1'000'000 hours)
    Common::DateTime tooFarEnd = start + chrono::hours(1000000);

    // Making traits and response pointers
    unique_ptr<Core::ComputingTraitPercentiles> goodTrait =
            make_unique<Core::ComputingTraitPercentiles>(responseId, start, normalEnd, ranks, nbPointsPerHour, computingOption);

    unique_ptr<Core::PercentilesData> goodResult = nullptr;

    unique_ptr<Core::ComputingTraitPercentiles> failTrait =
            make_unique<Core::ComputingTraitPercentiles>(responseId, start, tooFarEnd, ranks, nbPointsPerHour, computingOption);

    unique_ptr<Core::PercentilesData> failResult = nullptr;

    // Execution
    Xpert::executeRequestAndGetResult(move(goodTrait), xpertRequestResult, goodResult);
    Xpert::executeRequestAndGetResult(move(failTrait), xpertRequestResult, failResult);

    fructose_assert_eq(failResult.get(), nullptr);

    fructose_assert_ne(goodResult.get(), nullptr);
    fructose_assert_eq(goodResult->getNbRanks(), 3);
}

void TestXpertUtils::keyToPhrase_behavesCorrectly(const string& _testName)
{

    cout << _testName << endl;

    fructose_assert_eq(Xpert::keyToPhrase(""), "");
    fructose_assert_eq(Xpert::keyToPhrase("ab c"), "Ab c");
    fructose_assert_eq(Xpert::keyToPhrase("camelCase"), "Camel case");
}

void TestXpertUtils::getAgeIn_behavesCorrectly(const string& _testName)
{

    cout << _testName << endl;

    Common::DateTime birthDate = Common::DateTime("2000-01-01T12:00:00", TestUtils::date_format);
    Common::DateTime computationTime = Common::DateTime("2010-01-01T12:00:00", TestUtils::date_format);

    fructose_assert_eq(int(Xpert::getAgeIn(Core::CovariateType::AgeInDays, birthDate, computationTime)), 3653);
    fructose_assert_eq(int(Xpert::getAgeIn(Core::CovariateType::AgeInWeeks, birthDate, computationTime)), 521);
    fructose_assert_eq(int(Xpert::getAgeIn(Core::CovariateType::AgeInMonths, birthDate, computationTime)), 119);
    fructose_assert_eq(int(Xpert::getAgeIn(Core::CovariateType::AgeInYears, birthDate, computationTime)), 10);
    fructose_assert_exception(Xpert::getAgeIn(Core::CovariateType(-1), birthDate, computationTime), invalid_argument);
}
