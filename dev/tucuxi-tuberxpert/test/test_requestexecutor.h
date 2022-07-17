#ifndef TEST_REQUESTEXECUTOR_H
#define TEST_REQUESTEXECUTOR_H

#include "tuberxpert/flow/general/generalxpertflowstepprovider.h"
#include "tuberxpert/result/xpertqueryresult.h"

#include "testutils.h"

#include "fructose/fructose.h"

/// \brief Tests for RequestExecutor from the XpertFlowStepProvider.
///        The tests use the AdjustmentTraitCreator object and assume
///        that this object works as intended.
/// \date 25/06/2022
/// \author Herzig Melvyn
struct TestRequestExecutor : public fructose::test_base<TestRequestExecutor>
{

    /// \brief Check that there is an error if the adjustment trait of an XpertRequestResult is nullptr
    ///        in RequestExecutor.
    ///        The RequestExecutor must set the error in the XpertRequestResult and
    ///        shouldBeProcessed must return false
    /// \param _testName Name of the test
    void requestExecutor_failure_whenAdjustmentTraitNullptr(const std::string& _testName)
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
        TestUtils::flowStepProvider.getRequestExecutor()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), false);
        fructose_assert_eq(xpertRequestResult.getErrorMessage(), "No adjustment trait set.");
    }

    /// \brief The test loads an xpertRequest without assigning a drug model to it.
    ///        The RequestExecutor must set the error in the XpertRequestResult,
    ///        shouldBeProcessed must return false.
    /// \param _testName Name of the test
    void requestExecutor_failure_whenDrugModelNullptr(const std::string& _testName)
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
        std::unique_ptr<Tucuxi::Xpert::XpertQueryResult> xpertQueryResult;
        TestUtils::setupEnv(queryString, TestUtils::originalImatinibModelString, xpertQueryResult);

        Tucuxi::Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

        // Execute
        TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);
        xpertRequestResult.setDrugModel(nullptr);
        TestUtils::flowStepProvider.getRequestExecutor()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), false);
        fructose_assert_eq(xpertRequestResult.getErrorMessage(), "No drug model set.");
    }

    /// \brief This method modifies the trait of the XpertRequestResult to fail.
    ///        The returned adjustmentData must be nullptr, the XpertRequestResult receives an error message
    ///        and shouldContinuProcessing returns false.
    /// \param _testName Name of the test
    void requestExecutor_failure_whenRequestExecutionFailed(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>imatinib</queryId>
                                        <clientId>124568</clientId>
                                        <date>2018-07-07T13:00:00</date> <!-- Date the xml has been sent -->
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
        std::unique_ptr<Tucuxi::Xpert::XpertQueryResult> xpertQueryResult;
        TestUtils::setupEnv(queryString, TestUtils::originalImatinibModelString, xpertQueryResult);

        Tucuxi::Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

        // Execute
        // Prepare a new trait that is invalid by inverting start and end dates.
        TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);
        std::unique_ptr<Tucuxi::Core::ComputingTraitAdjustment> baseTrait =
                std::make_unique<Tucuxi::Core::ComputingTraitAdjustment>(*xpertRequestResult.getAdjustmentTrait());

        std::unique_ptr<Tucuxi::Core::ComputingTraitAdjustment> newInvalidTrait = nullptr;
        newInvalidTrait = std::make_unique<Tucuxi::Core::ComputingTraitAdjustment>(
                    "",
                    baseTrait->getEnd(),
                    baseTrait->getStart(),
                    baseTrait->getNbPointsPerHour(),
                    baseTrait->getComputingOption(),
                    baseTrait->getAdjustmentTime(),
                    Tucuxi::Core::BestCandidatesOption::BestDosage,
                    baseTrait->getLoadingOption(),
                    baseTrait->getRestPeriodOption(),
                    baseTrait->getSteadyStateTargetOption(),
                    baseTrait->getTargetExtractionOption(),
                    baseTrait->getFormulationAndRouteSelectionOption()
                    );

        xpertRequestResult.setAdjustmentTrait(*newInvalidTrait);

        TestUtils::flowStepProvider.getRequestExecutor()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), false);
        fructose_assert_eq(xpertRequestResult.getErrorMessage(), "Adjustment request execution failed.");
        fructose_assert_eq(xpertRequestResult.getAdjustmentData().get(), nullptr);
    }

    /// \brief This method checks that when a valid adjustment request is made,
    ///        the adjustmentData is not nullptr, houldContinuProcessing returns true and
    ///        their is an adjustment in the adjustmentData.
    ///
    /// \param _testName Name of the test
    void requestExecutor_getsCorrectAdjustmentData_whenRequestExecutionSucceed(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>imatinib</queryId>
                                        <clientId>124568</clientId>
                                        <date>2018-07-07T13:00:00</date> <!-- Date the xml has been sent -->
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
        std::unique_ptr<Tucuxi::Xpert::XpertQueryResult> xpertQueryResult;
        TestUtils::setupEnv(queryString, TestUtils::originalImatinibModelString, xpertQueryResult);

        Tucuxi::Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

        // Execute
        TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);
        TestUtils::flowStepProvider.getRequestExecutor()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
        fructose_assert_ne(xpertRequestResult.getAdjustmentData().get(), nullptr);
        fructose_assert_eq(xpertRequestResult.getAdjustmentData()->getAdjustments().empty(), false);
    }

    /// \brief This method checks that when a valid adjustment request is performed,
    ///        there are 9 statistics at steady state.
    ///        The method shouldContinueProcessing must return true.
    /// \param _testName Name of the test
    void requestExecutor_getsTheStatistics_whenRequestExecutionSucceed(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>imatinib</queryId>
                                        <clientId>124568</clientId>
                                        <date>2018-07-07T13:00:00</date> <!-- Date the xml has been sent -->
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
        TestUtils::setupEnv(queryString, TestUtils::originalImatinibModelString, xpertQueryResult);

        Tucuxi::Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

        // Execute
        TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);
        TestUtils::flowStepProvider.getRequestExecutor()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
        fructose_assert_eq(xpertRequestResult.getCycleStats().getStats().empty(), false);
        fructose_assert_eq(xpertRequestResult.getCycleStats().getStats()[0].size(), 9);
    }

    /// \brief This method checks that the typical, apriori and aposteriori parameters are
    ///        defined when the base trait of the XpertRequestResult is aposteriori. The parameters
    ///        groups are not empty and they have the same size.
    ///        The method shouldContinueProcessing must return true.
    /// \param _testName Name of the test
    void requestExecutor_getsTypicalAprioriAposterioriParameters_whenAposterioriTrait(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>imatinib</queryId>
                                        <clientId>124568</clientId>
                                        <date>2018-07-07T13:00:00</date> <!-- Date the xml has been sent -->
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
        TestUtils::flowStepProvider.getRequestExecutor()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
        fructose_assert_eq(xpertRequestResult.getParameters().size(), 3);
        fructose_assert_eq(xpertRequestResult.getParameters()[0].empty(), false);
        fructose_assert_eq(xpertRequestResult.getParameters()[0].size(), xpertRequestResult.getParameters()[1].size());
        fructose_assert_eq(xpertRequestResult.getParameters()[1].size(), xpertRequestResult.getParameters()[2].size());
    }

    /// \brief This method checks that the typical and apriori parameters are
    ///        defined when the base trait of the XpertRequestResult is apriori. The parameters
    ///        groups are not empty and they have the same size.
    ///        The method shouldContinueProcessing must return true.
    /// \param _testName Name of the test
    void requestExecutor_getsTypicalAprioriParameters_whenAprioriTrait(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>imatinib</queryId>
                                        <clientId>124568</clientId>
                                        <date>2018-07-07T13:00:00</date> <!-- Date the xml has been sent -->
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
        std::unique_ptr<Tucuxi::Xpert::XpertQueryResult> xpertQueryResult;
        TestUtils::setupEnv(queryString,TestUtils::originalImatinibModelString, xpertQueryResult);

        Tucuxi::Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

        // Execute
        TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);
        TestUtils::flowStepProvider.getRequestExecutor()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
        fructose_assert_eq(xpertRequestResult.getParameters().size(), 2);
        fructose_assert_eq(xpertRequestResult.getParameters()[0].empty(), false);
        fructose_assert_eq(xpertRequestResult.getParameters()[0].size(), xpertRequestResult.getParameters()[1].size());;
    }
};

#endif // TEST_REQUESTEXECUTOR_H
