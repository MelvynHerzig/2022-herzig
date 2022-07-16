#ifndef TEST_SAMPLEVALIDATOR_H
#define TEST_SAMPLEVALIDATOR_H

#include <numeric>
#include <string>
#include <memory>

#include "tucucore/drugtreatment/sample.h"
#include "tucucore/drugmodel/analyte.h"
#include "tucucore/definitions.h"
#include "tucucommon/datetime.h"
#include "tucucommon/unit.h"

#include "tuberxpert/flow/general/generalxpertflowstepprovider.h"
#include "tuberxpert/flow/general/samplevalidator.h"
#include "tuberxpert/query/xpertquerydata.h"
#include "tuberxpert/result/xpertqueryresult.h"
#include "tuberxpert/result/samplevalidationresult.h"

#include "testutils.h"

#include "fructose/fructose.h"

/// \brief Tests for SampleValidator from the XpertFlowStepProvider.
/// \date 06/06/2022
/// \author Herzig Melvyn
struct TestSampleValidator : public fructose::test_base<TestSampleValidator>
{
    /// \brief Unit used to create the percentile data.
    const Tucuxi::Common::TucuUnit unit{"ug/l"};

    /// \brief Create some percentile data and insert them into _percentileData.
    /// \param _percentileData PercentilesData object which receives the result.
    void createPercentilesData( Tucuxi::Core::PercentilesData& _percentileData){

        // Prepare the PercentiledData
        // There are going to be 99 percentiles (1-99) with 2 CycleDatas:
        // 1.1.2022 10:00:00 - 1.1.2022 11:00:00, 1.1.2022 11:00:00 - 1.1.2022 12:00:00
        Tucuxi::Common::DateTime d1{"2022-01-01T10:00:00", TestUtils::date_format};
        Tucuxi::Common::DateTime d2{"2022-01-01T11:00:00", TestUtils::date_format};
        Tucuxi::Common::DateTime d3{"2022-01-01T12:00:00", TestUtils::date_format};

        std::vector<Tucuxi::Common::DateTime> cyclesStarts{d1, d2};
        std::vector<Tucuxi::Common::DateTime> cyclesEnds{d2, d3};

        // For the 99 percentiles (1-99), each percentile start at p * 10 (1 = 10, 99 = 990)
        // The first cycle goes from p * 10 to p * 10 + 1 and the second from p * 10 + 1 to p * 10 + 2
        // (if p = 1 -> 10 - 11 , 11 - 12)
        // There will be 3 points per hour.
        // So, for the first percentile:
        // Cycle 1 [1.1.2022 10:00:00 - 1.1.2022 11:00:00], values: 10 10.5 11
        //                                                   times:  0  0.5  1
        // Cycle 2 [1.1.2022 11:00:00 - 1.1.2022 12:00:00], values: 11 11.5 12
        //                                                   times:  0  0.5  1

        const Tucuxi::Core::TimeOffsets times{0, 0.5, 1};

        // For each percentile
        for (size_t pi = 1; pi <= 99 ; ++pi){

            // Create the cycle data
            std::vector<Tucuxi::Core::CycleData> cycles;
            for (size_t cdi = 0; cdi < 2; ++cdi ){

                cycles.emplace_back(cyclesStarts[cdi], cyclesEnds[cdi], unit);

                // Prepare the concentrations
                Tucuxi::Core::Concentrations concentrations;
                for (size_t ci = 0; ci < 3; ++ci){
                    concentrations.emplace_back(pi * 10 + cdi + ci * 0.5);
                }

                cycles[cdi].addData(times, concentrations);
            }

            _percentileData.addPercentileData(cycles);
        }
    }

    /// \brief Check that there is an error if the treatment of an XpertRequestResult is nullptr
    ///        in SampleValidator.
    ///        The SampleValidator must set the error in the XpertRequestResult,
    ///        shouldBeProcessed must return false and there should be 0 SampleValidationResult.
    /// \param _testName Name of the test
    void sampleValidator_failure_whenTreatmentNullptr(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>imatinib</queryId>
                                        <clientId>124568</clientId>
                                        <date>2022-06-20T10:00:00</date> <!-- Date the xml has been sent -->
                                        <language>en</language>

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

        std::cout << _testName << std::endl;

        // Prepare the XpertRequestResult
        std::vector<std::string> models {TestUtils::originalImatinibModelString};
        std::unique_ptr<Tucuxi::Xpert::XpertQueryResult> xpertQueryResult;
        TestUtils::setupEnv(queryString, models, xpertQueryResult);

        Tucuxi::Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

        // Execute
        TestUtils::flowStepProvider.getSampleValidator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), false);
        fructose_assert_eq(xpertRequestResult.getErrorMessage(), "No treatment set.");
        fructose_assert_eq(xpertRequestResult.getSampleValidationResults().size(), 0);
    }

    /// \brief Check that there is an error if the treatment of an XpertRequestResult has
    ///        samples but no dosages in SampleValidator.
    ///        The SampleValidator must set the error in the XpertRequestResult,
    ///        shouldBeProcessed must return false and there should be 0 SampleValidationResult.
    /// \param _testName Name of the test
    void sampleValidator_failure_whenSamplesWithoutDosages(const std::string& _testName)
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

        std::cout << _testName << std::endl;

        // Prepare the XpertRequestResult
        std::vector<std::string> models {TestUtils::originalImatinibModelString};
        std::unique_ptr<Tucuxi::Xpert::XpertQueryResult> xpertQueryResult;
        TestUtils::setupEnv(queryString, models, xpertQueryResult);

        Tucuxi::Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

        // Execute
        TestUtils::flowStepProvider.getSampleValidator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), false);
        fructose_assert_eq(xpertRequestResult.getErrorMessage(), "Samples found but dosage history is empty.");
        fructose_assert_eq(xpertRequestResult.getSampleValidationResults().size(), 0);
    }

    /// \brief The test loads an xpertRequest without assigning a drug model to it.
    ///        The SampleValidator must set the error in the XpertRequestResult,
    ///        shouldBeProcessed must return false and there should be 0 SampleValidationResult.
    /// \param _testName Name of the test
    void sampleValidator_failure_whenDrugModelNullptr(const std::string& _testName)
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

        std::cout << _testName << std::endl;

        // Prepare the XpertRequestResult
        std::vector<std::string> models {TestUtils::originalImatinibModelString};
        std::unique_ptr<Tucuxi::Xpert::XpertQueryResult> xpertQueryResult;
        TestUtils::setupEnv(queryString, models, xpertQueryResult);

        Tucuxi::Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

        // Execute
        TestUtils::flowStepProvider.getSampleValidator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), false);
        fructose_assert_eq(xpertRequestResult.getErrorMessage(), "No drug model set.");
        fructose_assert_eq(xpertRequestResult.getSampleValidationResults().size(), 0);
    }

    /// \brief This test creates fake SampleValidationResult objects and checks
    ///        that the warnings are correct.
    ///           - If the group is 1, the message must be "99% of the population is above this measure".
    ///           - If the group is 10, the message must be "90% of the population is above this measure".
    ///           - If the group is 11, the message must be "".
    ///           - If the group is 90, the message must be "".
    ///           - If the group is 91, the message must be "90% of the population is below this measure".
    ///           - If the group is 100, the message must be "99% of the population is below this measure".
    ///
    /// \param _testName Name of the test
    void sampleValidationResult_computesCorrectWarning(const std::string& _testName)
    {

        std::cout << _testName << std::endl;

        // Load the translations file for the warning message computation
        TestUtils::loadTranslationsFile(TestUtils::englishTranslationFile);

        // Creating SampleValidationResult objects that are located in a different group. There are
        // 100 groups that are implicitly formed by the 99 percentiles.
        Tucuxi::Xpert::SampleValidationResult sr1 = Tucuxi::Xpert::SampleValidationResult(nullptr, 1);
        Tucuxi::Xpert::SampleValidationResult sr10 = Tucuxi::Xpert::SampleValidationResult(nullptr, 10);
        Tucuxi::Xpert::SampleValidationResult sr11 = Tucuxi::Xpert::SampleValidationResult(nullptr, 11);
        Tucuxi::Xpert::SampleValidationResult sr90 = Tucuxi::Xpert::SampleValidationResult(nullptr, 90);
        Tucuxi::Xpert::SampleValidationResult sr91 = Tucuxi::Xpert::SampleValidationResult(nullptr, 91);
        Tucuxi::Xpert::SampleValidationResult sr100 = Tucuxi::Xpert::SampleValidationResult(nullptr, 100);

        fructose_assert_eq(sr1.getWarning(), "99% of the population is above this measure");
        fructose_assert_eq(sr10.getWarning(), "90% of the population is above this measure");
        fructose_assert_eq(sr11.getWarning(), "");
        fructose_assert_eq(sr90.getWarning(), "");
        fructose_assert_eq(sr91.getWarning(), "90% of the population is below this measure");
        fructose_assert_eq(sr100.getWarning(), "99% of the population is below this measure");
    }

    /// \brief This method checks that the SampleValidator::findPosOver100Percentile method returns
    ///        the correct positions for a given set of samples and a given percentileData.
    ///
    ///        We test the SampleValidator class with SampleValidator::findPosOver100Percentile and not
    ///        with SampleValidator::perform because this last method performs a request
    ///        on the core that returns "unpredictable" percentiles. So, basically 90% of SampleValidator's
    ///        behavior can be verified with SampleValidator::findPosOver100Percentile.
    ///
    ///        Thus, this struct is friend with SampleValidator to acces this protected method.
    /// \param _testName Name of the test
    void findGroupPositionOver99Percentiles_behavesCorrectly_withGivenPercentilesDataAndSamples(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        Tucuxi::Core::PercentilesData percentilesData{""};
        createPercentilesData(percentilesData);

        // We perform 9 series of tests.
        // The times tested are from 1.1.2022 10:00:00 to 1.1.2022 12:00:00 every 15 minutes.
        // 18 concentrations are tested, 2 at each time: one slightly below the percentile
        //                                               one slightly above the percentile
        std::vector<Tucuxi::Common::DateTime> testDates{
            Tucuxi::Common::DateTime{"2022-01-01T10:00:00", TestUtils::date_format},
            Tucuxi::Common::DateTime{"2022-01-01T10:15:00", TestUtils::date_format},
            Tucuxi::Common::DateTime{"2022-01-01T10:30:00", TestUtils::date_format},
            Tucuxi::Common::DateTime{"2022-01-01T10:45:00", TestUtils::date_format},
            Tucuxi::Common::DateTime{"2022-01-01T11:00:00", TestUtils::date_format},
            Tucuxi::Common::DateTime{"2022-01-01T11:15:00", TestUtils::date_format},
            Tucuxi::Common::DateTime{"2022-01-01T11:30:00", TestUtils::date_format},
            Tucuxi::Common::DateTime{"2022-01-01T11:45:00", TestUtils::date_format},
            Tucuxi::Common::DateTime{"2022-01-01T12:00:00", TestUtils::date_format},
        };

        Tucuxi::Xpert::SampleValidator sampleValidator;
        // For each percentile
        for (size_t pi = 1; pi <= 99 ; ++pi){
            for (size_t ti = 0; ti < 9; ++ti) {
                double percentileValue = pi * 10 + ti * 0.25;

                // Create a sample that is slightly above and a sample that is slightly below the percentile
                std::unique_ptr<Tucuxi::Core::Sample> slightlyAbove = std::make_unique<Tucuxi::Core::Sample>(testDates[ti], Tucuxi::Core::AnalyteId{""}, percentileValue + 0.01 , unit);
                std::unique_ptr<Tucuxi::Core::Sample> slightlyBelow = std::make_unique<Tucuxi::Core::Sample>(testDates[ti], Tucuxi::Core::AnalyteId{""}, percentileValue - 0.01 , unit);

                unsigned groupSlightlyAbove = sampleValidator.findGroupPositionOver99Percentiles(&percentilesData, slightlyAbove);
                unsigned groupSlightlyBelow = sampleValidator.findGroupPositionOver99Percentiles(&percentilesData, slightlyBelow);

                // Compare
                fructose_assert_eq(groupSlightlyAbove, pi + 1);
                fructose_assert_eq(groupSlightlyBelow, pi);
            }
        }
    }

    /// \brief This method checks that the method SampleValidator::findPosOver100Percentile throws
    ///        an invalid_argument exception when a sample unit cannot be converted into the unit
    ///        of the cycleData of the percentilesData object.
    /// \param _testName Name of the test
    void findGroupPositionOver99Percentiles_throwInvalidArgument_whenSampleUnitConversionFailure(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        // Prepare percentilesData and SampleValidator
        Tucuxi::Core::PercentilesData percentilesData{""};
        createPercentilesData(percentilesData);

        Tucuxi::Xpert::SampleValidator sampleValidator;

        // Prepare a sample with "kg" as unit.
        // "kg" cannot be converted in "ug/l".
        std::unique_ptr<Tucuxi::Core::Sample> sample = std::make_unique<Tucuxi::Core::Sample>(
                    Tucuxi::Common::DateTime{"2022-01-01T11:30:00", TestUtils::date_format},
                    Tucuxi::Core::AnalyteId{""},
                    1,
                    Tucuxi::Common::TucuUnit{"kg"});

        // Compare
        fructose_assert_exception(sampleValidator.findGroupPositionOver99Percentiles(&percentilesData, sample), std::invalid_argument);
    }

    /// \brief This method checks that the method SampleValidator::findPosOver100Percentile throws
    ///        an invalid_argument exception when a sample date cannot be found in the cycleData objects.
    /// \param _testName Name of the test
    void findGroupPositionOver99Percentiles_throwInvalidArgument_whenSampleDateNotFoundInCycleData(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        Tucuxi::Core::PercentilesData percentilesData{""};
        createPercentilesData(percentilesData);

         // Prepare percentilesData and SampleValidator
        Tucuxi::Xpert::SampleValidator sampleValidator;

        // Prepare a sample with a date in 2023.
        // 2023 is not between  "2022-01-01T10:00:00" and ""2022-01-01T12:00:00"".
        std::unique_ptr<Tucuxi::Core::Sample> sample = std::make_unique<Tucuxi::Core::Sample>(
                    Tucuxi::Common::DateTime{"2023-01-01T11:30:00", TestUtils::date_format},
                    Tucuxi::Core::AnalyteId{""},
                    1,
                    unit);

        // Compare
        fructose_assert_exception(sampleValidator.findGroupPositionOver99Percentiles(&percentilesData, sample), std::invalid_argument);
    }

    /// \brief This method performs a real sample validation with 3 samples. The vector retrived by
    ///        XpertRequestResult::getSampleValidationResults must be sorted by sample date.
    ///        We only check the sample date. The other interesting values, such as:
    ///        percentile, warning type, warning message are already tested by more deterministic tests.
    ///
    ///        The smallest date comes first.
    /// \param _testName Name of the test
    void getSampleValidationResult_isSorted_whenNotEmpty(const std::string& _testName)
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


        std::cout << _testName << std::endl;

        // Prepare the XpertRequestResult
        std::unique_ptr<Tucuxi::Xpert::XpertQueryResult> xpertQueryResult;
        TestUtils::setupEnv(queryString, TestUtils::originalImatinibModelString, xpertQueryResult);

        Tucuxi::Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

        // Execute
        TestUtils::flowStepProvider.getSampleValidator()->perform(xpertRequestResult);

        const std::vector<Tucuxi::Xpert::SampleValidationResult>& sampleValidationResults = xpertRequestResult.getSampleValidationResults();

        // Compare
        fructose_assert_eq(sampleValidationResults.size(), xpertRequestResult.getTreatment()->getSamples().size());

        fructose_assert_eq(sampleValidationResults[0].getSource()->getDate(), Tucuxi::Common::DateTime("2018-07-06T13:00:00", TestUtils::date_format))
        fructose_assert_eq(sampleValidationResults[1].getSource()->getDate(), Tucuxi::Common::DateTime("2018-07-07T06:00:30", TestUtils::date_format))
        fructose_assert_eq(sampleValidationResults[2].getSource()->getDate(), Tucuxi::Common::DateTime("2018-07-08T07:00:00", TestUtils::date_format))
    }
};

#endif // TEST_SAMPLEVALIDATOR_H
