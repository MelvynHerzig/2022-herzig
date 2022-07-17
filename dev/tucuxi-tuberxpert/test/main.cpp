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

    testTargetValidator.add_test("targetValidator failure whent treatment nullptr.", &TestTargetValidator::targetValidator_failure_whenTreatmentNullptr);
    testTargetValidator.add_test("targetValidator failure when drug model nullptr.", &TestTargetValidator::targetValidator_failure_whenDrugModelNullptr);
    testTargetValidator.add_test("targetValidator success when no target in treatment.", &TestTargetValidator::targetValidator_success_whenNoTargetInTreatment);
    testTargetValidator.add_test("targetValidator failure when target with same active moiety and type in treatment.", &TestTargetValidator::targetValidator_failure_whenTargetWithSameActiveMoietyAndTypeInTreatment);
    testTargetValidator.add_test("targetValidator success when target with same active moiety but different type in treatment.", &TestTargetValidator::targetValidator_success_whenTargetWithSameActiveMoietyButDifferentTypeInTreatment);
    testTargetValidator.add_test("targetValidator failure when treatment target with active moiety not in drug model.", &TestTargetValidator::targetValidator_failure_whenTreatmentTargetWithActiveMoietyNotInDrugModel);


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

    testAdjustmentTraitCreator.add_test("adjustmentTraitCreator failure when treatment nullptr.", &TestAdjustmentTraitCreator::adjustmentTraitCreator_failure_whenTreatmentNullptr);
    testAdjustmentTraitCreator.add_test("adjustmentTraitCreator failure when drug model nullptr.", &TestAdjustmentTraitCreator::adjustmentTraitCreator_failure_whenDrugModelNullptr);
    testAdjustmentTraitCreator.add_test("adjustmentTraitCreator sets 20 points per hour.", &TestAdjustmentTraitCreator::adjustmentTraitCreator_sets20PointsPerHour);
    testAdjustmentTraitCreator.add_test("adjustmentTraitCreator sets good fixed computing options.", &TestAdjustmentTraitCreator::adjustmentTraitCreator_setsGoodFixedComputingOptions);
    testAdjustmentTraitCreator.add_test("adjustmentTraitCreator sets a priori parameter type when no dosage and no sample.", &TestAdjustmentTraitCreator::adjustmentTraitCreator_setsAprioriParameterType_whenNoDosageAndNoSample);
    testAdjustmentTraitCreator.add_test("adjustmentTraitCreator sets a priori parameter type when dosages but no sample.", &TestAdjustmentTraitCreator::adjustmentTraitCreator_setsAprioriParameterType_whenDosagesButNoSample);
    testAdjustmentTraitCreator.add_test("adjustmentTraitCreator sets a posteriori parameter type when dosages and samples.", &TestAdjustmentTraitCreator::adjustmentTraitCreator_setsAposterioriParameterType_whenDosagesAndSamples);
    testAdjustmentTraitCreator.add_test("adjustmentTraitCreator sets correct adjustment time when set in XpertRequest.", &TestAdjustmentTraitCreator::adjustmentTraitCreator_setsCorrectAdjustmentTime_whenSetInXpertRequest);
    testAdjustmentTraitCreator.add_test("adjustmentTraitCreator sets adjustment time to computing time plus one hour when not set in XpertRequest and no dosage history.", &TestAdjustmentTraitCreator::adjustmentTraitCreator_setsAdjustmentTimeToComputingTimePlusOneHour_whenNotSetInXpertRequestAndNoDosageHistory);
    testAdjustmentTraitCreator.add_test("adjustmentTraitCreator sets adjustment time to computing time plus one hour when not set in XpertRequest and future dosage hstory.", &TestAdjustmentTraitCreator::adjustmentTraitCreator_setsAdjustmentTimeToComputingTimePlusOneHour_whenNotSetInXpertRequestAndFutureDosageHistory);
    testAdjustmentTraitCreator.add_test("adjustmentTraitCreator sets adjustment time to next intake time when not set in XpertRequest and ongoing dosage history.", &TestAdjustmentTraitCreator::adjustmentTraitCreator_setsAdjustmentTimeToNextIntakeTime_whenNotSetInXpertRequestAndOngoingDosageHistory);
    testAdjustmentTraitCreator.add_test("adjustmentTraitCreator sets adjustment time with half-life approximation when not set in XpertRequest and completed dosage history.", &TestAdjustmentTraitCreator::adjustmentTraitCreator_setsAdjustmentTimeWithHalfLifeApproximation_whenNotSetInXpertRequestAndCompletedDosageHistory);
    testAdjustmentTraitCreator.add_test("adjustmentTraitCreator sets start and end dates correctly when standard treatment and dosage history.", &TestAdjustmentTraitCreator::adjustmentTraitCreator_setsStartAndEndDatesCorrectly_whenStandardTreatmentAndDosageHistory);
    testAdjustmentTraitCreator.add_test("adjustmentTraitCreator sets start and end dates correctly when standard treatment but no dosage history.", &TestAdjustmentTraitCreator::adjustmentTraitCreator_setsStartAndEndDatesCorrectly_whenStandardTreatmentButNoDosageHistory);
    testAdjustmentTraitCreator.add_test("adjustmentTraitCreator failure when standard treatment is over before adjustment time.", &TestAdjustmentTraitCreator::adjustmentTraitCreator_failure_whenStandardTreatmentIsOverBeforeAdjustmentTime);
    testAdjustmentTraitCreator.add_test("adjustmentTraitCreator sets start end dates on adjustment time when no standard treatment.", &TestAdjustmentTraitCreator::adjustmentTraitCreator_setsStartEndDatesOnAdjustmentTime_whenNoStandardTreatment);
    testAdjustmentTraitCreator.add_test("adjustmentTraitCreator best candidates option is best dosage per interval.", &TestAdjustmentTraitCreator::adjustmentTraitCreator_bestCandidatesOptionIsBestDosagePerInterval);
    testAdjustmentTraitCreator.add_test("adjustmentTraitCreator sets correct loading option when defined in XpertRequest.", &TestAdjustmentTraitCreator::adjustmentTraitCreator_setsCorrectLoadingOption_whenDefinedInXpertRequest);
    testAdjustmentTraitCreator.add_test("adjustmentTraitCreator sets correct loading option when not defined in XpertRequest.", &TestAdjustmentTraitCreator::adjustmentTraitCreator_setsCorrectLoadingOption_whenNotDefinedInXpertRequest);
    testAdjustmentTraitCreator.add_test("adjustmentTraitCreator sets correct rest period option when defined in XpertRequest.", &TestAdjustmentTraitCreator::adjustmentTraitCreator_setsCorrectRestPeriodOption_whenDefinedInXpertRequest);
    testAdjustmentTraitCreator.add_test("adjustmentTraitCreator sets correct rest period option when not defined in XpertRequest.", &TestAdjustmentTraitCreator::adjustmentTraitCreator_setsCorrectRestPeriodOption_whenNotDefinedInXpertRequest);
    testAdjustmentTraitCreator.add_test("adjustmentTraitCreator sets steady state target option to within treatment time range when standard treatment.", &TestAdjustmentTraitCreator::adjustmentTraitCreator_setsSteadyStateTargetOptionToWithinTreatmentTimeRange_whenStandardTreatment);
    testAdjustmentTraitCreator.add_test("adjustmentTraitCreator sets steady state target option to at steady state when no standard treatment.", &TestAdjustmentTraitCreator::adjustmentTraitCreator_setsSteadyStateTargetOptionToAtSteadyState_whenNoStandardTreatment);
    testAdjustmentTraitCreator.add_test("adjustmentTraitCreator sets correct target extraction option when set in XpertRequest.", &TestAdjustmentTraitCreator::adjustmentTraitCreator_setsCorrectTargetExtractionOption_whenSetInXpertRequest);
    testAdjustmentTraitCreator.add_test("adjustmentTraitCreator sets correct target extraction option when not set in XpertRequest.", &TestAdjustmentTraitCreator::adjustmentTraitCreator_setsCorrectTargetExtractionOption_whenNotSetInXpertRequest);
    testAdjustmentTraitCreator.add_test("adjustmentTraitCreator sets correct formulation and route selection option when set in XpertRequest.", &TestAdjustmentTraitCreator::adjustmentTraitCreator_setsCorrectFormulationAndRouteSelectionOption_whenSetInXpertRequest);
    testAdjustmentTraitCreator.add_test("adjustmentTraitCreator sets correct formulation and route selection option when not set in XpertRequest.", &TestAdjustmentTraitCreator::adjustmentTraitCreator_setsCorrectFormulationAndRouteSelectionOption_whenNotSetInXpertRequest);
    testAdjustmentTraitCreator.add_test("getLastIntake returns correct values when dosage history.", &TestAdjustmentTraitCreator::getLastIntake_returnsCorrectValues_whenDosageHistory);
    testAdjustmentTraitCreator.add_test("getLastIntake returns nullptr when no dosage history.", &TestAdjustmentTraitCreator::getLastIntake_returnsNullptr_whenNoDosageHistory);


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

    testRequestExecutor.add_test("requestExecutor failure when adjustment trait nullptr.", &TestRequestExecutor::requestExecutor_failure_whenAdjustmentTraitNullptr);
    testRequestExecutor.add_test("requestExecutor failure when drug model nullptr.", &TestRequestExecutor::requestExecutor_failure_whenDrugModelNullptr);
    testRequestExecutor.add_test("requestExecutor failure when request execution failed.", &TestRequestExecutor::requestExecutor_failure_whenRequestExecutionFailed);
    testRequestExecutor.add_test("requestExecutor gets correct adjustment data when request execution succeed.", &TestRequestExecutor::requestExecutor_getsCorrectAdjustmentData_whenRequestExecutionSucceed);
    testRequestExecutor.add_test("requestExecutor gets the statistics when request execution succeed.", &TestRequestExecutor::requestExecutor_getsTheStatistics_whenRequestExecutionSucceed);
    testRequestExecutor.add_test("requestExecutor gets typical apriori aposteriori parameters when aposteriori trait.", &TestRequestExecutor::requestExecutor_getsTypicalAprioriAposterioriParameters_whenAposterioriTrait);
    testRequestExecutor.add_test("requestExecutor gets typical apriori parameters when apriori trait.", &TestRequestExecutor::requestExecutor_getsTypicalAprioriParameters_whenAprioriTrait);

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
