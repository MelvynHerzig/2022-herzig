#include "computer.h"

#include <fstream>

#include "tucucommon/loggerhelper.h"
#include "tucucore/computingcomponent.h"
#include "tucucore/drugmodelrepository.h"

#include "tuberxpert/language/languagemanager.h"
#include "tuberxpert/query/xpertqueryimport.h"
#include "tuberxpert/query/xpertquerydata.h"
#include "tuberxpert/result/xpertqueryresult.h"
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

    // Read the file and extract its content.
    ifstream input_file(_inputFileName);
    if (!input_file.is_open()) {
        logHelper.error("Failed to open query file.");
        return ComputingStatus::IMPORT_ERROR;
    }
    string inputFileContent =
            string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());

    // Extract from string.
    return computeFromString(_drugPath, inputFileContent, _outputPath, _languagePath);
}

ComputingStatus Computer::computeFromString(
        const string& _drugPath,
        const string& _inputString,
        const string& _outputPath,
        const string& _languagePath) const
{

    Common::LoggerHelper logHelper;

    // Drug models repository creation.
    Common::ComponentManager* pCmpMgr = Common::ComponentManager::getInstance();

    auto drugModelRepository =
            dynamic_cast<Core::DrugModelRepository*>(Core::DrugModelRepository::createComponent());

    pCmpMgr->registerComponent("DrugModelRepository", drugModelRepository);

    drugModelRepository->addFolderPath(_drugPath);

    /*********************************************************************************
     *                               Query importation                               *
     * *******************************************************************************/

    unique_ptr<XpertQueryData> query = nullptr;

    XpertQueryImport importer;
    XpertQueryImport::Status importResult = importer.importFromString(query, _inputString);

    if (importResult != XpertQueryImport::Status::Ok) {

        logHelper.error("Query import error, see details : {}", importer.getErrorMessage());
        return ComputingStatus::IMPORT_ERROR;
    }

    XpertQueryResult xpertQueryResult(move(query), _outputPath);

    /*********************************************************************************
     *                             For each xpert request                            *
     * *******************************************************************************/

    unsigned nbUnfulfilledRequest = 0;
    for (XpertRequestResult& xpertRequestResult : xpertQueryResult.getXpertRequestResults()) {

        logHelper.info("---------------------------------------");
        logHelper.info("Processing request number: " +
                       to_string(xpertQueryResult.incrementRequestIndexBeingProcessed() + 1)); // +1 because it starts from 0


        // Get the XpertFlowStepProvider for the drug of the request.
        unique_ptr<AbstractXpertFlowStepProvider> xpertFlowStepProvider(nullptr);
        getXpertFlowStepProvider(xpertRequestResult.getXpertRequest().getDrugId(), xpertFlowStepProvider);

        // Execute each step provided by the selected XpertFlowStepProvider.
        executeFlow(xpertRequestResult, _languagePath, xpertFlowStepProvider);
        if (xpertRequestResult.shouldContinueProcessing() == false) {
            logHelper.error(xpertRequestResult.getErrorMessage());
            ++nbUnfulfilledRequest;
            continue;
        }

        // Here, the request has been fully processed without any problems.
    }

    pCmpMgr->unregisterComponent("DrugModelRepository");

    // If each request could be fully processed.
    if (nbUnfulfilledRequest == 0) {
        return ComputingStatus::ALL_REQUESTS_SUCCEEDED;

        // If some requests failed.
    } else if ( nbUnfulfilledRequest < xpertQueryResult.getXpertRequestResults().size()) {
        return ComputingStatus::SOME_REQUESTS_SUCCEEDED;

        // Else, if they all failed.
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

    if (_xpertRequestResult.shouldContinueProcessing() == false) {
        return;
    }

    logHelper.info("Extraction succeed.");

    /**************************************************************
     *                Loading the translations file                *
     * ************************************************************/
    logHelper.info("Loading translation file...");

    // Getting language manager.
    LanguageManager& langMgr = LanguageManager::getInstance();

    try {
        string translationsFileName = _languagePath +
                                  "/" +
                                  outputLangToString(_xpertRequestResult.getXpertRequest().getOutputLang()) +
                                  ".xml";
        ifstream ifStream(translationsFileName);

        // If the opening of the translation file failed.
        if (ifStream.fail()) {
            throw runtime_error("Could not open the translation file " + translationsFileName);
        }

        // Try loading the translation file. It may throw a LanguageException.
        string xmlLanguageString((istreambuf_iterator<char>(ifStream)), (istreambuf_iterator<char>()));
        langMgr.loadTranslations(xmlLanguageString);

        logHelper.info("Successfully loaded " + translationsFileName);

    } catch (const runtime_error& e) {

        // Somehow, the loading of the translation file failed.
        // Leave this xpertRequest and try the next one.
        return;
    }

    /**************************************************************
     *        Drug model selection and covariates validation      *
     * ************************************************************/
    logHelper.info("Validating covariates and selecting drug model...");

    _stepProvider->getCovariateValidatorAndModelSelector()->perform(_xpertRequestResult);

    // Check if the model selection was successful.
    if (_xpertRequestResult.shouldContinueProcessing() == false) {
        return;
    }

    logHelper.info("Covariates validated and drug model selected: " +
                   _xpertRequestResult.getDrugModel()->getDrugModelId());

    /**************************************************************
     *                        Doses validation                    *
     * ************************************************************/
    logHelper.info("Validating doses...");

    _stepProvider->getDoseValidator()->perform(_xpertRequestResult);

    // Check if the doses validation was successful.
    if (_xpertRequestResult.shouldContinueProcessing() == false) {
        return;
    }

    logHelper.info("Dosages successfully validated.");


    /**************************************************************
     *                       Samples validation                   *
     * ************************************************************/
    logHelper.info("Validating samples...");

    _stepProvider->getSampleValidator()->perform(_xpertRequestResult);

    // Check if the samples validation was successful.
    if (_xpertRequestResult.shouldContinueProcessing() == false) {
        return;
    }

    logHelper.info("Samples successfully validated.");


    /**************************************************************
     *                      Targets validation                    *
     * ************************************************************/
    logHelper.info("Validating targets...");

    _stepProvider->getTargetValidator()->perform(_xpertRequestResult);

    // Check if targets validation was successful.
    if (_xpertRequestResult.shouldContinueProcessing() == false) {
        return;
    }

    logHelper.info("Targets successfully validated.");

    /**************************************************************
     *                  Creating adjustment trait                 *
     * ************************************************************/
    logHelper.info("Creating adjustment trait...");

    _stepProvider->getAdjustmentTraitCreator()->perform(_xpertRequestResult);

    // Check if the adjustment trait creation was successful.
    if (_xpertRequestResult.shouldContinueProcessing() == false) {
        return;
    }

    logHelper.info("Adjustment trait successfully created.");

    /**************************************************************
     *                     Requests submission                    *
     * ************************************************************/
    logHelper.info("Submission of the adjustment request...");

    // Check if the submission of the adjustment request was successful.
    _stepProvider->getRequestExecutor()->perform(_xpertRequestResult);
    if (_xpertRequestResult.shouldContinueProcessing() == false) {
        return;
    }

    logHelper.info("Adjustment request submission succeed.");

    /**************************************************************
     *                      Report generation                     *
     * ************************************************************/
    logHelper.info("Generating report...");

    // Check if the report generation was successful.
    _stepProvider->getReportPrinter()->perform(_xpertRequestResult);
    if (_xpertRequestResult.shouldContinueProcessing() == false) {
        return;
    }

    logHelper.info("Report generation succeed.");

    // At this point, the XpertRequestResult has not received
    // any errors during the execution of the flow steps.
}

void Computer::getXpertFlowStepProvider(const string& _drugId,
                                        unique_ptr<AbstractXpertFlowStepProvider>& _xpertFlowStepProvider) const
{

    // The idea is the have this method return a XpertFlowStepProvider for each
    // drug. If a drug has no specialized XpertFlowStepProvider, the GeneralXpertFlowStepProvider
    // is returned. For example, like this:

    // if (_drugId == "imatinib") {
    //     _xpertFlowStepProvider = make_unique<XpertFlow::ImatinibXpertFlowStepProvider>();
    // } else if (_drugId == "rifampicin") {
    //     _xpertFlowStepProvider = make_unique<XpertFlow::RifampicinXpertFlowStepProvider>();
    // } else {
    //     _xpertFlowStepProvider = make_unique<XpertFlow::GeneralXpertFlowStepProvider>();
    // }


    // But for now, TuberXpert is implemented in a general way.
    // In future steps, this line should be removed for those commented above.
    _xpertFlowStepProvider = make_unique<GeneralXpertFlowStepProvider>();
}

} // namespace Xpert
} // namespace Tucuxi
