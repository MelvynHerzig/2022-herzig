#include "test_languagemanager.h"

using namespace std;
using namespace Tucuxi;

void TestLanguageManager::loadTranslations_behavesCorrectly(const string& _testName)
{

    string goodString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <translations
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="translations_file.xsd">

                                        <translation key="hello">Hello</translation>
                                        <translation key="world">World</translation>

                                    </translations>)";


    string missSpelledTranslationsString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                                       <translations
                                                           xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                                           xsi:noNamespaceSchemaLocation="translations_file.xsd">

                                                           <translation key="hello">Hello</translation>
                                                           <abc key="world">World</abc>

                                                       </translations>)";

    string noKeyAttributeString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                              <translations
                                                  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                                  xsi:noNamespaceSchemaLocation="translations_file.xsd">

                                                  <translation>Hello</translation>

                                              </translations>)";

    string badKeyAttributeString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                               <translations
                                                   xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                                   xsi:noNamespaceSchemaLocation="translations_file.xsd">

                                                   <translation yek="hello">Hello</translation>

                                               </translations>)";

    string nestedElement = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                       <translations
                                           xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                           xsi:noNamespaceSchemaLocation="translations_file.xsd">

                                           <translation yek="hello">Hello</translation>
                                           <translation key="nested">
                                               <nested> a value </nested>
                                           </translation>

                                       </translations>)";


    cout << _testName << endl;

    Xpert::LanguageManager& lm = Xpert::LanguageManager::getInstance();

    fructose_assert_exception(lm.loadTranslations(""), Xpert::LanguageException);
    fructose_assert_exception(lm.loadTranslations(missSpelledTranslationsString), Xpert::LanguageException);
    fructose_assert_exception(lm.loadTranslations(noKeyAttributeString), Xpert::LanguageException);
    fructose_assert_exception(lm.loadTranslations(badKeyAttributeString), Xpert::LanguageException);
    fructose_assert_exception(lm.loadTranslations(nestedElement), Xpert::LanguageException);
    fructose_assert_no_exception(lm.loadTranslations(goodString));
}

void TestLanguageManager::translate_behavesCorrectly(const string& _testName)
{

    string goodString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <translations
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="translations_file.xsd">

                                        <translation key="hello">Hello</translation>
                                        <translation key="world">World</translation>

                                    </translations>)";

    cout << _testName << endl;

    Xpert::LanguageManager& lm = Xpert::LanguageManager::getInstance();
    lm.loadTranslations(goodString);

    // Test translate
    // world key exists and return World, but "unknown key" is not part of test.xml
    fructose_assert_eq(lm.translate("world"), "World");
    fructose_assert_eq(lm.translate("unknown key"), lm.s_defaultTranslation);
}
