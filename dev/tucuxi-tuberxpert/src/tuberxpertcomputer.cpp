#include "tuberxpertcomputer.h"

#include <memory>
#include <fstream>

#include "tucucommon/loggerhelper.h"
#include "tucucore/drugmodelrepository.h"
#include "tucucore/computingcomponent.h"

#include "tuberxpert/language/languagemanager.h"
#include "tuberxpert/query/xpertqueryimport.h"
#include "tuberxpert/query/xpertquerydata.h"
#include "tuberxpert/result/globalresult.h"
#include "tuberxpert/utils/xpertutils.h"
#include "tuberxpert/flow/general/generalxpertflowstepprovider.h"


using namespace std;
using namespace Tucuxi;

TuberXpertComputer::TuberXpertComputer()
{}

ComputingStatus TuberXpertComputer::compute(
        const string& _drugPath,
        const string& _inputFileName,
        const string& _outputPath,
        const string& _languagePath) const
{

    Common::LoggerHelper logHelper;

    // Drug models repository creation
    Common::ComponentManager* pCmpMgr = Common::ComponentManager::getInstance();

    auto drugModelRepository =
            dynamic_cast<Core::DrugModelRepository*>(Core::DrugModelRepository::createComponent());

    pCmpMgr->registerComponent("DrugModelRepository", drugModelRepository);

    drugModelRepository->addFolderPath(_drugPath);

    /*********************************************************************************
     *                               Query Importation                               *
     * *******************************************************************************/

    unique_ptr<Xpert::XpertQueryData> query = nullptr;

    Xpert::XpertQueryImport importer;
    Xpert::XpertQueryImport::Status importResult = importer.importFromFile(query, _inputFileName);

    if (importResult != Xpert::XpertQueryImport::Status::Ok) {

        logHelper.error("Query import error, see details : {}", importer.getErrorMessage());
        return ComputingStatus::IMPORT_ERROR;
    }

    Xpert::GlobalResult globalResult(move(query), _outputPath);

    /*********************************************************************************
     *                             For each xpert resquest                           *
     * *******************************************************************************/

    unsigned nbUnfulfilledRequest = 0;
    for (Xpert::XpertRequestResult& xpertRequestResult : globalResult.getXpertRequestResults()) {

        logHelper.info("---------------------------------------");
        logHelper.info("Handling request number: " + to_string(globalResult.incrementRequestIndexBeingHandled() + 1)); // +1 because it starts from 0.

        // Check if query to core extraction was successful
        logHelper.info("Checking extraction state...");
        if (xpertRequestResult.shouldBeHandled() == false) {
            logHelper.error(xpertRequestResult.getErrorMessage());
            ++nbUnfulfilledRequest;
            continue;
        }
        logHelper.info("Extraction succeed.");

        // Get the flow step provider for the drug of the request
        unique_ptr<Xpert::AbstractXpertFlowStepProvider> xpertFlowStepProvider(nullptr);
        getXpertFlowStepProvider(xpertRequestResult, xpertFlowStepProvider);

        // Validate inputs data and prepare the missing informations (drug model/adjustment trait)
        logHelper.info("Validating query and preparing necessary information...");
        validateAndPrepareXpertRequest(xpertRequestResult, _languagePath, xpertFlowStepProvider);
        if (xpertRequestResult.shouldBeHandled() == false) {
            logHelper.error(xpertRequestResult.getErrorMessage());
            ++nbUnfulfilledRequest;
            continue;
        }

        logHelper.info("Query validation and information preparation succeed.");

        // Execute adjustment request
        logHelper.info("Submitting adjustment request...");

        makeAndExecuteAdjustmentRequest(xpertRequestResult);
        if (xpertRequestResult.shouldBeHandled() == false) {
            logHelper.error(xpertRequestResult.getErrorMessage());
            ++nbUnfulfilledRequest;
            continue;
        }

        logHelper.info("Adjustment request execution succeed.");

        // Genereate final report
        logHelper.info("Generating report...");

        xpertFlowStepProvider->getReportPrinter()->perform(xpertRequestResult);
        if (xpertRequestResult.shouldBeHandled() == false) {
            logHelper.error(xpertRequestResult.getErrorMessage());
            ++nbUnfulfilledRequest;
            continue;
        }

        logHelper.info("Report generation succeed.");

        // Here the request has been fully handled without problem
    }

    pCmpMgr->unregisterComponent("DrugModelRepository");

    // If all request were successfully handled.
    if (nbUnfulfilledRequest == 0) {
        return ComputingStatus::ALL_REQUESTS_SUCCEEDED;

        // If some failed.
    } else if ( nbUnfulfilledRequest < globalResult.getXpertRequestResults().size()) {
        return ComputingStatus::SOME_REQUESTS_SUCCEEDED;

        // Otherwise they all failed.
    } else {
        return ComputingStatus::NO_REQUESTS_SUCCEEDED;
    }

}

void TuberXpertComputer::validateAndPrepareXpertRequest(Xpert::XpertRequestResult& _xpertRequestResult,
                                                        const string& _languagePath,
                                                        const unique_ptr<Xpert::AbstractXpertFlowStepProvider>& _stepProvider) const
{

    Common::LoggerHelper logHelper;

    /**************************************************************
     *                Loading the translation file                *
     * ************************************************************/

    logHelper.info("Loading translation file...");

    // Getting language manager
    Xpert::LanguageManager& languageManager = Xpert::LanguageManager::getInstance();


    try {
        string languageFileName = _languagePath + "/" + varToString(_xpertRequestResult.getXpertRequest().getOutputLang()) + ".xml";
        ifstream ifs(languageFileName);

        // If language file opening failed.
        if (ifs.fail()) {
            throw runtime_error("Could not open the file " + languageFileName);
        }

        // Try loading the language file, it may throw a LanguageException.
        string xmlLanguageString((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
        languageManager.loadDictionary(xmlLanguageString);

        logHelper.info("Successfully loaded " + languageFileName);

    } catch (const runtime_error& e) {

        // Somehow, the acquisition of the language file failed.
        // Leave this requestXpert and try the next one.
        return;
    }

    /**************************************************************
     *                       Model selection                      *
     * ************************************************************/

    logHelper.info("Validating covariates and selecting drug model...");

    _stepProvider->getCovariateValidatorAndModelSelector()->perform(_xpertRequestResult);

    // Check if model selection was successful
    if (_xpertRequestResult.shouldBeHandled() == false) {
        return;
    }

    logHelper.info("Covariates validated and drug model selected: " + _xpertRequestResult.getDrugModel()->getDrugModelId());

    /**************************************************************
     *                       Dosages checking                     *
     * ************************************************************/

    logHelper.info("Checking dosages...");

    _stepProvider->getDoseValidator()->perform(_xpertRequestResult);

    // Check if dosages checking was successful
    if (_xpertRequestResult.shouldBeHandled() == false) {
        return;
    }

    logHelper.info("Dosages successfully validated.");


    /**************************************************************
     *                       Samples checking                     *
     * ************************************************************/

    logHelper.info("Checking samples...");

    _stepProvider->getSampleValidator()->perform(_xpertRequestResult);

    // Check if samples checking was successful
    if (_xpertRequestResult.shouldBeHandled() == false) {
        return;
    }

    logHelper.info("Samples successfully validated.");


    /**************************************************************
     *                       Targets checking                     *
     * ************************************************************/

    logHelper.info("Checking targets...");

    _stepProvider->getTargetValidator()->perform(_xpertRequestResult);

    // Check if targets checking was successful
    if (_xpertRequestResult.shouldBeHandled() == false) {
        return;
    }

    logHelper.info("Targets successfully validated.");

    /**************************************************************
     *                  Creating adjustment trait                 *
     * ************************************************************/

    logHelper.info("Creating adjustment trait...");

    _stepProvider->getAdjustmentTraitCreator()->perform(_xpertRequestResult);

    // Check if adjustment trait creation was successful
    if (_xpertRequestResult.shouldBeHandled() == false) {
        return;
    }

    logHelper.info("Adjustment trait successfully created.");

    // At that point the xpertRequestResult is still handleable (got no error).
}

void TuberXpertComputer::getXpertFlowStepProvider(Xpert::XpertRequestResult& _xpertRequestResult, unique_ptr<Xpert::AbstractXpertFlowStepProvider>& _xpertFlowStepProvider) const
{

    // The idea is the have this method that return a FlowStepProvider for each
    // drug. If a drug is not implemented, the general one is returned. For example, like this:

    // if (_xpertRequestResult.getXpertRequest().getDrugID() == "imatinib") {
    //     _xpertFlowStepProvider = make_unique<XpertFlow::ImatinibXpertFlowStepProvider>();
    // } else if (_xpertRequestResult.getXpertRequest().getDrugID() == "rifampicin") {
    //     _xpertFlowStepProvider = make_unique<XpertFlow::RifampicinXpertFlowStepProvider>();
    // } else {
    //     _xpertFlowStepProvider = make_unique<XpertFlow::GeneralXpertFlowStepProvider>();
    // }


    // But at the moment, TuberXpert is implemented in a general manner.
    // In future stages, this line should be removed for the commented ones on top of it.
    _xpertFlowStepProvider = make_unique<Xpert::GeneralXpertFlowStepProvider>();
}

void TuberXpertComputer::makeAndExecuteAdjustmentRequest(Xpert::XpertRequestResult& _xpertRequestResult) const
{
    // Create a copy of the adjustment trait.
    unique_ptr<Core::ComputingTraitAdjustment> baseAdjustmentTrait = make_unique<Core::ComputingTraitAdjustment>(*_xpertRequestResult.getAdjustmentTrait());

    // Create the computing request using the trait.
    Core::ComputingRequest computingRequest { "", *_xpertRequestResult.getDrugModel(), *_xpertRequestResult.getTreatment(), move(baseAdjustmentTrait)};

    // Create the response holder.
    unique_ptr<Core::ComputingResponse> computingResponse = make_unique<Core::ComputingResponse>("");

    // Starting computation.
    Core::IComputingService* computingComponent = dynamic_cast<Core::IComputingService*>(Core::ComputingComponent::createComponent());
    Core::ComputingStatus result = computingComponent->compute(computingRequest, computingResponse);

    // If request handling failed, return false.
    if (result != Core::ComputingStatus::Ok){

        _xpertRequestResult.setErrorMessage("Adjustment request execution failed.");
        return;

    // If it went well, set the response in the request result and return true.
    } else {

        // Extracting adjustment data pointer.
        Core::AdjustmentData* adjustmentData = dynamic_cast<Core::AdjustmentData*>(computingResponse->getUniquePointerData().release());

        // Putting into smart pointer since we released it.
        unique_ptr<Core::AdjustmentData> upAdjustmentData(adjustmentData);

        if (upAdjustmentData->getAdjustments().empty()) {
            _xpertRequestResult.setErrorMessage("No adjustment found.");
            return;
        }

        // Saving parameters
        _xpertRequestResult.addParameters(upAdjustmentData->getAdjustments().front().getData().front().m_parameters);

        // Saving the adjustment data into the request result
        _xpertRequestResult.setAdjustmentData(move(upAdjustmentData));

        return;
    }
}

void TuberXpertComputer::gatherAdditionalData(Tucuxi::Xpert::XpertRequestResult& _xpertRequestResult) const
{
    unique_ptr<Core::ComputingTraitAdjustment> baseAdjustmentTrait = make_unique<Core::ComputingTraitAdjustment>(*_xpertRequestResult.getAdjustmentTrait());
    Core::IComputingService* computingComponent = dynamic_cast<Core::IComputingService*>(Core::ComputingComponent::createComponent());

    // ------- statistics at steady state ---------

    // We update the end time to approximate a steady state in order to extract staistics at steady state.
    // It would have been ideal to do it with in one unique request but it is not possible to remove
    // the additional cycleData in the current state of the core.
    const Core::HalfLife& halfLife = _xpertRequestResult.getDrugModel()->getTimeConsiderations().getHalfLife();
    double hoursToAdd = Common::UnitManager::convertToUnit(halfLife.getValue(), halfLife.getUnit(), Common::TucuUnit("h"));
    Common::DateTime staeadyEndTime = baseAdjustmentTrait->getAdjustmentTime() + Duration(chrono::hours(int(halfLife.getMultiplier() * hoursToAdd)));

    // Create a computing trait that goes to steady state.
    unique_ptr<Core::ComputingTraitAdjustment> steadyAdustmentTrait = nullptr;
    tweakComputingTraitAdjustment(baseAdjustmentTrait,
                                  staeadyEndTime,
                                  1,
                                  baseAdjustmentTrait->getComputingOption().getParametersType(),
                                  steadyAdustmentTrait);

    // Create the computing request using the trait.
    Core::ComputingRequest computingRequestSteadyState { "", *_xpertRequestResult.getDrugModel(), *_xpertRequestResult.getTreatment(), move(steadyAdustmentTrait)};

    // Create the response holder.
    unique_ptr<Core::ComputingResponse> computingResponseSteadyState = make_unique<Core::ComputingResponse>("");

    // Starting computation.
    Core::ComputingStatus resultSteadyState = computingComponent->compute(computingRequestSteadyState, computingResponseSteadyState);

    if (resultSteadyState != Core::ComputingStatus::Ok){
        _xpertRequestResult.setErrorMessage("Failed to extract statistics at steady state.");
        return;
    } else {

        // Extracting adjustment data pointer.
        const Core::AdjustmentData* adjustmentData = dynamic_cast<const Core::AdjustmentData*>(computingResponseSteadyState->getData());

        // Extracting the statistics at steady state for the best adjustment.
        _xpertRequestResult.setCycleStats(adjustmentData->getAdjustments().front().getData().back().m_statistics);
    }

    // ------- Parameters "A priori" ---------

    // We don't want to extract them once again, so we check the base prediction type.
    if (baseAdjustmentTrait->getComputingOption().getParametersType() == Core::PredictionParameterType::Aposteriori) {

        // Create a computing trait a priori
        unique_ptr<Core::ComputingTraitAdjustment> aprioriAdustmentTrait = nullptr;
        tweakComputingTraitAdjustment(baseAdjustmentTrait,
                                      baseAdjustmentTrait->getEnd(),
                                      1,
                                      Core::PredictionParameterType::Apriori,
                                      aprioriAdustmentTrait);

        // Create the computing request using the trait.
        Core::ComputingRequest computingRequestApriori { "", *_xpertRequestResult.getDrugModel(), *_xpertRequestResult.getTreatment(), move(aprioriAdustmentTrait)};

        // Create the response holder.
        unique_ptr<Core::ComputingResponse> computingResponseApriori = make_unique<Core::ComputingResponse>("");

        // Starting computation.
        Core::ComputingStatus resultApriori = computingComponent->compute(computingRequestApriori, computingResponseApriori);

        if (resultApriori != Core::ComputingStatus::Ok){
            _xpertRequestResult.setErrorMessage("Failed to extract apriori parameters.");
            return;
        } else {

            // Extracting adjustment data pointer.
            const Core::AdjustmentData* adjustmentData = dynamic_cast<const Core::AdjustmentData*>(computingResponseApriori->getData());

            // Saving parameters
            _xpertRequestResult.addParameters(adjustmentData->getAdjustments().front().getData().front().m_parameters);
        }
    }

    // ------- Parameters "Typical patient" ---------

    // Create a computing trait for population
    unique_ptr<Core::ComputingTraitAdjustment> populationAdustmentTrait = nullptr;
    tweakComputingTraitAdjustment(baseAdjustmentTrait,
                                  baseAdjustmentTrait->getEnd(),
                                  1,
                                  Core::PredictionParameterType::Population,
                                  populationAdustmentTrait);

    // Create the computing request using the trait.
    Core::ComputingRequest computingRequestPopulation { "", *_xpertRequestResult.getDrugModel(), *_xpertRequestResult.getTreatment(), move(populationAdustmentTrait)};

    // Create the response holder.
    unique_ptr<Core::ComputingResponse> computingResponsePopulation = make_unique<Core::ComputingResponse>("");

    // Starting computation.
    Core::ComputingStatus resultPopulation = computingComponent->compute(computingRequestPopulation, computingResponsePopulation);

    if (resultPopulation != Core::ComputingStatus::Ok){
        _xpertRequestResult.setErrorMessage("Failed to extract population parameters.");
        return;
    } else {

        // Extracting adjustment data pointer.
        const Core::AdjustmentData* adjustmentData = dynamic_cast<const Core::AdjustmentData*>(computingResponsePopulation->getData());

        // Saving parameters
        _xpertRequestResult.addParameters(adjustmentData->getAdjustments().front().getData().front().m_parameters);
    }


}

void TuberXpertComputer::tweakComputingTraitAdjustment(const unique_ptr<Core::ComputingTraitAdjustment>& _toCopy,
                                                          const Common::DateTime& _end,
                                                          double _nbPointsPerHour,
                                                          Core::PredictionParameterType _predictionType,
                                                          std::unique_ptr<Core::ComputingTraitAdjustment>& _newAdjustment) const
{

    Core::ComputingOption newOptions{
        _predictionType,
        _toCopy->getComputingOption().getCompartmentsOption(),
        _toCopy->getComputingOption().retrieveStatistics(),
        _toCopy->getComputingOption().retrieveParameters(),
        _toCopy->getComputingOption().retrieveCovariates()
    };

    _newAdjustment = make_unique<Core::ComputingTraitAdjustment>(
                "",
                _toCopy->getStart(),
                _end,
                _nbPointsPerHour,
                newOptions,
                _toCopy->getAdjustmentTime(),
                Core::BestCandidatesOption::BestDosage,
                _toCopy->getLoadingOption(),
                _toCopy->getRestPeriodOption(),
                _toCopy->getSteadyStateTargetOption(),
                _toCopy->getTargetExtractionOption(),
                _toCopy->getFormulationAndRouteSelectionOption()
                );
}
