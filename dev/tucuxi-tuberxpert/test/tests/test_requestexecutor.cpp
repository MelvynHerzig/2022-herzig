#include "test_requestexecutor.h"

using namespace std;
using namespace Tucuxi;

void TestRequestExecutor::requestExecutor_failure_whenAdjustmentTraitNullptr(const string& _testName)
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
    TestUtils::flowStepProvider.getRequestExecutor()->perform(xpertRequestResult);

    // Compare
    fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), false);
    fructose_assert_eq(xpertRequestResult.getErrorMessage(), "No adjustment trait set.");
}

void TestRequestExecutor::requestExecutor_failure_whenDrugModelNullptr(const string& _testName)
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
    unique_ptr<Xpert::XpertQueryResult> xpertQueryResult;
    TestUtils::setupEnv(queryString, TestUtils::originalImatinibModelString, xpertQueryResult);

    Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

    // Execute
    TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);
    xpertRequestResult.setDrugModel(nullptr);
    TestUtils::flowStepProvider.getRequestExecutor()->perform(xpertRequestResult);

    // Compare
    fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), false);
    fructose_assert_eq(xpertRequestResult.getErrorMessage(), "No drug model set.");
}

void TestRequestExecutor::requestExecutor_failure_whenRequestExecutionFailed(const string& _testName)
{

    string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">

                                        <date>2018-07-07T13:00:00</date> <!-- Date the xml has been sent -->

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
    unique_ptr<Xpert::XpertQueryResult> xpertQueryResult;
    TestUtils::setupEnv(queryString, TestUtils::originalImatinibModelString, xpertQueryResult);

    Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

    // Execute
    // Prepare a new trait that is invalid by inverting start and end dates.
    TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);
    unique_ptr<Core::ComputingTraitAdjustment> baseTrait =
            make_unique<Core::ComputingTraitAdjustment>(*xpertRequestResult.getAdjustmentTrait());

    unique_ptr<Core::ComputingTraitAdjustment> newInvalidTrait = nullptr;
    newInvalidTrait = make_unique<Core::ComputingTraitAdjustment>(
                "",
                baseTrait->getEnd(),
                baseTrait->getStart(),
                baseTrait->getNbPointsPerHour(),
                baseTrait->getComputingOption(),
                baseTrait->getAdjustmentTime(),
                Core::BestCandidatesOption::BestDosage,
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

void TestRequestExecutor::requestExecutor_getsCorrectAdjustmentData_whenRequestExecutionSucceed(const string& _testName)
{

    string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">

                                        <date>2018-07-07T13:00:00</date> <!-- Date the xml has been sent -->

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
    unique_ptr<Xpert::XpertQueryResult> xpertQueryResult;
    TestUtils::setupEnv(queryString, TestUtils::originalImatinibModelString, xpertQueryResult);

    Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

    // Execute
    TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);
    TestUtils::flowStepProvider.getRequestExecutor()->perform(xpertRequestResult);

    // Compare
    fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
    fructose_assert_ne(xpertRequestResult.getAdjustmentData().get(), nullptr);
    fructose_assert_eq(xpertRequestResult.getAdjustmentData()->getAdjustments().empty(), false);
}

void TestRequestExecutor::requestExecutor_getsTheStatistics_whenRequestExecutionSucceed(const string& _testName)
{

    string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">

                                        <date>2018-07-07T13:00:00</date> <!-- Date the xml has been sent -->

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

    cout << _testName << endl;

    // Prepare the XpertRequestResult
    unique_ptr<Xpert::XpertQueryResult> xpertQueryResult;
    TestUtils::setupEnv(queryString, TestUtils::originalImatinibModelString, xpertQueryResult);

    Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

    // Execute
    TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);
    TestUtils::flowStepProvider.getRequestExecutor()->perform(xpertRequestResult);

    // Compare
    fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
    fructose_assert_eq(xpertRequestResult.getCycleStats().getStats().empty(), false);
    fructose_assert_eq(xpertRequestResult.getCycleStats().getStats()[0].size(), 9);
}

void TestRequestExecutor::requestExecutor_getsTypicalAprioriAposterioriParameters_whenAposterioriTrait(const string& _testName)
{

    string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">

                                        <date>2018-07-07T13:00:00</date> <!-- Date the xml has been sent -->

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

    cout << _testName << endl;

    // Prepare the XpertRequestResult
    unique_ptr<Xpert::XpertQueryResult> xpertQueryResult;
    TestUtils::setupEnv(queryString, TestUtils::originalImatinibModelString, xpertQueryResult);

    Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

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

void TestRequestExecutor::requestExecutor_getsTypicalAprioriParameters_whenAprioriTrait(const string& _testName)
{

    string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">

                                        <date>2018-07-07T13:00:00</date> <!-- Date the xml has been sent -->

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
    unique_ptr<Xpert::XpertQueryResult> xpertQueryResult;
    TestUtils::setupEnv(queryString,TestUtils::originalImatinibModelString, xpertQueryResult);

    Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

    // Execute
    TestUtils::flowStepProvider.getAdjustmentTraitCreator()->perform(xpertRequestResult);
    TestUtils::flowStepProvider.getRequestExecutor()->perform(xpertRequestResult);

    // Compare
    fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
    fructose_assert_eq(xpertRequestResult.getParameters().size(), 2);
    fructose_assert_eq(xpertRequestResult.getParameters()[0].empty(), false);
    fructose_assert_eq(xpertRequestResult.getParameters()[0].size(), xpertRequestResult.getParameters()[1].size());;
}
