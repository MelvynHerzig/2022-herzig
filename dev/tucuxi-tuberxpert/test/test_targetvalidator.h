#ifndef TEST_TARGETVALIDATOR_H
#define TEST_TARGETVALIDATOR_H

#include <memory>

#include "tuberxpert/result/xpertqueryresult.h"

#include "testutils.h"

#include "fructose/fructose.h"

/// \brief Tests for TargetValidator from the XpertFlowStepProvider.
/// \date 09/06/2022
/// \author Herzig Melvyn
struct TestTargetValidator : public fructose::test_base<TestTargetValidator>
{

    /// \brief Check that there is an error if the treatment of an XpertRequestResult is nullptr
    ///        in TargetValidator.
    ///        The TargetValidator must set the error in the XpertRequestResult and
    ///        shouldBeProcessed must return false
    /// \param _testName Name of the test
    void targetValidator_failure_whenTreatmentNullptr(const std::string& _testName)
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
        TestUtils::flowStepProvider.getTargetValidator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), false);
        fructose_assert_eq(xpertRequestResult.getErrorMessage(), "No treatment set.");
    }

    /// \brief The test loads an xpertRequest without assigning a drug model to it.
    ///        The TargetValidator must set the error in the XpertRequestResult and
    ///        shouldBeProcessed must return false.
    /// \param _testName Name of the test
    void targetValidator_failure_whenDrugModelNullptr(const std::string& _testName)
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
        TestUtils::flowStepProvider.getTargetValidator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), false);
        fructose_assert_eq(xpertRequestResult.getErrorMessage(), "No drug model set.");
    }

    /// \brief This method performs a target validation with a treatment that does not specify custom targets.
    ///        XpertRequestResult::shouldBeProcessed must return true.
    /// \param _testName Name of the test
    void targetValidator_success_whenNoTargetInTreatment(const std::string& _testName)
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
        TestUtils::flowStepProvider.getTargetValidator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
        fructose_assert_eq(xpertRequestResult.getErrorMessage(), "");
    }

    /// \brief This method performs a target validation with a treatment that specifies custom targets
    ///        that have the same active moiety and the sametarget type.
    ///        The XpertRequestResult must have an error and shouldContinueProcessing must return false.
    /// \param _testName Name of the test
    void targetValidator_failure_whenTargetWithSameActiveMoietyAndTypeInTreatment(const std::string& _testName)
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
                                                        <target>
                                                            <activeMoietyId>imatinib</activeMoietyId>
                                                            <targetType>residual</targetType>
                                                            <unit>mg/l</unit>
                                                            <min>20</min>
                                                            <best>25</best>
                                                            <max>30</max>
                                                            <inefficacyAlarm>15</inefficacyAlarm>
                                                            <toxicityAlarm>50</toxicityAlarm>
                                                        </target>
                                                        <target>
                                                            <activeMoietyId>imatinib</activeMoietyId>
                                                            <targetType>residual</targetType>
                                                            <unit>mg/l</unit>
                                                            <min>30</min>
                                                            <best>35</best>
                                                            <max>40</max>
                                                            <inefficacyAlarm>25</inefficacyAlarm>
                                                            <toxicityAlarm>60</toxicityAlarm>
                                                        </target>
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
        TestUtils::flowStepProvider.getTargetValidator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), false);
        fructose_assert_eq(xpertRequestResult.getErrorMessage(), "Two patient's targets with the same active moiety and the same target type detected.");
    }

    /// \brief This method performs a target validation with a treatment that specifes custom targets
    ///        that have the same active moiety but a different target type.
    ///        The XpertRequestResult must not contain any errors and shouldContinueProcessing must return true.
    /// \param _testName Name of the test
    void targetValidator_success_whenTargetWithSameActiveMoietyButDifferentTypeInTreatment(const std::string& _testName)
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
                                                        <target>
                                                            <activeMoietyId>imatinib</activeMoietyId>
                                                            <targetType>mean</targetType>
                                                            <unit>mg/l</unit>
                                                            <min>20</min>
                                                            <best>25</best>
                                                            <max>30</max>
                                                            <inefficacyAlarm>15</inefficacyAlarm>
                                                            <toxicityAlarm>50</toxicityAlarm>
                                                        </target>
                                                        <target>
                                                            <activeMoietyId>imatinib</activeMoietyId>
                                                            <targetType>residual</targetType>
                                                            <unit>mg/l</unit>
                                                            <min>30</min>
                                                            <best>35</best>
                                                            <max>40</max>
                                                            <inefficacyAlarm>25</inefficacyAlarm>
                                                            <toxicityAlarm>60</toxicityAlarm>
                                                        </target>
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
        TestUtils::flowStepProvider.getTargetValidator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
        fructose_assert_eq(xpertRequestResult.getErrorMessage(), "");
    }

    /// \brief This method performs a target validation with a treatment that specifes a custom target
    ///        that has a different active moiety than the imatinib drug model.
    ///        The XpertRequestResult must have an error and shouldContinueProcessing must return false.
    /// \param _testName Name of the test
    void targetValidator_failure_whenTreatmentTargetWithActiveMoietyNotInDrugModel(const std::string& _testName)
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
                                                        <target>
                                                            <activeMoietyId>randomactivemoiety</activeMoietyId>
                                                            <targetType>mean</targetType>
                                                            <unit>mg/l</unit>
                                                            <min>20</min>
                                                            <best>25</best>
                                                            <max>30</max>
                                                            <inefficacyAlarm>15</inefficacyAlarm>
                                                            <toxicityAlarm>50</toxicityAlarm>
                                                        </target>
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
        TestUtils::flowStepProvider.getTargetValidator()->perform(xpertRequestResult);

        // Compare
        fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), false);
        fructose_assert_eq(xpertRequestResult.getErrorMessage(), "A target is related to an active moiety that does not belong to the drug model: ch.tucuxi.imatinib.gotta2012");
    }
};

#endif // TEST_TARGETVALIDATOR_H
