#include <iostream>

#if defined(test_language)
#include "test_languagemanager.h"
#endif

#if defined(test_query)
#include "test_xpertqueryimport.h"
#endif

#if defined(test_xpertquerytocoreextractor)
#include "test_xpertquerytocoreextractor.h"
#endif

#if defined(test_xpertresultcreation)
#include "test_xpertresultcreation.h"
#endif

using namespace std;

/// \brief Testing program.
/// \date 21/04/2022
/// \author Herzig Melvyn
int main(int argc, char** argv)
{

    int res;

    /***********************************************************
     *                        Language                         *
     ***********************************************************/

#if defined(test_language)
    TestLanguageManager languageTests;

    languageTests.add_test("GetInstance", &TestLanguageManager::retrieveDictionary);
    languageTests.add_test("translate", &TestLanguageManager::wordTranslation);
    languageTests.add_test("Compute language file name", &TestLanguageManager::computeLanguageFileName);

    res = languageTests.run(argc, argv);
    if (res != 0) {
        std::cout << "Language module tests failed" << std::endl;
        exit(1);
    }
    else {
        std::cout << "Language module tests succeeded" << std::endl;
    }
#endif

    /***********************************************************
     *                    XpertQueryImport                     *
     ***********************************************************/


#if defined(test_query)
    TestXpertQueryImport queryImportTests;

    queryImportTests.add_test("Get complete admin element", &TestXpertQueryImport::retrieveCompleteAdmin);
    queryImportTests.add_test("Get no admin element", &TestXpertQueryImport::retrieveNoAdmin);
    queryImportTests.add_test("Get empty admin element", &TestXpertQueryImport::retrieveEmptyAdmin);
    queryImportTests.add_test("Get minimal persons", &TestXpertQueryImport::retrieveMinimalPerson);
    queryImportTests.add_test("Get minimal institutes", &TestXpertQueryImport::retrieveMinimalInstitute);
    queryImportTests.add_test("Get minimal coordinates", &TestXpertQueryImport::retrieveMinimalCoordinates);
    queryImportTests.add_test("Get an error when missing mandatory values in mandator person", &TestXpertQueryImport::errorWhenMissingMandatoryInMandatorPerson);
    queryImportTests.add_test("Get an error when missing mandatory values in mandator institute", &TestXpertQueryImport::errorWhenMissingMandatoryInMandatorInstitute);
    queryImportTests.add_test("Get an error when missing mandatory values in patient person", &TestXpertQueryImport::errorWhenMissingMandatoryInPatientPerson);
    queryImportTests.add_test("Get an error when missing mandatory values in patient institute", &TestXpertQueryImport::errorWhenMissingMandatoryInPatientInstitute);
    queryImportTests.add_test("Get complete request xpert element", &TestXpertQueryImport::retrieveCompleteRequestXpert);
    queryImportTests.add_test("Get default request xpert element", &TestXpertQueryImport::retrieveDefaultRequestXpert);
    queryImportTests.add_test("Get an error when missing request xpert", &TestXpertQueryImport::errorWhenNoRequestXpert);
    queryImportTests.add_test("Get an error when missing mandatory values in request xpert", &TestXpertQueryImport::errorWhenMissingMandatoryRequestXpert);
    queryImportTests.add_test("Get an error when the xml document can't be created from string", &TestXpertQueryImport::errorWhenCreatingXmlDocumentFromString);
    queryImportTests.add_test("Get an error when the xml document can't be created from file", &TestXpertQueryImport::errorWhenCreatingXmlDocumentFromFile);

    res = queryImportTests.run(argc, argv);
    if (res != 0) {
        std::cout << "Query import module tests failed" << std::endl;
        exit(1);
    }
    else {
        std::cout << "Query import module tests succeeded" << std::endl;
    }
#endif


    /***********************************************************
     *                XpertQueryToCoreExtractor                *
     ***********************************************************/

#if defined(test_xpertquerytocoreextractor)
    TestXpertQueryToCoreExtractor queryToCoreExtractorTests;

    queryToCoreExtractorTests.add_test("Extract good drug treatment", &TestXpertQueryToCoreExtractor::extractGoodDrugTreatment);
    queryToCoreExtractorTests.add_test("Extract bad drug treatment", &TestXpertQueryToCoreExtractor::extractBadDrugTreatment);

    res = queryToCoreExtractorTests.run(argc, argv);
    if (res != 0) {
        std::cout << "Xpert query to core extractor tests failed" << std::endl;
        exit(1);
    }
    else {
        std::cout << "Xpert query to core extractor tests succeeded" << std::endl;
    }
#endif


    /***********************************************************
     *                  XpertResult creation                   *
     ***********************************************************/

#if defined(test_xpertresultcreation)
    TestXpertResultCreation xpertResultCreationTests;

    xpertResultCreationTests.add_test("Query pointer invalidation", &TestXpertResultCreation::queryPtrInvalidation);
    xpertResultCreationTests.add_test("Retrieve administrative data", &TestXpertResultCreation::retrieveAdministrativeData);
    xpertResultCreationTests.add_test("Retrieve xpert request results", &TestXpertResultCreation::retrieveXpertRequestResult);
    xpertResultCreationTests.add_test("Retrieve generation date", &TestXpertResultCreation::retrieveDate);

    res = xpertResultCreationTests.run(argc, argv);
    if (res != 0) {
        std::cout << "XpertResult creation tests failed" << std::endl;
        exit(1);
    }
    else {
        std::cout << "XpertResult creation tests succeeded\n";
    }
#endif

    return 0;
}
