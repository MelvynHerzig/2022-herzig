#ifndef TEST_SAMPLEVALIDATOR_H
#define TEST_SAMPLEVALIDATOR_H

#include <numeric>
#include <string>
#include <memory>

#include "tucucore/drugtreatment/sample.h"
#include "tucucore/definitions.h"
#include "tucucommon/datetime.h"
#include "tucucommon/unit.h"

#include "tuberxpert/flow/general/samplevalidator.h"
#include "tuberxpert/result/xpertqueryresult.h"
#include "tuberxpert/result/samplevalidationresult.h"

#include "testutils.h"

#include "fructose/fructose.h"

/// \brief Tests for SampleValidator from the GeneralXpertFlowStepProvider.
/// \date 06/06/2022
/// \author Herzig Melvyn
struct TestSampleValidator : public fructose::test_base<TestSampleValidator>
{
    /// \brief Unit used to create the percentile data.
    const Tucuxi::Common::TucuUnit unit{"ug/l"};

    /// \brief Create some percentile data and insert them into _percentileData.
    /// \param _percentileData PercentilesData object which receives the result.
    void createPercentilesData( Tucuxi::Core::PercentilesData& _percentileData);

    /// \brief Check that there is an error if the treatment of an XpertRequestResult is nullptr
    ///        in SampleValidator.
    ///        The SampleValidator must set the error in the XpertRequestResult,
    ///        shouldBeProcessed must return false and there should be 0 SampleValidationResult.
    /// \param _testName Name of the test
    void sampleValidator_failure_whenTreatmentNullptr(const std::string& _testName);

    /// \brief Check that there is an error if the treatment of an XpertRequestResult has
    ///        samples but no dosages in SampleValidator.
    ///        The SampleValidator must set the error in the XpertRequestResult,
    ///        shouldBeProcessed must return false and there should be 0 SampleValidationResult.
    /// \param _testName Name of the test
    void sampleValidator_failure_whenSamplesWithoutDosages(const std::string& _testName);

    /// \brief The test loads an xpertRequest without assigning a drug model to it.
    ///        The SampleValidator must set the error in the XpertRequestResult,
    ///        shouldBeProcessed must return false and there should be 0 SampleValidationResult.
    /// \param _testName Name of the test
    void sampleValidator_failure_whenDrugModelNullptr(const std::string& _testName);

    /// \brief This test creates fake SampleValidationResult objects and checks
    ///        that the warnings are correct.
    ///           - If the group is 1, the message must be "99% of the population is above this measure".
    ///           - If the group is 10, the message must be "90% of the population is above this measure".
    ///           - If the group is 11, the message must be "".
    ///           - If the group is 90, the message must be "".
    ///           - If the group is 91, the message must be "90% of the population is below this measure".
    ///           - If the group is 100, the message must be "99% of the population is below this measure".
    ///
    /// \param _testName Name of the test
    void sampleValidationResult_computesCorrectWarning(const std::string& _testName);

    /// \brief This method checks that the SampleValidator::findPosOver100Percentile method returns
    ///        the correct positions for a given set of samples and a given percentileData.
    ///
    ///        We test the SampleValidator class with SampleValidator::findPosOver100Percentile and not
    ///        with SampleValidator::perform because this last method performs a request
    ///        on the core that returns "unpredictable" percentiles. So, basically 90% of SampleValidator's
    ///        behavior can be verified with SampleValidator::findPosOver100Percentile.
    ///
    ///        Thus, this struct is friend with SampleValidator to acces this protected method.
    /// \param _testName Name of the test
    void findGroupPositionOver99Percentiles_behavesCorrectly_withGivenPercentilesDataAndSamples(const std::string& _testName);

    /// \brief This method checks that the method SampleValidator::findPosOver100Percentile throws
    ///        an invalid_argument exception when a sample unit cannot be converted into the unit
    ///        of the cycleData of the percentilesData object.
    /// \param _testName Name of the test
    void findGroupPositionOver99Percentiles_throwInvalidArgument_whenSampleUnitConversionFailure(const std::string& _testName);

    /// \brief This method checks that the method SampleValidator::findPosOver100Percentile throws
    ///        an invalid_argument exception when a sample date cannot be found in the cycleData objects.
    /// \param _testName Name of the test
    void findGroupPositionOver99Percentiles_throwInvalidArgument_whenSampleDateNotFoundInCycleData(const std::string& _testName);

    /// \brief This method performs a real sample validation with 3 samples. The vector retrived by
    ///        XpertRequestResult::getSampleValidationResults must be sorted by sample date.
    ///        We only check the sample date. The other interesting values, such as:
    ///        percentile, warning type, warning message are already tested by more deterministic tests.
    ///
    ///        The smallest date comes first.
    /// \param _testName Name of the test
    void getSampleValidationResult_isSorted_whenNotEmpty(const std::string& _testName);
};

#endif // TEST_SAMPLEVALIDATOR_H
