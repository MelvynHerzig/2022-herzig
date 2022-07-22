#ifndef TEST_ADJUSTMENTTRAITCREATOR_H
#define TEST_ADJUSTMENTTRAITCREATOR_H

#include <memory>

#include "tuberxpert/result/xpertrequestresult.h"
#include "tuberxpert/result/xpertqueryresult.h"

#include "testutils.h"

#include "fructose/fructose.h"

/// \brief Tests for AdjustmentTraitCreator from the GeneralXpertFlowStepProvider
///        This class performs several adjustment creations to check if the result is the
///        expected one. Each test tests an attribute (or a subset of attributes) of the returned
///        adjustment trait.
/// \date 15/07/2022
/// \author Herzig Melvyn
struct TestAdjustmentTraitCreator : public fructose::test_base<TestAdjustmentTraitCreator>
{

    /// \brief Check that there is an error if the treatment of an XpertRequestResult is nullptr
    ///        in AdjustmentTraitCreator.
    ///        The AdjustmentTraitCreator must set the error in the XpertRequestResult and
    ///        shouldBeProcessed must return false
    /// \param _testName Name of the test
    void adjustmentTraitCreator_failure_whenTreatmentNullptr(const std::string& _testName);

    /// \brief The test loads an xpertRequest without assigning a drug model to it.
    ///        The AdjustmentTraitCreator must set the error in the XpertRequestResult and
    ///        shouldBeProcessed must return false.
    /// \param _testName Name of the test
    void adjustmentTraitCreator_failure_whenDrugModelNullptr(const std::string& _testName);

    /// \brief This method checks that the adjustment trait creator defines the expected number of points
    ///        per hours (i.e. 20) and that there is no error (i.e. shouldContinueProcessing returns true).
    /// \param _testName Name of the test
    void adjustmentTraitCreator_sets20PointsPerHour(const std::string& _testName);

    /// \brief This method checks that the adjustment trait creator sets the expected computing options
    ///        (i.e. AllActiveMoieties, RetrieveStatistics, RetrieveParameters, RetrieveCovariates and forceUgPerLiter)
    ///        and that there is no error (i.e. shouldContinueProcessing returns true).
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsGoodFixedComputingOptions(const std::string& _testName);

    /// \brief This method checks that the adjustment trait creator sets the parameters type of
    ///        the computing options to Apriori when there is no dosage and no sample
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsAprioriParameterType_whenNoDosageAndNoSample(const std::string& _testName);

    /// \brief This method checks that the adjustment trait creator sets the parameters type of
    ///        the computing option to Apriori when there are dosages but no sample.
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsAprioriParameterType_whenDosagesButNoSample(const std::string& _testName);

    /// \brief This method checks that the adjustment trait creator sets the parameters type of
    ///        the computing option to Aposteriori when there are dosages and samples.
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsAposterioriParameterType_whenDosagesAndSamples(const std::string& _testName);


    /// \brief This method checks that the adjustment time matches the adjustment time of the xpert request request when it is set.
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsCorrectAdjustmentTime_whenSetInXpertRequest(const std::string& _testName);

    /// \brief This method checks that the adjustment time matches the computing time (2022-06-20 10h) plus one hour (2022-06-20 11h)
    ///        when the adjustment time is not set in the xpertRequest and there is no dosage history.
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsAdjustmentTimeToComputingTimePlusOneHour_whenNotSetInXpertRequestAndNoDosageHistory(const std::string& _testName);

    /// \brief This method checks that the adjustment time matches the computing time (2022-06-20 10h00) plus one hour (2022-06-20 11h)
    ///        when there is a dosage history but in the future (2023) and the adjustment time is not set in the xpertRequest.
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsAdjustmentTimeToComputingTimePlusOneHour_whenNotSetInXpertRequestAndFutureDosageHistory(const std::string& _testName);

    /// \brief This method checks that the adjustment time matches the next intake time (2022-06-20 20h00) when there
    ///        is an ongoing dosage history (computing time is 20-6-2022 and dosage time range goes from 19-6-2022 to 21-6-2022)
    ///        and the adjustment time is not set in the xpertRequest. There must be no error (i.e. shouldContinueProcessing returns true).
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsAdjustmentTimeToNextIntakeTime_whenNotSetInXpertRequestAndOngoingDosageHistory(const std::string& _testName);

    /// \brief This method checks that the adjustment time is the time resulting from the half-life (x2) added
    ///        to the last intake until the computation time is reached. In the test, the dosage history is in
    ///        2018 (from 6.7 9h00 to 8.7 9h00). Therefore, the last intake is on 7.7.2018 21h.
    ///        Since the half-life of imatinib is 12 hours, 2 x the hal-life is 1 day. Thus, if we add 1 day on top of
    ///        8.7.2018 9h until the computation time (20.6.2022 10h) is reached, the adjustment time should be
    ///        20.6.2022 21h. There must be no error (i.e. shouldContinueProcessing returns true).
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsAdjustmentTimeWithHalfLifeApproximation_whenNotSetInXpertRequestAndCompletedDosageHistory(const std::string& _testName);

    /// \brief This method checks that the start and end dates of the adjustment are the start time plus
    ///        the duration of the standard treatment when there is one and a dosage history.
    ///        In this case, the dosage history starts on 19.6.2022 8h00 and the standard treatment lasts
    ///        for 4 days. Therefore, the start and end dates should be 19.6.2022 8h00 and 23.6.2022 8h00.
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsStartAndEndDatesCorrectly_whenStandardTreatmentAndDosageHistory(const std::string& _testName);

    /// \brief This method checks that the start and end dates of the adjustment are the computation time plus
    ///        the standard treatment duration when there is one but no dosage history.
    ///        In this case, the computation time is on 20.6.2022 10h and the standard treatment lasts
    ///        for 4 days. Therefore, the start and end dates should be 20.6.2022 10h and 24.6.2022 10h.
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsStartAndEndDatesCorrectly_whenStandardTreatmentButNoDosageHistory(const std::string& _testName);

    /// \brief This method checks that the adjustment trait creation fails when there is a standard treatment
    ///        that ends before the adjustment time. In this test, the dosage history starts on
    ///        6.7.2018 8h and the standard treatment lasts for 4 days. Thus, the start and end dates of the adjustment
    ///        should be 6.7.2018 8h - 10.7.2018 8h. Therefore, the treatment is already completed at the time of
    ///        the adjustment time set in the request (20.06.2022 10h).
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    /// \param _testName Name of the test
    void adjustmentTraitCreator_failure_whenStandardTreatmentIsOverBeforeAdjustmentTime(const std::string& _testName);

    /// \brief This method checks that the start and end dates are placed in relation to the adjustment time.
    ///        The start must correspond to the adjustment time.
    ///        The end must correspond to the start plus seven days.
    ///        In this test, the adjustment time is 20.6.2022 20h. Therefore, the start should be 20.6.2022 20h
    ///        and the end should be 27.6.2022 20h
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    ///        It doesn't matter if the adjustment time is manually set or not.
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsStartEndDatesOnAdjustmentTime_whenNoStandardTreatment(const std::string& _testName);

    /// \brief This method checks that best candidates option is best dosage per interval.
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    ///
    /// \param _testName Name of the test
    void adjustmentTraitCreator_bestCandidatesOptionIsBestDosagePerInterval(const std::string& _testName);

    /// \brief This method checks that the loading dose option is retrieved from the xpertRequest
    ///        when it is defined. The request indicates that no loading dose is allowed.
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    ///
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsCorrectLoadingOption_whenDefinedInXpertRequest(const std::string& _testName);

    /// \brief This method checks that the loading dose option is retrieved from the drug model
    ///        when the request doesn't defines it.
    ///
    ///        At this time, the "isLoadingDoseRecommended" value is not defined in the drug model files. By default,
    ///        the drug model importer sets it to true. The result of this test should be tweaked if the imatinib
    ///        model string is updated.
    ///
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    ///
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsCorrectLoadingOption_whenNotDefinedInXpertRequest(const std::string& _testName);


    /// \brief This method checks that the rest period option is retrieved from the xpertRequest
    ///        when it is defined. The request indicates that no rest period is allowed.
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    ///
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsCorrectRestPeriodOption_whenDefinedInXpertRequest(const std::string& _testName);

    /// \brief This method checks that the rest period option is retrieved from the drug model
    ///        when the request doesn't defines it.
    ///
    ///        At this time, the "isRestPeriodRecommended" value is not defined in the drug model files. By default,
    ///        the drug model importer sets it to true. The result of this test should be tweaked if the imatinib
    ///        model string is updated.
    ///
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    ///
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsCorrectRestPeriodOption_whenNotDefinedInXpertRequest(const std::string& _testName);

    /// \brief This method checks that the steady state target option is within treatment time range
    ///        when there is a standard treatment.
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    ///
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsSteadyStateTargetOptionToWithinTreatmentTimeRange_whenStandardTreatment(const std::string& _testName);

    /// \brief This method checks that the steady state target option is at steady state
    ///        when there is no standard treatment.
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    ///
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsSteadyStateTargetOptionToAtSteadyState_whenNoStandardTreatment(const std::string& _testName);

    /// \brief This method checks that the xpertRequest target extraction option is used when it
    ///        is defined. Here, the request defines it to aprioriValues.
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    ///
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsCorrectTargetExtractionOption_whenSetInXpertRequest(const std::string& _testName);

    /// \brief This method checks that the target extraction option is "DefinitionIfNoIndividualTarget"
    ///        when it is not defined in the xpertRequest.
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    ///
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsCorrectTargetExtractionOption_whenNotSetInXpertRequest(const std::string& _testName);

    /// \brief This method checks that the xpertRequest formulation and route selection option is used when it
    ///        is defined. Here, the request defines it to "allFormulationAndRoutes".
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    ///
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsCorrectFormulationAndRouteSelectionOption_whenSetInXpertRequest(const std::string& _testName);

    /// \brief This method checks that the last formulation and route selection option is
    ///        "lastFormulationAndRoute" when it is not defined in the xpertRequest.
    ///        There must be no error (i.e. shouldContinueProcessing returns true).
    ///
    /// \param _testName Name of the test
    void adjustmentTraitCreator_setsCorrectFormulationAndRouteSelectionOption_whenNotSetInXpertRequest(const std::string& _testName);

    /// \brief This method checks that the last intake is correctly set in the
    ///        XpertRequestResult. The test makes a request on imatinib with a treatment.
    ///        The last intake should be 19.5.2022 20h 400mg.
    /// \param _testName Name of the test
    void getLastIntake_returnsCorrectValues_whenDosageHistory(const std::string& _testName);

    /// \brief This method checks that the last intake is nullptr in the
    ///        XpertRequestResult. The test makes a request on imatinib without a treatment.
    /// \param _testName Name of the test
    void getLastIntake_returnsNullptr_whenNoDosageHistory(const std::string& _testName);
};

#endif // TEST_ADJUSTMENTTRAITCREATOR_H
