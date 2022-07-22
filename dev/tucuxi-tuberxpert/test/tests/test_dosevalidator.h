#ifndef TEST_DOSEVALIDATOR_H
#define TEST_DOSEVALIDATOR_H

#include "fructose/fructose.h"

#include <memory>

#include "tuberxpert/result/xpertqueryresult.h"
#include "tuberxpert/result/xpertrequestresult.h"
#include "tuberxpert/utils/xpertutils.h"

#include "testutils.h"

/// \brief Tests for DoseValidator from the GeneralXpertFlowStepProvider.
/// \date 21/04/2022
/// \author Herzig Melvyn
struct TestDoseValidator : public fructose::test_base<TestDoseValidator>
{
    /// \brief Check that there is an error if the treatment of an XpertRequestResult is nullptr
    ///        in DoseValidator.
    ///        The DoseValidator must set the error in the XpertRequestResult,
    ///        shouldBeProcessed must return false and there should be 0 DoseValidationResult.
    /// \param _testName Name of the test
    void doseValidator_failure_whenTreatmentNullptr(const std::string& _testName);

    /// \brief The test loads an xpertRequest without assigning a drug model to it.
    ///        The DoseValidator must set the error in the XpertRequestResult,
    ///        shouldBeProcessed must return false and there should be 0 DoseValidationResult.
    /// \param _testName Name of the test
    void doseValidator_failure_whenDrugModelNullptr(const std::string& _testName);

    /// \brief The test loads a query that has no dose. After the DoseValidator flow step,
    ///        when calling getDoseValidationResults of the XpertRequestResult, the returned
    ///        map must be empty and the shouldContinueProcessing method should return true.
    /// \param _testName Name of the test
    void getDoseValidationResults_isEmpty_whenNoDoseInQuery(const std::string& _testName);

    /// \brief The test loads a query that has a dose. The dose is too low compared to the
    ///        recommended doses of the imatinib model. After the DoseValidator flow step,
    ///        when calling getDoseValidationResults of the XpertRequestResult, the returned
    ///        map must not be empty and the unique DoseValidationResult it contains
    ///        should have a warning message.
    ///
    /// \param _testName Name of the test
    void doseValidationResult_hasWarning_whenUnderDosing(const std::string& _testName);

    /// \brief The test loads a query that has a dose. The dose is too high compared to the
    ///        recommended doses of the imatinib model. After the DoseValidator flow step,
    ///        when calling getDoseValidationResults of the XpertRequestResult, the returned
    ///        map must not be empty and the unique DoseValidationResult it contains
    ///        should have a warning message.
    ///
    /// \param _testName Name of the test
    void doseValidationResult_hasWarning_whenOverDosing(const std::string& _testName);

    /// \brief The test loads a query that has a dose. The dose has non-covertible unit.
    ///        After the DoseValidator flow step, when calling getDoseValidationResults of the XpertRequestResult,
    ///        the returned map must be empty, the shouldContinueProcessing method should return false
    ///        and there is an error message.
    ///
    /// \param _testName Name of the test
    void doseValidator_failure_whenBadDoseUnit(const std::string& _testName);

    /// \brief The test loads a query that has a dose. The dose has a nasal route that is not compatible with the
    ///        imatinib model. After the DoseValidator flow step, when calling getDoseValidationResults of the XpertRequestResult,
    ///        the returned map must be empty, the shouldContinueProcessing method should return false and there is an error message.
    /// \param _testName Name of the test
    void doseValidator_failure_whenFormulationAndRouteNotSupportedByModel(const std::string& _testName);

    /// \brief The test loads a query that has three dosage time ranges. The dosage time ranges mix all the possible
    ///        dosge types. After the DoseValidator flow step,
    ///        when calling getDoseValidationResults of the XpertRequestResult, the returned
    ///        map must have the same size than the number of doses and the DoseValidationResults it contains
    ///        should have a correct dose, a correct unit and a correct warning message.
    /// \param _testName Name of the test
    void doseValidator_returnsCorrectValues_withAllDosageTypeAndMultipleTimeRange(const std::string& _testName);
};

#endif // TEST_DOSEVALIDATOR_H
