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
    void loadTranslations_behavesCorrectly(const std::string& _testName);

    /// \brief Checks that translate method behaves correctly.
    ///        If the key is known, the translation is returned.
    ///        If the key is unknown, the default translation is returned.
    /// \param _testName Name of the test
    void translate_behavesCorrectly(const std::string& _testName);
};

#endif // TEST_LANGUAGEMANAGER_H
