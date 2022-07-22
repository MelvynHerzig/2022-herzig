#ifndef TEST_XPERTUTILS_H
#define TEST_XPERTUTILS_H

#include "testutils.h"

#include "tuberxpert/language/languageexception.h"

#include "tuberxpert/utils/xpertutils.h"
#include "tuberxpert/result/xpertqueryresult.h"

#include "fructose/fructose.h"

/// \brief Tests for the methods in tuberxpert/utils/xpertutils.h.
/// \date 03/06/2022
/// \author Herzig Melvyn
struct TestXpertUtils : public fructose::test_base<TestXpertUtils>
{

    /// \brief Convert all the DataType to string. Check that the string contains the correct value.
    ///        Check that the value are expected.
    ///        - Bool must be "bool"
    ///        - Int must be "int"
    ///        - Double must be "doule"
    ///        - Date must be "date".
    /// \param _testName Name of the test.
    void dataTypeToString_behavesCorrectly_withAllDataType(const std::string& _testName);

    /// \brief Convert some doubles to string. Check that the strings contain two decimals.
    ///        Check that the value are expected.
    ///        - 5.411111 must be 5.41
    ///        - 6 must be 6.00
    /// \param _testName Name of the test.
    void doubleToString_behavesCorrectly(const std::string& _testName);

    /// \brief Convert all the CovariateType to string. Check that the values are expected.
    ///        - Model must be "default"
    ///        - Patient must be "patient"
    ///        - -1 -> invalid_argument
    /// \param _testName Name of the test.
    void covariateTypeToString_behavesCorrectly_withAllTheCovariateTypesAndUnsupportedValues(const std::string& _testName);

    /// \brief Convert the OutputLang to string. Check that the values are expected or a LanguageException.
    ///        At the moment the only supported languages are: English and French.
    ///        - ENGNLISH must be "en"
    ///        - FRENCH must be "fr"
    ///        - -1 -> LanguageException
    /// \param _testName Name of the test.
    void outputLangToString_behavesCorrectly_withAllOutputLangAndUnsupportedValues(const std::string& _testName);

    /// \brief Convert the WarningLevel to string. Check that the values are expected or an invalid_argument.
    ///        - CRITICAL must be "critical"
    ///        - NORMAL must be "normal"
    /// \param _testName Name of the test.
    void warningLevelToString_behavesCorrectly_withAllWarningLevelAndUnsupportedValues(const std::string& _testName);

    /// \brief Convert a DateTime to a formatted date time string for xml. Check that the string
    ///        contains the correct value.
    ///        The 1.1.2022 at 10h must be "2022-01-01T10:00:00"
    /// \param _testName Name of the test.
    void dateTimeToXmlString_behavesCorrectly(const std::string& _testName);

    /// \brief Convert some TimeOfDay to string. Checks that the values are expected.
    ///        - 8h 0m 0s must be "8h"
    ///        - 8h 30m 1s must be "8h30"
    /// \param _testName Name of the test.
    void timeToString_behavesCorrectly(const std::string& _testName);

    /// \brief Convert some dateTime to a date time strings for html/pdf. Check that the strings
    ///        contain the correct value.
    ///        - The 1.1.2022 at 10h (asking for the time indication) must be "1.1.2022 10h"
    ///        - The 1.1.2022 at 10h (not asking for the time indication) must be "1.1.2022"
    /// \param _testName Name of the test.
    void dateTimeToString_behavesCorrectly(const std::string& _testName);

    /// \brief Convert some covariate string values. Check that
    ///        resulting strings are the expected values.
    ///        - "1.0" Bool (no id) must be "Yes"
    ///        - "0.0" Bool (no id) must be "No"
    ///        - "1.0" Sex (no id) must be "Man"
    ///        - "0.0" Sex (no id) must be "Woman"
    ///        - "0.5" Sex (no id) must be "Undefined"
    ///        - "42.00" Int age must be "42"
    ///        - "72.652222" Double bodyweight must be "72.65"
    /// \param _testName Name of the test.
    void beautifyString_behavesCorrectly(const std::string& _testName);

    /// \brief Check that the method getStringWithEnglishFallback returns the correct value.
    ///        - A translatableString with the required language must returns the targeted translation.
    ///        - A translatableString without the required language but English returns the English translation.
    ///        - A translatableString without the required language and without English returns an empty string.
    /// \param _testName Name of the test.
    void getStringWithEnglishFallback_behavesCorrectly(const std::string& _testName);

    /// \brief Test that the getOldestDosageTimeRangeStart works as expected.
    ///        This test forms a dosage history with two time ranges:
    ///         1) 2022-01-01 10h00 - 2022-01-02 13h00
    ///         2) 2022-01-03 14h00 - 2022-01-04 16h00
    ///
    ///        Once it requires the oldest start time with a computation time of 2022-01-01 14h00
    ///        (just after the first time range) which should return 2022-01-01 10h00
    ///
    ///        Once it requires the oldest start time with a computation time of 2022-01-01 09h00
    ///        (before both time ranges) which should return the reference time itself.
    /// \param _testName Name of the test.
    void getOldestDosageTimeRangeStart_behavesCorrectly(const std::string& _testName);

    /// \brief Tests that the getLatestDosageTimeRangeStart works as expected.
    ///        This test forms a dosage history with two time ranges:
    ///         1) 2022-01-01 10h00 - 2022-01-02 13h00
    ///         2) 2022-01-03 14h00 - 2022-01-04 16h00
    ///
    ///        Once it requires the latest start time with a computation time of 2022-01-07 15h00
    ///        (after both time ranges) which should return 2022-01-03 14h00
    ///
    ///        Once it requires the oldest start time with a computation time of 2022-01-01 09h00
    ///        (before both time ranges) which should return an undefined date time.
    ///
    /// \param _testName Name of the test.
    void getLatestDosageTimeRangeStart_behavesCorrectly(const std::string& _testName);

    /// \brief This method tests the computeFileName method.
    ///        It computes a XpertRequestResult based on classic imatinib request.
    ///        The names returned should be:
    ///        - "random/path/imatinib_1_11-7-2018_13h45m30s.xml" with the path and the extension.
    ///        - "imatinib_1_11-7-2018_13h45m30s.xml" without the path.
    ///        - "imatinib_1_11-7-2018_13h45m30s" without the path and without the exension.
    /// \param _testName Name of the test
    void computeFileName_behavesCorrectly(const std::string& _testName);

    /// \brief Check that the method executeRequestAndGetResult sets the result to nullptr when
    ///        the execution fails or that the resulting pointer is correctly retrieved. The test
    ///        prepares two percentile traits, one that must fail and one that must succeed.
    ///        One trait is prepared with a too large period which will fail.
    ///        The other is a valid trait like the ones used in samplevalidator.cpp.
    ///        - The first response must be nullptr.
    ///        - The second response must not be nullptr and must contain 3 percentiles.
    /// \param _testName Name of the test
    void executeRequestAndGetResult_behavesCorrectly(const std::string& _testName);

    /// \brief Test that a camel case key is correctly converted into a phrase.
    ///        - "" must be ""
    ///        - "ab c" must be "Ab c"
    ///        - "camelCase" must be "camelCase"
    /// \param _testName Name of the test
    void keyToPhrase_behavesCorrectly(const std::string& _testName);

    /// \brief Test the age calculator getAgeIn method.
    ///        The birthday is set "2000-01-01T12:00:00" and the computation time to "2010-01-01T12:00:00"
    ///        - The age in days must be 3653
    ///        - The age in weeks must be 521
    ///        - The age in months must be 119
    ///        - The age in years must be 10
    ///        - The age an invalid CovariateType throws an invalid_argument
    /// \param _testName Name of the test
    void getAgeIn_behavesCorrectly(const std::string& _testName);
};

#endif // TEST_XPERTUTILS_H
