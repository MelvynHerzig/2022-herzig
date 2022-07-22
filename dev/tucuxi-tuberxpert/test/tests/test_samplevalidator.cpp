#include "test_samplevalidator.h"

using namespace std;
using namespace Tucuxi;

void TestSampleValidator::createPercentilesData(Core::PercentilesData& _percentileData){

    // Prepare the PercentiledData
    // There are going to be 99 percentiles (1-99) with 2 CycleDatas:
    // 1.1.2022 10:00:00 - 1.1.2022 11:00:00, 1.1.2022 11:00:00 - 1.1.2022 12:00:00
    Common::DateTime d1{"2022-01-01T10:00:00", TestUtils::date_format};
    Common::DateTime d2{"2022-01-01T11:00:00", TestUtils::date_format};
    Common::DateTime d3{"2022-01-01T12:00:00", TestUtils::date_format};

    vector<Common::DateTime> cyclesStarts{d1, d2};
    vector<Common::DateTime> cyclesEnds{d2, d3};

    // For the 99 percentiles (1-99), each percentile start at p * 10 (1 = 10, 99 = 990)
    // The first cycle goes from p * 10 to p * 10 + 1 and the second from p * 10 + 1 to p * 10 + 2
    // (if p = 1 -> 10 - 11 , 11 - 12)
    // There will be 3 points per hour.
    // So, for the first percentile:
    // Cycle 1 [1.1.2022 10:00:00 - 1.1.2022 11:00:00], values: 10 10.5 11
    //                                                   times:  0  0.5  1
    // Cycle 2 [1.1.2022 11:00:00 - 1.1.2022 12:00:00], values: 11 11.5 12
    //                                                   times:  0  0.5  1

    const Core::TimeOffsets times{0, 0.5, 1};

    // For each percentile
    for (size_t pi = 1; pi <= 99 ; ++pi){

        // Create the cycle data
        vector<Core::CycleData> cycles;
        for (size_t cdi = 0; cdi < 2; ++cdi ){

            cycles.emplace_back(cyclesStarts[cdi], cyclesEnds[cdi], unit);

            // Prepare the concentrations
            Core::Concentrations concentrations;
            for (size_t ci = 0; ci < 3; ++ci){
                concentrations.emplace_back(pi * 10 + cdi + ci * 0.5);
            }

            cycles[cdi].addData(times, concentrations);
        }

        _percentileData.addPercentileData(cycles);
    }
}

void TestSampleValidator::sampleValidator_failure_whenTreatmentNullptr(const string& _testName)
{
    string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">

                                        <date>2022-06-20T10:00:00</date> <!-- Date the xml has been sent -->

                                        <drugTreatment>
                                            <!-- All the information regarding the patient -->
                                            <patient>
                                                <covariates>
                                                </covariates>
                                            </patient>
                                            <!-- List of the drugs informations we have concerning the patient -->
                                            <drugs>

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
                                            </xpertRequest>
                                        </requests>
                                    </query>)";

    cout << _testName << endl;

    // Prepare the XpertRequestResult
    vector<string> models {TestUtils::originalImatinibModelString};
    unique_ptr<Xpert::XpertQueryResult> xpertQueryResult;
    TestUtils::setupEnv(queryString, models, xpertQueryResult);

    Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

    // Execute
    TestUtils::flowStepProvider.getSampleValidator()->perform(xpertRequestResult);

    // Compare
    fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), false);
    fructose_assert_eq(xpertRequestResult.getErrorMessage(), "No treatment set.");
    fructose_assert_eq(xpertRequestResult.getSampleValidationResults().size(), 0);
}

void TestSampleValidator::sampleValidator_failure_whenSamplesWithoutDosages(const string& _testName)
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
                                            </xpertRequest>
                                        </requests>
                                    </query>)";

    cout << _testName << endl;

    // Prepare the XpertRequestResult
    vector<string> models {TestUtils::originalImatinibModelString};
    unique_ptr<Xpert::XpertQueryResult> xpertQueryResult;
    TestUtils::setupEnv(queryString, models, xpertQueryResult);

    Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

    // Execute
    TestUtils::flowStepProvider.getSampleValidator()->perform(xpertRequestResult);

    // Compare
    fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), false);
    fructose_assert_eq(xpertRequestResult.getErrorMessage(), "Samples found but dosage history is empty.");
    fructose_assert_eq(xpertRequestResult.getSampleValidationResults().size(), 0);
}

void TestSampleValidator::sampleValidator_failure_whenDrugModelNullptr(const string& _testName)
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
                                                        </dosageHistory>
                                                    </treatment>
                                                    <!-- Samples history -->
                                                    <samples>
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
                                            </xpertRequest>
                                        </requests>
                                    </query>)";

    cout << _testName << endl;

    // Prepare the XpertRequestResult
    vector<string> models {TestUtils::originalImatinibModelString};
    unique_ptr<Xpert::XpertQueryResult> xpertQueryResult;
    TestUtils::setupEnv(queryString, models, xpertQueryResult);

    Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

    // Execute
    TestUtils::flowStepProvider.getSampleValidator()->perform(xpertRequestResult);

    // Compare
    fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), false);
    fructose_assert_eq(xpertRequestResult.getErrorMessage(), "No drug model set.");
    fructose_assert_eq(xpertRequestResult.getSampleValidationResults().size(), 0);
}

void TestSampleValidator::sampleValidationResult_computesCorrectWarning(const string& _testName)
{

    cout << _testName << endl;

    // Load the translations file for the warning message computation
    TestUtils::loadTranslationsFile(TestUtils::englishTranslationFile);

    // Creating SampleValidationResult objects that are located in a different group. There are
    // 100 groups that are implicitly formed by the 99 percentiles.
    Xpert::SampleValidationResult sr1 = Xpert::SampleValidationResult(nullptr, 1);
    Xpert::SampleValidationResult sr10 = Xpert::SampleValidationResult(nullptr, 10);
    Xpert::SampleValidationResult sr11 = Xpert::SampleValidationResult(nullptr, 11);
    Xpert::SampleValidationResult sr90 = Xpert::SampleValidationResult(nullptr, 90);
    Xpert::SampleValidationResult sr91 = Xpert::SampleValidationResult(nullptr, 91);
    Xpert::SampleValidationResult sr100 = Xpert::SampleValidationResult(nullptr, 100);

    fructose_assert_eq(sr1.getWarning(), "99% of the population is above this measure");
    fructose_assert_eq(sr10.getWarning(), "90% of the population is above this measure");
    fructose_assert_eq(sr11.getWarning(), "");
    fructose_assert_eq(sr90.getWarning(), "");
    fructose_assert_eq(sr91.getWarning(), "90% of the population is below this measure");
    fructose_assert_eq(sr100.getWarning(), "99% of the population is below this measure");
}

void TestSampleValidator::findGroupPositionOver99Percentiles_behavesCorrectly_withGivenPercentilesDataAndSamples(const string& _testName)
{
    cout << _testName << endl;

    Core::PercentilesData percentilesData{""};
    createPercentilesData(percentilesData);

    // We perform 9 series of tests.
    // The times tested are from 1.1.2022 10:00:00 to 1.1.2022 12:00:00 every 15 minutes.
    // 18 concentrations are tested, 2 at each time: one slightly below the percentile
    //                                               one slightly above the percentile
    vector<Common::DateTime> testDates{
        Common::DateTime{"2022-01-01T10:00:00", TestUtils::date_format},
        Common::DateTime{"2022-01-01T10:15:00", TestUtils::date_format},
        Common::DateTime{"2022-01-01T10:30:00", TestUtils::date_format},
        Common::DateTime{"2022-01-01T10:45:00", TestUtils::date_format},
        Common::DateTime{"2022-01-01T11:00:00", TestUtils::date_format},
        Common::DateTime{"2022-01-01T11:15:00", TestUtils::date_format},
        Common::DateTime{"2022-01-01T11:30:00", TestUtils::date_format},
        Common::DateTime{"2022-01-01T11:45:00", TestUtils::date_format},
        Common::DateTime{"2022-01-01T12:00:00", TestUtils::date_format},
    };

    Xpert::SampleValidator sampleValidator;
    // For each percentile
    for (size_t pi = 1; pi <= 99 ; ++pi){
        for (size_t ti = 0; ti < 9; ++ti) {
            double percentileValue = pi * 10 + ti * 0.25;

            // Create a sample that is slightly above and a sample that is slightly below the percentile
            unique_ptr<Core::Sample> slightlyAbove = make_unique<Core::Sample>(testDates[ti], Core::AnalyteId{""}, percentileValue + 0.01 , unit);
            unique_ptr<Core::Sample> slightlyBelow = make_unique<Core::Sample>(testDates[ti], Core::AnalyteId{""}, percentileValue - 0.01 , unit);

            unsigned groupSlightlyAbove = sampleValidator.findGroupPositionOver99Percentiles(&percentilesData, slightlyAbove);
            unsigned groupSlightlyBelow = sampleValidator.findGroupPositionOver99Percentiles(&percentilesData, slightlyBelow);

            // Compare
            fructose_assert_eq(groupSlightlyAbove, pi + 1);
            fructose_assert_eq(groupSlightlyBelow, pi);
        }
    }
}

void TestSampleValidator::findGroupPositionOver99Percentiles_throwInvalidArgument_whenSampleUnitConversionFailure(const string& _testName)
{
    cout << _testName << endl;

    // Prepare percentilesData and SampleValidator
    Core::PercentilesData percentilesData{""};
    createPercentilesData(percentilesData);

    Xpert::SampleValidator sampleValidator;

    // Prepare a sample with "kg" as unit.
    // "kg" cannot be converted in "ug/l".
    unique_ptr<Core::Sample> sample = make_unique<Core::Sample>(
                Common::DateTime{"2022-01-01T11:30:00", TestUtils::date_format},
                Core::AnalyteId{""},
                1,
                Common::TucuUnit{"kg"});

    // Compare
    fructose_assert_exception(sampleValidator.findGroupPositionOver99Percentiles(&percentilesData, sample), invalid_argument);
}

void TestSampleValidator::findGroupPositionOver99Percentiles_throwInvalidArgument_whenSampleDateNotFoundInCycleData(const string& _testName)
{
    cout << _testName << endl;

    Core::PercentilesData percentilesData{""};
    createPercentilesData(percentilesData);

    // Prepare percentilesData and SampleValidator
    Xpert::SampleValidator sampleValidator;

    // Prepare a sample with a date in 2023.
    // 2023 is not between  "2022-01-01T10:00:00" and ""2022-01-01T12:00:00"".
    unique_ptr<Core::Sample> sample = make_unique<Core::Sample>(
                Common::DateTime{"2023-01-01T11:30:00", TestUtils::date_format},
                Core::AnalyteId{""},
                1,
                unit);

    // Compare
    fructose_assert_exception(sampleValidator.findGroupPositionOver99Percentiles(&percentilesData, sample), invalid_argument);
}

void TestSampleValidator::getSampleValidationResult_isSorted_whenNotEmpty(const string& _testName)
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
                                                            <sampleDate>2018-07-08T07:00:00</sampleDate>
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
                                                            <sampleDate>2018-07-06T13:00:00</sampleDate>
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
                                            </xpertRequest>
                                        </requests>
                                    </query>)";


    cout << _testName << endl;

    // Prepare the XpertRequestResult
    unique_ptr<Xpert::XpertQueryResult> xpertQueryResult;
    TestUtils::setupEnv(queryString, TestUtils::originalImatinibModelString, xpertQueryResult);

    Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

    // Execute
    TestUtils::flowStepProvider.getSampleValidator()->perform(xpertRequestResult);

    const vector<Xpert::SampleValidationResult>& sampleValidationResults = xpertRequestResult.getSampleValidationResults();

    // Compare
    fructose_assert_eq(sampleValidationResults.size(), xpertRequestResult.getTreatment()->getSamples().size());

    fructose_assert_eq(sampleValidationResults[0].getSource()->getDate(), Common::DateTime("2018-07-06T13:00:00", TestUtils::date_format))
            fructose_assert_eq(sampleValidationResults[1].getSource()->getDate(), Common::DateTime("2018-07-07T06:00:30", TestUtils::date_format))
            fructose_assert_eq(sampleValidationResults[2].getSource()->getDate(), Common::DateTime("2018-07-08T07:00:00", TestUtils::date_format))
}
