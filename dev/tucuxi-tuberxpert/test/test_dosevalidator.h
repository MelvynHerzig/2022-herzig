#ifndef TEST_DOSEVALIDATOR_H
#define TEST_DOSEVALIDATOR_H

#include "fructose/fructose.h"

#include <memory>

#include "tuberxpert/result/xpertqueryresult.h"
#include "tuberxpert/result/xpertrequestresult.h"
#include "tuberxpert/utils/xpertutils.h"

#include "testutils.h"

/// \brief Tests for DoseValidator from the XpertFlowStepProvider.
/// \date 21/04/2022
/// \author Herzig Melvyn
struct TestDoseValidator : public fructose::test_base<TestDoseValidator>
{
    /// \brief Check that there is an error if the treatment of an XpertRequestResult is nullptr
    ///        in DoseValidator.
    ///        The DoseValidator must set the error in the XpertRequestResult,
    ///        shouldBeProcessed must return false and there should be 0 DoseValidationResult.
    /// \param _testName Name of the test
    void doseValidator_failure_whenTreatmentNullptr(const std::string& _testName)
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
        TestUtils::flowStepProvider.getDoseValidator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), false);
        fructose_assert_eq(xpertRequestResult.getErrorMessage(), "No treatment set.");
        fructose_assert_eq(xpertRequestResult.getDoseValidationResults().size(), 0);
    }

    /// \brief The test loads an xpertRequest without assigning a drug model to it.
    ///        The DoseValidator must set the error in the XpertRequestResult,
    ///        shouldBeProcessed must return false and there should be 0 DoseValidationResult.
    /// \param _testName Name of the test
    void doseValidator_failure_whenDrugModelNullptr(const std::string& _testName)
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
        TestUtils::flowStepProvider.getDoseValidator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), false);
        fructose_assert_eq(xpertRequestResult.getErrorMessage(), "No drug model set.");
        fructose_assert_eq(xpertRequestResult.getDoseValidationResults().size(), 0);
    }

    /// \brief The test loads a query that has no dose. After the DoseValidator flow step,
    ///        when calling getDoseValidationResults of the XpertRequestResult, the returned
    ///        map must be empty and the shouldContinueProcessing method should return true.
    /// \param _testName Name of the test
    void getDoseValidationResults_isEmpty_whenNoDoseInQuery(const std::string& _testName)
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
        std::unique_ptr<Tucuxi::Xpert::XpertQueryResult> xpertQueryResult;
        TestUtils::setupEnv(queryString, TestUtils::originalImatinibModelString, xpertQueryResult);

        Tucuxi::Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

        // Execute
        TestUtils::flowStepProvider.getDoseValidator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
        fructose_assert_eq(xpertRequestResult.getDoseValidationResults().size(), 0);

    }

    /// \brief The test loads a query that has a dose. The dose is too low compared to the
    ///        recommended doses of the imatinib model. After the DoseValidator flow step,
    ///        when calling getDoseValidationResults of the XpertRequestResult, the returned
    ///        map must not be empty and the unique DoseValidationResult it contains
    ///        should have a warning message.
    ///
    /// \param _testName Name of the test
    void doseValidationResult_hasWarning_whenUnderDosing(const std::string& _testName)
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
                                                        <dosageHistory>
                                                            <dosageTimeRange>
                                                                <start>2018-07-06T08:00:00</start>
                                                                <end>2018-07-08T08:00:00</end>
                                                                <dosage>
                                                                    <dosageLoop>
                                                                        <lastingDosage>
                                                                            <interval>12:00:00</interval>
                                                                            <dose>
                                                                                <value>1</value>
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
        TestUtils::flowStepProvider.getDoseValidator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
        fructose_assert_eq(xpertRequestResult.getDoseValidationResults().size(), 1);
        fructose_assert_eq(xpertRequestResult.getDoseValidationResults().begin()->second.getWarning(), "Minimum recommended dosage reached (100.00 mg)");
    }

    /// \brief The test loads a query that has a dose. The dose is too high compared to the
    ///        recommended doses of the imatinib model. After the DoseValidator flow step,
    ///        when calling getDoseValidationResults of the XpertRequestResult, the returned
    ///        map must not be empty and the unique DoseValidationResult it contains
    ///        should have a warning message.
    ///
    /// \param _testName Name of the test
    void doseValidationResult_hasWarning_whenOverDosing(const std::string& _testName)
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
                                                        <dosageHistory>
                                                            <dosageTimeRange>
                                                                <start>2018-07-06T08:00:00</start>
                                                                <end>2018-07-08T08:00:00</end>
                                                                <dosage>
                                                                    <dosageLoop>
                                                                        <lastingDosage>
                                                                            <interval>12:00:00</interval>
                                                                            <dose>
                                                                                <value>10000000</value>
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
        TestUtils::flowStepProvider.getDoseValidator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
        fructose_assert_eq(xpertRequestResult.getDoseValidationResults().size(), 1);
        fructose_assert_eq(xpertRequestResult.getDoseValidationResults().begin()->second.getWarning(), "Maximum recommended dosage reached (400.00 mg)");
    }

    /// \brief The test loads a query that has a dose. The dose has non-covertible unit.
    ///        After the DoseValidator flow step, when calling getDoseValidationResults of the XpertRequestResult,
    ///        the returned map must be empty, the shouldContinueProcessing method should return false
    ///        and there is an error message.
    ///
    /// \param _testName Name of the test
    void doseValidator_failure_whenBadDoseUnit(const std::string& _testName)
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
                                                                                <unit>month</unit>
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
        TestUtils::flowStepProvider.getDoseValidator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), false);
        fructose_assert_eq(xpertRequestResult.getDoseValidationResults().size(), 0);
        fructose_assert_eq(xpertRequestResult.getErrorMessage(), "Patient dosage error found, details: Error in unit conversion");
    }

    /// \brief The test loads a query that has a dose. The dose has a nasal route that is not compatible with the
    ///        imatinib model. After the DoseValidator flow step, when calling getDoseValidationResults of the XpertRequestResult,
    ///        the returned map must be empty, the shouldContinueProcessing method should return false and there is an error message.
    /// \param _testName Name of the test
    void doseValidator_failure_whenFormulationAndRouteNotSupportedByModel(const std::string& _testName)
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
                                                                                <administrationRoute>nasal</administrationRoute>
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
        TestUtils::flowStepProvider.getDoseValidator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), false);
        fructose_assert_eq(xpertRequestResult.getDoseValidationResults().size(), 0);
        fructose_assert_eq(xpertRequestResult.getErrorMessage(), "Patient dosage error found, details: No corresponding full formulation and route found for a dosage.");
    }

    /// \brief The test loads a query that has three dosage time ranges. The dosage time ranges mix all the possible
    ///        dosge types. After the DoseValidator flow step,
    ///        when calling getDoseValidationResults of the XpertRequestResult, the returned
    ///        map must have the same size than the number of doses and the DoseValidationResults it contains
    ///        should have a correct dose, a correct unit and a correct warning message.
    /// \param _testName Name of the test
    void doseValidator_returnsCorrectValues_withAllDosageTypeAndMultipleTimeRange(const std::string& _testName)
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
                                                        <dosageHistory>
                                                            <dosageTimeRange>
                                                                <start>2018-07-06T08:00:00</start>
                                                                <end>2018-07-08T08:00:00</end>
                                                                <dosage>
                                                                    <dosageLoop>
                                                                        <lastingDosage>
                                                                            <interval>12:00:00</interval>
                                                                            <dose>
                                                                                <value>3000</value>
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
                                                            <dosageTimeRange>
                                                                <start>2018-07-09T08:00:00</start>
                                                                <end>2018-07-10T08:00:00</end>
                                                                <dosage>
                                                                    <dosageSteadyState>
                                                                        <lastDoseDate>2018-07-09T20:00:00</lastDoseDate>
                                                                        <lastingDosage>
                                                                            <interval>12:00:00</interval>
                                                                            <dose>
                                                                                <value>3</value>
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
                                                                    </dosageSteadyState>
                                                                </dosage>
                                                            </dosageTimeRange>
                                                            <dosageTimeRange>
                                                                <start>2018-07-11T08:00:00</start>
                                                                <end>2018-08-12T08:00:00</end>
                                                                <dosage>
                                                                    <dosageSequence>
                                                                        <dosageRepeat>
                                                                            <iterations>4</iterations>
                                                                            <dailyDosage>
                                                                                <time>10:00:00</time>
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
                                                                            </dailyDosage>
                                                                        </dosageRepeat>
                                                                        <dosageRepeat>
                                                                            <iterations>2</iterations>
                                                                            <weeklyDosage>
                                                                                <day>2</day>
                                                                                <time>11:00:00</time>
                                                                                <dose>
                                                                                    <value>0.39</value>
                                                                                    <unit>g</unit>
                                                                                    <infusionTimeInMinutes>60</infusionTimeInMinutes>
                                                                                </dose>
                                                                                <formulationAndRoute>
                                                                                    <formulation>parenteralSolution</formulation>
                                                                                    <administrationName>foo bar</administrationName>
                                                                                    <administrationRoute>oral</administrationRoute>
                                                                                    <absorptionModel>extravascular</absorptionModel>
                                                                                </formulationAndRoute>
                                                                            </weeklyDosage>
                                                                        </dosageRepeat>
                                                                    </dosageSequence>
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
        TestUtils::flowStepProvider.getDoseValidator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
        fructose_assert_eq(xpertRequestResult.getDoseValidationResults().size(), 4);
        fructose_assert_eq(xpertRequestResult.getErrorMessage(), "");

        // All doses are diffrent and we are going to use this to get the correct asserts.
        // This is because map are not guaranteed in the same order. This is not really elegant but it makes
        // the trick.
        bool first = false, second = false, third = false, fourth = false;
        for(auto doseIt = xpertRequestResult.getDoseValidationResults().begin(); doseIt != xpertRequestResult.getDoseValidationResults().end(); ++doseIt) {
             const Tucuxi::Core::SingleDose* mapKeyDose = doseIt->first;

             // Checking that the map key is equal to the pointer in the the DoseValidationResult.
             // Must be true for all the doses.
             fructose_assert_eq(doseIt->second.getSource(), mapKeyDose);

             // Now, check the specific elements.
             switch(int(mapKeyDose->getDose())) {
                case 3000 :
                 fructose_assert_eq(Tucuxi::Xpert::doubleToString(mapKeyDose->getDose()), "3000.00");
                 fructose_assert_eq(mapKeyDose->getDoseUnit().toString(), "mg");
                 fructose_assert_eq(doseIt->second.getWarning(), "Maximum recommended dosage reached (400.00 mg)");
                 first = true;
                 break;

                case 3 :
                 fructose_assert_eq(Tucuxi::Xpert::doubleToString(mapKeyDose->getDose()), "3.00");
                 fructose_assert_eq(mapKeyDose->getDoseUnit().toString(), "mg");
                 fructose_assert_eq(doseIt->second.getWarning(), "Minimum recommended dosage reached (100.00 mg)");
                 second = true;
                 break;

                case 400 :
                 fructose_assert_eq(Tucuxi::Xpert::doubleToString(mapKeyDose->getDose()), "400.00");
                 fructose_assert_eq(mapKeyDose->getDoseUnit().toString(), "mg");
                 fructose_assert_eq(doseIt->second.getWarning(), "");
                 third = true;
                 break;

                case 0 :
                 fructose_assert_eq(Tucuxi::Xpert::doubleToString(mapKeyDose->getDose()), "0.39");
                 fructose_assert_eq(mapKeyDose->getDoseUnit().toString(), "g");
                 fructose_assert_eq(doseIt->second.getWarning(), "");
                 fourth = true;
                 break;
             }
        }

        fructose_assert_eq(first && second && third && fourth, true);
    }
};

#endif // TEST_DOSEVALIDATOR_H
