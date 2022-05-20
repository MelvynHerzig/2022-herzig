#include <iostream>
#include <memory>
#include <map>

#include "tucucommon/loggerhelper.h"
#include "tucucommon/utils.h"
#include "cxxopts/include/cxxopts.hpp"

#include "language/languagemanager.h"
#include "query/xpertqueryimport.h"
#include "query/xpertquerydata.h"
#include "result/result.h"
#include "result/validation/modelselector.h"

using namespace std;

/// \brief BAD_ARGUMENTS_ERROR Exit code when everything went fine.
const int BAD_ARGUMENTS_ERROR = -1;

/// \brief EXECUTION_SUCCESS Exit code when everything went fine.
const int EXECUTION_SUCCESS = EXIT_SUCCESS;

/// \brief IMPORT_ERROR Exit code when the query could not be loaded.
const int IMPORT_ERROR = 1;

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
    string languagePath = "../language/";
    bool allGood = parse(argc, argv, drugPath, inputFileName, outputFileName, languagePath);
    if(not allGood){
        return BAD_ARGUMENTS_ERROR;
    }

     cout << drugPath << " " << inputFileName << " " << outputFileName << endl;

     /*********************************************************************************
      *                                Result Wrapper                                 *
      * *******************************************************************************/

     Tucuxi::XpertResult::Result result;

    /*********************************************************************************
     *                               Query Importation                               *
     * *******************************************************************************/

    Tucuxi::XpertQuery::XpertQueryImport importer;
    Tucuxi::XpertQuery::XpertQueryImport::Status importResult = importer.importFromFile(result.getQuery(), inputFileName);

    if (importResult != Tucuxi::XpertQuery::XpertQueryImport::Status::Ok) {

       logHelper.error("Error, see details : {}", importer.getErrorMessage());
        return IMPORT_ERROR;
    }

    /*********************************************************************************
     *                               Getting Drug Models                             *
     * *******************************************************************************/

    Tucuxi::XpertResult::ModelSelector modelSelector{drugPath};
    map<Tucuxi::Query::DrugData*, std::string> modelIdPerDrug;
    modelSelector.getBestModelForQueryDrugs(*result.getQuery(), modelIdPerDrug);


    logHelper.info("Tuberxpert console application is exiting...");

    Tucuxi::Common::LoggerHelper::beforeExit();

    return EXECUTION_SUCCESS;
}
