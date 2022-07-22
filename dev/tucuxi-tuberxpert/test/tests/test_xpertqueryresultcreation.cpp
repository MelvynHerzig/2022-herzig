#include "test_xpertqueryresultcreation.h"

using namespace std;
using namespace Tucuxi;


void TestXpertQueryResultCreation::xpertQueryResultCreation_takesXpertQueryDataOwnership(const string& _testName)
{

    string xmlString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">

                                        <date>2018-07-11T13:45:30</date>

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
                                                <drugId>rifampicin</drugId>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                            </xpertRequest>
                                            <xpertRequest>
                                                <drugId>imatinib</drugId>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <adjustmentDate>2018-08-06T08:00:00</adjustmentDate>
                                            </xpertRequest>
                                        </requests>
                                    </query>
                                    )";

    cout << _testName << endl;

    unique_ptr<Xpert::XpertQueryData> query = nullptr;

    Xpert::XpertQueryImport importer;
    Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

    // Must not be nullptr.
    fructose_assert_ne(query.get(), nullptr);

    Xpert::XpertQueryResult xpertQueryResult(move(query), "");

    fructose_assert_eq(importResult, Xpert::XpertQueryImport::Status::Ok);
    fructose_assert_eq(query.get(), nullptr);
}

void TestXpertQueryResultCreation::getAdminDataOfXpertQueyResult_returnsCorrectValues_withOrWithoutAdminInQuery(const string& _testName)
{

    string emptyAdminString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                            <query version="1.0"
                                                xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                                xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">

                                                <date>2018-07-11T13:45:30</date>

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

    string fullAdminString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
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
                                                        <clinicalData key="goodNote"> nice </clinicalData>
                                                        <clinicalData key="badNote"> <yet>random note</yet> </clinicalData>
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
                                                    </xpertRequest>
                                                </requests>
                                            </query>
                                            )";

    cout << _testName << endl;

    // Import the queries

    unique_ptr<Xpert::XpertQueryData> queryEmptyAdmin = nullptr;
    unique_ptr<Xpert::XpertQueryData> queryCompleteAdmin = nullptr;

    Xpert::XpertQueryImport importer;
    Xpert::XpertQueryImport::Status importResultEmptyAdmin = importer.importFromString(queryEmptyAdmin, emptyAdminString);
    Xpert::XpertQueryImport::Status importResultFullAdmin = importer.importFromString(queryCompleteAdmin, fullAdminString);

    // Execute

    Xpert::XpertQueryResult xpertQueryResultWithEmptyAdmin(move(queryEmptyAdmin), "");
    Xpert::XpertQueryResult xpertQueryResultWithFullAdmin(move(queryCompleteAdmin), "");

    // Compare

    fructose_assert_eq(importResultEmptyAdmin, Xpert::XpertQueryImport::Status::Ok);
    fructose_assert_eq(importResultFullAdmin, Xpert::XpertQueryImport::Status::Ok);

    fructose_assert_eq(xpertQueryResultWithEmptyAdmin.getAdminData().get(), nullptr);
    fructose_assert_ne(xpertQueryResultWithFullAdmin.getAdminData().get(), nullptr);

    const Xpert::PersonData& mandator = xpertQueryResultWithFullAdmin.getAdminData()->getMandator()->getPerson();
    const Xpert::AddressData& mandatorAddress = *mandator.getAddress();
    const Xpert::PhoneData& mandatorPhone = *mandator.getPhone();
    const Xpert::EmailData& mandatorEmail = *mandator.getEmail();
    const Xpert::InstituteData& mandatorInstitute = *xpertQueryResultWithFullAdmin.getAdminData()->getMandator()->getInstitute();
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

    const Xpert::PersonData& patient =  xpertQueryResultWithFullAdmin.getAdminData()->getPatient()->getPerson();
    const Xpert::AddressData& patientAddress = *patient.getAddress();
    const Xpert::PhoneData& patientPhone = *patient.getPhone();
    const Xpert::EmailData& patientEmail = *patient.getEmail();
    const Xpert::InstituteData& patientInstitute = *xpertQueryResultWithFullAdmin.getAdminData()->getPatient()->getInstitute();
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

    fructose_assert_eq(xpertQueryResultWithFullAdmin.getAdminData()->getClinicalDatas()->getData().find("goodNote")->second, " nice ");
    fructose_assert_eq(xpertQueryResultWithFullAdmin.getAdminData()->getClinicalDatas()->getData().find("badNote")->second, "");
}

void TestXpertQueryResultCreation::getXpertRequestResultsOfXpertQueryResult_returnsCorrectValues_withValidAndInvalidXpertRequest(const string& _testName)
{

    string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                            <query version="1.0"
                                                xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                                xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">

                                                <date>2018-07-11T13:45:30</date>

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
                                                    <xpertRequest>
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
                                                    </xpertRequest>
                                                </requests>
                                            </query>
                                            )";

    cout << _testName << endl;

    // Import the query
    unique_ptr<Xpert::XpertQueryData> query = nullptr;

    Xpert::XpertQueryImport importer;
    Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, queryString);

    // Execute
    Xpert::XpertQueryResult xpertQueryResult(move(query), "");

    // Compare
    fructose_assert_eq(importResult, Xpert::XpertQueryImport::Status::Ok);

    fructose_assert_eq(xpertQueryResult.getXpertRequestResults().size(), 2);

    fructose_assert_eq(xpertQueryResult.getXpertRequestResults()[0].getDrugModel(), nullptr);
    fructose_assert_eq(xpertQueryResult.getXpertRequestResults()[0].getErrorMessage(), "");
    fructose_assert_ne(xpertQueryResult.getXpertRequestResults()[0].getTreatment().get(), nullptr);
    fructose_assert_eq(xpertQueryResult.getXpertRequestResults()[0].getXpertRequest().getDrugId(), "rifampicin");
    fructose_assert_eq(xpertQueryResult.getXpertRequestResults()[0].shouldContinueProcessing(), true);

    fructose_assert_eq(xpertQueryResult.getXpertRequestResults()[1].getDrugModel(), nullptr);
    fructose_assert_eq(xpertQueryResult.getXpertRequestResults()[1].getErrorMessage(), "No drug matching. Could not extract drug treatment.");
    fructose_assert_eq(xpertQueryResult.getXpertRequestResults()[1].getTreatment().get(), nullptr);
    fructose_assert_eq(xpertQueryResult.getXpertRequestResults()[1].getXpertRequest().getDrugId(), "imatinib");
    fructose_assert_eq(xpertQueryResult.getXpertRequestResults()[1].shouldContinueProcessing(), false);
}

void TestXpertQueryResultCreation::getXpertRequestDataOfXpertRequestResult_returnsCorrectValues_withFullAndMinimalXpertRequest(const string& _testName)
{

    string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                            <query version="1.0"
                                                xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                                xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">

                                                <date>2018-07-11T13:45:30</date>

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
                                                            <formulationAndRouteSelectionOption>defaultFormulationAndRoute</formulationAndRouteSelectionOption>
                                                        </options>
                                                    </xpertRequest>
                                                    <xpertRequest>
                                                        <drugId>imatinib</drugId>
                                                        <output>
                                                            <format>html</format>
                                                            <language>fr</language>
                                                        </output>
                                                    </xpertRequest>
                                                </requests>
                                            </query>
                                            )";

    cout << _testName << endl;

    // Import the query
    unique_ptr<Xpert::XpertQueryData> query = nullptr;

    Xpert::XpertQueryImport importer;
    Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, queryString);

    // Execute
    Xpert::XpertQueryResult xpertQueryResult(move(query), "");

    // Compare
    fructose_assert_eq(importResult, Xpert::XpertQueryImport::Status::Ok);

    const Xpert::XpertRequestData& completeXpertRequest = xpertQueryResult.getXpertRequestResults()[0].getXpertRequest();
    const Xpert::XpertRequestData& minimalXpertRequest = xpertQueryResult.getXpertRequestResults()[1].getXpertRequest();

    fructose_assert_eq(completeXpertRequest.getDrugId(), "rifampicin");
    fructose_assert_eq(completeXpertRequest.getOutputLang() == Xpert::OutputLang::ENGLISH, true);
    fructose_assert_eq(completeXpertRequest.getOutputFormat() == Xpert::OutputFormat::XML, true);
    fructose_assert_eq(completeXpertRequest.getAdjustmentTime(), Common::DateTime("2018-07-06T08:00:00", TestUtils::date_format));
    fructose_assert_eq(completeXpertRequest.getLoadingOption() == Xpert::LoadingOption::NoLoadingDose, true);
    fructose_assert_eq(completeXpertRequest.getRestPeriodOption() == Xpert::RestPeriodOption::NoRestPeriod, true);
    fructose_assert_eq(completeXpertRequest.getTargetExtractionOption() == Core::TargetExtractionOption::PopulationValues, true);
    fructose_assert_eq(completeXpertRequest.getFormulationAndRouteSelectionOption() == Core::FormulationAndRouteSelectionOption::DefaultFormulationAndRoute, true);


    fructose_assert_eq(minimalXpertRequest.getDrugId(), "imatinib");
    fructose_assert_eq(minimalXpertRequest.getOutputLang() == Xpert::OutputLang::FRENCH, true);
    fructose_assert_eq(minimalXpertRequest.getOutputFormat() == Xpert::OutputFormat::HTML, true);
    fructose_assert_eq(minimalXpertRequest.getAdjustmentTime().isUndefined(), true);
    fructose_assert_eq(minimalXpertRequest.getLoadingOption() == Xpert::LoadingOption::Unspecified, true);
    fructose_assert_eq(minimalXpertRequest.getRestPeriodOption() == Xpert::RestPeriodOption::Unspecified, true);
    fructose_assert_eq(minimalXpertRequest.getTargetExtractionOption() == Core::TargetExtractionOption::DefinitionIfNoIndividualTarget, true);
    fructose_assert_eq(minimalXpertRequest.getFormulationAndRouteSelectionOption() == Core::FormulationAndRouteSelectionOption::LastFormulationAndRoute, true);
}

void TestXpertQueryResultCreation::getComputationTimeOfXpertRequestResult_returnsCorrectDateTime(const string& _testName)
{

    string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                     <query version="1.0"
                                         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                         xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">

                                         <date>2018-07-11T13:45:30</date>

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

    cout << _testName << endl;

    // Import the query
    unique_ptr<Xpert::XpertQueryData> query = nullptr;

    Xpert::XpertQueryImport importer;
    Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, queryString);

    // Execute
    Xpert::XpertQueryResult xpertQueryResult(move(query), "");

    // Compare
    fructose_assert_eq(importResult, Xpert::XpertQueryImport::Status::Ok);
    fructose_assert_eq(xpertQueryResult.getComputationTime(), Common::DateTime("2018-07-11T13:45:30", TestUtils::date_format));

}
