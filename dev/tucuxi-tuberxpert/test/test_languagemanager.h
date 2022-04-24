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

    /// \brief Checks that getInstance behave correctly.
    /// \param _testName Name of the test
    void retrieveDictionary(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        // Test constructors
        // bad_file.xml doesn't exist, but test.xml does.
        // Must be executed in that order. Otherwhise "bad_file" is ignored because "test" already loaded.
        fructose_assert_exception(Tucuxi::Language::LanguageManager::getInstance("bad_file"), Tucuxi::Language::LanguageException)
        fructose_assert_no_exception(Tucuxi::Language::LanguageManager::getInstance("test"))
    }

    /// \brief Checks that translate behave correctly.
    /// \param _testName Name of the test
    void wordTranslation(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

         std::unique_ptr<Tucuxi::Language::LanguageManager>& upLangMgr = Tucuxi::Language::LanguageManager::getInstance("fr");

        // Test translate
        // world key exists and return World, but "unknown key" is not part of test.xml
        fructose_assert_eq(upLangMgr->translate("world"), "World");
        fructose_assert_eq(upLangMgr->translate("unknown key"), upLangMgr->defaultTranslation);
    }
};

#endif // TEST_LANGUAGEMANAGER_H
