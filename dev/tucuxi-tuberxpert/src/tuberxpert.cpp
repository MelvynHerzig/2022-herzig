#include <iostream>
#include <memory>
#include <fstream>

#include "tucucommon/loggerhelper.h"
#include "cxxopts/include/cxxopts.hpp"
#include "tucucore/drugmodelrepository.h"

#include "language/languagemanager.h"
#include "query/xpertqueryimport.h"
#include "query/xpertquerydata.h"
#include "result/xpertresult.h"
#include "result/validation/modelselector.h"

using namespace std;

/*********************************************************************************
 *                                 Result Codes                                  *
 * *******************************************************************************/

/// \brief BAD_ARGUMENTS_ERROR Exit code when everything went fine.
const int BAD_ARGUMENTS_ERROR = -1;

/// \brief EXECUTION_SUCCESS Exit code when everything went fine.
const int EXECUTION_SUCCESS = EXIT_SUCCESS;

/// \brief IMPORT_ERROR Exit code when the query could not be loaded.
const int IMPORT_ERROR = 1;

/// \brief SOME_REQUESTS_FAILED Exit code when some of the requests could not get fulfilled.
const int SOME_REQUESTS_FAILED = 2;

/// \brief ALL_REQUESTS_FAILED Exit code when all the requests could not get fulfilled.
const int ALL_REQUESTS_FAILED = 3;

/// \brief Parses the program arguments argc/argv to extract drugPath, inputFileName and outputFileName.
/// \param argc Program argument count.
/// \param argv Program arguments.
/// \param drugPath String value to store parsed drug files directory.
/// \param inputFileName String value to store parsed input file path.
/// \param outputFileName String value to store parsed output file path.
/// \return true if parsing went ok otherwise false.
bool parse(int argc, char* argv[], string& drugPath, string& inputFileName, string& outputFileName, string& languagePath)
{
    Tucuxi::Common::LoggerHelper logHelper;

    try {

        cxxopts::Options options(argv[0], " - TuberXpert command line");
        options.positional_help("[optional args]").show_positional_help();

        options.allow_unrecognised_options().add_options()
                ("d,drugpath", "Drug files directory path", cxxopts::value<string>())
                ("i,input", "Input request file path", cxxopts::value<string>())
                ("o,output", "Output response file path", cxxopts::value<string>())
                ("l,languagepath", "Language files directory path", cxxopts::value<string>())
                ("help", "Print help");


        auto result = options.parse(argc, argv);

        if (result.count("help") > 0) {
            cout << options.help({"", "Group"}) << endl;
            return EXIT_SUCCESS;
        }

        if (result.count("drugpath") > 0) {
            drugPath = result["drugpath"].as<string>();
        } else {
            cout << "The drug files directory is mandatory" << endl << endl;
            cout << options.help({"", "Group"}) << endl;
            return false;
        }

        if (result.count("input") > 0) {
            inputFileName = result["input"].as<string>();
        }
        else {
            cout << "The input file is mandatory" << endl << endl;
            cout << options.help({"", "Group"}) << endl;
            return false;
        }

        if (result.count("output") > 0) {
            outputFileName = result["output"].as<string>();
        }
        else {
            cout << "The output file is mandatory" << endl << endl;
            cout << options.help({"", "Group"}) << endl;
            return false;
        }


        if (result.count("languagepath") > 0) {
            languagePath = result["languagepath"].as<string>();
        }

        logHelper.info("Drugs directory : {}", drugPath);
        logHelper.info("Input file : {}", inputFileName);
        logHelper.info("Output file name : {}", outputFileName);
        logHelper.info("Language directory : {}", languagePath);

        return true;
    }
    catch (const cxxopts::OptionException& e) {
        logHelper.error("error parsing options: {}", e.what());

        return false;
    }
}

/// \brief The tuberXpert console application.
/// The tuberXpert console application offers a simple command line interface to
/// launch a clinical decision support system.
int main(int argc, char** argv)
{
    // Get application folder
    string appFolder = Tucuxi::Common::Utils::getAppFolder(argv);

    Tucuxi::Common::LoggerHelper::init(appFolder + "/tuberxpert.log");
    Tucuxi::Common::LoggerHelper logHelper;

    logHelper.info("********************************************************");
    logHelper.info("Tuberxpert console application is starting up...");

    // Parsing program arguments
    string drugPath, inputFileName, outputFileName;
    string languagePath = "../language";
    bool allGood = parse(argc, argv, drugPath, inputFileName, outputFileName, languagePath);
    if(not allGood){
        return BAD_ARGUMENTS_ERROR;
    }

    // Drug models repository creation
    Tucuxi::Common::ComponentManager* pCmpMgr = Tucuxi::Common::ComponentManager::getInstance();

    auto drugModelRepository =
            dynamic_cast<Tucuxi::Core::DrugModelRepository*>(Tucuxi::Core::DrugModelRepository::createComponent());

    pCmpMgr->registerComponent("DrugModelRepository", drugModelRepository);

    drugModelRepository->addFolderPath(drugPath);

    // Getting language manager
    Tucuxi::XpertLanguage::LanguageManager& languageManager = Tucuxi::XpertLanguage::LanguageManager::getInstance();

    /*********************************************************************************
     *                               Query Importation                               *
     * *******************************************************************************/

    unique_ptr<Tucuxi::XpertQuery::XpertQueryData> query = nullptr;

    Tucuxi::XpertQuery::XpertQueryImport importer;
    Tucuxi::XpertQuery::XpertQueryImport::Status importResult = importer.importFromFile(query, inputFileName);

    if (importResult != Tucuxi::XpertQuery::XpertQueryImport::Status::Ok) {

        logHelper.error("Query import error, see details : {}", importer.getErrorMessage());
        return IMPORT_ERROR;
    }

    Tucuxi::XpertResult::XpertResult xpertResult(std::move(query));

    /*********************************************************************************
     *                             For each xpert resquest                           *
     * *******************************************************************************/

    unsigned nbUnfulfilledRequest = 0;
    unsigned requestNbBeingHandled = 0;
    for (Tucuxi::XpertResult::XpertRequestResult& xpertRequestResult : xpertResult.getXpertRequestResults()) {

        logHelper.info("\n---------------------------------------");
        logHelper.info("Handling request number: " + to_string(++requestNbBeingHandled));

        // Check if extraction was successfull
        logHelper.info("\nChecking extraction state...");
        if (xpertRequestResult.shouldBeHandled() == false) {
            logHelper.error(xpertRequestResult.getErrorMessage().value());
            ++nbUnfulfilledRequest;
            continue;
        }

        /**************************************************************
         *                  Load the translation file                 *
         * ************************************************************/

        logHelper.info("\nLoading translation file...");

        try {
            string languageFileName = languagePath + "/" + languageManager.computeLanguageFileName(xpertRequestResult.getXpertRequest()->getOutputLang());
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

        logHelper.info("\nSelecting drug model...");

        Tucuxi::XpertResult::BestDrugModelSelector bestDrugModelSelector;
        bestDrugModelSelector.getBestDrugModel(xpertRequestResult);

        // Check if model selection was successfull
        if (xpertRequestResult.shouldBeHandled() == false) {
            logHelper.error(xpertRequestResult.getErrorMessage().value());
            ++nbUnfulfilledRequest;
            continue;
        }
    }

    logHelper.info("Tuberxpert console application is exiting...");
    logHelper.info("********************************************************");

    pCmpMgr->unregisterComponent("DrugModelRepository");

    Tucuxi::Common::LoggerHelper::beforeExit();

    return EXECUTION_SUCCESS;
}
