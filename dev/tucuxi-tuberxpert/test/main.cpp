#include <iostream>

#if defined(test_language)
#include "test_language.h"
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
    TestLanguage languageTests;

    languageTests.add_test("GetInstance", &TestLanguage::retrieveDictionary);
    languageTests.add_test("translate", &TestLanguage::wordTranslation);

    res = languageTests.run(argc, argv);
    if (res != 0) {
        printf("Language module test failed\n");
        exit(1);
    }
    else {
        std::cout << "Language module test succeeded\n";
    }
#endif

    return 0;
}
