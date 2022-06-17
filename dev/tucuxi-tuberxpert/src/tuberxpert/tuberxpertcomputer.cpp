#include "tuberxpertcomputer.h"

#include <memory>
#include <fstream>

#include "tucucommon/loggerhelper.h"
#include "tucucore/drugmodelrepository.h"

#include "tuberxpert/language/languagemanager.h"
#include "tuberxpert/query/xpertqueryimport.h"
#include "tuberxpert/query/xpertquerydata.h"
#include "tuberxpert/result/xpertresult.h"
#include "tuberxpert/result/validation/modelselector.h"
#include "tuberxpert/result/validation/dosevalidator.h"
#include "tuberxpert/result/validation/samplevalidator.h"
#include "tuberxpert/result/validation/targetvalidator.h"
#include "tuberxpert/result/request/adjustmenttraitcreator.h"
#include "tuberxpert/utils/xpertutils.h"

using namespace std;

namespace Tucuxi {
namespace XpertComputer {

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

    // Getting language manager
    Tucuxi::XpertLanguage::LanguageManager& languageManager = XpertLanguage::LanguageManager::getInstance();

    /*********************************************************************************
     *                               Query Importation                               *
     * *******************************************************************************/

    unique_ptr<XpertQuery::XpertQueryData> query = nullptr;

    XpertQuery::XpertQueryImport importer;
    XpertQuery::XpertQueryImport::Status importResult = importer.importFromFile(query, _inputFileName);

    if (importResult != XpertQuery::XpertQueryImport::Status::Ok) {

        logHelper.error("Query import error, see details : {}", importer.getErrorMessage());
        return ComputingStatus::IMPORT_ERROR;
    }

    XpertResult::XpertResult xpertResult(move(query));

    /*********************************************************************************
     *                             For each xpert resquest                           *
     * *******************************************************************************/

    unsigned nbUnfulfilledRequest = 0;
    unsigned requestNbBeingHandled = 0;
    for (XpertResult::XpertRequestResult& xpertRequestResult : xpertResult.getXpertRequestResults()) {

        logHelper.info("\n---------------------------------------");
        logHelper.info("Handling request number: " + to_string(++requestNbBeingHandled));

        // Check if extraction was successfull
        logHelper.info("\nChecking extraction state...");
        if (xpertRequestResult.shouldBeHandled() == false) {
            logHelper.error(xpertRequestResult.getErrorMessage());
            ++nbUnfulfilledRequest;
            continue;
        }

        /**************************************************************
         *                Loading the translation file                *
         * ************************************************************/

        logHelper.info("\nLoading translation file...");

        try {
            string languageFileName = _languagePath + "/" + XpertUtils::outputLangToString(xpertRequestResult.getXpertRequest().getOutputLang()) + ".xml";
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
            ++nbUnfulfilledRequest;
            continue;
        }

        /**************************************************************
         *                       Model selection                      *
         * ************************************************************/

        logHelper.info("Selecting drug model...");

        XpertResult::BestDrugModelSelector bestDrugModelSelector;
        bestDrugModelSelector.getBestDrugModel(xpertRequestResult);

        // Check if model selection was successfull
        if (xpertRequestResult.shouldBeHandled() == false) {
            logHelper.error(xpertRequestResult.getErrorMessage());
            ++nbUnfulfilledRequest;
            continue;
        }

        logHelper.info("Drug model selected: " + xpertRequestResult.getDrugModel()->getDrugModelId());

        /**************************************************************
         *                       Dosages checking                     *
         * ************************************************************/

        logHelper.info("Checking dosages...");

        XpertResult::DoseValidator doseValidator;
        doseValidator.getDoseValidations(xpertRequestResult);

        // Check if dosages checking was successfull
        if (xpertRequestResult.shouldBeHandled() == false) {
            logHelper.error(xpertRequestResult.getErrorMessage());
            ++nbUnfulfilledRequest;
            continue;
        }

        logHelper.info("Dosages successfully validated.");


        /**************************************************************
         *                       Samples checking                     *
         * ************************************************************/

        logHelper.info("Checking samples...");

        XpertResult::SampleValidator sampleValidator;
        sampleValidator.getSampleValidations(xpertRequestResult);

        // Check if samples checking was successfull
        if (xpertRequestResult.shouldBeHandled() == false) {
            logHelper.error(xpertRequestResult.getErrorMessage());
            ++nbUnfulfilledRequest;
            continue;
        }

        logHelper.info("Samples successfully validated.");


        /**************************************************************
         *                       Targets checking                     *
         * ************************************************************/

        logHelper.info("Checking targets...");

        XpertResult::TargetValidator targetValidator;
        targetValidator.getTargetValidations(xpertRequestResult);

        // Check if targets checking was successfull
        if (xpertRequestResult.shouldBeHandled() == false) {
            logHelper.error(xpertRequestResult.getErrorMessage());
            ++nbUnfulfilledRequest;
            continue;
        }

        logHelper.info("Targets successfully validated.");

        /**************************************************************
         *                  Creating adjustment trait                 *
         * ************************************************************/

        logHelper.info("Creating adjustment trait...");

        XpertResult::AdjustmentTraitCreator adjustmentTraitCreator;
        adjustmentTraitCreator.createAdjustmentTrait(xpertRequestResult);

        // Check if trait creation was successfull
        if (xpertRequestResult.shouldBeHandled() == false) {
            logHelper.error(xpertRequestResult.getErrorMessage());
            ++nbUnfulfilledRequest;
            continue;
        }

        logHelper.info("Adjustment trait successfully created.");
    }

    pCmpMgr->unregisterComponent("DrugModelRepository");

    // If all request were successfully handled.
    if (nbUnfulfilledRequest == 0) {
        return ComputingStatus::ALL_REQUESTS_SUCCEEDED;

        // If some failed.
    } else if ( nbUnfulfilledRequest < xpertResult.getXpertRequestResults().size()) {
        return ComputingStatus::SOME_REQUESTS_SUCCEEDED;

        // Otherwise they all failed.
    } else {
        return ComputingStatus::NO_REQUESTS_SUCCEEDED;
    }

}

} // namespace XpertComputer
} // namespace Tucuxi
