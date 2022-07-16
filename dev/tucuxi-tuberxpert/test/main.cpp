#include <iostream>

#if defined(test_xpertutils)
#include "test_xpertutils.h"
#endif

#if defined(test_languagemanager)
#include "test_languagemanager.h"
#endif

#if defined(test_xpertqueryimport)
#include "test_xpertqueryimport.h"
#endif

#if defined(test_xpertquerytocoreextractor)
#include "test_xpertquerytocoreextractor.h"
#endif

#if defined(test_xpertqueryresultcreation)
#include "test_xpertqueryresultcreation.h"
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

#if defined(test_requestexecutor)
#include "test_requestexecutor.h"
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

    xpertUtilsTests.add_test("DataType to string behaves correctly with all dataTypes.", &TestXpertUtils::dataTypeToString_behavesCorrectly_withAllDataType);
    xpertUtilsTests.add_test("Double to string behaves correctly.", &TestXpertUtils::doubleToString_behavesCorrectly);
    xpertUtilsTests.add_test("CovariateType to string behaves correctly with all the covariateTypes.", &TestXpertUtils::covariateTypeToString_behavesCorrectly_withAllTheCovariateTypes);
    xpertUtilsTests.add_test("OutputLang to string behaves correctly with all outputLang and unsupported values.", &TestXpertUtils::outputLangToString_behavesCorrectly_withAllOutputLangAndUnsupportedValues);
    xpertUtilsTests.add_test("WarningLevel to string behaves correctly with all warningLevel and unsupported values.", &TestXpertUtils::warningLevelToString_behavesCorrectly_withAllWarningLevelAndUnsupportedValues);
    xpertUtilsTests.add_test("DateTime to xml string behaves correctly.", &TestXpertUtils::dateTimeToXmlString_behavesCorrectly);
    xpertUtilsTests.add_test("Time to string behaves correctly.", &TestXpertUtils::timeToString_behavesCorrectly);
    xpertUtilsTests.add_test("DateTime to string behaves correctly.", &TestXpertUtils::dateTimeToString_behavesCorrectly);
    xpertUtilsTests.add_test("Beautify strin _behaves correctly", &TestXpertUtils::beautifyString_behavesCorrectly);
    xpertUtilsTests.add_test("Get string with english fallback behaves correctly.", &TestXpertUtils::getStringWithEnglishFallback_behavesCorrectly);
    xpertUtilsTests.add_test("Get oldest dosageTimeRange start behaves correctly.", &TestXpertUtils::getOldestDosageTimeRangeStart_behavesCorrectly);
    xpertUtilsTests.add_test("Get latest dosageTimeRange start behaves correctly.", &TestXpertUtils::getLatestDosageTimeRangeStart_behavesCorrectly);
    xpertUtilsTests.add_test("Compute file name behaves correctly.", &TestXpertUtils::computeFileName_behavesCorrectly);
    xpertUtilsTests.add_test("Execute request and get result behaves correctly.", &TestXpertUtils::executeRequestAndGetResult_behavesCorrectly);
    xpertUtilsTests.add_test("Key to phrase behaves correctly.", &TestXpertUtils::keyToPhrase_behavesCorrectly);
    xpertUtilsTests.add_test("Get age in behaves correctly.", &TestXpertUtils::getAgeIn_behavesCorrectly);

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

#if defined(test_languagemanager)
    TestLanguageManager testLanguageManager;

    testLanguageManager.add_test("load translations behaves correctly.", &TestLanguageManager::loadTranslations_behavesCorrectly);
    testLanguageManager.add_test("translate behaves correctly.", &TestLanguageManager::translate_behavesCorrectly);

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


#if defined(test_xpertqueryimport)
    TestXpertQueryImport testXpertQueryImports;

    testXpertQueryImports.add_test("xpert query import behaves correctly with complete admin.", &TestXpertQueryImport::xpertQueryImport_getsAllValues_withCompleteAdmin);
    testXpertQueryImports.add_test("xpert query import gets nullptr AdminData with no admin element.", &TestXpertQueryImport::xpertQueryImport_getsNullptrAdminData_withNoAdminElement);
    testXpertQueryImports.add_test("xpert query import AdminData getters return nullptr with empty admin element.", &TestXpertQueryImport::xpertQueryImport_adminDataGettersReturnNullptr_withEmptyAdminElement);
    testXpertQueryImports.add_test("xpert query import missing values are nullptr or empty string with minimal person in mandator and patient.", &TestXpertQueryImport::xpertQueryImport_missingValuesAreNullptrOrEmptyString_withMinimalPersonInMandatorAndPatient);
    testXpertQueryImports.add_test("xpert query import missing values are nullptr or empty string with minimal institute in mandator and patient.", &TestXpertQueryImport::xpertQueryImport_missingValuesAreNullptrOrEmptyString_withMinimalInstituteInMandatorAndPatient);
    testXpertQueryImports.add_test("xpert query import missing values are empty string with minimal address phone and email", &TestXpertQueryImport::xpertQueryImport_missingValuesAreEmptyString_withMinimalAddressPhoneAndEmail);
    testXpertQueryImports.add_test("xpert query import import error with missing mandatory values in complete mandator person", &TestXpertQueryImport::xpertQueryImport_importError_withMissingMandatoryValuesInCompleteMandatorPerson);
    testXpertQueryImports.add_test("xpert query import import error with missing mandatory values in complete mandator institute", &TestXpertQueryImport::xpertQueryImport_importError_withMissingMandatoryValuesInCompleteMandatorInstitute);
    testXpertQueryImports.add_test("xpert query import import error with missing mandatory values in complete patient person", &TestXpertQueryImport::xpertQueryImport_importError_withMissingMandatoryValuesInCompletePatientPerson);
    testXpertQueryImports.add_test("xpert query import import error with missing mandatory values in complete patient institute", &TestXpertQueryImport::xpertQueryImport_importError_withMissingMandatoryValuesInCompletePatientInstitute);
    testXpertQueryImports.add_test("xpert query import gets all values with complete XpertRequest", &TestXpertQueryImport::xpertQueryImport_getsAllValues_withCompleteXpertRequest);
    testXpertQueryImports.add_test("xpert query import get default values with minimal XpertRequest", &TestXpertQueryImport::xpertQueryImport_getDefaultValues_WithMinimalXpertRequest);
    testXpertQueryImports.add_test("xpert query import import error without XpertRequest", &TestXpertQueryImport::xpertQueryImport_importError_withoutXpertRequest);
    testXpertQueryImports.add_test("xpert query import import error with missing mandatory in XpertRequest", &TestXpertQueryImport::xpertQueryImport_importError_withMissingMandatoryInXpertRequest);
    testXpertQueryImports.add_test("xpert query import import error with file badly formatted", &TestXpertQueryImport::xpertQueryImport_importError_withFileBadlyFormatted);
    testXpertQueryImports.add_test("xpert query import import error with non existing file", &TestXpertQueryImport::xpertQueryImport_importError_withNonExistingFile);

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

    testXpertQueryToCoreExtractor.add_test("extract drug treatment success with drug elements present once.", &TestXpertQueryToCoreExtractor::extractDrugTreatment_success_withDrugElementsPresentOnce);
    testXpertQueryToCoreExtractor.add_test("extract drug treatment failure with multiple or no drug elements", &TestXpertQueryToCoreExtractor::extractDrugTreatment_failure_withMultipleOrNoDrugElements);

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
     *               XpertQueryResult creation                 *
     ***********************************************************/

#if defined(test_xpertqueryresultcreation)
    TestXpertQueryResultCreation testXpertQueryResultCreation;

    testXpertQueryResultCreation.add_test("XpertQueryResult creation takes XpertQueryData ownership.", &TestXpertQueryResultCreation::xpertQueryResultCreation_takesXpertQueryDataOwnership);
    testXpertQueryResultCreation.add_test("getAdminData of XpertQueyResult returns correct values with or without admin in query.", &TestXpertQueryResultCreation::getAdminDataOfXpertQueyResult_returnsCorrectValues_withOrWithoutAdminInQuery);
    testXpertQueryResultCreation.add_test("getXpertRequestResults of XpertQueryResult returns correct values with valid and invalid xpertRequest.", &TestXpertQueryResultCreation::getXpertRequestResultsOfXpertQueryResult_returnsCorrectValues_withValidAndInvalidXpertRequest);
    testXpertQueryResultCreation.add_test("getXpertRequestData of XpertRequestResult returns correct values with full and minimal xpertRequest.", &TestXpertQueryResultCreation::getXpertRequestDataOfXpertRequestResult_returnsCorrectValues_withFullAndMinimalXpertRequest);
    testXpertQueryResultCreation.add_test("getComputationTime of XpertRequestResult returns correct DateTime.", &TestXpertQueryResultCreation::getComputationTimeOfXpertRequestResult_returnsCorrectDateTime);

    res = testXpertQueryResultCreation.run(argc, argv);
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

    testCovariateValidatorAndModelSelector.add_test("covariateValidatorAndModelSelector failure when treatment nullptr.", &TestCovariateValidatorAndModelSelector::covariateValidatorAndModelSelector_failure_whenTreatmentNullptr);
    testCovariateValidatorAndModelSelector.add_test("covariateValidatorAndModelSelector failure when no drug model for drug id.", &TestCovariateValidatorAndModelSelector::covariateValidatorAndModelSelector_failure_whenNoDrugModelForDrugId);
    testCovariateValidatorAndModelSelector.add_test("covariateValidatorAndModelSelector failure when treatment formulations and routes are different.", &TestCovariateValidatorAndModelSelector::covariateValidatorAndModelSelector_failure_whenTreatmentFormulationsAndRoutesAreDifferent);
    testCovariateValidatorAndModelSelector.add_test("covariateValidatorAndModelSelector failure with only one model with different formulation and route.", &TestCovariateValidatorAndModelSelector::covariateValidatorAndModelSelector_failure_withOnlyOneModelWithDifferentFormulationAndRoute);
    testCovariateValidatorAndModelSelector.add_test("covariateValidatorAndModelSelector success with one model with different and one model with same formulation and route.", &TestCovariateValidatorAndModelSelector::covariateValidatorAndModelSelector_success_withOneModelWithDifferentAndOneModelWithSameFormulationAndRoute);
    testCovariateValidatorAndModelSelector.add_test("covariateValidatorAndModelSelector failure with one model hard constraint not met.", &TestCovariateValidatorAndModelSelector::covariateValidatorAndModelSelector_failure_withOneModelHardConstraintNotMet);
    testCovariateValidatorAndModelSelector.add_test("covariateValidatorAndModelSelector success with one model hard constraint not met and other model ok.", &TestCovariateValidatorAndModelSelector::covariateValidatorAndModelSelector_success_withOneModelHardConstraintNotMetAndOtherModelOk);
    testCovariateValidatorAndModelSelector.add_test("covariateValidatorAndModelSelector success with one model soft constraint not met.", &TestCovariateValidatorAndModelSelector::covariateValidatorAndModelSelector_success_withOneModelSoftConstraintNotMet);
    testCovariateValidatorAndModelSelector.add_test("covariateValidatorAndModelSelector failure multiple birth date one model with age one model without.", &TestCovariateValidatorAndModelSelector::covariateValidatorAndModelSelector_failure_multipleBirthDateOneModelWithAgeOneModelWithout);
    testCovariateValidatorAndModelSelector.add_test("covariateValidatorAndModelSelector failure bad birth date data type one model with age one model without.", &TestCovariateValidatorAndModelSelector::covariateValidatorAndModelSelector_failure_badBirthDateDataTypeOneModelWithAgeOneModelWithout);
    testCovariateValidatorAndModelSelector.add_test("covariateValidatorAndModelSelector failure bad gist unit one model with gist one model without.", &TestCovariateValidatorAndModelSelector::covariateValidatorAndModelSelector_failure_badGistUnitOneModelWithGistOneModelWithout);
    testCovariateValidatorAndModelSelector.add_test("covariateValidatorAndModelSelector get the best model between three models and no tie.", &TestCovariateValidatorAndModelSelector::covariateValidatorAndModelSelector_getTheBestModel_betweenThreeModelsAndNoTie);
    testCovariateValidatorAndModelSelector.add_test("covariateValidatorAndModelSelector get the best model between two models with tie.", &TestCovariateValidatorAndModelSelector::covariateValidatorAndModelSelector_getTheBestModel_betweenTwoModelsWithTie);
    testCovariateValidatorAndModelSelector.add_test("covariateValidatorAndModelSelector failure when selected model not supporting requested language and english.", &TestCovariateValidatorAndModelSelector::covariateValidatorAndModelSelector_failure_whenSelectedModelNotSupportingRequestedLanguageAndEnglish);
    testCovariateValidatorAndModelSelector.add_test("covariateValidatorAndModelSelector get good translation with model that supports request language and model only english.", &TestCovariateValidatorAndModelSelector::covariateValidatorAndModelSelector_getGoodTranslations_withModelThatSupportsRequestLanguageAndModelOnlyEnglish);
    testCovariateValidatorAndModelSelector.add_test("getCovariateValidationResults returns correct values when covariateValidatorAndModelSelector success.", &TestCovariateValidatorAndModelSelector::getCovariateValidationResults_returnsCorrectValues_whenCovariateValidatorAndModelSelectorSuccess);


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

    testDoseValidator.add_test("doseValidator failure when treatment nullptr.", &TestDoseValidator::doseValidator_failure_whenTreatmentNullptr);
    testDoseValidator.add_test("doseValidator failure when drug model nullptr.", &TestDoseValidator::doseValidator_failure_whenDrugModelNullptr);
    testDoseValidator.add_test("getDoseValidationResults is empty when no dose in query.", &TestDoseValidator::getDoseValidationResults_isEmpty_whenNoDoseInQuery);
    testDoseValidator.add_test("doseValidationResult has warning when under dosing.", &TestDoseValidator::doseValidationResult_hasWarning_whenUnderDosing);
    testDoseValidator.add_test("doseValidationResult has warning when over dosing.", &TestDoseValidator::doseValidationResult_hasWarning_whenOverDosing);
    testDoseValidator.add_test("doseValidator failure when bad dose unit.", &TestDoseValidator::doseValidator_failure_whenBadDoseUnit);
    testDoseValidator.add_test("doseValidator failure when formulation and route not supported by model.", &TestDoseValidator::doseValidator_failure_whenFormulationAndRouteNotSupportedByModel);
    testDoseValidator.add_test("doseValidator returns correct values with all dosage type and multiple time range.", &TestDoseValidator::doseValidator_returnsCorrectValues_withAllDosageTypeAndMultipleTimeRange);


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

    testSampleValidator.add_test("sampleValidator failure when treatment nullptr.", &TestSampleValidator::sampleValidator_failure_whenTreatmentNullptr);
    testSampleValidator.add_test("sampleValidator failure when samples without dosages.", &TestSampleValidator::sampleValidator_failure_whenSamplesWithoutDosages);
    testSampleValidator.add_test("sampleValidator failure when drug model nullptr.", &TestSampleValidator::sampleValidator_failure_whenDrugModelNullptr);
    testSampleValidator.add_test("sampleValidationResult computes correct warning.", &TestSampleValidator::sampleValidationResult_computesCorrectWarning);
    testSampleValidator.add_test("findGroupPositionOver99Percentiles behaves correctly with given percentilesData and samples.", &TestSampleValidator::findGroupPositionOver99Percentiles_behavesCorrectly_withGivenPercentilesDataAndSamples);
    testSampleValidator.add_test("findGroupPositionOver99Percentiles throw invalid argument when sample unit conversion failure.", &TestSampleValidator::findGroupPositionOver99Percentiles_throwInvalidArgument_whenSampleUnitConversionFailure);
    testSampleValidator.add_test("findGroupPositionOver99Percentiles throw invalid argument when sample date not found in CycleData.", &TestSampleValidator::findGroupPositionOver99Percentiles_throwInvalidArgument_whenSampleDateNotFoundInCycleData);
    testSampleValidator.add_test("getSampleValidationResult is sorted when not empty.", &TestSampleValidator::getSampleValidationResult_isSorted_whenNotEmpty);

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

    /***********************************************************
     *                      RequestExecutor                    *
     ***********************************************************/

#if defined(test_requestexecutor)
    TestRequestExecutor testRequestExecutor;

    testRequestExecutor.add_test("Get an error when the adjustment trait is nullptr.", &TestRequestExecutor::errorWhenNoAdjustmentTrait);
    testRequestExecutor.add_test("Get an error when request failed.", &TestRequestExecutor::errorWhenRequestFailed);
    testRequestExecutor.add_test("Get correct adjustment data when request succeed.", &TestRequestExecutor::getCorrectAdjustmentDataWhenRequestSucceed);
    testRequestExecutor.add_test("Get correct statistics when request succeed.", &TestRequestExecutor::getCorrectStatisticsWhenRequestSucceed);
    testRequestExecutor.add_test("Get the typical, apriori and aposteriori parameters when the adjustment trait is aposteriori.", &TestRequestExecutor::getTypicalAprioriAposterioriParametersWhenAposterioriTrait);
    testRequestExecutor.add_test("Get the typical and apriori parameters when the adjustment trait is apriori.", &TestRequestExecutor::getTypicalAprioriParametersWhenAprioriTrait);

    res = testRequestExecutor.run(argc, argv);
    if (res != 0) {
        std::cout << "Request executor tests failed" << std::endl << std::endl;
        exit(1);
    } else {
        std::cout << "Request executor tests succeeded" << std::endl << std::endl;
    }
#endif


    return 0;
}
