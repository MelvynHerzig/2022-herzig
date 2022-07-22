#ifndef TEST_TARGETVALIDATOR_H
#define TEST_TARGETVALIDATOR_H

#include <memory>

#include "tuberxpert/result/xpertqueryresult.h"

#include "testutils.h"

#include "fructose/fructose.h"

/// \brief Tests for TargetValidator from the GeneralXpertFlowStepProvider.
/// \date 09/06/2022
/// \author Herzig Melvyn
struct TestTargetValidator : public fructose::test_base<TestTargetValidator>
{

    /// \brief Check that there is an error if the treatment of an XpertRequestResult is nullptr
    ///        in TargetValidator.
    ///        The TargetValidator must set the error in the XpertRequestResult and
    ///        shouldBeProcessed must return false
    /// \param _testName Name of the test
    void targetValidator_failure_whenTreatmentNullptr(const std::string& _testName);

    /// \brief The test loads an xpertRequest without assigning a drug model to it.
    ///        The TargetValidator must set the error in the XpertRequestResult and
    ///        shouldBeProcessed must return false.
    /// \param _testName Name of the test
    void targetValidator_failure_whenDrugModelNullptr(const std::string& _testName);

    /// \brief This method performs a target validation with a treatment that does not specify custom targets.
    ///        XpertRequestResult::shouldBeProcessed must return true.
    /// \param _testName Name of the test
    void targetValidator_success_whenNoTargetInTreatment(const std::string& _testName);

    /// \brief This method performs a target validation with a treatment that specifies custom targets
    ///        that have the same active moiety and the sametarget type.
    ///        The XpertRequestResult must have an error and shouldContinueProcessing must return false.
    /// \param _testName Name of the test
    void targetValidator_failure_whenTargetWithSameActiveMoietyAndTypeInTreatment(const std::string& _testName);

    /// \brief This method performs a target validation with a treatment that specifes custom targets
    ///        that have the same active moiety but a different target type.
    ///        The XpertRequestResult must not contain any errors and shouldContinueProcessing must return true.
    /// \param _testName Name of the test
    void targetValidator_success_whenTargetWithSameActiveMoietyButDifferentTypeInTreatment(const std::string& _testName);

    /// \brief This method performs a target validation with a treatment that specifes a custom target
    ///        that has a different active moiety than the imatinib drug model.
    ///        The XpertRequestResult must have an error and shouldContinueProcessing must return false.
    /// \param _testName Name of the test
    void targetValidator_failure_whenTreatmentTargetWithActiveMoietyNotInDrugModel(const std::string& _testName);
};

#endif // TEST_TARGETVALIDATOR_H
