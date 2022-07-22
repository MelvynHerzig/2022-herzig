#ifndef TEST_REQUESTEXECUTOR_H
#define TEST_REQUESTEXECUTOR_H

#include "tuberxpert/flow/general/generalxpertflowstepprovider.h"
#include "tuberxpert/result/xpertqueryresult.h"

#include "testutils.h"

#include "fructose/fructose.h"

/// \brief Tests for RequestExecutor from the GeneralXpertFlowStepProvider.
///        The tests use the AdjustmentTraitCreator object and assume
///        that this object works as intended.
/// \date 25/06/2022
/// \author Herzig Melvyn
struct TestRequestExecutor : public fructose::test_base<TestRequestExecutor>
{

    /// \brief Check that there is an error if the adjustment trait of an XpertRequestResult is nullptr
    ///        in RequestExecutor.
    ///        The RequestExecutor must set the error in the XpertRequestResult and
    ///        shouldBeProcessed must return false
    /// \param _testName Name of the test
    void requestExecutor_failure_whenAdjustmentTraitNullptr(const std::string& _testName);

    /// \brief The test loads an xpertRequest without assigning a drug model to it.
    ///        The RequestExecutor must set the error in the XpertRequestResult,
    ///        shouldBeProcessed must return false.
    /// \param _testName Name of the test
    void requestExecutor_failure_whenDrugModelNullptr(const std::string& _testName);

    /// \brief This method modifies the trait of the XpertRequestResult to fail.
    ///        The returned adjustmentData must be nullptr, the XpertRequestResult receives an error message
    ///        and shouldContinuProcessing returns false.
    /// \param _testName Name of the test
    void requestExecutor_failure_whenRequestExecutionFailed(const std::string& _testName);

    /// \brief This method checks that when a valid adjustment request is made,
    ///        the adjustmentData is not nullptr, houldContinuProcessing returns true and
    ///        their is an adjustment in the adjustmentData.
    ///
    /// \param _testName Name of the test
    void requestExecutor_getsCorrectAdjustmentData_whenRequestExecutionSucceed(const std::string& _testName);

    /// \brief This method checks that when a valid adjustment request is performed,
    ///        there are 9 statistics at steady state.
    ///        The method shouldContinueProcessing must return true.
    /// \param _testName Name of the test
    void requestExecutor_getsTheStatistics_whenRequestExecutionSucceed(const std::string& _testName);

    /// \brief This method checks that the typical, apriori and aposteriori parameters are
    ///        defined when the base trait of the XpertRequestResult is aposteriori. The parameters
    ///        groups are not empty and they have the same size.
    ///        The method shouldContinueProcessing must return true.
    /// \param _testName Name of the test
    void requestExecutor_getsTypicalAprioriAposterioriParameters_whenAposterioriTrait(const std::string& _testName);

    /// \brief This method checks that the typical and apriori parameters are
    ///        defined when the base trait of the XpertRequestResult is apriori. The parameters
    ///        groups are not empty and they have the same size.
    ///        The method shouldContinueProcessing must return true.
    /// \param _testName Name of the test
    void requestExecutor_getsTypicalAprioriParameters_whenAprioriTrait(const std::string& _testName);
};

#endif // TEST_REQUESTEXECUTOR_H
