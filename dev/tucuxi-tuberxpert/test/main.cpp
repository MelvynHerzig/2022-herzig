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

#if defined(test_modelselector)
#include "test_modelselector.h"
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
    TestLanguageManager languageTests;

    languageTests.add_test("GetInstance", &TestLanguageManager::retrieveDictionary);
    languageTests.add_test("translate", &TestLanguageManager::wordTranslation);

    res = languageTests.run(argc, argv);
    if (res != 0) {
        std::cout << "Language module tests failed" << std::endl << std::endl;
        exit(1);
    }
    else {
        std::cout << "Language module tests succeeded" << std::endl << std::endl;
    }
#endif

    /***********************************************************
     *                    XpertQueryImport                     *
     ***********************************************************/


#if defined(test_query)
    TestXpertQueryImport queryImportTests;

    queryImportTests.add_test("Gets complete admin element", &TestXpertQueryImport::retrieveCompleteAdmin);
    queryImportTests.add_test("Gets no admin element", &TestXpertQueryImport::retrieveNoAdmin);
    queryImportTests.add_test("Gets empty admin element", &TestXpertQueryImport::retrieveEmptyAdmin);
    queryImportTests.add_test("Gets minimal persons", &TestXpertQueryImport::retrieveMinimalPerson);
    queryImportTests.add_test("Gets minimal institutes", &TestXpertQueryImport::retrieveMinimalInstitute);
    queryImportTests.add_test("Gets minimal coordinates", &TestXpertQueryImport::retrieveMinimalCoordinates);
    queryImportTests.add_test("Gets an error when missing mandatory values in mandator person", &TestXpertQueryImport::errorWhenMissingMandatoryInMandatorPerson);
    queryImportTests.add_test("Gets an error when missing mandatory values in mandator institute", &TestXpertQueryImport::errorWhenMissingMandatoryInMandatorInstitute);
    queryImportTests.add_test("Gets an error when missing mandatory values in patient person", &TestXpertQueryImport::errorWhenMissingMandatoryInPatientPerson);
    queryImportTests.add_test("Gets an error when missing mandatory values in patient institute", &TestXpertQueryImport::errorWhenMissingMandatoryInPatientInstitute);
    queryImportTests.add_test("Gets complete request xpert element", &TestXpertQueryImport::retrieveCompleteRequestXpert);
    queryImportTests.add_test("Gets default request xpert element", &TestXpertQueryImport::retrieveDefaultRequestXpert);
    queryImportTests.add_test("Gets an error when missing request xpert", &TestXpertQueryImport::errorWhenNoRequestXpert);
    queryImportTests.add_test("Gets an error when missing mandatory values in request xpert", &TestXpertQueryImport::errorWhenMissingMandatoryRequestXpert);
    queryImportTests.add_test("Gets an error when the xml document can't be created from string", &TestXpertQueryImport::errorWhenCreatingXmlDocumentFromString);
    queryImportTests.add_test("Gets an error when the xml document can't be created from file", &TestXpertQueryImport::errorWhenCreatingXmlDocumentFromFile);

    res = queryImportTests.run(argc, argv);
    if (res != 0) {
        std::cout << "Query import module tests failed" << std::endl << std::endl;
        exit(1);
    }
    else {
        std::cout << "Query import module tests succeeded" << std::endl << std::endl;
    }
#endif


    /***********************************************************
     *                XpertQueryToCoreExtractor                *
     ***********************************************************/

#if defined(test_xpertquerytocoreextractor)
    TestXpertQueryToCoreExtractor queryToCoreExtractorTests;

    queryToCoreExtractorTests.add_test("Extracts good drug treatment", &TestXpertQueryToCoreExtractor::extractGoodDrugTreatment);
    queryToCoreExtractorTests.add_test("Extracts bad drug treatment", &TestXpertQueryToCoreExtractor::extractBadDrugTreatment);

    res = queryToCoreExtractorTests.run(argc, argv);
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
    TestXpertResultCreation xpertResultCreationTests;

    xpertResultCreationTests.add_test("Query pointer invalidation", &TestXpertResultCreation::queryPtrInvalidation);
    xpertResultCreationTests.add_test("Retrieves administrative data", &TestXpertResultCreation::retrieveAdministrativeData);
    xpertResultCreationTests.add_test("Retrieves xpert request results", &TestXpertResultCreation::retrieveXpertRequestResult);
    xpertResultCreationTests.add_test("Retrieves generation date", &TestXpertResultCreation::retrieveDate);

    res = xpertResultCreationTests.run(argc, argv);
    if (res != 0) {
        std::cout << "XpertResult creation tests failed" << std::endl << std::endl;
        exit(1);
    }
    else {
        std::cout << "XpertResult creation tests succeeded" << std::endl << std::endl;
    }
#endif

    /***********************************************************
     *                     ModelSelector                       *
     ***********************************************************/

#if defined(test_modelselector)
    TestModelSelector modelSelectorTests;

    modelSelectorTests.add_test("Gets an error when no treatment.", &TestModelSelector::errorWhenNoTreatment);
    modelSelectorTests.add_test("No corresponding model for drug id", &TestModelSelector::noCorrespondingModelForDrugId);
    modelSelectorTests.add_test("No result if the query formulations and routes are not equal.", &TestModelSelector::noResultIfQueryFormulationsAndRoutesAreNotEqual);
    modelSelectorTests.add_test("No result if only one model with incompatible formulation and route.", &TestModelSelector::noResultOneModelIncompatibleFormulationAndRoute);
    modelSelectorTests.add_test("Gets one result if there are one model with incompatible formulation and route and a second compatible.", &TestModelSelector::getResultOneModelIncompatibleFormulationAndRouteSecondCompatible);
    modelSelectorTests.add_test("No result if one model with incompatible hard constraint.", &TestModelSelector::noResultOneModelIncompatibleConstraint);
    modelSelectorTests.add_test("Gets one result if there are one model with hard constraint not respected and a second compatible.", &TestModelSelector::getResultOneModelIncompatibleConstraintSecondCompatible);
    modelSelectorTests.add_test("Gets one result if one model with partially incompatible soft constraint.", &TestModelSelector::getResultOneModelPartiallyCompatibleSoftConstraint);
    modelSelectorTests.add_test("No result when multiple birthdates even with models left.", &TestModelSelector::noResultMultipleBirthdate);
    modelSelectorTests.add_test("No result when birthdate with bad datatype even with models left.", &TestModelSelector::noResultBadDatatypeBirthdate);
    modelSelectorTests.add_test("No result when gist with bad unit even with models left.", &TestModelSelector::noResultBadUnitGist);
    modelSelectorTests.add_test("Gets the best result out of 3 models without tie.", &TestModelSelector::getResultOutofThreeNoTie);
    modelSelectorTests.add_test("Gets the best result out of 2 models with tie.", &TestModelSelector::getResultOutofTwoTie);
    modelSelectorTests.add_test("Selection failed when selected model covariates does not support request language without english fallbabk.", &TestModelSelector::requestFailDefinitionsNotSupportingLanguageWithoutEnglishBackup);
    modelSelectorTests.add_test("Checks that the translations match the requests languages.", &TestModelSelector::getGoodCovariateWarningTranslation);
    modelSelectorTests.add_test("Gets and checks the content of the covariate results.", &TestModelSelector::getCovariateResults);


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
     *                 AdjustmentTraitCreator                  *
     ***********************************************************/

#if defined(test_adjustmenttraitcreator)
    TestAdjustmentTraitCreator testAdjustmentTraitCreator;

    testAdjustmentTraitCreator.add_test("Get an error when no treatment.", &TestAdjustmentTraitCreator::errorWhenNoTreatment);
    testAdjustmentTraitCreator.add_test("Get an error when no drug model.", &TestAdjustmentTraitCreator::errorWhenNoDrugModel);
    testAdjustmentTraitCreator.add_test("Get the correct number of points per hour.", &TestAdjustmentTraitCreator::getTheCorrectPointPerHour);
    testAdjustmentTraitCreator.add_test("Get the correct computing option.", &TestAdjustmentTraitCreator::getTheCorrectComputingOption);

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
