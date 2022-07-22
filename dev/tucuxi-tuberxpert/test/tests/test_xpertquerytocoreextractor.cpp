#include "test_xpertquerytocoreextractor.h"

using namespace std;
using namespace Tucuxi;


void TestXpertQueryToCoreExtractor::extractDrugTreatment_success_withDrugElementsPresentOnce(const string& _testName)
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
                                            </xpertRequest>
                                        </requests>
                                    </query>
                                    )";

    cout << _testName << endl;

    // Extract the treatments
    unique_ptr<Xpert::XpertQueryData> query = nullptr;

    Xpert::XpertQueryImport importer;
    Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

    Xpert::XpertQueryToCoreExtractor extractor;

    string errorMessage0;
    unique_ptr<Core::DrugTreatment> drugTreatment0 = extractor.extractDrugTreatment(
                query->getXpertRequests()[0],
            *query,
            errorMessage0);

    string errorMessage1;
    unique_ptr<Core::DrugTreatment> drugTreatment1 = extractor.extractDrugTreatment(
                query->getXpertRequests()[1],
            *query,
            errorMessage1);

    // Compare
    fructose_assert_eq(importResult, Xpert::XpertQueryImport::Status::Ok);
    fructose_assert_eq(errorMessage0, "");
    fructose_assert_eq(errorMessage1, "");
    fructose_assert_ne(drugTreatment0.get(), nullptr);
    fructose_assert_ne(drugTreatment1.get(), nullptr);
}

void TestXpertQueryToCoreExtractor::extractDrugTreatment_failure_withMultipleOrNoDrugElements(const string& _testName)
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
                                            <xpertRequest>
                                                <drugId>imatinib</drugId>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                            </xpertRequest>
                                        </requests>
                                    </query>
                                    )";

    cout << _testName << endl;

    // Extract the treatments
    unique_ptr<Xpert::XpertQueryData> query = nullptr;

    Xpert::XpertQueryImport importer;
    Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

    Xpert::XpertQueryToCoreExtractor extractor;

    string errorMessage0;
    unique_ptr<Core::DrugTreatment> drugTreatment0 = extractor.extractDrugTreatment(
                query->getXpertRequests()[0],
            *query,
            errorMessage0);

    string errorMessage1;
    unique_ptr<Core::DrugTreatment> drugTreatment1 = extractor.extractDrugTreatment(
                query->getXpertRequests()[1],
            *query,
            errorMessage1);


    // Compare
    fructose_assert_eq(importResult, Xpert::XpertQueryImport::Status::Ok);
    fructose_assert_eq(errorMessage0, "Too many drugs matching. Could not extract drug treatment.");
    fructose_assert_eq(errorMessage1, "No drug matching. Could not extract drug treatment.");
    fructose_assert_eq(drugTreatment0.get(), nullptr);
    fructose_assert_eq(drugTreatment1.get(), nullptr);
}
