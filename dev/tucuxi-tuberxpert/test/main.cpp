#include <iostream>

#if defined(test_language)
#include "test_languagemanager.h"
#endif

#if defined(test_query)
#include "test_xpertqueryimport.h"
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

    res = languageTests.run(argc, argv);
    if (res != 0) {
        printf("Language module test failed\n");
        exit(1);
    }
    else {
        std::cout << "Language module test succeeded\n";
    }
#endif

#if defined(test_query)
    TestXpertQueryImport QueryImportTests;

    QueryImportTests.add_test("Get complete admin element", &TestXpertQueryImport::retrieveCompleteAdmin);
    QueryImportTests.add_test("Get no admin element", &TestXpertQueryImport::retrieveNoAdmin);
    QueryImportTests.add_test("Get empty admin element", &TestXpertQueryImport::retrieveEmptyAdmin);
    QueryImportTests.add_test("Get minimal persons", &TestXpertQueryImport::retrieveMinimalPerson);
    QueryImportTests.add_test("Get minimal institutes", &TestXpertQueryImport::retrieveMinimalInstitute);
    QueryImportTests.add_test("Get minimal coordinates", &TestXpertQueryImport::retrieveMinimalCoordinates);
    QueryImportTests.add_test("Get an error when missing mandatory values in mandator person", &TestXpertQueryImport::errorWhenMissingMandatoryInMandatorPerson);
    QueryImportTests.add_test("Get an error when missing mandatory values in mandator institute", &TestXpertQueryImport::errorWhenMissingMandatoryInMandatorInstitute);
    QueryImportTests.add_test("Get an error when missing mandatory values in patient person", &TestXpertQueryImport::errorWhenMissingMandatoryInPatientPerson);
    QueryImportTests.add_test("Get an error when missing mandatory values in patient institute", &TestXpertQueryImport::errorWhenMissingMandatoryInPatientInstitute);
    QueryImportTests.add_test("Get complete request xpert element", &TestXpertQueryImport::retrieveCompleteRequestXpert);
    QueryImportTests.add_test("Get default request xpert element", &TestXpertQueryImport::retrieveDefaultRequestXpert);
    QueryImportTests.add_test("Get an error when missing request xpert", &TestXpertQueryImport::errorWhenNoRequestXpert);
    QueryImportTests.add_test("Get an error when missing mandatory values in request xpert", &TestXpertQueryImport::errorWhenMissingMandatoryRequestXpert);
    QueryImportTests.add_test("Get an error when the xml document can't be created from string", &TestXpertQueryImport::errorWhenCreatingXmlDocumentFromString);
    QueryImportTests.add_test("Get an error when the xml document can't be created from file", &TestXpertQueryImport::errorWhenCreatingXmlDocumentFromFile);

    res = QueryImportTests.run(argc, argv);
    if (res != 0) {
        printf("Query import module test failed\n");
        exit(1);
    }
    else {
        std::cout << "Query import module test succeeded\n";
    }
#endif

    return 0;
}
