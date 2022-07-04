#include "computer.h"

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

namespace Tucuxi {
namespace Xpert {

Computer::Computer()
{}

ComputingStatus Computer::computeFromFile(const std::string& _drugPath,
                                          const std::string& _inputFileName,
                                          const std::string& _outputPath,
                                          const std::string& _languagePath) const
{
    Common::LoggerHelper logHelper;

    // Read the file and extract content
    ifstream input_file(_inputFileName);
    if (!input_file.is_open()) {
        logHelper.error("Failed to open input file.");
        return ComputingStatus::IMPORT_ERROR;
    }
    string inputFileContent = string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());

    return computeFromString(_drugPath, inputFileContent, _outputPath, _languagePath);
}

ComputingStatus Computer::computeFromString(
        const string& _drugPath,
        const string& _inputString,
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

    unique_ptr<XpertQueryData> query = nullptr;

    XpertQueryImport importer;
    XpertQueryImport::Status importResult = importer.importFromString(query, _inputString);

    if (importResult != XpertQueryImport::Status::Ok) {

        logHelper.error("Query import error, see details : {}", importer.getErrorMessage());
        return ComputingStatus::IMPORT_ERROR;
    }

    GlobalResult globalResult(move(query), _outputPath);

    /*********************************************************************************
     *                             For each xpert resquest                           *
     * *******************************************************************************/

    unsigned nbUnfulfilledRequest = 0;
    for (XpertRequestResult& xpertRequestResult : globalResult.getXpertRequestResults()) {

        logHelper.info("---------------------------------------");
        logHelper.info("Handling request number: " + to_string(globalResult.incrementRequestIndexBeingHandled() + 1)); // +1 because it starts from 0.

        // Get the flow step provider for the drug of the request.
        unique_ptr<AbstractXpertFlowStepProvider> xpertFlowStepProvider(nullptr);
        getXpertFlowStepProvider(xpertRequestResult, xpertFlowStepProvider);

        // Executes each step provided by the selected flow step provider.
        executeFlow(xpertRequestResult, _languagePath, xpertFlowStepProvider);
        if (xpertRequestResult.shouldBeHandled() == false) {
            logHelper.error(xpertRequestResult.getErrorMessage());
            ++nbUnfulfilledRequest;
            continue;
        }

        // Here the request has been fully handled without any problem.
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

void Computer::executeFlow(XpertRequestResult& _xpertRequestResult,
                           const string& _languagePath,
                           const unique_ptr<AbstractXpertFlowStepProvider>& _stepProvider) const
{

    Common::LoggerHelper logHelper;

    /**************************************************************
     *                      Check extraction                      *
     * ************************************************************/
    logHelper.info("Checking extraction state...");
    if (_xpertRequestResult.shouldBeHandled() == false) {
        return;
    }
    logHelper.info("Extraction succeed.");

    /**************************************************************
     *                Loading the translation file                *
     * ************************************************************/

    logHelper.info("Loading translation file...");

    // Getting language manager
    LanguageManager& languageManager = LanguageManager::getInstance();


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

    /**************************************************************
     *                     Requests execution                     *
     * ************************************************************/
    logHelper.info("Submitting adjustment request...");

    _stepProvider->getRequestExecutor()->perform(_xpertRequestResult);
    if (_xpertRequestResult.shouldBeHandled() == false) {
        return;
    }

    logHelper.info("Adjustment request execution succeed.");

    /**************************************************************
     *                      Report generation                     *
     * ************************************************************/
    logHelper.info("Generating report...");

    _stepProvider->getReportPrinter()->perform(_xpertRequestResult);
    if (_xpertRequestResult.shouldBeHandled() == false) {
        return;
    }

    logHelper.info("Report generation succeed.");

    // At that point the xpertRequestResult is still handleable (got no error).
}

void Computer::getXpertFlowStepProvider(XpertRequestResult& _xpertRequestResult, unique_ptr<AbstractXpertFlowStepProvider>& _xpertFlowStepProvider) const
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

} // namespace Xpert
} // namespace Tucuxi
