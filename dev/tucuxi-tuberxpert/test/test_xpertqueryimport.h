#ifndef TEST_XPERTQUERYIMPORT_H
#define TEST_XPERTQUERYIMPORT_H

#include "tuberxpert/query/xpertqueryimport.h"
#include "tuberxpert/query/xpertrequestdata.h"

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
                                           <xpertRequest>
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
                                                   <formulationAndRouteSelectionOption>allFormulationAndRoutes</formulationAndRouteSelectionOption>
                                               </options>
                                           </xpertRequest>
                                       </requests>
                                   </query>)";


        std::unique_ptr<Tucuxi::Xpert::XpertQueryData> query = nullptr;

        Tucuxi::Xpert::XpertQueryImport importer;
        Tucuxi::Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        const Tucuxi::Xpert::AdminData& pAdmin = *query->getAmin();

        fructose_assert_eq(importResult, Tucuxi::Xpert::XpertQueryImport::Status::Ok);

        const Tucuxi::Xpert::PersonData& mandator = pAdmin.getMandator()->getPerson();
        const Tucuxi::Xpert::AddressData& mandatorAddress = *mandator.getAddress();
        const Tucuxi::Xpert::PhoneData& mandatorPhone = *mandator.getPhone();
        const Tucuxi::Xpert::EmailData& mandatorEmail = *mandator.getEmail();
        const Tucuxi::Xpert::InstituteData& mandatorInstitute = *pAdmin.getMandator()->getInstitute();
        const Tucuxi::Xpert::AddressData& mandatorInstituteAddress = *mandatorInstitute.getAddress();
        const Tucuxi::Xpert::PhoneData& mandatorInstitutePhone = *mandatorInstitute.getPhone();
        const Tucuxi::Xpert::EmailData& mandatorInstituteEmail = *mandatorInstitute.getEmail();

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

        const Tucuxi::Xpert::PersonData& patient = pAdmin.getPatient()->getPerson();
        const Tucuxi::Xpert::AddressData& patientAddress = *patient.getAddress();
        const Tucuxi::Xpert::PhoneData& patientPhone = *patient.getPhone();
        const Tucuxi::Xpert::EmailData& patientEmail = *patient.getEmail();
        const Tucuxi::Xpert::InstituteData& patientInstitute = *pAdmin.getPatient()->getInstitute();
        const Tucuxi::Xpert::AddressData& patientInstituteAddress = *patientInstitute.getAddress();
        const Tucuxi::Xpert::PhoneData& patientInstitutePhone = *patientInstitute.getPhone();
        const Tucuxi::Xpert::EmailData& patientInstituteEmail = *patientInstitute.getEmail();

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

        fructose_assert_eq(pAdmin.getClinicalData()->getData().find("goodNote")->second, " nice ");
        fructose_assert_eq(pAdmin.getClinicalData()->getData().find("badNote")->second, "");
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
                                            <xpertRequest>
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
                                            </xpertRequest>
                                        </requests>
                                    </query>
                                    )";

        std::unique_ptr<Tucuxi::Xpert::XpertQueryData> query = nullptr;

        Tucuxi::Xpert::XpertQueryImport importer;
        Tucuxi::Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);


        fructose_assert_eq(importResult, Tucuxi::Xpert::XpertQueryImport::Status::Ok);
        fructose_assert_eq(query->getAmin().get(), nullptr);
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
                                            <xpertRequest>
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
                                            </xpertRequest>
                                        </requests>
                                    </query>
                                    )";

        std::unique_ptr<Tucuxi::Xpert::XpertQueryData> query = nullptr;

        Tucuxi::Xpert::XpertQueryImport importer;
        Tucuxi::Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        fructose_assert_eq(importResult, Tucuxi::Xpert::XpertQueryImport::Status::Ok);
        fructose_assert_ne(query->getAmin().get(), nullptr);

        fructose_assert_eq(query->getAmin()->getMandator().get(), nullptr);
        fructose_assert_eq(query->getAmin()->getPatient().get(), nullptr);
        fructose_assert_eq(query->getAmin()->getClinicalData().get(), nullptr);
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
                                            <xpertRequest>
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
                                            </xpertRequest>
                                        </requests>
                                    </query>
                                    )";

        std::unique_ptr<Tucuxi::Xpert::XpertQueryData> query = nullptr;

        Tucuxi::Xpert::XpertQueryImport importer;
        Tucuxi::Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        const std::unique_ptr<Tucuxi::Xpert::AdminData>& admin = query->getAmin();

        fructose_assert_eq(importResult, Tucuxi::Xpert::XpertQueryImport::Status::Ok);
        fructose_assert_ne(admin.get(), nullptr);

        const Tucuxi::Xpert::PersonData& mandator = admin->getMandator()->getPerson();
        const Tucuxi::Xpert::PersonData& patient = admin->getPatient()->getPerson();


        fructose_assert_eq(mandator.getId(), "");
        fructose_assert_eq(mandator.getTitle(), "");
        fructose_assert_eq(mandator.getAddress().get(), nullptr);
        fructose_assert_eq(mandator.getPhone().get(), nullptr);
        fructose_assert_eq(mandator.getEmail().get(), nullptr);
        fructose_assert_eq(admin->getMandator()->getInstitute().get(), nullptr);

        fructose_assert_eq(patient.getId(), "");
        fructose_assert_eq(patient.getTitle(), "");
        fructose_assert_eq(patient.getAddress().get(), nullptr);
        fructose_assert_eq(patient.getPhone().get(), nullptr);
        fructose_assert_eq(patient.getEmail().get(), nullptr);
        fructose_assert_eq(admin->getPatient()->getInstitute().get(), nullptr);
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
                                            <xpertRequest>
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
                                            </xpertRequest>
                                        </requests>
                                    </query>
                                    )";

        std::unique_ptr<Tucuxi::Xpert::XpertQueryData> query = nullptr;

        Tucuxi::Xpert::XpertQueryImport importer;
        Tucuxi::Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

         const std::unique_ptr<Tucuxi::Xpert::AdminData>& admin = query->getAmin();

        fructose_assert_eq(importResult, Tucuxi::Xpert::XpertQueryImport::Status::Ok);
        fructose_assert_ne(admin.get(), nullptr);

        const std::unique_ptr<Tucuxi::Xpert::InstituteData>& mandatorInstitute = admin->getMandator()->getInstitute();
        const std::unique_ptr<Tucuxi::Xpert::InstituteData>& patientInstitute = admin->getPatient()->getInstitute();


        fructose_assert_eq(mandatorInstitute->getId(), "");
        fructose_assert_eq(mandatorInstitute->getAddress().get(), nullptr);
        fructose_assert_eq(mandatorInstitute->getPhone().get(), nullptr);
        fructose_assert_eq(mandatorInstitute->getEmail().get(), nullptr);

        fructose_assert_eq(patientInstitute->getId(), "");
        fructose_assert_eq(patientInstitute->getAddress().get(), nullptr);
        fructose_assert_eq(patientInstitute->getPhone().get(), nullptr);
        fructose_assert_eq(patientInstitute->getEmail().get(), nullptr);
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
                                            <xpertRequest>
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
                                            </xpertRequest>
                                        </requests>
                                    </query>
                                    )";

        std::unique_ptr<Tucuxi::Xpert::XpertQueryData> query = nullptr;

        Tucuxi::Xpert::XpertQueryImport importer;
        Tucuxi::Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        const std::unique_ptr<Tucuxi::Xpert::AdminData>& pAdmin = query->getAmin();

        fructose_assert_eq(importResult, Tucuxi::Xpert::XpertQueryImport::Status::Ok);

        const Tucuxi::Xpert::PersonData& mandator = pAdmin->getMandator()->getPerson();
        const Tucuxi::Xpert::AddressData& mandatorAddress = *mandator.getAddress();
        const Tucuxi::Xpert::PhoneData& mandatorPhone = *mandator.getPhone();
        const Tucuxi::Xpert::EmailData& mandatorEmail = *mandator.getEmail();
        const Tucuxi::Xpert::InstituteData& mandatorInstitute = *pAdmin->getMandator()->getInstitute();
        const Tucuxi::Xpert::AddressData& mandatorInstituteAddress = *mandatorInstitute.getAddress();
        const Tucuxi::Xpert::PhoneData& mandatorInstitutePhone = *mandatorInstitute.getPhone();
        const Tucuxi::Xpert::EmailData& mandatorInstituteEmail = *mandatorInstitute.getEmail();

        const Tucuxi::Xpert::PersonData& patient = pAdmin->getPatient()->getPerson();
        const Tucuxi::Xpert::AddressData& patientAddress = *patient.getAddress();
        const Tucuxi::Xpert::PhoneData& patientPhone = *patient.getPhone();
        const Tucuxi::Xpert::EmailData& patientEmail = *patient.getEmail();
        const Tucuxi::Xpert::InstituteData& patientInstitute = *pAdmin->getPatient()->getInstitute();
        const Tucuxi::Xpert::AddressData& patientInstituteAddress = *patientInstitute.getAddress();
        const Tucuxi::Xpert::PhoneData& patientInstitutePhone = *patientInstitute.getPhone();
        const Tucuxi::Xpert::EmailData& patientInstituteEmail = *patientInstitute.getEmail();

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
                                            <xpertRequest>
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
                                            </xpertRequest>
                                        </requests>
                                    </query>
                                    )";

        std::unique_ptr<Tucuxi::Xpert::XpertQueryData> query = nullptr;

        Tucuxi::Xpert::XpertQueryImport importer;
        Tucuxi::Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        fructose_assert_eq(importResult, Tucuxi::Xpert::XpertQueryImport::Status::Error);
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
                                            <xpertRequest>
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
                                            </xpertRequest>
                                        </requests>
                                    </query>
                                    )";

        std::unique_ptr<Tucuxi::Xpert::XpertQueryData> query = nullptr;

        Tucuxi::Xpert::XpertQueryImport importer;
        Tucuxi::Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        fructose_assert_eq(importResult, Tucuxi::Xpert::XpertQueryImport::Status::Error);
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
                                            <xpertRequest>
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
                                            </xpertRequest>
                                        </requests>
                                    </query>
                                    )";

        std::unique_ptr<Tucuxi::Xpert::XpertQueryData> query = nullptr;

        Tucuxi::Xpert::XpertQueryImport importer;
        Tucuxi::Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        fructose_assert_eq(importResult, Tucuxi::Xpert::XpertQueryImport::Status::Error);
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
                                            <xpertRequest>
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
                                            </xpertRequest>
                                        </requests>
                                    </query>
                                    )";

        std::unique_ptr<Tucuxi::Xpert::XpertQueryData> query = nullptr;

        Tucuxi::Xpert::XpertQueryImport importer;
        Tucuxi::Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        fructose_assert_eq(importResult, Tucuxi::Xpert::XpertQueryImport::Status::Error);
        fructose_assert_ne(importer.getErrorMessage().find("name"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("street"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("postCode"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("city"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("number"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("address"), std::string::npos);
    }

    /// \brief Load an xml file with every possible data in xpertRequest node
    /// and check if the recieved values are expected.
    /// \param _testName Name of the test.
    void retrieveCompleteXpertRequest(const std::string& _testName)
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
                                                <clinicalDataEntry key="note">random note</clinicalDataEntry>
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
                                            <xpertRequest>
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
                                                    <formulationAndRouteSelectionOption>allFormulationAndRoutes</formulationAndRouteSelectionOption>
                                                </options>
                                            </xpertRequest>
                                        </requests>
                                    </query>
                                    )";

        std::unique_ptr<Tucuxi::Xpert::XpertQueryData> query = nullptr;

        Tucuxi::Xpert::XpertQueryImport importer;
        Tucuxi::Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);


        fructose_assert_eq(importResult, Tucuxi::Xpert::XpertQueryImport::Status::Ok);

        fructose_assert_eq(query->getXpertRequests().size(), 1);

        const Tucuxi::Xpert::XpertRequestData& xpertRequest = *(query->getXpertRequests()[0]);

        fructose_assert_eq(xpertRequest.getDrugID(), "rifampicin");
        fructose_assert_eq(xpertRequest.getOutputFormat() == Tucuxi::Xpert::OutputFormat::XML, true);
        fructose_assert_eq(xpertRequest.getOutputLang() == Tucuxi::Xpert::OutputLang::ENGLISH, true);
        fructose_assert_eq(xpertRequest.getAdjustmentTime(), DateTime("2018-07-06T08:00:00", "%Y-%m-%dT%H:%M:%S"));
        fructose_assert_eq(xpertRequest.getLoadingOption() == Tucuxi::Xpert::LoadingOption::NoLoadingDose, true);
        fructose_assert_eq(xpertRequest.getRestPeriodOption() == Tucuxi::Xpert::RestPeriodOption::NoRestPeriod, true);
        fructose_assert_eq(xpertRequest.getTargetExtractionOption() == Tucuxi::Core::TargetExtractionOption::PopulationValues, true);
        fructose_assert_eq(xpertRequest.getFormulationAndRouteSelectionOption() == Tucuxi::Core::FormulationAndRouteSelectionOption::AllFormulationAndRoutes, true);

    }

    /// \brief Load an xml file with default data in xpertRequest node
    /// and check if the recieved values are expected.
    /// \param _testName Name of the test.
    void retrieveDefaultXpertRequest(const std::string& _testName)
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
                                                <clinicalDataEntry key="note">random note</clinicalDataEntry>
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
                                            <xpertRequest>
                                                <drugId>rifampicin</drugId>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                            </xpertRequest>
                                        </requests>
                                    </query>
                                    )";

        std::unique_ptr<Tucuxi::Xpert::XpertQueryData> query = nullptr;

        Tucuxi::Xpert::XpertQueryImport importer;
        Tucuxi::Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);


        fructose_assert_eq(importResult, Tucuxi::Xpert::XpertQueryImport::Status::Ok);

        fructose_assert_eq(query->getXpertRequests().size(), 1);

        const Tucuxi::Xpert::XpertRequestData& xpertRequest = *(query->getXpertRequests()[0]);


        fructose_assert_eq(xpertRequest.getAdjustmentTime(), Tucuxi::Common::DateTime::undefinedDateTime());
        fructose_assert_eq(xpertRequest.getLoadingOption() == Tucuxi::Xpert::LoadingOption::Unspecified, true);
        fructose_assert_eq(xpertRequest.getRestPeriodOption() == Tucuxi::Xpert::RestPeriodOption::Unspecified, true);
        fructose_assert_eq(xpertRequest.getTargetExtractionOption() == Tucuxi::Core::TargetExtractionOption::DefinitionIfNoIndividualTarget, true);
        fructose_assert_eq(xpertRequest.getFormulationAndRouteSelectionOption() == Tucuxi::Core::FormulationAndRouteSelectionOption::LastFormulationAndRoute, true);
    }

    /// \brief Load an xml file without xpertRequest and check that error is returned.
    /// \param _testName Name of the test.
    void errorWhenNoXpertRequest(const std::string& _testName)
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
                                                <clinicalDataEntry key="note">random note</clinicalDataEntry>
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
                                        </requests>
                                    </query>
                                    )";

        std::unique_ptr<Tucuxi::Xpert::XpertQueryData> query = nullptr;

        Tucuxi::Xpert::XpertQueryImport importer;
        Tucuxi::Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        fructose_assert_eq(importResult, Tucuxi::Xpert::XpertQueryImport::Status::Error);
        fructose_assert_ne(importer.getErrorMessage().find("No xpertRequest found"), std::string::npos);
        fructose_assert_eq(query->getXpertRequests().size(), 0);
    }

    /// \brief Load an xml file without mandatory values in xpertRequest and check that error is returned.
    /// \param _testName Name of the test.
    void errorWhenMissingMandatoryXpertRequest(const std::string& _testName)
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
                                                <clinicalDataEntry key="note">random note</clinicalDataEntry>
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
                                            <xpertRequest>
                                                <adjustmentDate>2018-07-06T08:00:00</adjustmentDate>
                                                <options>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>populationValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>allFormulationAndRoutes</formulationAndRouteSelectionOption>
                                                </options>
                                            </xpertRequest>
                                        </requests>
                                    </query>
                                    )";

        std::unique_ptr<Tucuxi::Xpert::XpertQueryData> query = nullptr;

        Tucuxi::Xpert::XpertQueryImport importer;
        Tucuxi::Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        fructose_assert_eq(importResult, Tucuxi::Xpert::XpertQueryImport::Status::Error);
        fructose_assert_ne(importer.getErrorMessage().find("drugId"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("format"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("language"), std::string::npos);
    }

    /// \brief Load an xml file not well formatted.
    /// \param _testName Name of the test.
    void errorWhenCreatingXmlDocumentFromString(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        std::string xmlString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <badFormatted>
                                    )";

        std::unique_ptr<Tucuxi::Xpert::XpertQueryData> query = nullptr;

        Tucuxi::Xpert::XpertQueryImport importer;
        Tucuxi::Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        fructose_assert_eq(importResult, Tucuxi::Xpert::XpertQueryImport::Status::CantCreateXmlDocument);
    }

    /// \brief Tries to open a file that does not exist.
    /// \param _testName Name of the test.
    void errorWhenCreatingXmlDocumentFromFile(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::Xpert::XpertQueryData> query = nullptr;

        Tucuxi::Xpert::XpertQueryImport importer;
        Tucuxi::Xpert::XpertQueryImport::Status importResult = importer.importFromFile(query, "this file is not existing.xml");

        fructose_assert_eq(importResult, Tucuxi::Xpert::XpertQueryImport::Status::CantOpenFile);
    }
};

#endif // TEST_XPERTQUERYIMPORT_H
