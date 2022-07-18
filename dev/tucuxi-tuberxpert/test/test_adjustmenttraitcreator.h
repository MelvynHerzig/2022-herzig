#ifndef TEST_ADJUSTMENTTRAITCREATOR_H
#define TEST_ADJUSTMENTTRAITCREATOR_H

#include <memory>

#include "tuberxpert/result/xpertrequestresult.h"
#include "tuberxpert/result/xpertqueryresult.h"

#include "testutils.h"

#include "fructose/fructose.h"

/// \brief Tests for AdjustmentTraitCreator from the XpertFlowStepProvider.
///        This class performs several adjustment creations to check if the result is the expected one.
///        Each test tests an attribute (or a subset of attributes) of the returned adjustment trait.
/// \date 17/06/2022
/// \author Herzig Melvyn
struct TestAdjustmentTraitCreator : public fructose::test_base<TestAdjustmentTraitCreator>
{

    /// \brief Check that there is an error if the treatment of an XpertRequestResult is nullptr
    ///        in AdjustmentTraitCreator.
    ///        The AdjustmentTraitCreator must set the error in the XpertRequestResult and
    ///        shouldBeProcessed must return false
    /// \param _testName Name of the test
    void adjustmentTraitCreator_failure_whenTreatmentNullptr(const std::string& _testName)
    {
        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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

        std::cout << _testName << std::endl;

        // Prepare the XpertRequestResult
        std::vector<std::string> models {TestUtils::originalImatinibModelString};
        std::unique_ptr<Tucuxi::Xpert::XpertQueryResult> xpertQueryResult;
        TestUtils::setupEnv(queryString, models, xpertQueryResult);

        Tucuxi::Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

        // Execute
        TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), false);
        fructose_assert_eq(xpertRequestResult.getErrorMessage(), "No treatment set.");
    }

    /// \brief The test loads an xpertRequest without assigning a drug model to it.
    ///        The AdjustmentTraitCreator must set the error in the XpertRequestResult and
    ///        shouldBeProcessed must return false.
    /// \param _testName Name of the test
    void adjustmentTraitCreator_failure_whenDrugModelNullptr(const std::string& _testName)
    {
        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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
        TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), false);
        fructose_assert_eq(xpertRequestResult.getErrorMessage(), "No drug model set.");
    }

    /// \brief This method checks that the adjustment trait creator defines the expected number of points
    ///        per hours (i.e. 20) and that there is no error (i.e. shouldContinueProcessing returns true).
    /// \param _testName Name of the test
    void adjustmentTraitCreator_sets20PointsPerHour(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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
        std::unique_ptr<Tucuxi::Xpert::XpertQueryResult> xpertQueryResult;
        TestUtils::setupEnv(queryString, TestUtils::originalImatinibModelString, xpertQueryResult);

        Tucuxi::Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

        // Execute
        TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
        fructose_assert_eq(xpertRequestResult.getAdjustmentTrait()->getNbPointsPerHour(), 20);
    }

    /// \brief This method checks that the adjustment trait creator sets the expected computing options
    ///        (i.e. AllActiveMoieties, RetrieveStatistics, RetrieveParameters, RetrieveCovariates and forceUgPerLiter)
    ///        and that there is no error (i.e. shouldContinueProcessing returns true).
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsGoodFixedComputingOptions(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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
                                                <adjustmentDate>2018-07-06T08:00:00</adjustmentDate>
                                            </xpertRequest>
                                        </requests>
                                    </query>)";



        std::cout << _testName << std::endl;

        // Prepare the XpertRequestResult
        std::unique_ptr<Tucuxi::Xpert::XpertQueryResult> xpertQueryResult;
        TestUtils::setupEnv(queryString, TestUtils::originalImatinibModelString, xpertQueryResult);

        Tucuxi::Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

        // Execute
        TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);

        Tucuxi::Core::ComputingOption computingOption = xpertRequestResult.getAdjustmentTrait()->getComputingOption();
        fructose_assert_eq(computingOption.getCompartmentsOption() == Tucuxi::Core::CompartmentsOption::AllActiveMoieties, true);
        fructose_assert_eq(computingOption.retrieveStatistics() == Tucuxi::Core::RetrieveStatisticsOption::RetrieveStatistics, true);
        fructose_assert_eq(computingOption.retrieveParameters() == Tucuxi::Core::RetrieveParametersOption::RetrieveParameters, true);
        fructose_assert_eq(computingOption.retrieveCovariates() == Tucuxi::Core::RetrieveCovariatesOption::RetrieveCovariates, true);
        fructose_assert_eq(computingOption.forceUgPerLiter() == Tucuxi::Core::ForceUgPerLiterOption::Force, true);
    }

    /// \brief This method checks that the adjustment trait creator sets the parameters type of
    ///        the computing options to Apriori when there is no dosage and no sample
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsAprioriParameterType_whenNoDosageAndNoSample(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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
        std::unique_ptr<Tucuxi::Xpert::XpertQueryResult> xpertQueryResult;
        TestUtils::setupEnv(queryString, TestUtils::originalImatinibModelString, xpertQueryResult);

        Tucuxi::Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

        // Execute
        TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
        fructose_assert_eq(xpertRequestResult.getAdjustmentTrait()->getComputingOption().getParametersType() == Tucuxi::Core::PredictionParameterType::Apriori, true);
    }

    /// \brief This method checks that the adjustment trait creator sets the parameters type of
    ///        the computing option to Apriori when there are dosages but no sample.
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsAprioriParameterType_whenDosagesButNoSample(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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
        TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
        fructose_assert_eq(xpertRequestResult.getAdjustmentTrait()->getComputingOption().getParametersType() == Tucuxi::Core::PredictionParameterType::Apriori, true);
    }

    /// \brief This method checks that the adjustment trait creator sets the parameters type of
    ///        the computing option to Aposteriori when there are dosages and samples.
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsAposterioriParameterType_whenDosagesAndSamples(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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
                                                            <sampleId>10</sampleId>
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
        std::unique_ptr<Tucuxi::Xpert::XpertQueryResult> xpertQueryResult;
        TestUtils::setupEnv(queryString, TestUtils::originalImatinibModelString, xpertQueryResult);

        Tucuxi::Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

        // Execute
        TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
        fructose_assert_eq(xpertRequestResult.getAdjustmentTrait()->getComputingOption().getParametersType() == Tucuxi::Core::PredictionParameterType::Aposteriori, true);
    }


    /// \brief This method checks that the adjustment time matches the adjustment time of the xpert request request when it is set.
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsCorrectAdjustmentTime_whenSetInXpertRequest(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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
                                                <adjustmentDate>2018-07-06T08:00:00</adjustmentDate>
                                            </xpertRequest>
                                        </requests>
                                    </query>)";



        std::cout << _testName << std::endl;

        // Prepare the XpertRequestResult
        std::unique_ptr<Tucuxi::Xpert::XpertQueryResult> xpertQueryResult;
        TestUtils::setupEnv(queryString, TestUtils::originalImatinibModelString, xpertQueryResult);

        Tucuxi::Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

        // Execute
        TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
        fructose_assert_eq(xpertRequestResult.getAdjustmentTrait()->getAdjustmentTime(),  Tucuxi::Common::DateTime("2018-07-06T08:00:00", TestUtils::date_format));
    }

    /// \brief This method checks that the adjustment time matches the computing time (2022-06-20 10h) plus one hour (2022-06-20 11h)
    ///        when the adjustment time is not set in the xpertRequest and there is no dosage history.
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsAdjustmentTimeToComputingTimePlusOneHour_whenNotSetInXpertRequestAndNoDosageHistory(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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
                                                <!-- All the information regarding the drug -->
                                                <drug>
                                                    <drugId>imatinib</drugId>
                                                    <activePrinciple>something</activePrinciple>
                                                    <brandName>somebrand</brandName>
                                                    <atc>something</atc>
                                                    <!-- All the information regarding the treatment -->
                                                    <treatment>
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
        std::unique_ptr<Tucuxi::Xpert::XpertQueryResult> xpertQueryResult;
        TestUtils::setupEnv(queryString, TestUtils::originalImatinibModelString, xpertQueryResult);

        Tucuxi::Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

        // Execute
        TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
        fructose_assert_eq(xpertRequestResult.getAdjustmentTrait()->getAdjustmentTime(),  Tucuxi::Common::DateTime("2022-06-20T11:00:00", TestUtils::date_format));
    }

    /// \brief This method checks that the adjustment time matches the computing time (2022-06-20 10h00) plus one hour (2022-06-20 11h)
    ///        when there is a dosage history but in the future (2023) and the adjustment time is not set in the xpertRequest.
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsAdjustmentTimeToComputingTimePlusOneHour_whenNotSetInXpertRequestAndFutureDosageHistory(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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
                                                                <start>2023-07-06T08:00:00</start>
                                                                <end>2023-07-08T08:00:00</end>
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
        TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
        fructose_assert_eq(xpertRequestResult.getAdjustmentTrait()->getAdjustmentTime(),  Tucuxi::Common::DateTime("2022-06-20T11:00:00", TestUtils::date_format));
    }

    /// \brief This method checks that the adjustment time matches the next intake time (2022-06-20 20h00) when there
    ///        is an ongoing dosage history (computing time is 20-6-2022 and dosage time range goes from 19-6-2022 to 21-6-2022)
    ///        and the adjustment time is not set in the xpertRequest. There must be no error (i.e. shouldContinueProcessing returns true).
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsAdjustmentTimeToNextIntakeTime_whenNotSetInXpertRequestAndOngoingDosageHistory(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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
                                                                <start>2022-06-19T08:00:00</start>
                                                                <end>2022-06-21T08:00:00</end>
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
        TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
        fructose_assert_eq(xpertRequestResult.getAdjustmentTrait()->getAdjustmentTime(),  Tucuxi::Common::DateTime("2022-06-20T20:00:00", TestUtils::date_format));
    }

    /// \brief This method checks that the adjustment time is the time resulting from the half-life (x2) added
    ///        to the last intake until the computation time is reached. In the test, the dosage history is in
    ///        2018 (from 6.7 9h00 to 8.7 9h00). Therefore, the last intake is on 7.7.2018 21h.
    ///        Since the half-life of imatinib is 12 hours, 2 x the hal-life is 1 day. Thus, if we add 1 day on top of
    ///        8.7.2018 9h until the computation time (20.6.2022 10h) is reached, the adjustment time should be
    ///        20.6.2022 21h. There must be no error (i.e. shouldContinueProcessing returns true).
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsAdjustmentTimeWithHalfLifeApproximation_whenNotSetInXpertRequestAndCompletedDosageHistory(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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
                                                                <start>2018-07-06T09:00:00</start>
                                                                <end>2018-07-08T09:00:00</end>
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
        TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
        fructose_assert_eq(xpertRequestResult.getAdjustmentTrait()->getAdjustmentTime(),  Tucuxi::Common::DateTime("2022-06-20T21:00:00", TestUtils::date_format));
    }

    /// \brief This method checks that the start and end dates of the adjustment are the start time plus
    ///        the duration of the standard treatment when there is one and a dosage history.
    ///        In this case, the dosage history starts on 19.6.2022 8h00 and the standard treatment lasts
    ///        for 4 days. Therefore, the start and end dates should be 19.6.2022 8h00 and 23.6.2022 8h00.
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsStartAndEndDatesCorrectly_whenStandardTreatmentAndDosageHistory(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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
                                                <!-- All the information regarding the drug -->
                                                <drug>
                                                    <drugId>busulfan</drugId>
                                                    <activePrinciple>something</activePrinciple>
                                                    <brandName>somebrand</brandName>
                                                    <atc>something</atc>
                                                    <!-- All the information regarding the treatment -->
                                                    <treatment>
                                                        <dosageHistory>
                                                            <dosageTimeRange>
                                                                <start>2022-06-19T08:00:00</start>
                                                                <end>2022-06-21T08:00:00</end>
                                                                <dosage>
                                                                    <dosageLoop>
                                                                        <lastingDosage>
                                                                            <interval>06:00:00</interval>
                                                                            <dose>
                                                                                <value>10</value>
                                                                                <unit>mg</unit>
                                                                                <infusionTimeInMinutes>120</infusionTimeInMinutes>
                                                                            </dose>
                                                                            <formulationAndRoute>
                                                                                <formulation>parenteralSolution</formulation>
                                                                                <administrationName>foo bar</administrationName>
                                                                                <administrationRoute>intravenousDrip</administrationRoute>
                                                                                <absorptionModel>infusion</absorptionModel>
                                                                            </formulationAndRoute>
                                                                        </lastingDosage>
                                                                    </dosageLoop>
                                                                </dosage>
                                                            </dosageTimeRange>
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
                                                <drugId>busulfan</drugId>
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
        TestUtils::setupEnv(queryString, TestUtils::originalBusulfanModelString, xpertQueryResult);

        Tucuxi::Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

        // Execute
        TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
        fructose_assert_eq(xpertRequestResult.getAdjustmentTrait()->getStart(),  Tucuxi::Common::DateTime("2022-06-19T08:00:00", TestUtils::date_format));
        fructose_assert_eq(xpertRequestResult.getAdjustmentTrait()->getEnd(),  Tucuxi::Common::DateTime("2022-06-23T08:00:00", TestUtils::date_format));
    }

    /// \brief This method checks that the start and end dates of the adjustment are the computation time plus
    ///        the standard treatment duration when there is one but no dosage history.
    ///        In this case, the computation time is on 20.6.2022 10h and the standard treatment lasts
    ///        for 4 days. Therefore, the start and end dates should be 20.6.2022 10h and 24.6.2022 10h.
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsStartAndEndDatesCorrectly_whenStandardTreatmentButNoDosageHistory(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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
                                                <!-- All the information regarding the drug -->
                                                <drug>
                                                    <drugId>busulfan</drugId>
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
                                                <drugId>busulfan</drugId>
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
        TestUtils::setupEnv(queryString, TestUtils::originalBusulfanModelString, xpertQueryResult);

        Tucuxi::Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

        // Execute
        TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
        fructose_assert_eq(xpertRequestResult.getAdjustmentTrait()->getStart(),  Tucuxi::Common::DateTime("2022-06-20T10:00:00", TestUtils::date_format));
        fructose_assert_eq(xpertRequestResult.getAdjustmentTrait()->getEnd(),  Tucuxi::Common::DateTime("2022-06-24T10:00:00", TestUtils::date_format));
    }

    /// \brief This method checks that the adjustment trait creation fails when there is a standard treatment
    ///        that ends before the adjustment time. In this test, the dosage history starts on
    ///        6.7.2018 8h and the standard treatment lasts for 4 days. Thus, the start and end dates of the adjustment
    ///        should be 6.7.2018 8h - 10.7.2018 8h. Therefore, the treatment is already completed at the time of
    ///        the adjustment time set in the request (20.06.2022 10h).
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    /// \param _testName Name of the test
    void adjustmentTraitCreator_failure_whenStandardTreatmentIsOverBeforeAdjustmentTime(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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
                                                <!-- All the information regarding the drug -->
                                                <drug>
                                                    <drugId>busulfan</drugId>
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
                                                                            <interval>06:00:00</interval>
                                                                            <dose>
                                                                                <value>10</value>
                                                                                <unit>mg</unit>
                                                                                <infusionTimeInMinutes>120</infusionTimeInMinutes>
                                                                            </dose>
                                                                            <formulationAndRoute>
                                                                                <formulation>parenteralSolution</formulation>
                                                                                <administrationName>foo bar</administrationName>
                                                                                <administrationRoute>intravenousDrip</administrationRoute>
                                                                                <absorptionModel>infusion</absorptionModel>
                                                                            </formulationAndRoute>
                                                                        </lastingDosage>
                                                                    </dosageLoop>
                                                                </dosage>
                                                            </dosageTimeRange>
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
                                                <drugId>busulfan</drugId>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <adjustmentDate>2022-06-20T10:00:00</adjustmentDate>
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

        // Prepare the XpertRequestResult
        std::unique_ptr<Tucuxi::Xpert::XpertQueryResult> xpertQueryResult;
        TestUtils::setupEnv(queryString, TestUtils::originalBusulfanModelString, xpertQueryResult);

        Tucuxi::Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

        // Execute
        TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), false);
        fructose_assert_eq(xpertRequestResult.getErrorMessage(), "Based on the standard treatment in the model:ch.tucuxi.busulfan.paci2012, considering that the oldest dosage is the treatment start, the treatment is already over at the time of the adjustment.");
    }

    /// \brief This method checks that the start and end dates are placed in relation to the adjustment time.
    ///        The start must correspond to the adjustment time.
    ///        The end must correspond to the start plus seven days.
    ///        In this test, the adjustment time is 20.6.2022 20h. Therefore, the start should be 20.6.2022 20h
    ///        and the end should be 27.6.2022 20h
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    ///        It doesn't matter if the adjustment time is manually set or not.
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsStartEndDatesOnAdjustmentTime_whenNoStandardTreatment(const std::string& _testName)
    {
        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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
                                                                <start>2022-06-19T08:00:00</start>
                                                                <end>2022-06-21T08:00:00</end>
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
        TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
        fructose_assert_eq(xpertRequestResult.getAdjustmentTrait()->getStart(),  Tucuxi::Common::DateTime("2022-06-20T20:00:00", TestUtils::date_format));
        fructose_assert_eq(xpertRequestResult.getAdjustmentTrait()->getEnd(),  Tucuxi::Common::DateTime("2022-06-27T20:00:00", TestUtils::date_format));
    }

    /// \brief This method checks that best candidates option is best dosage per interval.
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    ///
    /// \param _testName Name of the test
    void adjustmentTraitCreator_bestCandidatesOptionIsBestDosagePerInterval(const std::string& _testName)
    {
        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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

        std::unique_ptr<Tucuxi::Xpert::XpertQueryResult> result = nullptr;

        // Prepare the XpertRequestResult
        std::unique_ptr<Tucuxi::Xpert::XpertQueryResult> xpertQueryResult;
        TestUtils::setupEnv(queryString, TestUtils::originalImatinibModelString, xpertQueryResult);

        Tucuxi::Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

        // Execute
        TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
        fructose_assert_eq(xpertRequestResult.getAdjustmentTrait()->getBestCandidatesOption() == Tucuxi::Core::BestCandidatesOption::BestDosagePerInterval, true);
    }

    /// \brief This method checks that the loading dose option is retrieved from the xpertRequest
    ///        when it is defined. The request indicates that no loading dose is allowed.
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    ///
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsCorrectLoadingOption_whenDefinedInXpertRequest(const std::string& _testName)
    {
        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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
                                                <options>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                </options>
                                            </xpertRequest>
                                        </requests>
                                    </query>)";

        std::cout << _testName << std::endl;

        // Prepare the XpertRequestResult
        std::unique_ptr<Tucuxi::Xpert::XpertQueryResult> xpertQueryResult;
        TestUtils::setupEnv(queryString, TestUtils::originalImatinibModelString, xpertQueryResult);

        Tucuxi::Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

        // Execute
        TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
        fructose_assert_eq(xpertRequestResult.getAdjustmentTrait()->getLoadingOption() == Tucuxi::Core::LoadingOption::NoLoadingDose, true);
    }

    /// \brief This method checks that the loading dose option is retrieved from the drug model
    ///        when the request doesn't defines it.
    ///
    ///        At this time, the "isLoadingDoseRecommended" value is not defined in the drug model files. By default,
    ///        the drug model importer sets it to true. The result of this test should be tweaked if the imatinib
    ///        model string is updated.
    ///
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    ///
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsCorrectLoadingOption_whenNotDefinedInXpertRequest(const std::string& _testName)
    {
        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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
        std::unique_ptr<Tucuxi::Xpert::XpertQueryResult> xpertQueryResult;
        TestUtils::setupEnv(queryString, TestUtils::originalImatinibModelString, xpertQueryResult);

        Tucuxi::Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

        // Execute
        TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
        fructose_assert_eq(xpertRequestResult.getAdjustmentTrait()->getLoadingOption() == Tucuxi::Core::LoadingOption::LoadingDoseAllowed, true);
    }


    /// \brief This method checks that the rest period option is retrieved from the xpertRequest
    ///        when it is defined. The request indicates that no rest period is allowed.
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    ///
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsCorrectRestPeriodOption_whenDefinedInXpertRequest(const std::string& _testName)
    {
        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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
                                                <options>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                </options>
                                            </xpertRequest>
                                        </requests>
                                    </query>)";

        std::cout << _testName << std::endl;

        // Prepare the XpertRequestResult
        std::unique_ptr<Tucuxi::Xpert::XpertQueryResult> xpertQueryResult;
        TestUtils::setupEnv(queryString, TestUtils::originalImatinibModelString, xpertQueryResult);

        Tucuxi::Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

        // Execute
        TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
        fructose_assert_eq(xpertRequestResult.getAdjustmentTrait()->getRestPeriodOption() == Tucuxi::Core::RestPeriodOption::NoRestPeriod, true);
    }

    /// \brief This method checks that the rest period option is retrieved from the drug model
    ///        when the request doesn't defines it.
    ///
    ///        At this time, the "isRestPeriodRecommended" value is not defined in the drug model files. By default,
    ///        the drug model importer sets it to true. The result of this test should be tweaked if the imatinib
    ///        model string is updated.
    ///
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    ///
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsCorrectRestPeriodOption_whenNotDefinedInXpertRequest(const std::string& _testName)
    {
        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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
                                                <options>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <!-- There is no rest period set -->
                                                    <targetExtractionOption>populationValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>allFormulationAndRoutes</formulationAndRouteSelectionOption>
                                                </options>
                                            </xpertRequest>
                                        </requests>
                                    </query>)";

        std::cout << _testName << std::endl;

        // Prepare the XpertRequestResult
        std::unique_ptr<Tucuxi::Xpert::XpertQueryResult> xpertQueryResult;
        TestUtils::setupEnv(queryString, TestUtils::originalImatinibModelString, xpertQueryResult);

        Tucuxi::Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

        // Execute
        TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
        fructose_assert_eq(xpertRequestResult.getAdjustmentTrait()->getRestPeriodOption() == Tucuxi::Core::RestPeriodOption::RestPeriodAllowed, true);
    }

    /// \brief This method checks that the steady state target option is within treatment time range
    ///        when there is a standard treatment.
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    ///
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsSteadyStateTargetOptionToWithinTreatmentTimeRange_whenStandardTreatment(const std::string& _testName)
    {
        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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
                                                <!-- All the information regarding the drug -->
                                                <drug>
                                                    <drugId>busulfan</drugId>
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
                                                <drugId>busulfan</drugId>
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
        TestUtils::setupEnv(queryString, TestUtils::originalBusulfanModelString, xpertQueryResult);

        Tucuxi::Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

        // Execute
        TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
        fructose_assert_eq(xpertRequestResult.getAdjustmentTrait()->getSteadyStateTargetOption() == Tucuxi::Core::SteadyStateTargetOption::WithinTreatmentTimeRange, true);
    }

    /// \brief This method checks that the steady state target option is at steady state
    ///        when there is no standard treatment.
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    ///
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsSteadyStateTargetOptionToAtSteadyState_whenNoStandardTreatment(const std::string& _testName)
    {
        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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
        std::unique_ptr<Tucuxi::Xpert::XpertQueryResult> xpertQueryResult;
        TestUtils::setupEnv(queryString, TestUtils::originalImatinibModelString, xpertQueryResult);

        Tucuxi::Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

        // Execute
        TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
        fructose_assert_eq(xpertRequestResult.getAdjustmentTrait()->getSteadyStateTargetOption() == Tucuxi::Core::SteadyStateTargetOption::AtSteadyState, true);
    }

    /// \brief This method checks that the xpertRequest target extraction option is used when it
    ///        is defined. Here, the request defines it to aprioriValues.
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    ///
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsCorrectTargetExtractionOption_whenSetInXpertRequest(const std::string& _testName)
    {
        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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
                                                <options>
                                                    <targetExtractionOption>aprioriValues</targetExtractionOption>
                                                </options>
                                            </xpertRequest>
                                        </requests>
                                    </query>)";

        std::cout << _testName << std::endl;

        // Prepare the XpertRequestResult
        std::unique_ptr<Tucuxi::Xpert::XpertQueryResult> xpertQueryResult;
        TestUtils::setupEnv(queryString, TestUtils::originalImatinibModelString, xpertQueryResult);

        Tucuxi::Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

        // Execute
        TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
        fructose_assert_eq(xpertRequestResult.getAdjustmentTrait()->getTargetExtractionOption() == Tucuxi::Core::TargetExtractionOption::AprioriValues, true);
    }

    /// \brief This method checks that the target extraction option is "DefinitionIfNoIndividualTarget"
    ///        when it is not defined in the xpertRequest.
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    ///
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsCorrectTargetExtractionOption_whenNotSetInXpertRequest(const std::string& _testName)
    {
        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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
        std::unique_ptr<Tucuxi::Xpert::XpertQueryResult> xpertQueryResult;
        TestUtils::setupEnv(queryString, TestUtils::originalImatinibModelString, xpertQueryResult);

        Tucuxi::Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

        // Execute
        TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
        fructose_assert_eq(xpertRequestResult.getAdjustmentTrait()->getTargetExtractionOption() == Tucuxi::Core::TargetExtractionOption::DefinitionIfNoIndividualTarget, true);
    }

    /// \brief This method checks that the xpertRequest formulation and route selection option is used when it
    ///        is defined. Here, the request defines it to "allFormulationAndRoutes".
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    ///
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsCorrectFormulationAndRouteSelectionOption_whenSetInXpertRequest(const std::string& _testName)
    {
        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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
                                                <options>
                                                    <formulationAndRouteSelectionOption>allFormulationAndRoutes</formulationAndRouteSelectionOption>
                                                </options>
                                            </xpertRequest>
                                        </requests>
                                    </query>)";

        std::cout << _testName << std::endl;

        // Prepare the XpertRequestResult
        std::unique_ptr<Tucuxi::Xpert::XpertQueryResult> xpertQueryResult;
        TestUtils::setupEnv(queryString, TestUtils::originalImatinibModelString, xpertQueryResult);

        Tucuxi::Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

        // Execute
        TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
        fructose_assert_eq(xpertRequestResult.getAdjustmentTrait()->getFormulationAndRouteSelectionOption() == Tucuxi::Core::FormulationAndRouteSelectionOption::AllFormulationAndRoutes, true);
    }

    /// \brief This method checks that the last formulation and route selection option is
    ///        "lastFormulationAndRoute" when it is not defined in the xpertRequest.
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    ///
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsCorrectFormulationAndRouteSelectionOption_whenNotSetInXpertRequest(const std::string& _testName)
    {
        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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
        std::unique_ptr<Tucuxi::Xpert::XpertQueryResult> xpertQueryResult;
        TestUtils::setupEnv(queryString, TestUtils::originalImatinibModelString, xpertQueryResult);

        Tucuxi::Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

        // Execute
        TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
        fructose_assert_eq(xpertRequestResult.getAdjustmentTrait()->getFormulationAndRouteSelectionOption() == Tucuxi::Core::FormulationAndRouteSelectionOption::LastFormulationAndRoute, true);
    }

    /// \brief This method checks that the last intake is correctly set in the
    ///        XpertRequestResult. The test makes a request on imatinib with a treatment.
    ///        The last intake should be 19.5.2022 20h 400mg.
    /// \param _testName Name of the test
    void getLastIntake_returnsCorrectValues_whenDosageHistory(const std::string& _testName)
    {
        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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
                                                                <start>2022-05-15T07:00:00</start>
                                                                <end>2022-05-19T07:00:00</end>
                                                                <dosage>
                                                                    <lastingDosage>
                                                                        <interval>06:00:00</interval>
                                                                        <dose>
                                                                            <value>200</value>
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
                                                                </dosage>
                                                            </dosageTimeRange>
                                                            <dosageTimeRange>
                                                                <start>2022-05-19T08:00:00</start>
                                                                <end>2022-05-21T08:00:00</end>
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
        TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.getLastIntake()->getEventTime(), Tucuxi::Common::DateTime("2022-05-20T20:00:00", TestUtils::date_format));
        fructose_assert_eq(xpertRequestResult.getLastIntake()->getDose(), 400);
        fructose_assert_eq(xpertRequestResult.getLastIntake()->getUnit().toString(), "mg");
    }

    /// \brief This method checks that the last intake is nullptr in the
    ///        XpertRequestResult. The test makes a request on imatinib without a treatment.
    /// \param _testName Name of the test
    void getLastIntake_returnsNullptr_whenNoDosageHistory(const std::string& _testName)
    {
        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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
        std::unique_ptr<Tucuxi::Xpert::XpertQueryResult> xpertQueryResult;
        TestUtils::setupEnv(queryString, TestUtils::originalImatinibModelString, xpertQueryResult);

        Tucuxi::Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

        // Execute
        TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.getLastIntake().get(), nullptr);
    }
};

#endif // TEST_ADJUSTMENTTRAITCREATOR_H
