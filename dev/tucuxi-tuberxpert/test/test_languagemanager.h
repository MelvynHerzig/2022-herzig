#ifndef TEST_LANGUAGEMANAGER_H
#define TEST_LANGUAGEMANAGER_H

#include <memory>

#include "../src/language/languagemanager.h"
#include "../src/language/languageexception.h"
#include "fructose/fructose.h"

/// \brief Tests for LanguageManager
/// \date 21/04/2022
/// \author Herzig Melvyn
struct TestLanguageManager : public fructose::test_base<TestLanguageManager>
{

    /// \brief Checks that the loadDictionary method behave as expected.
    /// \param _testName Name of the test
    void retrieveDictionary(const std::string& _testName)
    {

        std::string goodString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <dictionary
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="dictionary.xsd">

                                        <entry key="hello">Hello</entry>
                                        <entry key="world">World</entry>

                                    </dictionary>)";


        std::string missSpelledEntryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <dictionary
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="dictionary.xsd">

                                        <entry key="hello">Hello</entry>
                                        <abc key="world">World</abc>

                                    </dictionary>)";

        std::string noAttributeString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <dictionary
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="dictionary.xsd">

                                        <entry>Hello</entry>

                                    </dictionary>)";

        std::string badAttributeString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <dictionary
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="dictionary.xsd">

                                        <entry yek="hello">Hello</entry>

                                    </dictionary>)";

        std::string nestedElement = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <dictionary
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="dictionary.xsd">

                                        <entry yek="hello">Hello</entry>
                                        <entry key="nested">
                                            <nested> a value </nested>
                                        </entry>

                                    </dictionary>)";


        std::cout << _testName << std::endl;

        Tucuxi::XpertLanguage::LanguageManager& lm = Tucuxi::XpertLanguage::LanguageManager::getInstance();

        fructose_assert_exception(lm.loadDictionary(""), Tucuxi::XpertLanguage::LanguageException);
        fructose_assert_exception(lm.loadDictionary(missSpelledEntryString), Tucuxi::XpertLanguage::LanguageException);
        fructose_assert_exception(lm.loadDictionary(noAttributeString), Tucuxi::XpertLanguage::LanguageException);
        fructose_assert_exception(lm.loadDictionary(badAttributeString), Tucuxi::XpertLanguage::LanguageException);
        fructose_assert_exception(lm.loadDictionary(nestedElement), Tucuxi::XpertLanguage::LanguageException);
        fructose_assert_no_exception(lm.loadDictionary(goodString));
    }

    /// \brief Checks that translate behave correctly.
    /// \param _testName Name of the test
    void wordTranslation(const std::string& _testName)
    {

        std::string goodString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <dictionary
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="dictionary.xsd">

                                        <entry key="hello">Hello</entry>
                                        <entry key="world">World</entry>

                                    </dictionary>)";

        std::cout << _testName << std::endl;

        Tucuxi::XpertLanguage::LanguageManager& lm = Tucuxi::XpertLanguage::LanguageManager::getInstance();
        lm.loadDictionary(goodString);

        // Test translate
        // world key exists and return World, but "unknown key" is not part of test.xml
        fructose_assert_eq(lm.translate("world"), "World");
        fructose_assert_eq(lm.translate("unknown key"), lm.s_defaultTranslation);
    }

    /// \brief Checks the filename returned by computeLanguageFileName
    /// \param _testName Name of the test
    void computeLanguageFileName(const std::string& _testName)
    {

        std::cout << _testName << std::endl;

        Tucuxi::XpertLanguage::LanguageManager& lm = Tucuxi::XpertLanguage::LanguageManager::getInstance();

        // Test translate
        // world key exists and return World, but "unknown key" is not part of test.xml
        fructose_assert_eq(lm.computeLanguageFileName(Tucuxi::XpertQuery::OutputLang::ENGLISH), "en.xml");
        fructose_assert_eq(lm.computeLanguageFileName(Tucuxi::XpertQuery::OutputLang::FRENCH), "fr.xml");
        fructose_assert_exception(lm.computeLanguageFileName(Tucuxi::XpertQuery::OutputLang(-1)), Tucuxi::XpertLanguage::LanguageException);
    }
};

#endif // TEST_LANGUAGEMANAGER_H
