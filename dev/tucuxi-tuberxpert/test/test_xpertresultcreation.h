#ifndef TEST_XPERTRESULTCREATION_H
#define TEST_XPERTRESULTCREATION_H

#include "fructose/fructose.h"

#include "tuberxpert/language/languagemanager.h"
#include "tuberxpert/query/xpertquerydata.h"
#include "tuberxpert/query/xpertqueryimport.h"
#include "tuberxpert/result/xpertresult.h"

/// \brief Tests for TestXperResultCreation.
///        This struct tests the constructor of XpertResult with various queries and verifies
///        that the expected values can correctly be retrieved.
///        Basically this tests the step before selecting drug model.
/// \date 25/05/2022
/// \author Herzig Melvyn
struct TestXpertResultCreation : public fructose::test_base<TestXpertResultCreation>
{

    /// \brief Checks that the ownership of the query is taken by XpertResult
    /// \param _testName Name of the test
    void queryPtrInvalidation(const std::string& _testName)
    {

        std::string xmlString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">
                                        <queryId>rifampicin_1</queryId>
                                        <clientId>124568</clientId>
                                        <date>2018-07-11T13:45:30</date>
                                        <language>en</language>

                                        <drugTreatment>
                                            <!-- All the information regarding the patient -->
                                            <patient>
                                                <covariates>
                                                    <covariate>
                                                        <covariateId>ffm</covariateId>
                                                        <date>2018-07-11T10:45:30</date>
                                                        <value>40</value>
                                                        <unit>g</unit>
                                                        <dataType>double</dataType>
                                                        <nature>continuous</nature>
                                                    </covariate>
                                                </covariates>
                                            </patient>
                                            <!-- List of the drugs informations we have concerning the patient -->
                                            <drugs>
                                                <!-- All the information regarding the drug -->
                                                <drug>
                                                    <drugId>rifampicin</drugId>
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
                                                                                <value>800</value>
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
                                                            <sampleId>1</sampleId>
                                                            <sampleDate>2018-07-07T03:00:00</sampleDate>
                                                            <concentrations>
                                                                <concentration>
                                                                    <analyteId>rifampicin</analyteId>
                                                                    <value>7</value>
                                                                    <unit>mg/l</unit>
                                                                </concentration>
                                                            </concentrations>
                                                        </sample>
                                                    </samples>
                                                    <!-- Personalised targets -->
                                                    <targets>
                                                        <!-- It's the same node we can find in the drug model xml file -->
                                                        <target>
                                                            <activeMoietyId>rifampicin</activeMoietyId>
                                                            <targetType>residual</targetType>
                                                            <unit>mg/l</unit>
                                                            <min>20</min>
                                                            <best>25</best>
                                                            <max>30</max>
                                                            <inefficacyAlarm>15</inefficacyAlarm>
                                                            <toxicityAlarm>50</toxicityAlarm>
                                                        </target>
                                                    </targets>
                                                </drug>
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
                                                                <start>2018-08-06T08:00:00</start>
                                                                <end>2018-08-08T08:00:00</end>
                                                                <dosage>
                                                                    <dosageLoop>
                                                                        <lastingDosage>
                                                                            <interval>12:00:00</interval>
                                                                            <dose>
                                                                                <value>600</value>
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
                                                            <sampleId>2</sampleId>
                                                            <sampleDate>2018-08-07T03:00:00</sampleDate>
                                                            <concentrations>
                                                                <concentration>
                                                                    <analyteId>imatinb</analyteId>
                                                                    <value>5</value>
                                                                    <unit>mg/l</unit>
                                                                </concentration>
                                                            </concentrations>
                                                        </sample>
                                                        <sample>
                                                            <sampleId>3</sampleId>
                                                            <sampleDate>2018-07-07T07:00:00</sampleDate>
                                                            <concentrations>
                                                                <concentration>
                                                                    <analyteId>imatinib</analyteId>
                                                                    <value>0.6</value>
                                                                    <unit>mg/l</unit>
                                                                </concentration>
                                                            </concentrations>
                                                        </sample>
                                                    </samples>
                                                    <!-- Personalised targets -->
                                                    <targets>
                                                        <!-- It's the same node we can find in the drug model xml file -->
                                                        <target>
                                                            <activeMoietyId>imatinib</activeMoietyId>
                                                            <targetType>residual</targetType>
                                                            <unit>mg/l</unit>
                                                            <min>10</min>
                                                            <best>15</best>
                                                            <max>20</max>
                                                            <inefficacyAlarm>5</inefficacyAlarm>
                                                            <toxicityAlarm>40</toxicityAlarm>
                                                        </target>
                                                    </targets>
                                                </drug>
                                            </drugs>
                                        </drugTreatment>
                                        <!-- List of the requests we want the server to take care of -->
                                        <requests>
                                            <requestXpert>
                                                <drugId>rifampicin</drugId>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <adjustmentDate>2018-07-06T08:00:00</adjustmentDate>
                                                <options>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>populationValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>lastFormulationAndRoute</formulationAndRouteSelectionOption>
                                                </options>
                                            </requestXpert>
                                            <requestXpert>
                                                <drugId>imatinib</drugId>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <adjustmentDate>2018-08-06T08:00:00</adjustmentDate>
                                                <options>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>populationValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>lastFormulationAndRoute</formulationAndRouteSelectionOption>
                                                </options>
                                            </requestXpert>
                                        </requests>
                                    </query>
                                    )";

        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::XpertQuery::XpertQueryData> query = nullptr;

        Tucuxi::XpertQuery::XpertQueryImport importer;
        Tucuxi::XpertQuery::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        Tucuxi::XpertResult::XpertResult xpertResult(std::move(query));

        fructose_assert_eq(importResult, Tucuxi::XpertQuery::XpertQueryImport::Status::Ok);
        fructose_assert_eq(query.get(), nullptr);
    }

    /// \brief Checks that the administrative data are correctly retrieved.
    /// \param _testName Name of the test
    void retrieveAdministrativeData(const std::string& _testName)
    {

        std::string emptyAdminString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                            <query version="1.0"
                                                xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                                xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">
                                                <queryId>rifampicin_1</queryId>
                                                <clientId>124568</clientId>
                                                <date>2018-07-11T13:45:30</date>
                                                <language>en</language>

                                                <drugTreatment>
                                                    <!-- All the information regarding the patient -->
                                                    <patient>
                                                        <covariates>
                                                            <covariate>
                                                                <covariateId>ffm</covariateId>
                                                                <date>2018-07-11T10:45:30</date>
                                                                <value>40</value>
                                                                <unit>g</unit>
                                                                <dataType>double</dataType>
                                                                <nature>continuous</nature>
                                                            </covariate>
                                                        </covariates>
                                                    </patient>
                                                    <!-- List of the drugs informations we have concerning the patient -->
                                                    <drugs>
                                                        <!-- All the information regarding the drug -->
                                                        <drug>
                                                            <drugId>rifampicin</drugId>
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
                                                                                        <value>800</value>
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
                                                                    <sampleId>1</sampleId>
                                                                    <sampleDate>2018-07-07T03:00:00</sampleDate>
                                                                    <concentrations>
                                                                        <concentration>
                                                                            <analyteId>rifampicin</analyteId>
                                                                            <value>7</value>
                                                                            <unit>mg/l</unit>
                                                                        </concentration>
                                                                    </concentrations>
                                                                </sample>
                                                            </samples>
                                                            <!-- Personalised targets -->
                                                            <targets>
                                                                <!-- It's the same node we can find in the drug model xml file -->
                                                                <target>
                                                                    <activeMoietyId>rifampicin</activeMoietyId>
                                                                    <targetType>residual</targetType>
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
                                                    <requestXpert>
                                                        <drugId>rifampicin</drugId>
                                                        <output>
                                                            <format>xml</format>
                                                            <language>en</language>
                                                        </output>
                                                        <adjustmentDate>2018-07-06T08:00:00</adjustmentDate>
                                                        <options>
                                                            <loadingOption>noLoadingDose</loadingOption>
                                                            <restPeriodOption>noRestPeriod</restPeriodOption>
                                                            <targetExtractionOption>populationValues</targetExtractionOption>
                                                            <formulationAndRouteSelectionOption>lastFormulationAndRoute</formulationAndRouteSelectionOption>
                                                        </options>
                                                    </requestXpert>
                                                </requests>
                                            </query>
                                            )";

        std::string completeAdminString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                            <query version="1.0"
                                                xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                                xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">
                                                <queryId>rifampicin_1</queryId>
                                                <clientId>124568</clientId>
                                                <date>2018-07-11T13:45:30</date>
                                                <language>en</language>

                                                <admin>
                                                    <mandator>
                                                        <person>
                                                            <id>asdf</id>
                                                            <title>Dr.</title>
                                                            <firstName>John</firstName>
                                                            <lastName>Doe</lastName>
                                                            <address>
                                                                <street>Av. de l'Ours 2</street>
                                                                <postCode>1010</postCode>
                                                                <city>Lausanne</city>
                                                                <state>Vaud</state>
                                                                <country>Suisse</country>
                                                            </address>
                                                            <phone>
                                                                <number>0213140002</number>
                                                                <type>professional</type>
                                                            </phone>
                                                            <email>
                                                                <address>john.doe@chuv.com</address>
                                                                <type>professional</type>
                                                            </email>
                                                        </person>
                                                        <institute>
                                                            <id>456789</id>
                                                            <name>CHUV</name>
                                                            <address>
                                                                <street>Av. de l'Ours 1</street>
                                                                <postCode>1010</postCode>
                                                                <city>Lausanne</city>
                                                                <state>Vaud</state>
                                                                <country>Suisse</country>
                                                            </address>
                                                            <phone>
                                                                <number>0213140001</number>
                                                                <type>professional</type>
                                                            </phone>
                                                            <email>
                                                                <address>info@chuv.com</address>
                                                                <type>professional</type>
                                                            </email>
                                                        </institute>
                                                    </mandator>
                                                    <patient>
                                                        <person>
                                                            <id>123456</id>
                                                            <firstName>Alice</firstName>
                                                            <lastName>Aupaysdesmerveilles</lastName>
                                                            <address>
                                                                <street>Av. d'Ouchy 27</street>
                                                                <postCode>1006</postCode>
                                                                <city>Lausanne</city>
                                                                <state>Vaud</state>
                                                                <country>Suisse</country>
                                                            </address>
                                                            <phone>
                                                                <number>0216170002</number>
                                                                <type>professional</type>
                                                            </phone>
                                                            <email>
                                                                <address>alice.apdm@gmail.com</address>
                                                                <type>private</type>
                                                            </email>
                                                        </person>
                                                        <institute>
                                                            <id>1234</id>
                                                            <name>EHNV</name>
                                                            <address>
                                                                <street>Street name 2</street>
                                                                <postCode>1400</postCode>
                                                                <city>Yverdon-les-Bains</city>
                                                                <state>Vaud</state>
                                                                <country>Suisse</country>
                                                            </address>
                                                            <phone>
                                                                <number>0123456789</number>
                                                                <type>professional</type>
                                                            </phone>
                                                            <email>
                                                                <address>info@ehnv.com</address>
                                                                <type>professional</type>
                                                            </email>
                                                        </institute>
                                                    </patient>
                                                    <clinicalData>
                                                        <clinicalDataEntry key="goodNote"> nice </clinicalDataEntry>
                                                        <clinicalDataEntry key="badNote"> <yet>random note</yet> </clinicalDataEntry>
                                                    </clinicalData>
                                                </admin>

                                                <drugTreatment>
                                                    <!-- All the information regarding the patient -->
                                                    <patient>
                                                        <covariates>
                                                            <covariate>
                                                                <covariateId>ffm</covariateId>
                                                                <date>2018-07-11T10:45:30</date>
                                                                <value>40</value>
                                                                <unit>g</unit>
                                                                <dataType>double</dataType>
                                                                <nature>continuous</nature>
                                                            </covariate>
                                                        </covariates>
                                                    </patient>
                                                    <!-- List of the drugs informations we have concerning the patient -->
                                                    <drugs>
                                                        <!-- All the information regarding the drug -->
                                                        <drug>
                                                            <drugId>rifampicin</drugId>
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
                                                                                        <value>800</value>
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
                                                                    <sampleId>1</sampleId>
                                                                    <sampleDate>2018-07-07T03:00:00</sampleDate>
                                                                    <concentrations>
                                                                        <concentration>
                                                                            <analyteId>rifampicin</analyteId>
                                                                            <value>7</value>
                                                                            <unit>mg/l</unit>
                                                                        </concentration>
                                                                    </concentrations>
                                                                </sample>
                                                            </samples>
                                                            <!-- Personalised targets -->
                                                            <targets>
                                                                <!-- It's the same node we can find in the drug model xml file -->
                                                                <target>
                                                                    <activeMoietyId>rifampicin</activeMoietyId>
                                                                    <targetType>residual</targetType>
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
                                                    <requestXpert>
                                                        <drugId>rifampicin</drugId>
                                                        <output>
                                                            <format>xml</format>
                                                            <language>en</language>
                                                        </output>
                                                        <adjustmentDate>2018-07-06T08:00:00</adjustmentDate>
                                                        <options>
                                                            <loadingOption>noLoadingDose</loadingOption>
                                                            <restPeriodOption>noRestPeriod</restPeriodOption>
                                                            <targetExtractionOption>populationValues</targetExtractionOption>
                                                            <formulationAndRouteSelectionOption>lastFormulationAndRoute</formulationAndRouteSelectionOption>
                                                        </options>
                                                    </requestXpert>
                                                </requests>
                                            </query>
                                            )";

        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::XpertQuery::XpertQueryData> queryEmptyAdmin = nullptr;
        std::unique_ptr<Tucuxi::XpertQuery::XpertQueryData> queryCompleteAdmin = nullptr;

        Tucuxi::XpertQuery::XpertQueryImport importer;
        Tucuxi::XpertQuery::XpertQueryImport::Status importResultEmptyAdmin = importer.importFromString(queryEmptyAdmin, emptyAdminString);
        Tucuxi::XpertQuery::XpertQueryImport::Status importResultCompleteAdmin = importer.importFromString(queryCompleteAdmin, completeAdminString);

        Tucuxi::XpertResult::XpertResult xpertResultEmptyAdmin(std::move(queryEmptyAdmin));
        Tucuxi::XpertResult::XpertResult xpertResultCompleteAdmin(std::move(queryCompleteAdmin));

        fructose_assert_eq(importResultEmptyAdmin, Tucuxi::XpertQuery::XpertQueryImport::Status::Ok);
        fructose_assert_eq(importResultCompleteAdmin, Tucuxi::XpertQuery::XpertQueryImport::Status::Ok);
        fructose_assert_eq(queryEmptyAdmin.get(), nullptr);
        fructose_assert_eq(queryCompleteAdmin.get(), nullptr);

        fructose_assert_eq(xpertResultEmptyAdmin.getAdministrative().get(), nullptr);
        fructose_assert_ne(xpertResultCompleteAdmin.getAdministrative().get(), nullptr);

        const Tucuxi::XpertQuery::PersonalContact& mandator = xpertResultCompleteAdmin.getAdministrative()->getpMandator()->getpPerson();
        const Tucuxi::XpertQuery::Address& mandatorAddress = *mandator.getpAddress();
        const Tucuxi::XpertQuery::Phone& mandatorPhone = *mandator.getpPhone();
        const Tucuxi::XpertQuery::Email& mandatorEmail = *mandator.getpEmail();
        const Tucuxi::XpertQuery::InstituteContact& mandatorInstitute = *xpertResultCompleteAdmin.getAdministrative()->getpMandator()->getpInstitute();
        const Tucuxi::XpertQuery::Address& mandatorInstituteAddress = *mandatorInstitute.getpAddress();
        const Tucuxi::XpertQuery::Phone& mandatorInstitutePhone = *mandatorInstitute.getpPhone();
        const Tucuxi::XpertQuery::Email& mandatorInstituteEmail = *mandatorInstitute.getpEmail();

        fructose_assert_eq(mandator.getId(), "asdf");
        fructose_assert_eq(mandator.getTitle(), "Dr.");
        fructose_assert_eq(mandator.getFirstName(), "John");
        fructose_assert_eq(mandator.getLastName(), "Doe");
        fructose_assert_eq(mandatorAddress.getStreet(), "Av. de l'Ours 2");
        fructose_assert_eq(mandatorAddress.getPostCode(), 1010);
        fructose_assert_eq(mandatorAddress.getCity(), "Lausanne");
        fructose_assert_eq(mandatorAddress.getState(), "Vaud");
        fructose_assert_eq(mandatorAddress.getCountry(), "Suisse");
        fructose_assert_eq(mandatorPhone.getNumber(), "0213140002");
        fructose_assert_eq(mandatorPhone.getType(), "professional");
        fructose_assert_eq(mandatorEmail.getAddress(), "john.doe@chuv.com");
        fructose_assert_eq(mandatorEmail.getType(), "professional");
        fructose_assert_eq(mandatorInstitute.getId(), "456789");
        fructose_assert_eq(mandatorInstitute.getName(), "CHUV");
        fructose_assert_eq(mandatorInstituteAddress.getStreet(), "Av. de l'Ours 1");
        fructose_assert_eq(mandatorInstituteAddress.getPostCode(), 1010);
        fructose_assert_eq(mandatorInstituteAddress.getCity(), "Lausanne");
        fructose_assert_eq(mandatorInstituteAddress.getState(), "Vaud");
        fructose_assert_eq(mandatorInstituteAddress.getCountry(), "Suisse");
        fructose_assert_eq(mandatorInstitutePhone.getNumber(), "0213140001");
        fructose_assert_eq(mandatorInstitutePhone.getType(), "professional");
        fructose_assert_eq(mandatorInstituteEmail.getAddress(), "info@chuv.com");
        fructose_assert_eq(mandatorInstituteEmail.getType(), "professional");

        const Tucuxi::XpertQuery::PersonalContact& patient =  xpertResultCompleteAdmin.getAdministrative()->getpPatient()->getpPerson();
        const Tucuxi::XpertQuery::Address& patientAddress = *patient.getpAddress();
        const Tucuxi::XpertQuery::Phone& patientPhone = *patient.getpPhone();
        const Tucuxi::XpertQuery::Email& patientEmail = *patient.getpEmail();
        const Tucuxi::XpertQuery::InstituteContact& patientInstitute = *xpertResultCompleteAdmin.getAdministrative()->getpPatient()->getpInstitute();
        const Tucuxi::XpertQuery::Address& patientInstituteAddress = *patientInstitute.getpAddress();
        const Tucuxi::XpertQuery::Phone& patientInstitutePhone = *patientInstitute.getpPhone();
        const Tucuxi::XpertQuery::Email& patientInstituteEmail = *patientInstitute.getpEmail();

        fructose_assert_eq(patient.getId(), "123456");
        fructose_assert_eq(patient.getFirstName(), "Alice");
        fructose_assert_eq(patient.getLastName(), "Aupaysdesmerveilles");
        fructose_assert_eq(patientAddress.getStreet(), "Av. d'Ouchy 27");
        fructose_assert_eq(patientAddress.getPostCode(), 1006);
        fructose_assert_eq(patientAddress.getCity(), "Lausanne");
        fructose_assert_eq(patientAddress.getState(), "Vaud");
        fructose_assert_eq(patientAddress.getCountry(), "Suisse");
        fructose_assert_eq(patientPhone.getNumber(), "0216170002");
        fructose_assert_eq(patientPhone.getType(), "professional");
        fructose_assert_eq(patientEmail.getAddress(), "alice.apdm@gmail.com");
        fructose_assert_eq(patientEmail.getType(), "private");
        fructose_assert_eq(patientInstitute.getId(), "1234");
        fructose_assert_eq(patientInstitute.getName(), "EHNV");
        fructose_assert_eq(patientInstituteAddress.getStreet(), "Street name 2");
        fructose_assert_eq(patientInstituteAddress.getPostCode(), 1400);
        fructose_assert_eq(patientInstituteAddress.getCity(), "Yverdon-les-Bains");
        fructose_assert_eq(patientInstituteAddress.getState(), "Vaud");
        fructose_assert_eq(patientInstituteAddress.getCountry(), "Suisse");
        fructose_assert_eq(patientInstitutePhone.getNumber(), "0123456789");
        fructose_assert_eq(patientInstitutePhone.getType(), "professional");
        fructose_assert_eq(patientInstituteEmail.getAddress(), "info@ehnv.com");
        fructose_assert_eq(patientInstituteEmail.getType(), "professional");

        fructose_assert_eq(xpertResultCompleteAdmin.getAdministrative()->getpClinicalData()->getData().find("goodNote")->second, " nice ");
        fructose_assert_eq(xpertResultCompleteAdmin.getAdministrative()->getpClinicalData()->getData().find("badNote")->second, "");
    }

    /// \brief Checks that the vector of XpertRequestResult is valid.
    /// \param _testName Name of the test
    void retrieveXpertRequestResult(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                            <query version="1.0"
                                                xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                                xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">
                                                <queryId>rifampicin_1</queryId>
                                                <clientId>124568</clientId>
                                                <date>2018-07-11T13:45:30</date>
                                                <language>en</language>

                                                <drugTreatment>
                                                    <!-- All the information regarding the patient -->
                                                    <patient>
                                                        <covariates>
                                                            <covariate>
                                                                <covariateId>ffm</covariateId>
                                                                <date>2018-07-11T10:45:30</date>
                                                                <value>40</value>
                                                                <unit>g</unit>
                                                                <dataType>double</dataType>
                                                                <nature>continuous</nature>
                                                            </covariate>
                                                        </covariates>
                                                    </patient>
                                                    <!-- List of the drugs informations we have concerning the patient -->
                                                    <drugs>
                                                        <!-- All the information regarding the drug -->
                                                        <drug>
                                                            <drugId>rifampicin</drugId>
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
                                                                                        <value>800</value>
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
                                                                    <sampleId>1</sampleId>
                                                                    <sampleDate>2018-07-07T03:00:00</sampleDate>
                                                                    <concentrations>
                                                                        <concentration>
                                                                            <analyteId>rifampicin</analyteId>
                                                                            <value>7</value>
                                                                            <unit>mg/l</unit>
                                                                        </concentration>
                                                                    </concentrations>
                                                                </sample>
                                                            </samples>
                                                            <!-- Personalised targets -->
                                                            <targets>
                                                                <!-- It's the same node we can find in the drug model xml file -->
                                                                <target>
                                                                    <activeMoietyId>rifampicin</activeMoietyId>
                                                                    <targetType>residual</targetType>
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
                                                    <requestXpert>
                                                        <drugId>rifampicin</drugId>
                                                        <output>
                                                            <format>xml</format>
                                                            <language>en</language>
                                                        </output>
                                                        <adjustmentDate>2018-07-06T08:00:00</adjustmentDate>
                                                        <options>
                                                            <loadingOption>noLoadingDose</loadingOption>
                                                            <restPeriodOption>noRestPeriod</restPeriodOption>
                                                            <targetExtractionOption>populationValues</targetExtractionOption>
                                                            <formulationAndRouteSelectionOption>lastFormulationAndRoute</formulationAndRouteSelectionOption>
                                                        </options>
                                                    </requestXpert>
                                                    <requestXpert>
                                                        <drugId>imatinib</drugId>
                                                        <output>
                                                            <format>xml</format>
                                                            <language>en</language>
                                                        </output>
                                                        <adjustmentDate>2018-08-06T08:00:00</adjustmentDate>
                                                        <options>
                                                            <loadingOption>noLoadingDose</loadingOption>
                                                            <restPeriodOption>noRestPeriod</restPeriodOption>
                                                            <targetExtractionOption>populationValues</targetExtractionOption>
                                                            <formulationAndRouteSelectionOption>lastFormulationAndRoute</formulationAndRouteSelectionOption>
                                                        </options>
                                                    </requestXpert>
                                                </requests>
                                            </query>
                                            )";

        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::XpertQuery::XpertQueryData> query = nullptr;

        Tucuxi::XpertQuery::XpertQueryImport importer;
        Tucuxi::XpertQuery::XpertQueryImport::Status importResult = importer.importFromString(query, queryString);

        Tucuxi::XpertResult::XpertResult xpertResult(std::move(query));

        fructose_assert_eq(importResult, Tucuxi::XpertQuery::XpertQueryImport::Status::Ok);

        fructose_assert_eq(xpertResult.getXpertRequestResults().size(), 2);

        fructose_assert_eq(xpertResult.getXpertRequestResults()[0].getDrugModel(), nullptr);
        fructose_assert_eq(xpertResult.getXpertRequestResults()[0].getErrorMessage(), "");
        fructose_assert_ne(xpertResult.getXpertRequestResults()[0].getTreatment().get(), nullptr);
        fructose_assert_eq(xpertResult.getXpertRequestResults()[0].getXpertRequest().getDrugID(), "rifampicin");
        fructose_assert_eq(xpertResult.getXpertRequestResults()[0].shouldBeHandled(), true);

        fructose_assert_eq(xpertResult.getXpertRequestResults()[1].getDrugModel(), nullptr);
        fructose_assert_eq(xpertResult.getXpertRequestResults()[1].getErrorMessage(), "No drug matching. Could not extract drug treatment.");
        fructose_assert_eq(xpertResult.getXpertRequestResults()[1].getTreatment().get(), nullptr);
        fructose_assert_eq(xpertResult.getXpertRequestResults()[1].getXpertRequest().getDrugID(), "imatinib");
        fructose_assert_eq(xpertResult.getXpertRequestResults()[1].shouldBeHandled(), false);
    }

    /// \brief Checks that date of the XpertResult is between the time before its creation and
    ///        the time after its creation.
    /// \param _testName Name of the test
    void retrieveDate(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                            <query version="1.0"
                                                xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                                xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">
                                                <queryId>rifampicin_1</queryId>
                                                <clientId>124568</clientId>
                                                <date>2018-07-11T13:45:30</date>
                                                <language>en</language>

                                                <drugTreatment>
                                                    <!-- All the information regarding the patient -->
                                                    <patient>
                                                        <covariates>
                                                            <covariate>
                                                                <covariateId>ffm</covariateId>
                                                                <date>2018-07-11T10:45:30</date>
                                                                <value>40</value>
                                                                <unit>g</unit>
                                                                <dataType>double</dataType>
                                                                <nature>continuous</nature>
                                                            </covariate>
                                                        </covariates>
                                                    </patient>
                                                    <!-- List of the drugs informations we have concerning the patient -->
                                                    <drugs>
                                                        <!-- All the information regarding the drug -->
                                                        <drug>
                                                            <drugId>rifampicin</drugId>
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
                                                                                        <value>800</value>
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
                                                                    <sampleId>1</sampleId>
                                                                    <sampleDate>2018-07-07T03:00:00</sampleDate>
                                                                    <concentrations>
                                                                        <concentration>
                                                                            <analyteId>rifampicin</analyteId>
                                                                            <value>7</value>
                                                                            <unit>mg/l</unit>
                                                                        </concentration>
                                                                    </concentrations>
                                                                </sample>
                                                            </samples>
                                                            <!-- Personalised targets -->
                                                            <targets>
                                                                <!-- It's the same node we can find in the drug model xml file -->
                                                                <target>
                                                                    <activeMoietyId>rifampicin</activeMoietyId>
                                                                    <targetType>residual</targetType>
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
                                                    <requestXpert>
                                                        <drugId>rifampicin</drugId>
                                                        <output>
                                                            <format>xml</format>
                                                            <language>en</language>
                                                        </output>
                                                        <adjustmentDate>2018-07-06T08:00:00</adjustmentDate>
                                                        <options>
                                                            <loadingOption>noLoadingDose</loadingOption>
                                                            <restPeriodOption>noRestPeriod</restPeriodOption>
                                                            <targetExtractionOption>populationValues</targetExtractionOption>
                                                            <formulationAndRouteSelectionOption>lastFormulationAndRoute</formulationAndRouteSelectionOption>
                                                        </options>
                                                    </requestXpert>
                                                    <requestXpert>
                                                        <drugId>imatinib</drugId>
                                                        <output>
                                                            <format>xml</format>
                                                            <language>en</language>
                                                        </output>
                                                        <adjustmentDate>2018-08-06T08:00:00</adjustmentDate>
                                                        <options>
                                                            <loadingOption>noLoadingDose</loadingOption>
                                                            <restPeriodOption>noRestPeriod</restPeriodOption>
                                                            <targetExtractionOption>populationValues</targetExtractionOption>
                                                            <formulationAndRouteSelectionOption>lastFormulationAndRoute</formulationAndRouteSelectionOption>
                                                        </options>
                                                    </requestXpert>
                                                </requests>
                                            </query>
                                            )";

        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::XpertQuery::XpertQueryData> query = nullptr;

        Tucuxi::XpertQuery::XpertQueryImport importer;
        Tucuxi::XpertQuery::XpertQueryImport::Status importResult = importer.importFromString(query, queryString);

        Tucuxi::Common::DateTime preDateTime = Tucuxi::Common::DateTime::now();
        Tucuxi::XpertResult::XpertResult xpertResult(std::move(query));
        Tucuxi::Common::DateTime postDateTime = Tucuxi::Common::DateTime::now();

        fructose_assert_eq(importResult, Tucuxi::XpertQuery::XpertQueryImport::Status::Ok);

        fructose_assert_eq(xpertResult.getGenerationDate() >= preDateTime, true);
        fructose_assert_eq(xpertResult.getGenerationDate() <= postDateTime, true);
        fructose_assert_eq(xpertResult.getGenerationDate().isUndefined(), false);
    }

};

#endif // TEST_XPERTRESULTCREATION_H
