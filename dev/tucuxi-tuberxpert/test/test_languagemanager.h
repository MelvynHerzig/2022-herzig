#ifndef TEST_LANGUAGEMANAGER_H
#define TEST_LANGUAGEMANAGER_H

#include <memory>

#include "tuberxpert/language/languagemanager.h"
#include "tuberxpert/language/languageexception.h"
#include "fructose/fructose.h"

/// \brief Tests for LanguageManager.
/// \date 21/04/2022
/// \author Herzig Melvyn
struct TestLanguageManager : public fructose::test_base<TestLanguageManager>
{

    /// \brief Check that the loadTranslations method behaves as expected.
    ///        If the xml is not well formatted, the loading must throw a languageException.
    /// \param _testName Name of the test
    void loadTranslations_behavesCorrectly(const std::string& _testName)
    {

        std::string goodString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <translations
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="translations_file.xsd">

                                        <translation key="hello">Hello</translation>
                                        <translation key="world">World</translation>

                                    </translations>)";


        std::string missSpelledTranslationsString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                                       <translations
                                                           xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                                           xsi:noNamespaceSchemaLocation="translations_file.xsd">

                                                           <translation key="hello">Hello</translation>
                                                           <abc key="world">World</abc>

                                                       </translations>)";

        std::string noKeyAttributeString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                              <translations
                                                  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                                  xsi:noNamespaceSchemaLocation="translations_file.xsd">

                                                  <translation>Hello</translation>

                                              </translations>)";

        std::string badKeyAttributeString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                               <translations
                                                   xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                                   xsi:noNamespaceSchemaLocation="translations_file.xsd">

                                                   <translation yek="hello">Hello</translation>

                                               </translations>)";

        std::string nestedElement = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                       <translations
                                           xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                           xsi:noNamespaceSchemaLocation="translations_file.xsd">

                                           <translation yek="hello">Hello</translation>
                                           <translation key="nested">
                                               <nested> a value </nested>
                                           </translation>

                                       </translations>)";


        std::cout << _testName << std::endl;

        Tucuxi::Xpert::LanguageManager& lm = Tucuxi::Xpert::LanguageManager::getInstance();

        fructose_assert_exception(lm.loadTranslations(""), Tucuxi::Xpert::LanguageException);
        fructose_assert_exception(lm.loadTranslations(missSpelledTranslationsString), Tucuxi::Xpert::LanguageException);
        fructose_assert_exception(lm.loadTranslations(noKeyAttributeString), Tucuxi::Xpert::LanguageException);
        fructose_assert_exception(lm.loadTranslations(badKeyAttributeString), Tucuxi::Xpert::LanguageException);
        fructose_assert_exception(lm.loadTranslations(nestedElement), Tucuxi::Xpert::LanguageException);
        fructose_assert_no_exception(lm.loadTranslations(goodString));
    }

    /// \brief Checks that translate method behaves correctly.
    ///        If the key is known, the translation is returned.
    ///        If the key is unknown, the default translation is returned.
    /// \param _testName Name of the test
    void translate_behavesCorrectly(const std::string& _testName)
    {

        std::string goodString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <translations
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="translations_file.xsd">

                                        <translation key="hello">Hello</translation>
                                        <translation key="world">World</translation>

                                    </translations>)";

        std::cout << _testName << std::endl;

        Tucuxi::Xpert::LanguageManager& lm = Tucuxi::Xpert::LanguageManager::getInstance();
        lm.loadTranslations(goodString);

        // Test translate
        // world key exists and return World, but "unknown key" is not part of test.xml
        fructose_assert_eq(lm.translate("world"), "World");
        fructose_assert_eq(lm.translate("unknown key"), lm.s_defaultTranslation);
    }
};

#endif // TEST_LANGUAGEMANAGER_H
