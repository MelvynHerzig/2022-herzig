#ifndef TEST_XPERTQUERYIMPORT_H
#define TEST_XPERTQUERYIMPORT_H

#include "../src/query/xpertqueryimport.h"
#include "../src/query/xpertrequestdata.h"

#include "fructose/fructose.h"

/// \brief Tests for XpertQueryImport
/// \date 27/04/2022
/// \author Herzig Melvyn
struct TestXpertQueryImport : public fructose::test_base<TestXpertQueryImport>
{
    /// \brief Load an xml file with every possible data in admin node
    /// and check if the recieved values are expected.
    /// \param _testName Name of the test.
    void retrieveCompleteAdmin(const std::string& _testName)
    {
        std::cout << _testName << std::endl;


        std::string xmlString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                   <query version="1.0"
                                       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                       xsi:noNamespaceSchemaLocation="custom_computing_query.xsd">
                                       <queryId>rifampicin_1</queryId>
                                       <clientId>124568</clientId>
                                       <date>2018-07-11T13:45:30</date> <!-- Date the xml has been sent -->
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
                                               <note>random note</note>
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
                                                       <unit>kg</unit>
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
                                                           <sampleId>123456</sampleId>
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
                                               <localComputation>true</localComputation>
                                               <output>
                                                   <format>xml</format>
                                                   <language>en</language>
                                               </output>
                                               <adjustmentDate>2018-07-06T08:00:00</adjustmentDate>
                                               <options>
                                                   <bestCandidatesOption>bestDosage</bestCandidatesOption>
                                                   <loadingOption>noLoadingDose</loadingOption>
                                                   <restPeriodOption>noRestPeriod</restPeriodOption>
                                                   <targetExtractionOption>populationValues</targetExtractionOption>
                                                   <formulationAndRouteSelectionOption>allFormulationAndRoutes</formulationAndRouteSelectionOption>
                                               </options>
                                           </requestXpert>
                                           <request>
                                               <requestId>population_1</requestId>
                                               <drugId>rifampicin</drugId>
                                               <drugModelId>ch.tucuxi.rifampicin.svensson2017</drugModelId>
                                               <predictionTraits>
                                                 <computingOption>
                                                   <parametersType>population</parametersType>
                                                   <compartmentOption>allActiveMoieties</compartmentOption>
                                                   <retrieveStatistics>true</retrieveStatistics>
                                                   <retrieveParameters>true</retrieveParameters>
                                                   <retrieveCovariates>true</retrieveCovariates>
                                                 </computingOption>
                                                 <nbPointsPerHour>20</nbPointsPerHour>
                                                 <dateInterval>
                                                     <start>2018-07-06T08:00:00</start>
                                                     <end>2018-07-08T08:00:00</end>
                                                 </dateInterval>
                                               </predictionTraits>
                                           </request>
                                       </requests>
                                   </query>)";


        std::unique_ptr<Tucuxi::XpertQuery::XpertQueryData> query = nullptr;

        Tucuxi::XpertQuery::XpertQueryImport importer;
        Tucuxi::XpertQuery::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        auto optAdmin = query->getpAdministrative();
        const Tucuxi::XpertQuery::AdministrativeData& pAdmin = optAdmin.value();

        fructose_assert_eq(importResult, Tucuxi::XpertQuery::XpertQueryImport::Status::Ok);

        const Tucuxi::XpertQuery::PersonalContact& mandator = pAdmin.getpMandator()->get().getpPerson();
        const Tucuxi::XpertQuery::Address& mandatorAddress = mandator.getpAddress()->get();
        const Tucuxi::XpertQuery::Phone& mandatorPhone = mandator.getpPhone()->get();
        const Tucuxi::XpertQuery::Email& mandatorEmail = mandator.getpEmail()->get();
        const Tucuxi::XpertQuery::InstituteContact& mandatorInstitute = pAdmin.getpMandator()->get().getpInstitute()->get();
        const Tucuxi::XpertQuery::Address& mandatorInstituteAddress = mandatorInstitute.getpAddress()->get();
        const Tucuxi::XpertQuery::Phone& mandatorInstitutePhone = mandatorInstitute.getpPhone()->get();
        const Tucuxi::XpertQuery::Email& mandatorInstituteEmail = mandatorInstitute.getpEmail()->get();

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

        const Tucuxi::XpertQuery::PersonalContact& patient = pAdmin.getpPatient()->get().getpPerson();
        const Tucuxi::XpertQuery::Address& patientAddress = patient.getpAddress()->get();
        const Tucuxi::XpertQuery::Phone& patientPhone = patient.getpPhone()->get();
        const Tucuxi::XpertQuery::Email& patientEmail = patient.getpEmail()->get();
        const Tucuxi::XpertQuery::InstituteContact& patientInstitute = pAdmin.getpPatient()->get().getpInstitute()->get();
        const Tucuxi::XpertQuery::Address& patientInstituteAddress = patientInstitute.getpAddress()->get();
        const Tucuxi::XpertQuery::Phone& patientInstitutePhone = patientInstitute.getpPhone()->get();
        const Tucuxi::XpertQuery::Email& patientInstituteEmail = patientInstitute.getpEmail()->get();

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

        fructose_assert_eq(pAdmin.getpClinicalData()->get().getData().find("note")->second, "random note");
    }

    /// \brief Load an xml file with no admin element
    /// \param _testName Name of the test.
    void retrieveNoAdmin(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        std::string xmlString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="custom_computing_query.xsd">
                                        <queryId>rifampicin_1</queryId>
                                        <clientId>124568</clientId>
                                        <date>2018-07-11T13:45:30</date> <!-- Date the xml has been sent -->
                                        <language>en</language>

                                        <drugTreatment>
                                            <!-- All the information regarding the patient -->
                                            <patient>
                                                <covariates>
                                                    <covariate>
                                                        <covariateId>ffm</covariateId>
                                                        <date>2018-07-11T10:45:30</date>
                                                        <value>40</value>
                                                        <unit>kg</unit>
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
                                                            <sampleId>123456</sampleId>
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
                                                <localComputation>true</localComputation>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <adjustmentDate>2018-07-06T08:00:00</adjustmentDate>
                                                <options>
                                                    <bestCandidatesOption>bestDosage</bestCandidatesOption>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>populationValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>lastFormulationAndRoute</formulationAndRouteSelectionOption>
                                                </options>
                                            </requestXpert>
                                            <request>
                                                <requestId>population_1</requestId>
                                                <drugId>rifampicin</drugId>
                                                <drugModelId>ch.tucuxi.rifampicin.svensson2017</drugModelId>
                                                <predictionTraits>
                                                  <computingOption>
                                                    <parametersType>population</parametersType>
                                                    <compartmentOption>allActiveMoieties</compartmentOption>
                                                    <retrieveStatistics>true</retrieveStatistics>
                                                    <retrieveParameters>true</retrieveParameters>
                                                    <retrieveCovariates>true</retrieveCovariates>
                                                  </computingOption>
                                                  <nbPointsPerHour>20</nbPointsPerHour>
                                                  <dateInterval>
                                                      <start>2018-07-06T08:00:00</start>
                                                      <end>2018-07-08T08:00:00</end>
                                                  </dateInterval>
                                                </predictionTraits>
                                            </request>
                                        </requests>
                                    </query>
                                    )";

        std::unique_ptr<Tucuxi::XpertQuery::XpertQueryData> query = nullptr;

        Tucuxi::XpertQuery::XpertQueryImport importer;
        Tucuxi::XpertQuery::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        auto optAdmin = query->getpAdministrative();

        fructose_assert_eq(importResult, Tucuxi::XpertQuery::XpertQueryImport::Status::Ok);
        fructose_assert_eq(optAdmin.has_value(), false);
    }

    /// \brief Load an xml file with admin but no mandator, no patient and no clinical data.
    /// \param _testName Name of the test.
    void retrieveEmptyAdmin(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        std::string xmlString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="custom_computing_query.xsd">
                                        <queryId>rifampicin_1</queryId>
                                        <clientId>124568</clientId>
                                        <date>2018-07-11T13:45:30</date> <!-- Date the xml has been sent -->
                                        <language>en</language>

                                        <admin></admin>

                                        <drugTreatment>
                                            <!-- All the information regarding the patient -->
                                            <patient>
                                                <covariates>
                                                    <covariate>
                                                        <covariateId>ffm</covariateId>
                                                        <date>2018-07-11T10:45:30</date>
                                                        <value>40</value>
                                                        <unit>kg</unit>
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
                                                            <sampleId>123456</sampleId>
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
                                                <localComputation>true</localComputation>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <adjustmentDate>2018-07-06T08:00:00</adjustmentDate>
                                                <options>
                                                    <bestCandidatesOption>bestDosage</bestCandidatesOption>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>populationValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>lastFormulationAndRoute</formulationAndRouteSelectionOption>
                                                </options>
                                            </requestXpert>
                                            <request>
                                                <requestId>population_1</requestId>
                                                <drugId>rifampicin</drugId>
                                                <drugModelId>ch.tucuxi.rifampicin.svensson2017</drugModelId>
                                                <predictionTraits>
                                                  <computingOption>
                                                    <parametersType>population</parametersType>
                                                    <compartmentOption>allActiveMoieties</compartmentOption>
                                                    <retrieveStatistics>true</retrieveStatistics>
                                                    <retrieveParameters>true</retrieveParameters>
                                                    <retrieveCovariates>true</retrieveCovariates>
                                                  </computingOption>
                                                  <nbPointsPerHour>20</nbPointsPerHour>
                                                  <dateInterval>
                                                      <start>2018-07-06T08:00:00</start>
                                                      <end>2018-07-08T08:00:00</end>
                                                  </dateInterval>
                                                </predictionTraits>
                                            </request>
                                        </requests>
                                    </query>
                                    )";

        std::unique_ptr<Tucuxi::XpertQuery::XpertQueryData> query = nullptr;

        Tucuxi::XpertQuery::XpertQueryImport importer;
        Tucuxi::XpertQuery::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        auto optAdmin = query->getpAdministrative();

        fructose_assert_eq(importResult, Tucuxi::XpertQuery::XpertQueryImport::Status::Ok);
        fructose_assert_eq(optAdmin.has_value(), true);

        const Tucuxi::XpertQuery::AdministrativeData& pAdmin = optAdmin.value();

        fructose_assert_eq(pAdmin.getpMandator().has_value(), false);
        fructose_assert_eq(pAdmin.getpPatient().has_value(), false);
        fructose_assert_eq(pAdmin.getpClinicalData().has_value(), false);
    }

    /// \brief Load an xml file with admin and minimal persons element
    /// \param _testName Name of the test.
    void retrieveMinimalPerson(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        std::string xmlString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="custom_computing_query.xsd">
                                        <queryId>rifampicin_1</queryId>
                                        <clientId>124568</clientId>
                                        <date>2018-07-11T13:45:30</date> <!-- Date the xml has been sent -->
                                        <language>en</language>

                                        <admin>
                                            <mandator>
                                                <person>
                                                    <firstName>John</firstName>
                                                    <lastName>Doe</lastName>
                                                </person>
                                            </mandator>
                                            <patient>
                                                <person>
                                                    <firstName>Alice</firstName>
                                                    <lastName>Aupaysdesmerveilles</lastName>
                                                </person>
                                            </patient>
                                        </admin>

                                        <drugTreatment>
                                            <!-- All the information regarding the patient -->
                                            <patient>
                                                <covariates>
                                                    <covariate>
                                                        <covariateId>ffm</covariateId>
                                                        <date>2018-07-11T10:45:30</date>
                                                        <value>40</value>
                                                        <unit>kg</unit>
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
                                                            <sampleId>123456</sampleId>
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
                                                <localComputation>true</localComputation>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <adjustmentDate>2018-07-06T08:00:00</adjustmentDate>
                                                <options>
                                                    <bestCandidatesOption>bestDosage</bestCandidatesOption>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>populationValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>lastFormulationAndRoute</formulationAndRouteSelectionOption>
                                                </options>
                                            </requestXpert>
                                            <request>
                                                <requestId>population_1</requestId>
                                                <drugId>rifampicin</drugId>
                                                <drugModelId>ch.tucuxi.rifampicin.svensson2017</drugModelId>
                                                <predictionTraits>
                                                  <computingOption>
                                                    <parametersType>population</parametersType>
                                                    <compartmentOption>allActiveMoieties</compartmentOption>
                                                    <retrieveStatistics>true</retrieveStatistics>
                                                    <retrieveParameters>true</retrieveParameters>
                                                    <retrieveCovariates>true</retrieveCovariates>
                                                  </computingOption>
                                                  <nbPointsPerHour>20</nbPointsPerHour>
                                                  <dateInterval>
                                                      <start>2018-07-06T08:00:00</start>
                                                      <end>2018-07-08T08:00:00</end>
                                                  </dateInterval>
                                                </predictionTraits>
                                            </request>
                                        </requests>
                                    </query>
                                    )";

        std::unique_ptr<Tucuxi::XpertQuery::XpertQueryData> query = nullptr;

        Tucuxi::XpertQuery::XpertQueryImport importer;
        Tucuxi::XpertQuery::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        auto optAdmin = query->getpAdministrative();

        fructose_assert_eq(importResult, Tucuxi::XpertQuery::XpertQueryImport::Status::Ok);
        fructose_assert_eq(optAdmin.has_value(), true);

        const Tucuxi::XpertQuery::AdministrativeData& pAdmin = optAdmin.value();
        const Tucuxi::XpertQuery::PersonalContact& mandator = pAdmin.getpMandator()->get().getpPerson();
        const Tucuxi::XpertQuery::PersonalContact& patient = pAdmin.getpPatient()->get().getpPerson();


        fructose_assert_eq(mandator.getId(), "");
        fructose_assert_eq(mandator.getTitle(), "");
        fructose_assert_eq(mandator.getpAddress().has_value(), false);
        fructose_assert_eq(mandator.getpPhone().has_value(), false);
        fructose_assert_eq(mandator.getpEmail().has_value(), false);
        fructose_assert_eq(pAdmin.getpMandator()->get().getpInstitute().has_value(), false);

        fructose_assert_eq(patient.getId(), "");
        fructose_assert_eq(patient.getTitle(), "");
        fructose_assert_eq(patient.getpAddress().has_value(), false);
        fructose_assert_eq(patient.getpPhone().has_value(), false);
        fructose_assert_eq(patient.getpEmail().has_value(), false);
        fructose_assert_eq(pAdmin.getpPatient()->get().getpInstitute().has_value(), false);
    }

    /// \brief Load an xml file with admin and minimal institutes elements
    /// \param _testName Name of the test.
    void retrieveMinimalInstitute(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        std::string xmlString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="custom_computing_query.xsd">
                                        <queryId>rifampicin_1</queryId>
                                        <clientId>124568</clientId>
                                        <date>2018-07-11T13:45:30</date> <!-- Date the xml has been sent -->
                                        <language>en</language>

                                        <admin>
                                            <mandator>
                                                <person>
                                                    <firstName>John</firstName>
                                                    <lastName>Doe</lastName>
                                                </person>
                                                <institute>
                                                    <name>CHUV</name>
                                                </institute>
                                            </mandator>
                                            <patient>
                                                <person>
                                                    <firstName>Alice</firstName>
                                                    <lastName>Aupaysdesmerveilles</lastName>
                                                </person>
                                                <institute>
                                                    <name>EHNV</name>
                                                </institute>
                                            </patient>
                                        </admin>

                                        <drugTreatment>
                                            <!-- All the information regarding the patient -->
                                            <patient>
                                                <covariates>
                                                    <covariate>
                                                        <covariateId>ffm</covariateId>
                                                        <date>2018-07-11T10:45:30</date>
                                                        <value>40</value>
                                                        <unit>kg</unit>
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
                                                            <sampleId>123456</sampleId>
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
                                                <localComputation>true</localComputation>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <adjustmentDate>2018-07-06T08:00:00</adjustmentDate>
                                                <options>
                                                    <bestCandidatesOption>bestDosage</bestCandidatesOption>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>populationValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>lastFormulationAndRoute</formulationAndRouteSelectionOption>
                                                </options>
                                            </requestXpert>
                                            <request>
                                                <requestId>population_1</requestId>
                                                <drugId>rifampicin</drugId>
                                                <drugModelId>ch.tucuxi.rifampicin.svensson2017</drugModelId>
                                                <predictionTraits>
                                                  <computingOption>
                                                    <parametersType>population</parametersType>
                                                    <compartmentOption>allActiveMoieties</compartmentOption>
                                                    <retrieveStatistics>true</retrieveStatistics>
                                                    <retrieveParameters>true</retrieveParameters>
                                                    <retrieveCovariates>true</retrieveCovariates>
                                                  </computingOption>
                                                  <nbPointsPerHour>20</nbPointsPerHour>
                                                  <dateInterval>
                                                      <start>2018-07-06T08:00:00</start>
                                                      <end>2018-07-08T08:00:00</end>
                                                  </dateInterval>
                                                </predictionTraits>
                                            </request>
                                        </requests>
                                    </query>
                                    )";

        std::unique_ptr<Tucuxi::XpertQuery::XpertQueryData> query = nullptr;

        Tucuxi::XpertQuery::XpertQueryImport importer;
        Tucuxi::XpertQuery::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        auto optAdmin = query->getpAdministrative();

        fructose_assert_eq(importResult, Tucuxi::XpertQuery::XpertQueryImport::Status::Ok);
        fructose_assert_eq(optAdmin.has_value(), true);

        const Tucuxi::XpertQuery::AdministrativeData& pAdmin = optAdmin.value();
        const Tucuxi::XpertQuery::InstituteContact& mandatorInstitute = pAdmin.getpMandator()->get().getpInstitute()->get();
        const Tucuxi::XpertQuery::InstituteContact& patientInstitute = pAdmin.getpPatient()->get().getpInstitute()->get();


        fructose_assert_eq(mandatorInstitute.getId(), "");
        fructose_assert_eq(mandatorInstitute.getpAddress().has_value(), false);
        fructose_assert_eq(mandatorInstitute.getpPhone().has_value(), false);
        fructose_assert_eq(mandatorInstitute.getpEmail().has_value(), false);

        fructose_assert_eq(patientInstitute.getId(), "");
        fructose_assert_eq(patientInstitute.getpAddress().has_value(), false);
        fructose_assert_eq(patientInstitute.getpPhone().has_value(), false);
        fructose_assert_eq(patientInstitute.getpEmail().has_value(), false);
    }

    /// \brief Load an xml file with admin and minimal coordinates elements (address, phone and email)
    /// \param _testName Name of the test.
    void retrieveMinimalCoordinates(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        std::string xmlString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="custom_computing_query.xsd">
                                        <queryId>rifampicin_1</queryId>
                                        <clientId>124568</clientId>
                                        <date>2018-07-11T13:45:30</date> <!-- Date the xml has been sent -->
                                        <language>en</language>

                                        <admin>
                                            <mandator>
                                                <person>
                                                    <firstName>John</firstName>
                                                    <lastName>Doe</lastName>
                                                    <address>
                                                        <street>Av. de l'Ours 2</street>
                                                        <postCode>1010</postCode>
                                                        <city>Lausanne</city>
                                                    </address>
                                                    <phone>
                                                        <number>0213140002</number>
                                                    </phone>
                                                    <email>
                                                        <address>john.doe@chuv.com</address>
                                                    </email>
                                                </person>
                                                <institute>
                                                    <name>CHUV</name>
                                                    <address>
                                                        <street>Av. de l'Ours 1</street>
                                                        <postCode>1010</postCode>
                                                        <city>Lausanne</city>
                                                    </address>
                                                    <phone>
                                                        <number>0213140001</number>
                                                    </phone>
                                                    <email>
                                                        <address>info@chuv.com</address>
                                                    </email>
                                                </institute>
                                            </mandator>
                                            <patient>
                                                <person>
                                                    <firstName>Alice</firstName>
                                                    <lastName>Aupaysdesmerveilles</lastName>
                                                    <address>
                                                        <street>Av. d'Ouchy 27</street>
                                                        <postCode>1006</postCode>
                                                        <city>Lausanne</city>
                                                    </address>
                                                    <phone>
                                                        <number>0216170002</number>
                                                    </phone>
                                                    <email>
                                                        <address>alice.apdm@gmail.com</address>
                                                    </email>
                                                </person>
                                                <institute>
                                                    <id>1234</id>
                                                    <name>EHNV</name>
                                                    <address>
                                                        <street>Street name 2</street>
                                                        <postCode>1400</postCode>
                                                        <city>Yverdon-les-Bains</city>
                                                    </address>
                                                    <phone>
                                                        <number>0123456789</number>
                                                    </phone>
                                                    <email>
                                                        <address>info@ehnv.com</address>
                                                    </email>
                                                </institute>
                                            </patient>
                                        </admin>

                                        <drugTreatment>
                                            <!-- All the information regarding the patient -->
                                            <patient>
                                                <covariates>
                                                    <covariate>
                                                        <covariateId>ffm</covariateId>
                                                        <date>2018-07-11T10:45:30</date>
                                                        <value>40</value>
                                                        <unit>kg</unit>
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
                                                            <sampleId>123456</sampleId>
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
                                                <localComputation>true</localComputation>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <adjustmentDate>2018-07-06T08:00:00</adjustmentDate>
                                                <options>
                                                    <bestCandidatesOption>bestDosage</bestCandidatesOption>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>populationValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>lastFormulationAndRoute</formulationAndRouteSelectionOption>
                                                </options>
                                            </requestXpert>
                                            <request>
                                                <requestId>population_1</requestId>
                                                <drugId>rifampicin</drugId>
                                                <drugModelId>ch.tucuxi.rifampicin.svensson2017</drugModelId>
                                                <predictionTraits>
                                                  <computingOption>
                                                    <parametersType>population</parametersType>
                                                    <compartmentOption>allActiveMoieties</compartmentOption>
                                                    <retrieveStatistics>true</retrieveStatistics>
                                                    <retrieveParameters>true</retrieveParameters>
                                                    <retrieveCovariates>true</retrieveCovariates>
                                                  </computingOption>
                                                  <nbPointsPerHour>20</nbPointsPerHour>
                                                  <dateInterval>
                                                      <start>2018-07-06T08:00:00</start>
                                                      <end>2018-07-08T08:00:00</end>
                                                  </dateInterval>
                                                </predictionTraits>
                                            </request>
                                        </requests>
                                    </query>
                                    )";

        std::unique_ptr<Tucuxi::XpertQuery::XpertQueryData> query = nullptr;

        Tucuxi::XpertQuery::XpertQueryImport importer;
        Tucuxi::XpertQuery::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        auto optAdmin = query->getpAdministrative();
        const Tucuxi::XpertQuery::AdministrativeData& pAdmin = optAdmin.value();

        fructose_assert_eq(importResult, Tucuxi::XpertQuery::XpertQueryImport::Status::Ok);
        fructose_assert_eq(optAdmin.has_value(), true);

        const Tucuxi::XpertQuery::PersonalContact& mandator = pAdmin.getpMandator()->get().getpPerson();
        const Tucuxi::XpertQuery::Address& mandatorAddress = mandator.getpAddress()->get();
        const Tucuxi::XpertQuery::Phone& mandatorPhone = mandator.getpPhone()->get();
        const Tucuxi::XpertQuery::Email& mandatorEmail = mandator.getpEmail()->get();
        const Tucuxi::XpertQuery::InstituteContact& mandatorInstitute = pAdmin.getpMandator()->get().getpInstitute()->get();
        const Tucuxi::XpertQuery::Address& mandatorInstituteAddress = mandatorInstitute.getpAddress()->get();
        const Tucuxi::XpertQuery::Phone& mandatorInstitutePhone = mandatorInstitute.getpPhone()->get();
        const Tucuxi::XpertQuery::Email& mandatorInstituteEmail = mandatorInstitute.getpEmail()->get();

        const Tucuxi::XpertQuery::PersonalContact& patient = pAdmin.getpPatient()->get().getpPerson();
        const Tucuxi::XpertQuery::Address& patientAddress = patient.getpAddress()->get();
        const Tucuxi::XpertQuery::Phone& patientPhone = patient.getpPhone()->get();
        const Tucuxi::XpertQuery::Email& patientEmail = patient.getpEmail()->get();
        const Tucuxi::XpertQuery::InstituteContact& patientInstitute = pAdmin.getpPatient()->get().getpInstitute()->get();
        const Tucuxi::XpertQuery::Address& patientInstituteAddress = patientInstitute.getpAddress()->get();
        const Tucuxi::XpertQuery::Phone& patientInstitutePhone = patientInstitute.getpPhone()->get();
        const Tucuxi::XpertQuery::Email& patientInstituteEmail = patientInstitute.getpEmail()->get();

        fructose_assert_eq(mandatorAddress.getState(), "");
        fructose_assert_eq(mandatorInstituteAddress.getState(), "");
        fructose_assert_eq(patientAddress.getState(), "");
        fructose_assert_eq(patientInstituteAddress.getState(), "");

        fructose_assert_eq(mandatorAddress.getCountry(), "");
        fructose_assert_eq(mandatorInstituteAddress.getCountry(), "");
        fructose_assert_eq(patientAddress.getCountry(), "");
        fructose_assert_eq(patientInstituteAddress.getCountry(), "");

        fructose_assert_eq(mandatorPhone.getType(), "");
        fructose_assert_eq(mandatorInstitutePhone.getType(), "");
        fructose_assert_eq(patientPhone.getType(), "");
        fructose_assert_eq(patientInstitutePhone.getType(), "");

        fructose_assert_eq(mandatorEmail.getType(), "");
        fructose_assert_eq(mandatorInstituteEmail.getType(), "");
        fructose_assert_eq(patientEmail.getType(), "");
        fructose_assert_eq(patientInstituteEmail.getType(), "");
    }

    /// \brief Load an xml file with admin but mandatory values are missing.
    /// \param _testName Name of the test.
    void errorWhenMissingMandatoryInMandatorPerson(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        std::string xmlString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="custom_computing_query.xsd">
                                        <queryId>rifampicin_1</queryId>
                                        <clientId>124568</clientId>
                                        <date>2018-07-11T13:45:30</date> <!-- Date the xml has been sent -->
                                        <language>en</language>

                                        <admin>
                                            <mandator>
                                                <person>
                                                    <address>
                                                    </address>
                                                    <phone>
                                                    </phone>
                                                    <email>
                                                    </email>
                                                </person>
                                            </mandator>
                                        </admin>

                                        <drugTreatment>
                                            <!-- All the information regarding the patient -->
                                            <patient>
                                                <covariates>
                                                    <covariate>
                                                        <covariateId>ffm</covariateId>
                                                        <date>2018-07-11T10:45:30</date>
                                                        <value>40</value>
                                                        <unit>kg</unit>
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
                                                            <sampleId>123456</sampleId>
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
                                                <localComputation>true</localComputation>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <adjustmentDate>2018-07-06T08:00:00</adjustmentDate>
                                                <options>
                                                    <bestCandidatesOption>bestDosage</bestCandidatesOption>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>populationValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>lastFormulationAndRoute</formulationAndRouteSelectionOption>
                                                </options>
                                            </requestXpert>
                                            <request>
                                                <requestId>population_1</requestId>
                                                <drugId>rifampicin</drugId>
                                                <drugModelId>ch.tucuxi.rifampicin.svensson2017</drugModelId>
                                                <predictionTraits>
                                                  <computingOption>
                                                    <parametersType>population</parametersType>
                                                    <compartmentOption>allActiveMoieties</compartmentOption>
                                                    <retrieveStatistics>true</retrieveStatistics>
                                                    <retrieveParameters>true</retrieveParameters>
                                                    <retrieveCovariates>true</retrieveCovariates>
                                                  </computingOption>
                                                  <nbPointsPerHour>20</nbPointsPerHour>
                                                  <dateInterval>
                                                      <start>2018-07-06T08:00:00</start>
                                                      <end>2018-07-08T08:00:00</end>
                                                  </dateInterval>
                                                </predictionTraits>
                                            </request>
                                        </requests>
                                    </query>
                                    )";

        std::unique_ptr<Tucuxi::XpertQuery::XpertQueryData> query = nullptr;

        Tucuxi::XpertQuery::XpertQueryImport importer;
        Tucuxi::XpertQuery::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        fructose_assert_eq(importResult, Tucuxi::XpertQuery::XpertQueryImport::Status::Error);
        fructose_assert_ne(importer.getErrorMessage().find("firstName"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("lastName"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("street"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("postCode"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("city"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("number"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("address"), std::string::npos);
    }

    /// \brief Load an xml file with admin but mandatory values are missing.
    /// \param _testName Name of the test.
    void errorWhenMissingMandatoryInMandatorInstitute(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        std::string xmlString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="custom_computing_query.xsd">
                                        <queryId>rifampicin_1</queryId>
                                        <clientId>124568</clientId>
                                        <date>2018-07-11T13:45:30</date> <!-- Date the xml has been sent -->
                                        <language>en</language>

                                        <admin>
                                            <mandator>
                                                <person>
                                                    <firstName>John</firstName>
                                                    <lastName>Doe</lastName>
                                                    <address>
                                                        <street>Av. de l'Ours 2</street>
                                                        <postCode>1010</postCode>
                                                        <city>Lausanne</city>
                                                    </address>
                                                    <phone>
                                                        <number>0213140002</number>
                                                    </phone>
                                                    <email>
                                                        <address>john.doe@chuv.com</address>
                                                    </email>
                                                </person>
                                                <institute>
                                                    <address>
                                                    </address>
                                                    <phone>
                                                    </phone>
                                                    <email>
                                                    </email>
                                                </institute>
                                            </mandator>
                                        </admin>

                                        <drugTreatment>
                                            <!-- All the information regarding the patient -->
                                            <patient>
                                                <covariates>
                                                    <covariate>
                                                        <covariateId>ffm</covariateId>
                                                        <date>2018-07-11T10:45:30</date>
                                                        <value>40</value>
                                                        <unit>kg</unit>
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
                                                            <sampleId>123456</sampleId>
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
                                                <localComputation>true</localComputation>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <adjustmentDate>2018-07-06T08:00:00</adjustmentDate>
                                                <options>
                                                    <bestCandidatesOption>bestDosage</bestCandidatesOption>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>populationValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>lastFormulationAndRoute</formulationAndRouteSelectionOption>
                                                </options>
                                            </requestXpert>
                                            <request>
                                                <requestId>population_1</requestId>
                                                <drugId>rifampicin</drugId>
                                                <drugModelId>ch.tucuxi.rifampicin.svensson2017</drugModelId>
                                                <predictionTraits>
                                                  <computingOption>
                                                    <parametersType>population</parametersType>
                                                    <compartmentOption>allActiveMoieties</compartmentOption>
                                                    <retrieveStatistics>true</retrieveStatistics>
                                                    <retrieveParameters>true</retrieveParameters>
                                                    <retrieveCovariates>true</retrieveCovariates>
                                                  </computingOption>
                                                  <nbPointsPerHour>20</nbPointsPerHour>
                                                  <dateInterval>
                                                      <start>2018-07-06T08:00:00</start>
                                                      <end>2018-07-08T08:00:00</end>
                                                  </dateInterval>
                                                </predictionTraits>
                                            </request>
                                        </requests>
                                    </query>
                                    )";

        std::unique_ptr<Tucuxi::XpertQuery::XpertQueryData> query = nullptr;

        Tucuxi::XpertQuery::XpertQueryImport importer;
        Tucuxi::XpertQuery::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        fructose_assert_eq(importResult, Tucuxi::XpertQuery::XpertQueryImport::Status::Error);
        fructose_assert_ne(importer.getErrorMessage().find("name"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("street"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("postCode"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("city"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("number"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("address"), std::string::npos);
    }

    /// \brief Load an xml file with admin but mandatory values are missing.
    /// \param _testName Name of the test.
    void errorWhenMissingMandatoryInPatientPerson(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        std::string xmlString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="custom_computing_query.xsd">
                                        <queryId>rifampicin_1</queryId>
                                        <clientId>124568</clientId>
                                        <date>2018-07-11T13:45:30</date> <!-- Date the xml has been sent -->
                                        <language>en</language>

                                        <admin>
                                            <patient>
                                                <person>
                                                    <address>
                                                    </address>
                                                    <phone>
                                                    </phone>
                                                    <email>
                                                    </email>
                                                </person>
                                            </patient>
                                        </admin>

                                        <drugTreatment>
                                            <!-- All the information regarding the patient -->
                                            <patient>
                                                <covariates>
                                                    <covariate>
                                                        <covariateId>ffm</covariateId>
                                                        <date>2018-07-11T10:45:30</date>
                                                        <value>40</value>
                                                        <unit>kg</unit>
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
                                                            <sampleId>123456</sampleId>
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
                                                <localComputation>true</localComputation>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <adjustmentDate>2018-07-06T08:00:00</adjustmentDate>
                                                <options>
                                                    <bestCandidatesOption>bestDosage</bestCandidatesOption>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>populationValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>lastFormulationAndRoute</formulationAndRouteSelectionOption>
                                                </options>
                                            </requestXpert>
                                            <request>
                                                <requestId>population_1</requestId>
                                                <drugId>rifampicin</drugId>
                                                <drugModelId>ch.tucuxi.rifampicin.svensson2017</drugModelId>
                                                <predictionTraits>
                                                  <computingOption>
                                                    <parametersType>population</parametersType>
                                                    <compartmentOption>allActiveMoieties</compartmentOption>
                                                    <retrieveStatistics>true</retrieveStatistics>
                                                    <retrieveParameters>true</retrieveParameters>
                                                    <retrieveCovariates>true</retrieveCovariates>
                                                  </computingOption>
                                                  <nbPointsPerHour>20</nbPointsPerHour>
                                                  <dateInterval>
                                                      <start>2018-07-06T08:00:00</start>
                                                      <end>2018-07-08T08:00:00</end>
                                                  </dateInterval>
                                                </predictionTraits>
                                            </request>
                                        </requests>
                                    </query>
                                    )";

        std::unique_ptr<Tucuxi::XpertQuery::XpertQueryData> query = nullptr;

        Tucuxi::XpertQuery::XpertQueryImport importer;
        Tucuxi::XpertQuery::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        fructose_assert_eq(importResult, Tucuxi::XpertQuery::XpertQueryImport::Status::Error);
        fructose_assert_ne(importer.getErrorMessage().find("firstName"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("lastName"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("street"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("postCode"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("city"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("number"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("address"), std::string::npos);
    }

    /// \brief Load an xml file with admin but mandatory values are missing.
    /// \param _testName Name of the test.
    void errorWhenMissingMandatoryInPatientInstitute(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        std::string xmlString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="custom_computing_query.xsd">
                                        <queryId>rifampicin_1</queryId>
                                        <clientId>124568</clientId>
                                        <date>2018-07-11T13:45:30</date> <!-- Date the xml has been sent -->
                                        <language>en</language>

                                        <admin>
                                            <patient>
                                                <person>
                                                    <firstName>John</firstName>
                                                    <lastName>Doe</lastName>
                                                    <address>
                                                        <street>Av. de l'Ours 2</street>
                                                        <postCode>1010</postCode>
                                                        <city>Lausanne</city>
                                                    </address>
                                                    <phone>
                                                        <number>0213140002</number>
                                                    </phone>
                                                    <email>
                                                        <address>john.doe@chuv.com</address>
                                                    </email>
                                                </person>
                                                <institute>
                                                    <address>
                                                    </address>
                                                    <phone>
                                                    </phone>
                                                    <email>
                                                    </email>
                                                </institute>
                                            </patient>
                                        </admin>

                                        <drugTreatment>
                                            <!-- All the information regarding the patient -->
                                            <patient>
                                                <covariates>
                                                    <covariate>
                                                        <covariateId>ffm</covariateId>
                                                        <date>2018-07-11T10:45:30</date>
                                                        <value>40</value>
                                                        <unit>kg</unit>
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
                                                            <sampleId>123456</sampleId>
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
                                                <localComputation>true</localComputation>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <adjustmentDate>2018-07-06T08:00:00</adjustmentDate>
                                                <options>
                                                    <bestCandidatesOption>bestDosage</bestCandidatesOption>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>populationValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>lastFormulationAndRoute</formulationAndRouteSelectionOption>
                                                </options>
                                            </requestXpert>
                                            <request>
                                                <requestId>population_1</requestId>
                                                <drugId>rifampicin</drugId>
                                                <drugModelId>ch.tucuxi.rifampicin.svensson2017</drugModelId>
                                                <predictionTraits>
                                                  <computingOption>
                                                    <parametersType>population</parametersType>
                                                    <compartmentOption>allActiveMoieties</compartmentOption>
                                                    <retrieveStatistics>true</retrieveStatistics>
                                                    <retrieveParameters>true</retrieveParameters>
                                                    <retrieveCovariates>true</retrieveCovariates>
                                                  </computingOption>
                                                  <nbPointsPerHour>20</nbPointsPerHour>
                                                  <dateInterval>
                                                      <start>2018-07-06T08:00:00</start>
                                                      <end>2018-07-08T08:00:00</end>
                                                  </dateInterval>
                                                </predictionTraits>
                                            </request>
                                        </requests>
                                    </query>
                                    )";

        std::unique_ptr<Tucuxi::XpertQuery::XpertQueryData> query = nullptr;

        Tucuxi::XpertQuery::XpertQueryImport importer;
        Tucuxi::XpertQuery::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        fructose_assert_eq(importResult, Tucuxi::XpertQuery::XpertQueryImport::Status::Error);
        fructose_assert_ne(importer.getErrorMessage().find("name"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("street"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("postCode"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("city"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("number"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("address"), std::string::npos);
    }

    /// \brief Load an xml file with every possible data in requestXpert node
    /// and check if the recieved values are expected.
    /// \param _testName Name of the test.
    void retrieveCompleteRequestXpert(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        std::string xmlString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="custom_computing_query.xsd">
                                        <queryId>rifampicin_1</queryId>
                                        <clientId>124568</clientId>
                                        <date>2018-07-11T13:45:30</date> <!-- Date the xml has been sent -->
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
                                                <note>random note</note>
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
                                                        <unit>kg</unit>
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
                                                            <sampleId>123456</sampleId>
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
                                                <localComputation>true</localComputation>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <adjustmentDate>2018-07-06T08:00:00</adjustmentDate>
                                                <options>
                                                    <bestCandidatesOption>bestDosage</bestCandidatesOption>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>populationValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>allFormulationAndRoutes</formulationAndRouteSelectionOption>
                                                </options>
                                            </requestXpert>
                                            <request>
                                                <requestId>population_1</requestId>
                                                <drugId>rifampicin</drugId>
                                                <drugModelId>ch.tucuxi.rifampicin.svensson2017</drugModelId>
                                                <predictionTraits>
                                                  <computingOption>
                                                    <parametersType>population</parametersType>
                                                    <compartmentOption>allActiveMoieties</compartmentOption>
                                                    <retrieveStatistics>true</retrieveStatistics>
                                                    <retrieveParameters>true</retrieveParameters>
                                                    <retrieveCovariates>true</retrieveCovariates>
                                                  </computingOption>
                                                  <nbPointsPerHour>20</nbPointsPerHour>
                                                  <dateInterval>
                                                      <start>2018-07-06T08:00:00</start>
                                                      <end>2018-07-08T08:00:00</end>
                                                  </dateInterval>
                                                </predictionTraits>
                                            </request>
                                        </requests>
                                    </query>
                                    )";

        std::unique_ptr<Tucuxi::XpertQuery::XpertQueryData> query = nullptr;

        Tucuxi::XpertQuery::XpertQueryImport importer;
        Tucuxi::XpertQuery::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);


        fructose_assert_eq(importResult, Tucuxi::XpertQuery::XpertQueryImport::Status::Ok);

        fructose_assert_eq(query->getXpertRequests().size(), 1);

        const Tucuxi::XpertQuery::XpertRequestData& xpertRequest = *(query->getXpertRequests()[0]);

        fructose_assert_eq(xpertRequest.getDrugID(), "rifampicin");
        fructose_assert_eq(xpertRequest.getLocalComputation(), true);
        fructose_assert_eq(xpertRequest.getOutputFormat() == Tucuxi::XpertQuery::OutputFormat::XML, true);
        fructose_assert_eq(xpertRequest.getOutputLang() == Tucuxi::XpertQuery::OutputLang::ENGLISH, true);
        fructose_assert_eq(xpertRequest.getAdjustmentTime(), DateTime("2018-07-06T08:00:00", "%Y-%m-%dT%H:%M:%S"));
        fructose_assert_eq(xpertRequest.getBestCandidatesOption() == Tucuxi::Core::BestCandidatesOption::BestDosage, true);
        fructose_assert_eq(xpertRequest.getLoadingOption() == Tucuxi::Core::LoadingOption::NoLoadingDose, true);
        fructose_assert_eq(xpertRequest.getRestPeriodOption() == Tucuxi::Core::RestPeriodOption::NoRestPeriod, true);
        fructose_assert_eq(xpertRequest.getTargetExtractionOption() == Tucuxi::Core::TargetExtractionOption::PopulationValues, true);
        fructose_assert_eq(xpertRequest.getFormulationAndRouteSelectionOption() == Tucuxi::Core::FormulationAndRouteSelectionOption::AllFormulationAndRoutes, true);

    }

    /// \brief Load an xml file with default data in requestXpert node
    /// and check if the recieved values are expected.
    /// \param _testName Name of the test.
    void retrieveDefaultRequestXpert(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        std::string xmlString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="custom_computing_query.xsd">
                                        <queryId>rifampicin_1</queryId>
                                        <clientId>124568</clientId>
                                        <date>2018-07-11T13:45:30</date> <!-- Date the xml has been sent -->
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
                                                <note>random note</note>
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
                                                        <unit>kg</unit>
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
                                                            <sampleId>123456</sampleId>
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
                                                <localComputation>true</localComputation>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                            </requestXpert>
                                            <request>
                                                <requestId>population_1</requestId>
                                                <drugId>rifampicin</drugId>
                                                <drugModelId>ch.tucuxi.rifampicin.svensson2017</drugModelId>
                                                <predictionTraits>
                                                  <computingOption>
                                                    <parametersType>population</parametersType>
                                                    <compartmentOption>allActiveMoieties</compartmentOption>
                                                    <retrieveStatistics>true</retrieveStatistics>
                                                    <retrieveParameters>true</retrieveParameters>
                                                    <retrieveCovariates>true</retrieveCovariates>
                                                  </computingOption>
                                                  <nbPointsPerHour>20</nbPointsPerHour>
                                                  <dateInterval>
                                                      <start>2018-07-06T08:00:00</start>
                                                      <end>2018-07-08T08:00:00</end>
                                                  </dateInterval>
                                                </predictionTraits>
                                            </request>
                                        </requests>
                                    </query>
                                    )";

        std::unique_ptr<Tucuxi::XpertQuery::XpertQueryData> query = nullptr;

        Tucuxi::XpertQuery::XpertQueryImport importer;
        Tucuxi::XpertQuery::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);


        fructose_assert_eq(importResult, Tucuxi::XpertQuery::XpertQueryImport::Status::Ok);

        fructose_assert_eq(query->getXpertRequests().size(), 1);

        const Tucuxi::XpertQuery::XpertRequestData& xpertRequest = *(query->getXpertRequests()[0]);


        fructose_assert_eq(xpertRequest.getAdjustmentTime(), Tucuxi::Common::DateTime::undefinedDateTime());
        fructose_assert_eq(xpertRequest.getBestCandidatesOption() == Tucuxi::Core::BestCandidatesOption::BestDosagePerInterval, true);
        fructose_assert_eq(xpertRequest.getLoadingOption() == Tucuxi::Core::LoadingOption::LoadingDoseAllowed, true);
        fructose_assert_eq(xpertRequest.getRestPeriodOption() == Tucuxi::Core::RestPeriodOption::RestPeriodAllowed, true);
        fructose_assert_eq(xpertRequest.getTargetExtractionOption() == Tucuxi::Core::TargetExtractionOption::DefinitionIfNoIndividualTarget, true);
        fructose_assert_eq(xpertRequest.getFormulationAndRouteSelectionOption() == Tucuxi::Core::FormulationAndRouteSelectionOption::LastFormulationAndRoute, true);
    }

    /// \brief Load an xml file without requestXpert and check that error is returned.
    /// \param _testName Name of the test.
    void errorWhenNoRequestXpert(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        std::string xmlString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="custom_computing_query.xsd">
                                        <queryId>rifampicin_1</queryId>
                                        <clientId>124568</clientId>
                                        <date>2018-07-11T13:45:30</date> <!-- Date the xml has been sent -->
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
                                                <note>random note</note>
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
                                                        <unit>kg</unit>
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
                                                            <sampleId>123456</sampleId>
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
                                            <request>
                                                <requestId>population_1</requestId>
                                                <drugId>rifampicin</drugId>
                                                <drugModelId>ch.tucuxi.rifampicin.svensson2017</drugModelId>
                                                <predictionTraits>
                                                  <computingOption>
                                                    <parametersType>population</parametersType>
                                                    <compartmentOption>allActiveMoieties</compartmentOption>
                                                    <retrieveStatistics>true</retrieveStatistics>
                                                    <retrieveParameters>true</retrieveParameters>
                                                    <retrieveCovariates>true</retrieveCovariates>
                                                  </computingOption>
                                                  <nbPointsPerHour>20</nbPointsPerHour>
                                                  <dateInterval>
                                                      <start>2018-07-06T08:00:00</start>
                                                      <end>2018-07-08T08:00:00</end>
                                                  </dateInterval>
                                                </predictionTraits>
                                            </request>
                                        </requests>
                                    </query>
                                    )";

        std::unique_ptr<Tucuxi::XpertQuery::XpertQueryData> query = nullptr;

        Tucuxi::XpertQuery::XpertQueryImport importer;
        Tucuxi::XpertQuery::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        fructose_assert_eq(importResult, Tucuxi::XpertQuery::XpertQueryImport::Status::Error);
        fructose_assert_ne(importer.getErrorMessage().find("No requestXpert found"), std::string::npos);
        fructose_assert_eq(query->getXpertRequests().size(), 0);
    }

    /// \brief Load an xml file without mandatory values in requestXpert and check that error is returned.
    /// \param _testName Name of the test.
    void errorWhenMissingMandatoryRequestXpert(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        std::string xmlString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="custom_computing_query.xsd">
                                        <queryId>rifampicin_1</queryId>
                                        <clientId>124568</clientId>
                                        <date>2018-07-11T13:45:30</date> <!-- Date the xml has been sent -->
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
                                                <note>random note</note>
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
                                                        <unit>kg</unit>
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
                                                            <sampleId>123456</sampleId>
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
                                                <adjustmentDate>2018-07-06T08:00:00</adjustmentDate>
                                                <options>
                                                    <bestCandidatesOption>bestDosage</bestCandidatesOption>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>populationValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>allFormulationAndRoutes</formulationAndRouteSelectionOption>
                                                </options>
                                            </requestXpert>
                                            <request>
                                                <requestId>population_1</requestId>
                                                <drugId>rifampicin</drugId>
                                                <drugModelId>ch.tucuxi.rifampicin.svensson2017</drugModelId>
                                                <predictionTraits>
                                                  <computingOption>
                                                    <parametersType>population</parametersType>
                                                    <compartmentOption>allActiveMoieties</compartmentOption>
                                                    <retrieveStatistics>true</retrieveStatistics>
                                                    <retrieveParameters>true</retrieveParameters>
                                                    <retrieveCovariates>true</retrieveCovariates>
                                                  </computingOption>
                                                  <nbPointsPerHour>20</nbPointsPerHour>
                                                  <dateInterval>
                                                      <start>2018-07-06T08:00:00</start>
                                                      <end>2018-07-08T08:00:00</end>
                                                  </dateInterval>
                                                </predictionTraits>
                                            </request>
                                        </requests>
                                    </query>
                                    )";

        std::unique_ptr<Tucuxi::XpertQuery::XpertQueryData> query = nullptr;

        Tucuxi::XpertQuery::XpertQueryImport importer;
        Tucuxi::XpertQuery::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        fructose_assert_eq(importResult, Tucuxi::XpertQuery::XpertQueryImport::Status::Error);
        fructose_assert_ne(importer.getErrorMessage().find("drugId"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("localComputation"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("format"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("language"), std::string::npos);
    }
};

#endif // TEST_XPERTQUERYIMPORT_H
