#ifndef TEST_XPERTUTILS_H
#define TEST_XPERTUTILS_H

#include "../src/language/languageexception.h"
#include "../src/utils/xpertutils.h"

#include "fructose/fructose.h"

/// \brief Tests for utility methods of TuberXpert.
/// \date 03/06/2022
/// \author Herzig Melvyn
struct TestXpertUtils : public fructose::test_base<TestXpertUtils>
{

    /// \brief Converts output lang to string. Checks that the requested language get its corresponding
    ///        string or an exception if it is not supported.
    /// \param _testName Name of the test.
    void convertOutputLangToString(const std::string& _testName)
    {

        std::cout << _testName << std::endl;

        fructose_assert_eq(Tucuxi::XpertUtils::outputLangToString(Tucuxi::XpertQuery::OutputLang::ENGLISH), "en");
        fructose_assert_eq(Tucuxi::XpertUtils::outputLangToString(Tucuxi::XpertQuery::OutputLang::FRENCH), "fr");
        fructose_assert_exception(Tucuxi::XpertUtils::outputLangToString(Tucuxi::XpertQuery::OutputLang(-1)), Tucuxi::XpertLanguage::LanguageException);
    }


    /// \brief Converts double to string. Checks that the string contains two decimals.
    /// \param _testName Name of the test.
    void convertDoubleToString(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        fructose_assert_eq(Tucuxi::XpertUtils::varToString(5.411111), "5.41");
        fructose_assert_eq(Tucuxi::XpertUtils::varToString(6), "6.00");
    }

    /// \brief Tests that the getStringWithEnglishFallback returns the targeted language or
    ///        the default language (en) or empty string.
    /// \param _testName Name of the test.
    void getStringFromTranslatableWithFallback(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        Tucuxi::Common::TranslatableString ts1("translatable string in en");
        ts1.setString("chaine de caractere traductible en fr", "fr");

        Tucuxi::Common::TranslatableString ts2("translatable string in en");


        fructose_assert_eq(Tucuxi::XpertUtils::getStringWithEnglishFallback(ts1, Tucuxi::XpertQuery::OutputLang::FRENCH), "chaine de caractere traductible en fr");
        fructose_assert_eq(Tucuxi::XpertUtils::getStringWithEnglishFallback(ts2, Tucuxi::XpertQuery::OutputLang::FRENCH), "translatable string in en");
    }
};

#endif // TEST_XPERTUTILS_H
