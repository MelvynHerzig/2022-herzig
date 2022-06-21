#include "tuberxpertcomputer.h"

#include <memory>
#include <fstream>

#include "tucucommon/loggerhelper.h"
#include "tucucore/drugmodelrepository.h"
#include "tucucore/computingcomponent.h"

#include "tuberxpert/language/languagemanager.h"
#include "tuberxpert/query/xpertqueryimport.h"
#include "tuberxpert/query/xpertquerydata.h"
#include "tuberxpert/result/xpertresult.h"
#include "tuberxpert/utils/xpertutils.h"
#include "tuberxpert/flow/general/generalxpertflowstepprovider.h"


using namespace std;

namespace Tucuxi {
namespace Xpert {

TuberXpertComputer::TuberXpertComputer()
{}

ComputingStatus TuberXpertComputer::compute(
        const string& _drugPath,
        const string& _inputFileName,
        const string& _outputFileName,
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

    unique_ptr<XpertQueryData> query = nullptr;

    XpertQueryImport importer;
    XpertQueryImport::Status importResult = importer.importFromFile(query, _inputFileName);

    if (importResult != XpertQueryImport::Status::Ok) {

        logHelper.error("Query import error, see details : {}", importer.getErrorMessage());
        return ComputingStatus::IMPORT_ERROR;
    }

    XpertGlobalResult xpertGlobalResult(move(query));

    /*********************************************************************************
     *                             For each xpert resquest                           *
     * *******************************************************************************/

    unsigned nbUnfulfilledRequest = 0;
    unsigned requestNbBeingHandled = 0;
    for (XpertRequestResult& xpertRequestResult : xpertGlobalResult.getXpertRequestResults()) {

        logHelper.info("---------------------------------------");
        logHelper.info("Handling request number: " + to_string(++requestNbBeingHandled));

        // Check if query to core extraction was successful
        logHelper.info("Checking extraction state...");
        if (xpertRequestResult.shouldBeHandled() == false) {
            logHelper.error(xpertRequestResult.getErrorMessage());
            ++nbUnfulfilledRequest;
            continue;
        }

        // Get the flow step provider for the drug of the request
        unique_ptr<AbstractXpertFlowStepProvider> xpertFlowStepProvider(nullptr);
        getXpertFlowStepProvider(xpertRequestResult, xpertFlowStepProvider);

        // Validate inputs data and prepare the missing informations (drug model/adjustment trait)
        if (validateAndPrepareXpertRequest(xpertRequestResult, _languagePath, xpertFlowStepProvider) == false) {
            ++nbUnfulfilledRequest;
            continue;
        }

        // Execute adjustment request
        if (makeAndExecuteAdjustmentRequest(xpertRequestResult) == false) {
            ++nbUnfulfilledRequest;
            continue;
        }

        // Genereate final report
        // TODO

    }

    pCmpMgr->unregisterComponent("DrugModelRepository");

    // If all request were successfully handled.
    if (nbUnfulfilledRequest == 0) {
        return ComputingStatus::ALL_REQUESTS_SUCCEEDED;

        // If some failed.
    } else if ( nbUnfulfilledRequest < xpertGlobalResult.getXpertRequestResults().size()) {
        return ComputingStatus::SOME_REQUESTS_SUCCEEDED;

        // Otherwise they all failed.
    } else {
        return ComputingStatus::NO_REQUESTS_SUCCEEDED;
    }

}

bool TuberXpertComputer::validateAndPrepareXpertRequest(XpertRequestResult& _xpertRequestResult,
                                                        const string& _languagePath,
                                                        const unique_ptr<AbstractXpertFlowStepProvider>& _stepProvider) const
{

    Common::LoggerHelper logHelper;

    /**************************************************************
     *                Loading the translation file                *
     * ************************************************************/

    logHelper.info("Loading translation file...");

    // Getting language manager
    LanguageManager& languageManager = LanguageManager::getInstance();


    try {
        string languageFileName = _languagePath + "/" + outputLangToString(_xpertRequestResult.getXpertRequest().getOutputLang()) + ".xml";
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
        logHelper.error(e.what());
        return false;
    }

    /**************************************************************
     *                       Model selection                      *
     * ************************************************************/

    logHelper.info("Validating covariates and selecting drug model...");

    _stepProvider->getCovariateValidatorAndModelSelector()->perform(_xpertRequestResult);

    // Check if model selection was successful
    if (_xpertRequestResult.shouldBeHandled() == false) {
        logHelper.error(_xpertRequestResult.getErrorMessage());
        return false;
    }

    logHelper.info("Covariates validated and drug model selected: " + _xpertRequestResult.getDrugModel()->getDrugModelId());

    /**************************************************************
     *                       Dosages checking                     *
     * ************************************************************/

    logHelper.info("Checking dosages...");

    _stepProvider->getDoseValidator()->perform(_xpertRequestResult);

    // Check if dosages checking was successful
    if (_xpertRequestResult.shouldBeHandled() == false) {
        logHelper.error(_xpertRequestResult.getErrorMessage());
        return false;
    }

    logHelper.info("Dosages successfully validated.");


    /**************************************************************
     *                       Samples checking                     *
     * ************************************************************/

    logHelper.info("Checking samples...");

    _stepProvider->getSampleValidator()->perform(_xpertRequestResult);

    // Check if samples checking was successful
    if (_xpertRequestResult.shouldBeHandled() == false) {
        logHelper.error(_xpertRequestResult.getErrorMessage());
        return false;
    }

    logHelper.info("Samples successfully validated.");


    /**************************************************************
     *                       Targets checking                     *
     * ************************************************************/

    logHelper.info("Checking targets...");

    _stepProvider->getTargetValidator()->perform(_xpertRequestResult);

    // Check if targets checking was successful
    if (_xpertRequestResult.shouldBeHandled() == false) {
        logHelper.error(_xpertRequestResult.getErrorMessage());
        return false;
    }

    logHelper.info("Targets successfully validated.");

    /**************************************************************
     *                  Creating adjustment trait                 *
     * ************************************************************/

    logHelper.info("Creating adjustment trait...");

    _stepProvider->getAdjustmentTraitCreator()->perform(_xpertRequestResult);

    // Check if adjustment trait creation was successful
    if (_xpertRequestResult.shouldBeHandled() == false) {
        logHelper.error(_xpertRequestResult.getErrorMessage());
        return false;
    }

    logHelper.info("Adjustment trait successfully created.");


    return true;
}

void TuberXpertComputer::getXpertFlowStepProvider(XpertRequestResult& _xpertRequestResult, unique_ptr<Xpert::AbstractXpertFlowStepProvider>& _xpertFlowStepProvider) const
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
    _xpertFlowStepProvider = make_unique<GeneralXpertFlowStepProvider>();
}

bool TuberXpertComputer::makeAndExecuteAdjustmentRequest(XpertRequestResult& _xpertRequestResult) const
{
    Common::LoggerHelper logHelper;
    logHelper.info("Submitting adjustment request...");

    // Create a copy of the adjustment trait.
    unique_ptr<Core::ComputingTraitAdjustment> copyComputingTraitAdjustment = make_unique<Core::ComputingTraitAdjustment>(*_xpertRequestResult.getAdjustmentTrait());

    // Create the computing request.
    Core::ComputingRequest computingRequest { "", *_xpertRequestResult.getDrugModel(), *_xpertRequestResult.getTreatment(), move(copyComputingTraitAdjustment)};

    // Create the response holder.
    unique_ptr<Core::ComputingResponse> computingResponse = make_unique<Core::ComputingResponse>("");

    // Starting percentiles computation.
    Core::IComputingService* computingComponent = dynamic_cast<Core::IComputingService*>(Core::ComputingComponent::createComponent());
    Core::ComputingStatus result = computingComponent->compute(computingRequest, computingResponse);

    // If request handling failed, return false.
    if (result != Core::ComputingStatus::Ok){

        logHelper.error("Adjustment request execution failed.");
        return false;

    // If it went well, set the response in the result and return true.
    } else {

        logHelper.info("Adjustment request execution success.");

        // Extracting adjustment data pointer.
        Core::AdjustmentData* adjustmentData = dynamic_cast<Core::AdjustmentData*>(computingResponse->getUniquePointerData().release());

        // Putting into smart pointer since we released it.
        unique_ptr<Core::AdjustmentData> upAdjustmentData(adjustmentData);

        // Saving the adjustment data into the request result
        _xpertRequestResult.setAdjustmentData(move(upAdjustmentData));

        return true;
    }
}

} // namespace Xpert
} // namespace Tucuxi
