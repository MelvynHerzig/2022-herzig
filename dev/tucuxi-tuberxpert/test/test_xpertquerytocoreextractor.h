#ifndef TEST_XPERTQUERYTOCOREEXTRACTOR_H
#define TEST_XPERTQUERYTOCOREEXTRACTOR_H

#include <string>
#include <memory>

#include "fructose/fructose.h"

#include "tucucore/drugtreatment/drugtreatment.h"

#include "tuberxpert/query/xpertquerydata.h"
#include "tuberxpert/query/xpertqueryimport.h"
#include "tuberxpert/query/xpertquerytocoreextractor.h"
#include "tuberxpert/language/languagemanager.h"

/// \brief Tests for XpertQueryToCoreExtractor.
///        This struct tests only the methods created by XpertQueryToCoreExtractor.
///        Therefore, it doesn't test the inherited methods. Here, to goal is to check
///        that the returned drugTreatment is not null and that the error message is empty string.
/// \date 25/05/2022
/// \author Herzig Melvyn
struct TestXpertQueryToCoreExtractor : public fructose::test_base<TestXpertQueryToCoreExtractor>
{

    /// \brief Check that the drug treatment is normally extracted for
    ///        the xpertRequest that requires a valid drug. A drug is valid
    ///        if it appears only once.
    ///        There are two xpertRequests.
    ///        One for imatinib and one for rifampicin. There is exactly
    ///        one drug element for each of these drugs. The extraction
    ///        must succeed without any error and the treatment pointers are not nullptr.
    /// \param _testName Name of the test
    void extractDrugTreatment_success_withDrugElementsPresentOnce(const std::string& _testName)
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

        std::cout << _testName << std::endl;

        // Extract the treatments
        std::unique_ptr<Tucuxi::Xpert::XpertQueryData> query = nullptr;

        Tucuxi::Xpert::XpertQueryImport importer;
        Tucuxi::Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        Tucuxi::Xpert::XpertQueryToCoreExtractor extractor;

        std::string errorMessage0;
        std::unique_ptr<Tucuxi::Core::DrugTreatment> drugTreatment0 = extractor.extractDrugTreatment(
                            query->getXpertRequests()[0],
                            *query,
                            errorMessage0);

        std::string errorMessage1;
        std::unique_ptr<Tucuxi::Core::DrugTreatment> drugTreatment1 = extractor.extractDrugTreatment(
                            query->getXpertRequests()[1],
                            *query,
                            errorMessage1);

        // Compare
        fructose_assert_eq(importResult, Tucuxi::Xpert::XpertQueryImport::Status::Ok);
        fructose_assert_eq(errorMessage0, "");
        fructose_assert_eq(errorMessage1, "");
        fructose_assert_ne(drugTreatment0.get(), nullptr);
        fructose_assert_ne(drugTreatment1.get(), nullptr);
    }

    /// \brief When there is no drug or multiple drugs that match a requested drug, it checks
    ///        that an error message is returned and that the drugTreatment pointers are nullptr.
    ///        There are two xpertRequests.
    ///        One for rifampicin and one for imatinib.
    ///        In this case, rifampicin is present twice and imatinib none.
    /// \param _testName Name of the test
    void extractDrugTreatment_failure_withMultipleOrNoDrugElements(const std::string& _testName)
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

        std::cout << _testName << std::endl;

        // Extract the treatments
        std::unique_ptr<Tucuxi::Xpert::XpertQueryData> query = nullptr;

        Tucuxi::Xpert::XpertQueryImport importer;
        Tucuxi::Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        Tucuxi::Xpert::XpertQueryToCoreExtractor extractor;

        std::string errorMessage0;
        std::unique_ptr<Tucuxi::Core::DrugTreatment> drugTreatment0 = extractor.extractDrugTreatment(
                            query->getXpertRequests()[0],
                            *query,
                            errorMessage0);

        std::string errorMessage1;
        std::unique_ptr<Tucuxi::Core::DrugTreatment> drugTreatment1 = extractor.extractDrugTreatment(
                            query->getXpertRequests()[1],
                            *query,
                            errorMessage1);


        // Compare
        fructose_assert_eq(importResult, Tucuxi::Xpert::XpertQueryImport::Status::Ok);
        fructose_assert_eq(errorMessage0, "Too many drugs matching. Could not extract drug treatment.");
        fructose_assert_eq(errorMessage1, "No drug matching. Could not extract drug treatment.");
        fructose_assert_eq(drugTreatment0.get(), nullptr);
        fructose_assert_eq(drugTreatment1.get(), nullptr);
    }
};

#endif // TEST_XPERTQUERYTOCOREEXTRACTOR_H
