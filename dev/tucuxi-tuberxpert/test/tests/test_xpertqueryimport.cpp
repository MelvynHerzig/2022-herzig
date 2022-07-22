#include "test_xpertqueryimport.h"

using namespace std;
using namespace Tucuxi;

void TestXpertQueryImport::xpertQueryImport_getsAllValues_withCompleteAdmin(const string& _testName)
{
    cout << _testName << endl;


    string xmlString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                   <query version="1.0"
                                       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                       xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">

                                       <date>2018-07-11T13:45:30</date>

                                       <admin>
                                           <mandator>
                                               <person>
                                                   <id>asdf</id>
                                                   <title>Dr.</title>
                                                   <firstName>John</firstName>
                                                   <lastName>Doe</lastName>
                                                   <address>
                                                       <street>Av. de l'Ours 2</street>
                                                       <postalCode>1010</postalCode>
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
                                                       <postalCode>1010</postalCode>
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
                                                       <postalCode>1006</postalCode>
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
                                                       <postalCode>1400</postalCode>
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
                                           <clinicalDatas>
                                               <clinicalData key="goodNote"> nice </clinicalDataEntry>
                                               <clinicalData key="badNote"> <yet>random note</yet> </clinicalDataEntry>
                                           </clinicalDatas>
                                       </admin>

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
                                                   <drugId>rifampicin</drugId>
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


    unique_ptr<Xpert::XpertQueryData> query = nullptr;

    Xpert::XpertQueryImport importer;
    Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

    const Xpert::AdminData& pAdmin = *query->getAdminData();

    fructose_assert_eq(importResult, Xpert::XpertQueryImport::Status::Ok);

    const Xpert::PersonData& mandator = pAdmin.getMandator()->getPerson();
    const Xpert::AddressData& mandatorAddress = *mandator.getAddress();
    const Xpert::PhoneData& mandatorPhone = *mandator.getPhone();
    const Xpert::EmailData& mandatorEmail = *mandator.getEmail();
    const Xpert::InstituteData& mandatorInstitute = *pAdmin.getMandator()->getInstitute();
    const Xpert::AddressData& mandatorInstituteAddress = *mandatorInstitute.getAddress();
    const Xpert::PhoneData& mandatorInstitutePhone = *mandatorInstitute.getPhone();
    const Xpert::EmailData& mandatorInstituteEmail = *mandatorInstitute.getEmail();

    fructose_assert_eq(mandator.getId(), "asdf");
    fructose_assert_eq(mandator.getTitle(), "Dr.");
    fructose_assert_eq(mandator.getFirstName(), "John");
    fructose_assert_eq(mandator.getLastName(), "Doe");
    fructose_assert_eq(mandatorAddress.getStreet(), "Av. de l'Ours 2");
    fructose_assert_eq(mandatorAddress.getPostalCode(), 1010);
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
    fructose_assert_eq(mandatorInstituteAddress.getPostalCode(), 1010);
    fructose_assert_eq(mandatorInstituteAddress.getCity(), "Lausanne");
    fructose_assert_eq(mandatorInstituteAddress.getState(), "Vaud");
    fructose_assert_eq(mandatorInstituteAddress.getCountry(), "Suisse");
    fructose_assert_eq(mandatorInstitutePhone.getNumber(), "0213140001");
    fructose_assert_eq(mandatorInstitutePhone.getType(), "professional");
    fructose_assert_eq(mandatorInstituteEmail.getAddress(), "info@chuv.com");
    fructose_assert_eq(mandatorInstituteEmail.getType(), "professional");

    const Xpert::PersonData& patient = pAdmin.getPatient()->getPerson();
    const Xpert::AddressData& patientAddress = *patient.getAddress();
    const Xpert::PhoneData& patientPhone = *patient.getPhone();
    const Xpert::EmailData& patientEmail = *patient.getEmail();
    const Xpert::InstituteData& patientInstitute = *pAdmin.getPatient()->getInstitute();
    const Xpert::AddressData& patientInstituteAddress = *patientInstitute.getAddress();
    const Xpert::PhoneData& patientInstitutePhone = *patientInstitute.getPhone();
    const Xpert::EmailData& patientInstituteEmail = *patientInstitute.getEmail();

    fructose_assert_eq(patient.getId(), "123456");
    fructose_assert_eq(patient.getFirstName(), "Alice");
    fructose_assert_eq(patient.getLastName(), "Aupaysdesmerveilles");
    fructose_assert_eq(patientAddress.getStreet(), "Av. d'Ouchy 27");
    fructose_assert_eq(patientAddress.getPostalCode(), 1006);
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
    fructose_assert_eq(patientInstituteAddress.getPostalCode(), 1400);
    fructose_assert_eq(patientInstituteAddress.getCity(), "Yverdon-les-Bains");
    fructose_assert_eq(patientInstituteAddress.getState(), "Vaud");
    fructose_assert_eq(patientInstituteAddress.getCountry(), "Suisse");
    fructose_assert_eq(patientInstitutePhone.getNumber(), "0123456789");
    fructose_assert_eq(patientInstitutePhone.getType(), "professional");
    fructose_assert_eq(patientInstituteEmail.getAddress(), "info@ehnv.com");
    fructose_assert_eq(patientInstituteEmail.getType(), "professional");

    fructose_assert_eq(pAdmin.getClinicalDatas()->getData().find("goodNote")->second, " nice ");
    fructose_assert_eq(pAdmin.getClinicalDatas()->getData().find("badNote")->second, "");
}

void TestXpertQueryImport::xpertQueryImport_getsNullptrAdminData_withNoAdminElement(const string& _testName)
{
    cout << _testName << endl;

    string xmlString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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
                                                    <drugId>rifampicin</drugId>
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

    unique_ptr<Xpert::XpertQueryData> query = nullptr;

    Xpert::XpertQueryImport importer;
    Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);


    fructose_assert_eq(importResult, Xpert::XpertQueryImport::Status::Ok);
    fructose_assert_eq(query->getAdminData().get(), nullptr);
}

void TestXpertQueryImport::xpertQueryImport_adminDataGettersReturnNullptr_withEmptyAdminElement(const string& _testName)
{
    cout << _testName << endl;

    string xmlString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">
                                        <date>2018-07-11T13:45:30</date> <!-- Date the xml has been sent -->

                                        <admin></admin>

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
                                                    <drugId>rifampicin</drugId>
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

    unique_ptr<Xpert::XpertQueryData> query = nullptr;

    Xpert::XpertQueryImport importer;
    Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

    fructose_assert_eq(importResult, Xpert::XpertQueryImport::Status::Ok);
    fructose_assert_ne(query->getAdminData().get(), nullptr);

    fructose_assert_eq(query->getAdminData()->getMandator().get(), nullptr);
    fructose_assert_eq(query->getAdminData()->getPatient().get(), nullptr);
    fructose_assert_eq(query->getAdminData()->getClinicalDatas().get(), nullptr);
}

void TestXpertQueryImport::xpertQueryImport_missingValuesAreNullptrOrEmptyString_withMinimalPersonInMandatorAndPatient(const string& _testName)
{
    cout << _testName << endl;

    string xmlString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">
                                        <date>2018-07-11T13:45:30</date> <!-- Date the xml has been sent -->

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

    unique_ptr<Xpert::XpertQueryData> query = nullptr;

    Xpert::XpertQueryImport importer;
    Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

    const unique_ptr<Xpert::AdminData>& admin = query->getAdminData();

    fructose_assert_eq(importResult, Xpert::XpertQueryImport::Status::Ok);
    fructose_assert_ne(admin.get(), nullptr);

    const Xpert::PersonData& mandator = admin->getMandator()->getPerson();
    const Xpert::PersonData& patient = admin->getPatient()->getPerson();


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

void TestXpertQueryImport::xpertQueryImport_missingValuesAreNullptrOrEmptyString_withMinimalInstituteInMandatorAndPatient(const string& _testName)
{
    cout << _testName << endl;

    string xmlString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">
                                        <date>2018-07-11T13:45:30</date> <!-- Date the xml has been sent -->

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

    unique_ptr<Xpert::XpertQueryData> query = nullptr;

    Xpert::XpertQueryImport importer;
    Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

    const unique_ptr<Xpert::AdminData>& admin = query->getAdminData();

    fructose_assert_eq(importResult, Xpert::XpertQueryImport::Status::Ok);
    fructose_assert_ne(admin.get(), nullptr);

    const unique_ptr<Xpert::InstituteData>& mandatorInstitute = admin->getMandator()->getInstitute();
    const unique_ptr<Xpert::InstituteData>& patientInstitute = admin->getPatient()->getInstitute();


    fructose_assert_eq(mandatorInstitute->getId(), "");
    fructose_assert_eq(mandatorInstitute->getAddress().get(), nullptr);
    fructose_assert_eq(mandatorInstitute->getPhone().get(), nullptr);
    fructose_assert_eq(mandatorInstitute->getEmail().get(), nullptr);

    fructose_assert_eq(patientInstitute->getId(), "");
    fructose_assert_eq(patientInstitute->getAddress().get(), nullptr);
    fructose_assert_eq(patientInstitute->getPhone().get(), nullptr);
    fructose_assert_eq(patientInstitute->getEmail().get(), nullptr);
}

void TestXpertQueryImport::xpertQueryImport_missingValuesAreEmptyString_withMinimalAddressPhoneAndEmail(const string& _testName)
{
    cout << _testName << endl;

    string xmlString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">
                                        <date>2018-07-11T13:45:30</date> <!-- Date the xml has been sent -->
                                        <language>en</language>

                                        <admin>
                                            <mandator>
                                                <person>
                                                    <firstName>John</firstName>
                                                    <lastName>Doe</lastName>
                                                    <address>
                                                        <street>Av. de l'Ours 2</street>
                                                        <postalCode>1010</postalCode>
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
                                                        <postalCode>1010</postalCode>
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
                                                        <postalCode>1006</postalCode>
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
                                                        <postalCode>1400</postalCode>
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

    unique_ptr<Xpert::XpertQueryData> query = nullptr;

    Xpert::XpertQueryImport importer;
    Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

    const unique_ptr<Xpert::AdminData>& pAdmin = query->getAdminData();

    fructose_assert_eq(importResult, Xpert::XpertQueryImport::Status::Ok);

    const Xpert::PersonData& mandator = pAdmin->getMandator()->getPerson();
    const Xpert::AddressData& mandatorAddress = *mandator.getAddress();
    const Xpert::PhoneData& mandatorPhone = *mandator.getPhone();
    const Xpert::EmailData& mandatorEmail = *mandator.getEmail();
    const Xpert::InstituteData& mandatorInstitute = *pAdmin->getMandator()->getInstitute();
    const Xpert::AddressData& mandatorInstituteAddress = *mandatorInstitute.getAddress();
    const Xpert::PhoneData& mandatorInstitutePhone = *mandatorInstitute.getPhone();
    const Xpert::EmailData& mandatorInstituteEmail = *mandatorInstitute.getEmail();

    const Xpert::PersonData& patient = pAdmin->getPatient()->getPerson();
    const Xpert::AddressData& patientAddress = *patient.getAddress();
    const Xpert::PhoneData& patientPhone = *patient.getPhone();
    const Xpert::EmailData& patientEmail = *patient.getEmail();
    const Xpert::InstituteData& patientInstitute = *pAdmin->getPatient()->getInstitute();
    const Xpert::AddressData& patientInstituteAddress = *patientInstitute.getAddress();
    const Xpert::PhoneData& patientInstitutePhone = *patientInstitute.getPhone();
    const Xpert::EmailData& patientInstituteEmail = *patientInstitute.getEmail();

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

void TestXpertQueryImport::xpertQueryImport_importError_withMissingMandatoryValuesInCompleteMandatorPerson(const string& _testName)
{
    cout << _testName << endl;

    string xmlString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">
                                        <date>2018-07-11T13:45:30</date> <!-- Date the xml has been sent -->

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

    unique_ptr<Xpert::XpertQueryData> query = nullptr;

    Xpert::XpertQueryImport importer;
    Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

    fructose_assert_eq(importResult, Xpert::XpertQueryImport::Status::Error);
    fructose_assert_ne(importer.getErrorMessage().find("firstName"), string::npos);
    fructose_assert_ne(importer.getErrorMessage().find("lastName"), string::npos);
    fructose_assert_ne(importer.getErrorMessage().find("street"), string::npos);
    fructose_assert_ne(importer.getErrorMessage().find("postalCode"), string::npos);
    fructose_assert_ne(importer.getErrorMessage().find("city"), string::npos);
    fructose_assert_ne(importer.getErrorMessage().find("number"), string::npos);
    fructose_assert_ne(importer.getErrorMessage().find("address"), string::npos);
}

void TestXpertQueryImport::xpertQueryImport_importError_withMissingMandatoryValuesInCompleteMandatorInstitute(const string& _testName)
{
    cout << _testName << endl;

    string xmlString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">
                                        <date>2018-07-11T13:45:30</date> <!-- Date the xml has been sent -->

                                        <admin>
                                            <mandator>
                                                <person>
                                                    <firstName>John</firstName>
                                                    <lastName>Doe</lastName>
                                                    <address>
                                                        <street>Av. de l'Ours 2</street>
                                                        <postalCode>1010</postalCode>
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

    unique_ptr<Xpert::XpertQueryData> query = nullptr;

    Xpert::XpertQueryImport importer;
    Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

    fructose_assert_eq(importResult, Xpert::XpertQueryImport::Status::Error);
    fructose_assert_ne(importer.getErrorMessage().find("name"), string::npos);
    fructose_assert_ne(importer.getErrorMessage().find("street"), string::npos);
    fructose_assert_ne(importer.getErrorMessage().find("postalCode"), string::npos);
    fructose_assert_ne(importer.getErrorMessage().find("city"), string::npos);
    fructose_assert_ne(importer.getErrorMessage().find("number"), string::npos);
    fructose_assert_ne(importer.getErrorMessage().find("address"), string::npos);
}

void TestXpertQueryImport::xpertQueryImport_importError_withMissingMandatoryValuesInCompletePatientPerson(const string& _testName)
{
    cout << _testName << endl;

    string xmlString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">
                                        <date>2018-07-11T13:45:30</date> <!-- Date the xml has been sent -->

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

    unique_ptr<Xpert::XpertQueryData> query = nullptr;

    Xpert::XpertQueryImport importer;
    Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

    fructose_assert_eq(importResult, Xpert::XpertQueryImport::Status::Error);
    fructose_assert_ne(importer.getErrorMessage().find("firstName"), string::npos);
    fructose_assert_ne(importer.getErrorMessage().find("lastName"), string::npos);
    fructose_assert_ne(importer.getErrorMessage().find("street"), string::npos);
    fructose_assert_ne(importer.getErrorMessage().find("postalCode"), string::npos);
    fructose_assert_ne(importer.getErrorMessage().find("city"), string::npos);
    fructose_assert_ne(importer.getErrorMessage().find("number"), string::npos);
    fructose_assert_ne(importer.getErrorMessage().find("address"), string::npos);
}

void TestXpertQueryImport::xpertQueryImport_importError_withMissingMandatoryValuesInCompletePatientInstitute(const string& _testName)
{
    cout << _testName << endl;

    string xmlString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">
                                        <date>2018-07-11T13:45:30</date> <!-- Date the xml has been sent -->

                                        <admin>
                                            <patient>
                                                <person>
                                                    <firstName>John</firstName>
                                                    <lastName>Doe</lastName>
                                                    <address>
                                                        <street>Av. de l'Ours 2</street>
                                                        <postalCode>1010</postalCode>
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

    unique_ptr<Xpert::XpertQueryData> query = nullptr;

    Xpert::XpertQueryImport importer;
    Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

    fructose_assert_eq(importResult, Xpert::XpertQueryImport::Status::Error);
    fructose_assert_ne(importer.getErrorMessage().find("name"), string::npos);
    fructose_assert_ne(importer.getErrorMessage().find("street"), string::npos);
    fructose_assert_ne(importer.getErrorMessage().find("postalCode"), string::npos);
    fructose_assert_ne(importer.getErrorMessage().find("city"), string::npos);
    fructose_assert_ne(importer.getErrorMessage().find("number"), string::npos);
    fructose_assert_ne(importer.getErrorMessage().find("address"), string::npos);
}

void TestXpertQueryImport::xpertQueryImport_getsAllValues_withCompleteXpertRequest(const string& _testName)
{
    cout << _testName << endl;

    string xmlString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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
                                                    <drugId>rifampicin</drugId>
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

    unique_ptr<Xpert::XpertQueryData> query = nullptr;

    Xpert::XpertQueryImport importer;
    Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);


    fructose_assert_eq(importResult, Xpert::XpertQueryImport::Status::Ok);

    fructose_assert_eq(query->getXpertRequests().size(), 1);

    const Xpert::XpertRequestData& xpertRequest = *(query->getXpertRequests()[0]);

    fructose_assert_eq(xpertRequest.getDrugId(), "rifampicin");
    fructose_assert_eq(xpertRequest.getOutputFormat() == Xpert::OutputFormat::XML, true);
    fructose_assert_eq(xpertRequest.getOutputLang() == Xpert::OutputLang::ENGLISH, true);
    fructose_assert_eq(xpertRequest.getAdjustmentTime(), DateTime("2018-07-06T08:00:00", "%Y-%m-%dT%H:%M:%S"));
    fructose_assert_eq(xpertRequest.getLoadingOption() == Xpert::LoadingOption::NoLoadingDose, true);
    fructose_assert_eq(xpertRequest.getRestPeriodOption() == Xpert::RestPeriodOption::NoRestPeriod, true);
    fructose_assert_eq(xpertRequest.getTargetExtractionOption() == Core::TargetExtractionOption::PopulationValues, true);
    fructose_assert_eq(xpertRequest.getFormulationAndRouteSelectionOption() == Core::FormulationAndRouteSelectionOption::AllFormulationAndRoutes, true);

}

void TestXpertQueryImport::xpertQueryImport_getDefaultValues_withMinimalXpertRequest(const string& _testName)
{
    cout << _testName << endl;

    string xmlString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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
                                                    <drugId>rifampicin</drugId>
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
                                                <drugId>rifampicin</drugId>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                            </xpertRequest>
                                        </requests>
                                    </query>
                                    )";

    unique_ptr<Xpert::XpertQueryData> query = nullptr;

    Xpert::XpertQueryImport importer;
    Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);


    fructose_assert_eq(importResult, Xpert::XpertQueryImport::Status::Ok);

    fructose_assert_eq(query->getXpertRequests().size(), 1);

    const Xpert::XpertRequestData& xpertRequest = *(query->getXpertRequests()[0]);


    fructose_assert_eq(xpertRequest.getAdjustmentTime(), Common::DateTime::undefinedDateTime());
    fructose_assert_eq(xpertRequest.getLoadingOption() == Xpert::LoadingOption::Unspecified, true);
    fructose_assert_eq(xpertRequest.getRestPeriodOption() == Xpert::RestPeriodOption::Unspecified, true);
    fructose_assert_eq(xpertRequest.getTargetExtractionOption() == Core::TargetExtractionOption::DefinitionIfNoIndividualTarget, true);
    fructose_assert_eq(xpertRequest.getFormulationAndRouteSelectionOption() == Core::FormulationAndRouteSelectionOption::LastFormulationAndRoute, true);
}

void TestXpertQueryImport::xpertQueryImport_importError_withoutXpertRequest(const string& _testName)
{
    cout << _testName << endl;

    string xmlString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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
                                                    <drugId>rifampicin</drugId>
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
                                        </requests>
                                    </query>
                                    )";

    unique_ptr<Xpert::XpertQueryData> query = nullptr;

    Xpert::XpertQueryImport importer;
    Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

    fructose_assert_eq(importResult, Xpert::XpertQueryImport::Status::Error);
    fructose_assert_ne(importer.getErrorMessage().find("No xpertRequest found"), string::npos);
    fructose_assert_eq(query->getXpertRequests().size(), 0);
}

void TestXpertQueryImport::xpertQueryImport_importError_withMissingMandatoryInXpertRequest(const string& _testName)
{
    cout << _testName << endl;

    string xmlString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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
                                                    <drugId>rifampicin</drugId>
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

    unique_ptr<Xpert::XpertQueryData> query = nullptr;

    Xpert::XpertQueryImport importer;
    Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

    fructose_assert_eq(importResult, Xpert::XpertQueryImport::Status::Error);
    fructose_assert_ne(importer.getErrorMessage().find("drugId"), string::npos);
    fructose_assert_ne(importer.getErrorMessage().find("format"), string::npos);
    fructose_assert_ne(importer.getErrorMessage().find("language"), string::npos);
}

void TestXpertQueryImport::xpertQueryImport_importError_withFileBadlyFormatted(const string& _testName)
{
    cout << _testName << endl;

    string xmlString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <badFormatted>
                                    )";

    unique_ptr<Xpert::XpertQueryData> query = nullptr;

    Xpert::XpertQueryImport importer;
    Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

    fructose_assert_eq(importResult, Xpert::XpertQueryImport::Status::CantCreateXmlDocument);
}

void TestXpertQueryImport::xpertQueryImport_importError_withNonExistingFile(const string& _testName)
{
    cout << _testName << endl;

    unique_ptr<Xpert::XpertQueryData> query = nullptr;

    Xpert::XpertQueryImport importer;
    Xpert::XpertQueryImport::Status importResult = importer.importFromFile(query, "this file is not existing.xml");

    fructose_assert_eq(importResult, Xpert::XpertQueryImport::Status::CantOpenFile);
}
