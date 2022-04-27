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

    QueryImportTests.add_test("Get complete admin", &TestXpertQueryImport::retrieveCompleteAdmin);

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
