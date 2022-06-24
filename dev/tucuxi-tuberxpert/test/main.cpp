#include <iostream>

#if defined(test_xpertutils)
#include "test_xpertutils.h"
#endif

#if defined(test_language)
#include "test_languagemanager.h"
#endif

#if defined(test_query)
#include "test_xpertqueryimport.h"
#endif

#if defined(test_xpertquerytocoreextractor)
#include "test_xpertquerytocoreextractor.h"
#endif

#if defined(test_globalresultcreation)
#include "test_globalresultcreation.h"
#endif

#if defined(test_covariatevalidatorandmodelselector)
#include "test_covariatevalidatorandmodelselector.h"
#endif

#if defined(test_dosevalidator)
#include "test_dosevalidator.h"
#endif

#if defined(test_samplevalidator)
#include "test_samplevalidator.h"
#endif

#if defined(test_targetvalidator)
#include "test_targetvalidator.h"
#endif

#if defined(test_adjustmenttraitcreator)
#include "test_adjustmenttraitcreator.h"
#endif


using namespace std;

/// \brief Testing program.
/// \date 21/04/2022
/// \author Herzig Melvyn
int main(int argc, char** argv)
{

    int res;


    /***********************************************************
     *                       XpertUtils                        *
     ***********************************************************/

#if defined(test_xpertutils)
    TestXpertUtils xpertUtilsTests;

    xpertUtilsTests.add_test("Convert output lang to string", &TestXpertUtils::convertOutputLangToString);
    xpertUtilsTests.add_test("Convert double to string", &TestXpertUtils::convertDoubleToString);
    xpertUtilsTests.add_test("Convert covariate type to string", &TestXpertUtils::convertCovariateTypeToString);
    xpertUtilsTests.add_test("Convert warning level to string", &TestXpertUtils::convertWarningLevelToString);
    xpertUtilsTests.add_test("Get a translatable string with english fallback.", &TestXpertUtils::getStringFromTranslatableWithFallback);
    xpertUtilsTests.add_test("The values returned by getOldestTimeRangeStart are correct.", &TestXpertUtils::getOldestDosageTimeRangeStartReturnsCorrectValues);
    xpertUtilsTests.add_test("The values returned by getLatestTimeRangeStart are correct.", &TestXpertUtils::getLatestDosageTimeRangeStartReturnsCorrectValues);
    xpertUtilsTests.add_test("The value returned by computeFileName is correct.", &TestXpertUtils::computeFileNameReturnsCorrectValues);

    res = xpertUtilsTests.run(argc, argv);
    if (res != 0) {
        std::cout << "XpertUtils tests failed" << std::endl << std::endl;
        exit(1);
    }
    else {
        std::cout << "XpertUtils tests succeeded" << std::endl << std::endl;
    }
#endif

    /***********************************************************
     *                        Language                         *
     ***********************************************************/

#if defined(test_language)
    TestLanguageManager testLanguageManager;

    testLanguageManager.add_test("GetInstance", &TestLanguageManager::retrieveDictionary);
    testLanguageManager.add_test("translate", &TestLanguageManager::wordTranslation);

    res = testLanguageManager.run(argc, argv);
    if (res != 0) {
        std::cout << "Language manager tests failed" << std::endl << std::endl;
        exit(1);
    }
    else {
        std::cout << "Language manager tests succeeded" << std::endl << std::endl;
    }
#endif

    /***********************************************************
     *                    XpertQueryImport                     *
     ***********************************************************/


#if defined(test_query)
    TestXpertQueryImport testXpertQueryImports;

    testXpertQueryImports.add_test("Gets complete admin element", &TestXpertQueryImport::retrieveCompleteAdmin);
    testXpertQueryImports.add_test("Gets no admin element", &TestXpertQueryImport::retrieveNoAdmin);
    testXpertQueryImports.add_test("Gets empty admin element", &TestXpertQueryImport::retrieveEmptyAdmin);
    testXpertQueryImports.add_test("Gets minimal persons", &TestXpertQueryImport::retrieveMinimalPerson);
    testXpertQueryImports.add_test("Gets minimal institutes", &TestXpertQueryImport::retrieveMinimalInstitute);
    testXpertQueryImports.add_test("Gets minimal coordinates", &TestXpertQueryImport::retrieveMinimalCoordinates);
    testXpertQueryImports.add_test("Gets an error when missing mandatory values in mandator person", &TestXpertQueryImport::errorWhenMissingMandatoryInMandatorPerson);
    testXpertQueryImports.add_test("Gets an error when missing mandatory values in mandator institute", &TestXpertQueryImport::errorWhenMissingMandatoryInMandatorInstitute);
    testXpertQueryImports.add_test("Gets an error when missing mandatory values in patient person", &TestXpertQueryImport::errorWhenMissingMandatoryInPatientPerson);
    testXpertQueryImports.add_test("Gets an error when missing mandatory values in patient institute", &TestXpertQueryImport::errorWhenMissingMandatoryInPatientInstitute);
    testXpertQueryImports.add_test("Gets complete request xpert element", &TestXpertQueryImport::retrieveCompleteRequestXpert);
    testXpertQueryImports.add_test("Gets default request xpert element", &TestXpertQueryImport::retrieveDefaultRequestXpert);
    testXpertQueryImports.add_test("Gets an error when missing request xpert", &TestXpertQueryImport::errorWhenNoRequestXpert);
    testXpertQueryImports.add_test("Gets an error when missing mandatory values in request xpert", &TestXpertQueryImport::errorWhenMissingMandatoryRequestXpert);
    testXpertQueryImports.add_test("Gets an error when the xml document can't be created from string", &TestXpertQueryImport::errorWhenCreatingXmlDocumentFromString);
    testXpertQueryImports.add_test("Gets an error when the xml document can't be created from file", &TestXpertQueryImport::errorWhenCreatingXmlDocumentFromFile);

    res = testXpertQueryImports.run(argc, argv);
    if (res != 0) {
        std::cout << "Xpert query import tests failed" << std::endl << std::endl;
        exit(1);
    }
    else {
        std::cout << "Xpert query import tests succeeded" << std::endl << std::endl;
    }
#endif


    /***********************************************************
     *                XpertQueryToCoreExtractor                *
     ***********************************************************/

#if defined(test_xpertquerytocoreextractor)
    TestXpertQueryToCoreExtractor testXpertQueryToCoreExtractor;

    testXpertQueryToCoreExtractor.add_test("Extracts good drug treatment", &TestXpertQueryToCoreExtractor::extractGoodDrugTreatment);
    testXpertQueryToCoreExtractor.add_test("Extracts bad drug treatment", &TestXpertQueryToCoreExtractor::extractBadDrugTreatment);

    res = testXpertQueryToCoreExtractor.run(argc, argv);
    if (res != 0) {
        std::cout << "Xpert query to core extractor tests failed" << std::endl << std::endl;
        exit(1);
    }
    else {
        std::cout << "Xpert query to core extractor tests succeeded" << std::endl << std::endl;
    }
#endif


    /***********************************************************
     *                  GlobalResult creation                   *
     ***********************************************************/

#if defined(test_globalresultcreation)
    TestGlobalResultCreation testGlobalResultCreation;

    testGlobalResultCreation.add_test("Query pointer invalidation", &TestGlobalResultCreation::queryPtrInvalidation);
    testGlobalResultCreation.add_test("Retrieves administrative data", &TestGlobalResultCreation::retrieveAdministrativeData);
    testGlobalResultCreation.add_test("Retrieves xpert request results", &TestGlobalResultCreation::retrieveXpertRequestResult);
    testGlobalResultCreation.add_test("Retrieves generation date", &TestGlobalResultCreation::retrieveComputationTime);

    res = testGlobalResultCreation.run(argc, argv);
    if (res != 0) {
        std::cout << "GlobalResult creation tests failed" << std::endl << std::endl;
        exit(1);
    }
    else {
        std::cout << "GlobalResult creation tests succeeded" << std::endl << std::endl;
    }
#endif

    /***********************************************************
     *          CovariateValidatorAndModelSelector             *
     ***********************************************************/

#if defined(test_covariatevalidatorandmodelselector)
    TestCovariateValidatorAndModelSelector testCovariateValidatorAndModelSelector;

    testCovariateValidatorAndModelSelector.add_test("Gets an error when no treatment.", &TestCovariateValidatorAndModelSelector::errorWhenNoTreatment);
    testCovariateValidatorAndModelSelector.add_test("No corresponding model for drug id", &TestCovariateValidatorAndModelSelector::noCorrespondingModelForDrugId);
    testCovariateValidatorAndModelSelector.add_test("No result if the query formulations and routes are not equal.", &TestCovariateValidatorAndModelSelector::noResultIfQueryFormulationsAndRoutesAreNotEqual);
    testCovariateValidatorAndModelSelector.add_test("No result if only one model with incompatible formulation and route.", &TestCovariateValidatorAndModelSelector::noResultOneModelIncompatibleFormulationAndRoute);
    testCovariateValidatorAndModelSelector.add_test("Gets one result if there are one model with incompatible formulation and route and a second compatible.", &TestCovariateValidatorAndModelSelector::getResultOneModelIncompatibleFormulationAndRouteSecondCompatible);
    testCovariateValidatorAndModelSelector.add_test("No result if one model with incompatible hard constraint.", &TestCovariateValidatorAndModelSelector::noResultOneModelIncompatibleConstraint);
    testCovariateValidatorAndModelSelector.add_test("Gets one result if there are one model with hard constraint not respected and a second compatible.", &TestCovariateValidatorAndModelSelector::getResultOneModelIncompatibleConstraintSecondCompatible);
    testCovariateValidatorAndModelSelector.add_test("Gets one result if one model with partially incompatible soft constraint.", &TestCovariateValidatorAndModelSelector::getResultOneModelPartiallyCompatibleSoftConstraint);
    testCovariateValidatorAndModelSelector.add_test("No result when multiple birthdates even with models left.", &TestCovariateValidatorAndModelSelector::noResultMultipleBirthdate);
    testCovariateValidatorAndModelSelector.add_test("No result when birthdate with bad datatype even with models left.", &TestCovariateValidatorAndModelSelector::noResultBadDatatypeBirthdate);
    testCovariateValidatorAndModelSelector.add_test("No result when gist with bad unit even with models left.", &TestCovariateValidatorAndModelSelector::noResultBadUnitGist);
    testCovariateValidatorAndModelSelector.add_test("Gets the best result out of 3 models without tie.", &TestCovariateValidatorAndModelSelector::getResultOutofThreeNoTie);
    testCovariateValidatorAndModelSelector.add_test("Gets the best result out of 2 models with tie.", &TestCovariateValidatorAndModelSelector::getResultOutofTwoTie);
    testCovariateValidatorAndModelSelector.add_test("Selection failed when selected model covariates does not support request language without english fallbabk.", &TestCovariateValidatorAndModelSelector::requestFailDefinitionsNotSupportingLanguageWithoutEnglishBackup);
    testCovariateValidatorAndModelSelector.add_test("Checks that the translations match the requests languages.", &TestCovariateValidatorAndModelSelector::getGoodCovariateWarningTranslation);
    testCovariateValidatorAndModelSelector.add_test("Gets and checks the content of the covariate results.", &TestCovariateValidatorAndModelSelector::getCovariateResults);


    res = testCovariateValidatorAndModelSelector.run(argc, argv);
    if (res != 0) {
        std::cout << "Covariate validator and model selector tests failed" << std::endl << std::endl;
        exit(1);
    }
    else {
        std::cout << "Covariate validator and Model selector tests succeeded" << std::endl << std::endl;
    }
#endif


    /***********************************************************
     *                      DoseValidator                      *
     ***********************************************************/

#if defined(test_dosevalidator)
    TestDoseValidator testDoseValidator;

    testDoseValidator.add_test("Gets an error when no treatment.", &TestDoseValidator::errorWhenNoTreatment);
    testDoseValidator.add_test("Gets an error when no drug model.", &TestDoseValidator::errorWhenNoDrugModel);
    testDoseValidator.add_test("The resulting map is empty when there is no dosage.", &TestDoseValidator::emptyResultWhenNoDosages);
    testDoseValidator.add_test("The resulting dose result contains a warning when underdosing.", &TestDoseValidator::warningUnderdose);
    testDoseValidator.add_test("The resulting dose result contains a warning when overdosing.", &TestDoseValidator::warningOverdose);
    testDoseValidator.add_test("Gets an error when the unit conversion fails.", &TestDoseValidator::errorFailUnitConversion);
    testDoseValidator.add_test("Gets an error when the formulation and route not supported by model.", &TestDoseValidator::errorFormulationAndRouteNotSupported);
    testDoseValidator.add_test("Checks that result is fine with multiple dosage type and timerange.", &TestDoseValidator::multipleDosageTypeAndDosageTimeRange);


    res = testDoseValidator.run(argc, argv);
    if (res != 0) {
        std::cout << "Dose validator tests failed" << std::endl << std::endl;
        exit(1);
    } else {
        std::cout << "Dose validator tests succeeded" << std::endl << std::endl;
    }
#endif

    /***********************************************************
     *                     SampleValidator                     *
     ***********************************************************/

#if defined(test_samplevalidator)
    TestSampleValidator testSampleValidator;

    testSampleValidator.add_test("Gets an error when no treatment.", &TestSampleValidator::errorWhenNoTreatment);
    testSampleValidator.add_test("Gets an error when there are samples but no dosage.", &TestSampleValidator::errorWhenSamplesButNoDosage);
    testSampleValidator.add_test("Gets an error when no drug model.", &TestSampleValidator::errorWhenNoDrugModel);
    testSampleValidator.add_test("Gets warnings for a given group position over 99 percentiles.", &TestSampleValidator::warningForGroupPositionOver99Percentiles);
    testSampleValidator.add_test("Gets some group positions over 99 percentile.", &TestSampleValidator::findGroupPositionOver99Percentiles);
    testSampleValidator.add_test("Gets an exception when the sample unit cannot be converted.", &TestSampleValidator::getExceptionUnitConversion);
    testSampleValidator.add_test("Gets an exception when the sample date cannot be found.", &TestSampleValidator::getExceptionDateNotFound);
    testSampleValidator.add_test("Gets the expected amount of samples results.", &TestSampleValidator::getTheExpectedAmountOfResults);

    res = testSampleValidator.run(argc, argv);
    if (res != 0) {
        std::cout << "Sample validator tests failed" << std::endl << std::endl;
        exit(1);
    } else {
        std::cout << "Sample validator tests succeeded" << std::endl << std::endl;
    }
#endif

    /***********************************************************
     *                     TargetValidator                     *
     ***********************************************************/

#if defined(test_targetvalidator)
    TestTargetValidator testTargetValidator;

    testTargetValidator.add_test("Gets an error when no treatment.", &TestTargetValidator::errorWhenNoTreatment);
    testTargetValidator.add_test("Gets an error when no drug model.", &TestTargetValidator::errorWhenNoDrugModel);
    testTargetValidator.add_test("No error when no patient's target.", &TestTargetValidator::noErrorWhenNoTarget);
    testTargetValidator.add_test("Gets an error when two patient's targets have the same active moiety and type.", &TestTargetValidator::errorWhenTargetsWithSameActiveMoietyAndType);
    testTargetValidator.add_test("No error when two patient's targets have the same active moiety but different type.", &TestTargetValidator::noErrorWhenTargetsWithSameActiveMoietyButDifferentType);
    testTargetValidator.add_test("Gets an error when one patient's target have a different active moiety than the ones from drug model.", &TestTargetValidator::errorWhenTargetsWithActiveMoietyNotInDrugModel);


    res = testTargetValidator.run(argc, argv);
    if (res != 0) {
        std::cout << "Target validator tests failed" << std::endl << std::endl;
        exit(1);
    } else {
        std::cout << "Target validator tests succeeded" << std::endl << std::endl;
    }
#endif

    /***********************************************************
     *                  AdjustmentTraitCreator                 *
     ***********************************************************/

#if defined(test_adjustmenttraitcreator)
    TestAdjustmentTraitCreator testAdjustmentTraitCreator;

    testAdjustmentTraitCreator.add_test("Get an error when no treatment.", &TestAdjustmentTraitCreator::errorWhenNoTreatment);
    testAdjustmentTraitCreator.add_test("Get an error when no drug model.", &TestAdjustmentTraitCreator::errorWhenNoDrugModel);
    testAdjustmentTraitCreator.add_test("The number of points per hour is twenty.", &TestAdjustmentTraitCreator::nbPointsPerJourIsTwenty);
    testAdjustmentTraitCreator.add_test("The computing option retrieves all (statistics, covariates and parameters) and is on all active moieties", &TestAdjustmentTraitCreator::computingOptionIsRetrieveAllAndAllActiveMoieties);
    testAdjustmentTraitCreator.add_test("The computing option prediction parameters type is Apriori when no dosage and no sample", &TestAdjustmentTraitCreator::computingOptionIsAprioriWhenNoDosageAndNoSample);
    testAdjustmentTraitCreator.add_test("The computing option prediction parameters type is Apriori when dosage but no sample", &TestAdjustmentTraitCreator::computingOptionIsAprioriWhenDosageButNoSample);
    testAdjustmentTraitCreator.add_test("The computing option prediction parameters type is Aposteriori when dosage and sample", &TestAdjustmentTraitCreator::computingOptionIsAposterioriWhenDosageAndSample);
    testAdjustmentTraitCreator.add_test("The adjustment time is the adjustment time of the request when set manually.", &TestAdjustmentTraitCreator::adjustmentTimeIsRequestAdjustmentTimeWhenManuallySet);
    testAdjustmentTraitCreator.add_test("The adjustment time is the computing time plus one hour when there is no dosage history and it is not manually set.", &TestAdjustmentTraitCreator::adjustmentTimeIsComputingTimePlusOneHourWithoutDosageHistoryAndNotManuallySet);
    testAdjustmentTraitCreator.add_test("The adjustment time is the computing time plus one hour when there is a dosage history in the future and it is not manually set.", &TestAdjustmentTraitCreator::adjustmentTimeIsComputingTimePlusOneHourWithFutureDosageHistoryAndNotManuallySet);
    testAdjustmentTraitCreator.add_test("The adjustment time is the next intake time when there is an ongoing dosage history and not manually set.", &TestAdjustmentTraitCreator::adjustmentTimeIsNextIntakeTimeWhenOngoingDosageHistoryAndNotManuallySet);
    testAdjustmentTraitCreator.add_test("The adjustment time is half life approximated when the dosage history is over and not manually set.", &TestAdjustmentTraitCreator::adjustmentTimeIsHalfLifeApproximatedWhenDosageHistoryOverAndNotManuallySet);
    testAdjustmentTraitCreator.add_test("The start and end date times are the dosage history start plus the standard treatment duration when there are an on ongoing standard treatment and a dosage history.", &TestAdjustmentTraitCreator::startEndDatesAreDosageHistoryStartPlusStandardTreatmentDurationWhenOngoingStandardTreatmentAndDosageHistory);
    testAdjustmentTraitCreator.add_test("The start and end date times are computation time plus the standard treatment duration when there is a standard treatment but no dosage history.", &TestAdjustmentTraitCreator::startEndDatesAreDosageHistoryStartPlusStandardTreatmentDurationWhenOngoingStandardTreatmentAndDosageHistory);
    testAdjustmentTraitCreator.add_test("Get an error when there is a standard treatment but it is already over.", &TestAdjustmentTraitCreator::errorWhenStandardTreatmentIsOverBeforeComputationTime);
    testAdjustmentTraitCreator.add_test("The start and end date times are defined based on the adjustment time when there is no standard treatment.", &TestAdjustmentTraitCreator::startEndDatesAreDefinedOnAdjustmentTimeWhenNoStandardTreatment);
    testAdjustmentTraitCreator.add_test("The best candidates option is best dosage per interval.", &TestAdjustmentTraitCreator::bestCandidatesOptionIsBestDosagePerInterval);
    testAdjustmentTraitCreator.add_test("The loading option is retrieved from request when set.", &TestAdjustmentTraitCreator::loadingOptionIsRetrievedFromRequestWhenSet);
    testAdjustmentTraitCreator.add_test("The loading option is retrieved from drug model when not set.", &TestAdjustmentTraitCreator::loadingOptionIsRetrievedFromDrugModelWhenNotSet);
    testAdjustmentTraitCreator.add_test("The rest period option is retrieved from request when set.", &TestAdjustmentTraitCreator::restPeriodOptionIsRetrievedFromRequestWhenSet);
    testAdjustmentTraitCreator.add_test("The rest period option is retrieved from drug model when not set.", &TestAdjustmentTraitCreator::restPeriodOptionIsRetrievedFromDrugModelWhenNotSet);
    testAdjustmentTraitCreator.add_test("The steady state target option is within treatment time range when there is a standard treatment.", &TestAdjustmentTraitCreator::steadyStateTargetOptionIsWithinTreatmentTimeRangeWhenStandardTreatment);
    testAdjustmentTraitCreator.add_test("The steady state target option is at steady state when there is no standard treatment.", &TestAdjustmentTraitCreator::steadyStateTargetOptionIsAtSteadyStateWhenNoStandardTreatment);
    testAdjustmentTraitCreator.add_test("The target extraction option is retrieved from request when set.", &TestAdjustmentTraitCreator::targetExtractionOptionIsRetrievedFromRequestWhenSet);
    testAdjustmentTraitCreator.add_test("The target extraction option is population values when not set.", &TestAdjustmentTraitCreator::targetExtractionOptionIsDefinitionIfNoIndividualTargetWhenNotSet);
    testAdjustmentTraitCreator.add_test("The formulation and route selection option is retrieved from request when set.", &TestAdjustmentTraitCreator::formulationAndRouteSelectionOptionIsRetrievedFromRequestWhenSet);
    testAdjustmentTraitCreator.add_test("The formulation and route selection option is last formulation and route when not set.", &TestAdjustmentTraitCreator::formulationAndRouteSelectionOptionIsLastFormulationAndRouteWhenNotSet);
    testAdjustmentTraitCreator.add_test("The last intake is correctly set in the XpertRequestResult when there is a treatment.", &TestAdjustmentTraitCreator::lastIntakeIsCorrectlySetWithTreatment);
    testAdjustmentTraitCreator.add_test("The last intake is nullptr in the XpertRequestResult when there is no treatment.", &TestAdjustmentTraitCreator::lastIntakeIsNullptrWithoutTreatment);


    res = testAdjustmentTraitCreator.run(argc, argv);
    if (res != 0) {
        std::cout << "Adjustment trait creator tests failed" << std::endl << std::endl;
        exit(1);
    } else {
        std::cout << "Adjustment trait creator tests succeeded" << std::endl << std::endl;
    }
#endif


    return 0;
}
