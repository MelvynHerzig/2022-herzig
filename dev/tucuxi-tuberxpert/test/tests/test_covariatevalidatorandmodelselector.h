#ifndef TEST_COVARIATEVALIDATORANDMODELSELECTOR_H
#define TEST_COVARIATEVALIDATORANDMODELSELECTOR_H

#include <vector>
#include <memory>

#include "tuberxpert/result/xpertrequestresult.h"
#include "tuberxpert/result/xpertqueryresult.h"
#include "tuberxpert/utils/xpertutils.h"

#include "testutils.h"

#include "fructose/fructose.h"

/// \brief Tests for CovariateValidatorAndModelSelector from the GeneralXpertFlowStepProvider.
/// \date 27/05/2022
/// \author Herzig Melvyn
struct TestCovariateValidatorAndModelSelector : public fructose::test_base<TestCovariateValidatorAndModelSelector>
{
    /// \brief Check that there is an error if the treatment of an XpertRequestResult is nullptr
    ///        in CovariateValidatorAndModelSelector.
    ///        The CovariateValidatorAndDrugModelSelector must set the error in the XpertRequestResult,
    ///        shouldBeProcessed must return false and the drug model must always be nullptr.
    /// \param _testName Name of the test
    void covariateValidatorAndModelSelector_failure_whenTreatmentNullptr(const std::string& _testName);

    /// \brief The test loads an xpertRequest that asks for a drug that is not in the drug model
    ///        repository. The CovariateValidatorAndDrugModelSelector must set the error in the XpertRequestResult,
    ///        shouldBeProcessed must return false and the drug model must always be nullptr.
    /// \param _testName Name of the test
    void covariateValidatorAndModelSelector_failure_whenNoDrugModelForDrugId(const std::string& _testName);

    /// \brief Check that all formulations and routes of the xpert query are the same.
    ///        When it is not the case, the corresponding XpertRequestResult gets its error message set.
    ///        There are two requests, one that has a route that changes and the other has an absorption that changes.
    ///        Both must fail.
    /// \param _testName Name of the test
    void covariateValidatorAndModelSelector_failure_whenTreatmentFormulationsAndRoutesAreDifferent(const std::string& _testName);

    /// \brief If the model is incompatible in terms of formulation and route and is the only one
    ///        in the directory, check that the XpertRequestResult error is set and that no drug model is assigned.
    ///        Treatment absorption is defined as intravascular, which is not compatible with the
    ///        original imatinib drug model absorption.
    /// \param _testName Name of the test
    void covariateValidatorAndModelSelector_failure_withOnlyOneModelWithDifferentFormulationAndRoute(const std::string& _testName);

    /// \brief If the model is incompatible in terms of formulation and route but there is another compatible
    ///        in the directory, check that the XpertRequestResult error is not set and that the second model is chosen.
    ///        The query has an absorption model defined as extravascular. The first model is set to bolus (modified model of the imatinib)
    ///        and the second model to extra (original model of the imatinib).
    /// \param _testName Name of the test
    void covariateValidatorAndModelSelector_success_withOneModelWithDifferentAndOneModelWithSameFormulationAndRoute(const std::string& _testName);

    /// \brief If the model is incompatible in terms of constraints and it is the only model
    ///        in the directory, check that the XpertRequestResult error is set and that no drug model is assigned.
    ///        In this case, the patient is too old for the hard constraint age of the original model of imatinib.
    /// \param _testName Name of the test
    void covariateValidatorAndModelSelector_failure_withOneModelHardConstraintNotMet(const std::string& _testName);

    /// \brief If the model is incompatible in terms of constraints, but there is antoher compatible model
    ///        in the directory, check that the XpertRequestResult error is not set and that the second model is chosen.
    ///        The first model (original model of the imatinib) has the hard constraint of age not met. The second model
    ///        (modified model of the imatinib) hard age constraint has been modified to return true in all cases.
    /// \param _testName Name of the test
    void covariateValidatorAndModelSelector_success_withOneModelHardConstraintNotMetAndOtherModelOk(const std::string& _testName);

    /// \brief If the model is partially compatible in terms of constraints and is the only one
    ///        in the directory, check that the XpertRequestResult error is not set and that the model is chosen.
    ///        In this case, the patient is overweighted for the soft body weight constraint of
    ///        the original imatinib model.
    /// \param _testName Name of the test
    void covariateValidatorAndModelSelector_success_withOneModelSoftConstraintNotMet(const std::string& _testName);

    /// \brief The query defines several birth dates and the model requires an age.
    ///        This is not logical, so the XpertRequestResult gets its error message set and
    ///        is no more processable. It stops the search even if there are other models after.
    ///        The first model is the original imatinib with age
    ///        as a covariate and the second is a modified version without the age (which could have
    ///        been chosen). We need to return an error and stop the run because the first model may
    ///        be better than the second when corrected.
    /// \param _testName Name of the test
    void covariateValidatorAndModelSelector_failure_multipleBirthDateOneModelWithAgeOneModelWithout(const std::string& _testName);


    /// \brief The query sets one birth date with a wrong data type and the model requires an age.
    ///        This is not manageable, so it stops the search of the drug model even if there is a second
    ///        potentially valid model.
    ///        The first model is the original imatinib with age
    ///        as a covariate and the second is a modified version without the age (which could have
    ///        been chosen). We need to return an error and stop the run because the first model may
    ///        be better than the second when corrected.
    /// \param _testName Name of the test
    void covariateValidatorAndModelSelector_failure_badBirthDateDataTypeOneModelWithAgeOneModelWithout(const std::string& _testName);

    /// covariateValidatorAndModelSelector_getTheBestModel_betweenThreeModelsAndNoTie one gist with the wrong unit and the model requires this covariate.
    ///        This is not manageable, so stop the search of drug model even if there is a second
    ///        potentially valid model. The first model is the original imatinib model and the second
    ///        is a modified imatinib model that does not use gist as a covariate (which probably would
    ///        have worked). We need to return an error and stop the run because the first model may
    ///        be better than the second when corrected.
    /// \param _testName Name of the test
    void covariateValidatorAndModelSelector_failure_badGistUnitOneModelWithGistOneModelWithout(const std::string& _testName);

    /// \brief There is one query with 4 covariates (bodyweight3, bodyweight4 ... bodyweight6) and three versions
    ///        of the imatinib model:
    ///         - The first has two out of two definitions missing from query covariates (score 2) (bodyweight 1 & 2)
    ///         - The second has one out of two definitions missing from query covariates. The present covariate is
    ///           respecting the definition validation. (score 1) (bodyweight 3 & 4)
    ///         - The third has two out of two definitions present in query covariates. The present covariates
    ///           don't respect the definitions validation. (score 2) (bodyweight 5 & 6)
    ///        The second model must be chosen.
    ///        The models are custom imatinib models that extend the original model with bodyweight1/2/3/4/5/6.
    ///        For simplicity, we leave the original covariate definitions but ignore them since none of their value
    ///        are defined in the query.
    /// \param _testName Name of the test
    void covariateValidatorAndModelSelector_getTheBestModel_betweenThreeModelsAndNoTie(const std::string& _testName);

    /// \brief There is one query with 1 covariate (bodyweight 3) and two versions of the imatinib model:
    ///         - The first has one definition missing from query covariates (score 1) (bodyweight1)
    ///         - The second has one out of two definitions missing from query covariates.
    ///           The present covariate is respecting the definition validation. (score 1) (bodyweight2 & 3)
    ///        There is a tie, but the second model is chosen because it has the most covariate definitions.
    ///        The models are custom imatinib models that extend the original model with bodyweight1/2/3.
    ///        For simplicity, we leave the original covariate definitions but ignore them since none of their value
    ///        are defined in the query.
    /// \param _testName Name of the test
    void covariateValidatorAndModelSelector_getTheBestModel_betweenTwoModelsWithTie(const std::string& _testName);


    /// \brief Check that the CovariateValidatiorAndModelSelector fails when the selected drug model does
    ///        not support the requested language in its covariate definitions.
    ///        This test makes 3 requests requiring French on 3 different drugs whose drug model
    ///        covariates does not support french.
    ///        There are 3 request to test that it fails on the covariate name, the covariate description
    ///        and the covariate error message.
    ///        The 3 drug models are based on the original imatinib model but the languages of the first covariate
    ///        has been changed in 3 different positions.
    /// \param _testName Name of the test
    void covariateValidatorAndModelSelector_failure_whenSelectedModelNotSupportingRequestedLanguageAndEnglish(const std::string& _testName);

    /// \brief Check that the name, the description and the warning messages of a covariateValidationResult
    ///        are in the requested language or in English when the requested language is not available.
    ///        This test makes 2 requests requiring French for 2 different drugs.
    ///        Once time drug model supports French, one time it supports only English.
    ///        The first model is the original imatinib model and the second is a modified version.
    ///        This test only checks the body weight covariate validation.
    /// \param _testName Name of the test
    void covariateValidatorAndModelSelector_getGoodTranslations_withModelThatSupportsRequestLanguageAndModelOnlyEnglish(const std::string& _testName);


    /// \brief Check that the CovariateValidationResults are correctly defined when a valid model is found.
    ///        Some coariates are present several times, others not even once. This test is based
    ///        on the original imatinib model. In addition, the covariate results must be sorted
    ///        by name (in English as requested in the query) and then by measurement date.
    /// \param _testName Name of the test
    void getCovariateValidationResults_returnsCorrectValues_whenCovariateValidatorAndModelSelectorSuccess(const std::string& _testName);
};

#endif // TEST_COVARIATEVALIDATORANDMODELSELECTOR_H
