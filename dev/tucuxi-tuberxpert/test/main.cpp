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

#if defined(test_xpertresultcreation)
#include "test_xpertresultcreation.h"
#endif

#if defined(test_generalcovariatevalidatorandmodelselector)
#include "test_generalcovariatevalidatorandmodelselector.h"
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

#if defined(test_generaladjustmenttraitcreator)
#include "test_generaladjustmenttraitcreator.h"
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

    xpertUtilsTests.add_test("Converts output lang to string", &TestXpertUtils::convertOutputLangToString);
    xpertUtilsTests.add_test("Converts double to string", &TestXpertUtils::convertDoubleToString);
    xpertUtilsTests.add_test("Get a translatable string with english fallback.", &TestXpertUtils::getStringFromTranslatableWithFallback);

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
     *                  XpertResult creation                   *
     ***********************************************************/

#if defined(test_xpertresultcreation)
    TestXpertResultCreation testXpertResultCreation;

    testXpertResultCreation.add_test("Query pointer invalidation", &TestXpertResultCreation::queryPtrInvalidation);
    testXpertResultCreation.add_test("Retrieves administrative data", &TestXpertResultCreation::retrieveAdministrativeData);
    testXpertResultCreation.add_test("Retrieves xpert request results", &TestXpertResultCreation::retrieveXpertRequestResult);
    testXpertResultCreation.add_test("Retrieves generation date", &TestXpertResultCreation::retrieveDate);

    res = testXpertResultCreation.run(argc, argv);
    if (res != 0) {
        std::cout << "XpertResult creation tests failed" << std::endl << std::endl;
        exit(1);
    }
    else {
        std::cout << "XpertResult creation tests succeeded" << std::endl << std::endl;
    }
#endif

    /***********************************************************
     *       GeneralCovariateValidatorAndModelSelector         *
     ***********************************************************/

#if defined(test_generalcovariamodelselector)
    TestGeneralCovariateValidatorAndModelSelector testGeneralCovariateValidatorAndModelSelector;

    testGeneralCovariateValidatorAndModelSelector.add_test("Gets an error when no treatment.", &TestModelSelector::errorWhenNoTreatment);
    testGeneralCovariateValidatorAndModelSelector.add_test("No corresponding model for drug id", &TestModelSelector::noCorrespondingModelForDrugId);
    testGeneralCovariateValidatorAndModelSelector.add_test("No result if the query formulations and routes are not equal.", &TestModelSelector::noResultIfQueryFormulationsAndRoutesAreNotEqual);
    testGeneralCovariateValidatorAndModelSelector.add_test("No result if only one model with incompatible formulation and route.", &TestModelSelector::noResultOneModelIncompatibleFormulationAndRoute);
    testGeneralCovariateValidatorAndModelSelector.add_test("Gets one result if there are one model with incompatible formulation and route and a second compatible.", &TestModelSelector::getResultOneModelIncompatibleFormulationAndRouteSecondCompatible);
    testGeneralCovariateValidatorAndModelSelector.add_test("No result if one model with incompatible hard constraint.", &TestModelSelector::noResultOneModelIncompatibleConstraint);
    testGeneralCovariateValidatorAndModelSelector.add_test("Gets one result if there are one model with hard constraint not respected and a second compatible.", &TestModelSelector::getResultOneModelIncompatibleConstraintSecondCompatible);
    testGeneralCovariateValidatorAndModelSelector.add_test("Gets one result if one model with partially incompatible soft constraint.", &TestModelSelector::getResultOneModelPartiallyCompatibleSoftConstraint);
    testGeneralCovariateValidatorAndModelSelector.add_test("No result when multiple birthdates even with models left.", &TestModelSelector::noResultMultipleBirthdate);
    testGeneralCovariateValidatorAndModelSelector.add_test("No result when birthdate with bad datatype even with models left.", &TestModelSelector::noResultBadDatatypeBirthdate);
    testGeneralCovariateValidatorAndModelSelector.add_test("No result when gist with bad unit even with models left.", &TestModelSelector::noResultBadUnitGist);
    testGeneralCovariateValidatorAndModelSelector.add_test("Gets the best result out of 3 models without tie.", &TestModelSelector::getResultOutofThreeNoTie);
    testGeneralCovariateValidatorAndModelSelector.add_test("Gets the best result out of 2 models with tie.", &TestModelSelector::getResultOutofTwoTie);
    testGeneralCovariateValidatorAndModelSelector.add_test("Selection failed when selected model covariates does not support request language without english fallbabk.", &TestModelSelector::requestFailDefinitionsNotSupportingLanguageWithoutEnglishBackup);
    testGeneralCovariateValidatorAndModelSelector.add_test("Checks that the translations match the requests languages.", &TestModelSelector::getGoodCovariateWarningTranslation);
    testGeneralCovariateValidatorAndModelSelector.add_test("Gets and checks the content of the covariate results.", &TestModelSelector::getCovariateResults);


    res = modelSelectorTests.run(argc, argv);
    if (res != 0) {
        std::cout << "Model selector tests failed" << std::endl << std::endl;
        exit(1);
    }
    else {
        std::cout << "Model selector tests succeeded" << std::endl << std::endl;
    }
#endif


    /***********************************************************
     *                     DoseValidator                       *
     ***********************************************************/

#if defined(test_dosevalidator)
    TestDoseValidator doseValidatorTests;

    doseValidatorTests.add_test("Gets an error when no treatment.", &TestDoseValidator::errorWhenNoTreatment);
    doseValidatorTests.add_test("Gets an error when no drug model.", &TestDoseValidator::errorWhenNoDrugModel);
    doseValidatorTests.add_test("The resulting map is empty when there is no dosage.", &TestDoseValidator::emptyResultWhenNoDosages);
    doseValidatorTests.add_test("The resulting dose result contains a warning when underdosing.", &TestDoseValidator::warningUnderdose);
    doseValidatorTests.add_test("The resulting dose result contains a warning when overdosing.", &TestDoseValidator::warningOverdose);
    doseValidatorTests.add_test("Gets an error when the unit conversion fails.", &TestDoseValidator::errorFailUnitConversion);
    doseValidatorTests.add_test("Gets an error when the formulation and route not supported by model.", &TestDoseValidator::errorFormulationAndRouteNotSupported);
    doseValidatorTests.add_test("Checks that result is fine with multiple dosage type and timerange.", &TestDoseValidator::multipleDosageTypeAndDosageTimeRange);


    res = doseValidatorTests.run(argc, argv);
    if (res != 0) {
        std::cout << "Dose validator tests failed" << std::endl << std::endl;
        exit(1);
    } else {
        std::cout << "Dose validator tests succeeded" << std::endl << std::endl;
    }
#endif

    /***********************************************************
     *                    SampleValidator                      *
     ***********************************************************/

#if defined(test_samplevalidator)
    TestSampleValidator sampleValidatorTests;

    sampleValidatorTests.add_test("Gets an error when no treatment.", &TestSampleValidator::errorWhenNoTreatment);
    sampleValidatorTests.add_test("Gets an error when there are samples but no dosage.", &TestSampleValidator::errorWhenSamplesButNoDosage);
    sampleValidatorTests.add_test("Gets an error when no drug model.", &TestSampleValidator::errorWhenNoDrugModel);
    sampleValidatorTests.add_test("Gets warnings for a given group position over 99 percentiles.", &TestSampleValidator::warningForGroupPositionOver99Percentiles);
    sampleValidatorTests.add_test("Gets some group positions over 99 percentile.", &TestSampleValidator::findGroupPositionOver99Percentiles);
    sampleValidatorTests.add_test("Gets an exception when the sample unit cannot be converted.", &TestSampleValidator::getExceptionUnitConversion);
    sampleValidatorTests.add_test("Gets an exception when the sample date cannot be found.", &TestSampleValidator::getExceptionDateNotFound);
    sampleValidatorTests.add_test("Gets the expected amount of samples results.", &TestSampleValidator::getTheExpectedAmountOfResults);

    res = sampleValidatorTests.run(argc, argv);
    if (res != 0) {
        std::cout << "Sample validator tests failed" << std::endl << std::endl;
        exit(1);
    } else {
        std::cout << "Sample validator tests succeeded" << std::endl << std::endl;
    }
#endif

    /***********************************************************
     *                    TargetValidator                      *
     ***********************************************************/

#if defined(test_dosevalidator)
    TestTargetValidator targetValidatorTests;

    targetValidatorTests.add_test("Gets an error when no treatment.", &TestTargetValidator::errorWhenNoTreatment);
    targetValidatorTests.add_test("Gets an error when no drug model.", &TestTargetValidator::errorWhenNoDrugModel);
    targetValidatorTests.add_test("No error when no patient's target.", &TestTargetValidator::noErrorWhenNoTarget);
    targetValidatorTests.add_test("Gets an error when two patient's targets have the same active moiety and type.", &TestTargetValidator::errorWhenTargetsWithSameActiveMoietyAndType);
    targetValidatorTests.add_test("No error when two patient's targets have the same active moiety but different type.", &TestTargetValidator::noErrorWhenTargetsWithSameActiveMoietyButDifferentType);
    targetValidatorTests.add_test("Gets an error when one patient's target have a different active moiety than the ones from drug model.", &TestTargetValidator::errorWhenTargetsWithActiveMoietyNotInDrugModel);


    res = targetValidatorTests.run(argc, argv);
    if (res != 0) {
        std::cout << "Target validator tests failed" << std::endl << std::endl;
        exit(1);
    } else {
        std::cout << "Target validator tests succeeded" << std::endl << std::endl;
    }
#endif

    /***********************************************************
     *              GeneralAdjustmentTraitCreator              *
     ***********************************************************/

#if defined(test_generaladjustmenttraitcreator)
    TestGeneralAdjustmentTraitCreator testGeneralAdjustmentTraitCreator;

    testGeneralAdjustmentTraitCreator.add_test("Get an error when no treatment.", &TestGeneralAdjustmentTraitCreator::errorWhenNoTreatment);
    testGeneralAdjustmentTraitCreator.add_test("Get an error when no drug model.", &TestGeneralAdjustmentTraitCreator::errorWhenNoDrugModel);
    testGeneralAdjustmentTraitCreator.add_test("The number of points per hour is twenty.", &TestGeneralAdjustmentTraitCreator::nbPointsPerJourIsTwenty);
    testGeneralAdjustmentTraitCreator.add_test("The computing option retrieves all (statistics, covariates and parameters) and is on all active moieties", &TestGeneralAdjustmentTraitCreator::computingOptionIsRetrieveAllAndAllActiveMoieties);
    testGeneralAdjustmentTraitCreator.add_test("The computing option prediction parameters type is Apriori when no dosage and no sample", &TestGeneralAdjustmentTraitCreator::computingOptionIsAprioriWhenNoDosageAndNoSample);
    testGeneralAdjustmentTraitCreator.add_test("The computing option prediction parameters type is Apriori when dosage but no sample", &TestGeneralAdjustmentTraitCreator::computingOptionIsAprioriWhenDosageButNoSample);
    testGeneralAdjustmentTraitCreator.add_test("The computing option prediction parameters type is Aposteriori when dosage and sample", &TestGeneralAdjustmentTraitCreator::computingOptionIsAposterioriWhenDosageAndSample);
    testGeneralAdjustmentTraitCreator.add_test("The adjustment time is the adjustment time of the request when set manually.", &TestGeneralAdjustmentTraitCreator::adjustmentTimeIsRequestAdjustmentTimeWhenManuallySet);
    testGeneralAdjustmentTraitCreator.add_test("The adjustment time is the computing time plus one hour when there is no dosage history and it is not manually set.", &TestGeneralAdjustmentTraitCreator::adjustmentTimeIsComputingTimePlusOneHourWithoutDosageHistoryAndNotManuallySet);
    testGeneralAdjustmentTraitCreator.add_test("The adjustment time is the computing time plus one hour when there is a dosage history in the future and it is not manually set.", &TestGeneralAdjustmentTraitCreator::adjustmentTimeIsComputingTimePlusOneHourWithFutureDosageHistoryAndNotManuallySet);
    testGeneralAdjustmentTraitCreator.add_test("The adjustment time is the next intake time when there is an ongoing dosage history and not manually set.", &TestGeneralAdjustmentTraitCreator::adjustmentTimeIsNextIntakeTimeWhenOngoingDosageHistoryAndNotManuallySet);
    testGeneralAdjustmentTraitCreator.add_test("The adjustment time is half life approximated when the dosage history is over and not manually set.", &TestGeneralAdjustmentTraitCreator::adjustmentTimeIsHalfLifeApproximatedWhenDosageHistoryOverAndNotManuallySet);
    testGeneralAdjustmentTraitCreator.add_test("The start and end date times are the dosage history start plus the standard treatment duration when there are an on ongoing standard treatment and a dosage history.", &TestGeneralAdjustmentTraitCreator::startEndDatesAreDosageHistoryStartPlusStandardTreatmentDurationWhenOngoingStandardTreatmentAndDosageHistory);
    testGeneralAdjustmentTraitCreator.add_test("The start and end date times are computation time plus the standard treatment duration when there is a standard treatment but no dosage history.", &TestGeneralAdjustmentTraitCreator::startEndDatesAreDosageHistoryStartPlusStandardTreatmentDurationWhenOngoingStandardTreatmentAndDosageHistory);
    testGeneralAdjustmentTraitCreator.add_test("Get an error when there is a standard treatment but it is already over.", &TestGeneralAdjustmentTraitCreator::errorWhenStandardTreatmentIsOverBeforeComputationTime);
    testGeneralAdjustmentTraitCreator.add_test("The start and end date times are defined based on the adjustment time when there is no standard treatment.", &TestGeneralAdjustmentTraitCreator::startEndDatesAreDefinedOnAdjustmentTimeWhenNoStandardTreatment);
    testGeneralAdjustmentTraitCreator.add_test("The best candidates option is best dosage per interval.", &TestGeneralAdjustmentTraitCreator::bestCandidatesOptionIsBestDosagePerInterval);
    testGeneralAdjustmentTraitCreator.add_test("The loading option is retrieved from request when set.", &TestGeneralAdjustmentTraitCreator::loadingOptionIsRetrievedFromRequestWhenSet);
    testGeneralAdjustmentTraitCreator.add_test("The loading option is retrieved from drug model when not set.", &TestGeneralAdjustmentTraitCreator::loadingOptionIsRetrievedFromDrugModelWhenNotSet);
    testGeneralAdjustmentTraitCreator.add_test("The rest period option is retrieved from request when set.", &TestGeneralAdjustmentTraitCreator::restPeriodOptionIsRetrievedFromRequestWhenSet);
    testGeneralAdjustmentTraitCreator.add_test("The rest period option is retrieved from drug model when not set.", &TestGeneralAdjustmentTraitCreator::restPeriodOptionIsRetrievedFromDrugModelWhenNotSet);
    testGeneralAdjustmentTraitCreator.add_test("The steady state target option is within treatment time range when there is a standard treatment.", &TestGeneralAdjustmentTraitCreator::steadyStateTargetOptionIsWithinTreatmentTimeRangeWhenStandardTreatment);
    testGeneralAdjustmentTraitCreator.add_test("The steady state target option is at steady state when there is no standard treatment.", &TestGeneralAdjustmentTraitCreator::steadyStateTargetOptionIsAtSteadyStateWhenNoStandardTreatment);
    testGeneralAdjustmentTraitCreator.add_test("The target extraction option is retrieved from request when set.", &TestGeneralAdjustmentTraitCreator::targetExtractionOptionIsRetrievedFromRequestWhenSet);
    testGeneralAdjustmentTraitCreator.add_test("The target extraction option is population values when not set.", &TestGeneralAdjustmentTraitCreator::targetExtractionOptionIsDefinitionIfNoIndividualTargetWhenNotSet);
    testGeneralAdjustmentTraitCreator.add_test("The formulation and route selection option is retrieved from request when set.", &TestGeneralAdjustmentTraitCreator::formulationAndRouteSelectionOptionIsRetrievedFromRequestWhenSet);
    testGeneralAdjustmentTraitCreator.add_test("The formulation and route selection option is last formulation and route when not set.", &TestGeneralAdjustmentTraitCreator::formulationAndRouteSelectionOptionIsLastFormulationAndRouteWhenNotSet);


    res = testGeneralAdjustmentTraitCreator.run(argc, argv);
    if (res != 0) {
        std::cout << "Adjustment trait creator tests failed" << std::endl << std::endl;
        exit(1);
    } else {
        std::cout << "Adjustment trait creator tests succeeded" << std::endl << std::endl;
    }
#endif


    return 0;
}
