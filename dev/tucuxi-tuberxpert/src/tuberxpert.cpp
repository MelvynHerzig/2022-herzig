#include "tucucommon/loggerhelper.h"
#include "tucucommon/utils.h"

#include "tuberxpert/computer.h"
#include "cxxopts/include/cxxopts.hpp"

#include "pdf.h"

using namespace std;

/*********************************************************************************
 *                                 Result Codes                                  *
 * *******************************************************************************/

/// \brief CODE_BAD_ARGUMENTS_ERROR Exit code when everything went fine.
const int CODE_BAD_ARGUMENTS_ERROR = -2;

/// \brief CODE_IMPORT_ERROR Exit code when the query could not be loaded.
const int CODE_IMPORT_ERROR = -1;

/// \brief CODE_EXECUTION_SUCCESS Exit code when everything went fine.
const int CODE_ALL_REQUESTS_SUCCEEDED = 0;

/// \brief CODE_SOME_REQUESTS_SUCCEEDED Exit code when some of the requests could not get fulfilled.
const int CODE_SOME_REQUESTS_SUCCEEDED = 1;

/// \brief CODE_NO_REQUESTS_SUCCEEDED Exit code when all the requests could not get fulfilled.
const int CODE_NO_REQUESTS_SUCCEEDED = 2;

/// \brief Parses the program arguments argc/argv to extract drugPath, inputFileName and outputFileName.
/// \param argc Program argument count.
/// \param argv Program arguments.
/// \param drugPath String value to store parsed drug files directory.
/// \param inputFileName String value to store parsed input file path.
/// \param outputPath String value to store parsed output file path.
/// \param languagePath String value to store the location of the folder containing the translation files.
/// \return true if parsing went ok otherwise false.
bool parse(int argc, char* argv[], string& drugPath, string& inputFileName, string& outputPath, string& languagePath)
{
    Tucuxi::Common::LoggerHelper logHelper;

    try {

        cxxopts::Options options(argv[0], " - TuberXpert command line");
        options.positional_help("[optional args]").show_positional_help();

        options.allow_unrecognised_options().add_options()
                ("d,drugpath", "Drug files directory path", cxxopts::value<string>())
                ("i,input", "Input request file path", cxxopts::value<string>())
                ("o,outputpath", "Output response directory path", cxxopts::value<string>())
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

        if (result.count("outputpath") > 0) {
            outputPath = result["outputpath"].as<string>();
        }
        else {
            cout << "The output directory is mandatory" << endl << endl;
            cout << options.help({"", "Group"}) << endl;
            return false;
        }


        if (result.count("languagepath") > 0) {
            languagePath = result["languagepath"].as<string>();
        }

        logHelper.info("Drugs directory : {}", drugPath);
        logHelper.info("Input file : {}", inputFileName);
        logHelper.info("Output directory : {}", outputPath);
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
/// launch the computation.
int main(int argc, char** argv)
{
    // Parsing program arguments
    string drugPath, inputFileName, outputPath;
    string languagePath = "../language";
    bool allGood = parse(argc, argv, drugPath, inputFileName, outputPath, languagePath);
    if(not allGood){
        return CODE_BAD_ARGUMENTS_ERROR;
    }


    // Get application folder
    string appFolder = Tucuxi::Common::Utils::getAppFolder(argv);

    // Init logger
    Tucuxi::Common::LoggerHelper::init(appFolder + "/tuberxpert.log");
    Tucuxi::Common::LoggerHelper logHelper;

    logHelper.info("********************************************************");
    logHelper.info("Tuberxpert console application is starting up...");

    // Computation start
    Tucuxi::Xpert::Computer txc;
    Tucuxi::Xpert::ComputingStatus result = txc.computeFromFile(drugPath, inputFileName, outputPath, languagePath);

    logHelper.info("Tuberxpert console application is exiting...");
    logHelper.info("********************************************************");

    // Clean logger
    Tucuxi::Common::LoggerHelper::beforeExit();

    // Return code handling
    switch (result) {
        case Tucuxi::Xpert::ComputingStatus::IMPORT_ERROR:
            return CODE_IMPORT_ERROR;
        case Tucuxi::Xpert::ComputingStatus::ALL_REQUESTS_SUCCEEDED:
            return CODE_ALL_REQUESTS_SUCCEEDED;
        case Tucuxi::Xpert::ComputingStatus::SOME_REQUESTS_SUCCEEDED:
            return CODE_SOME_REQUESTS_SUCCEEDED;
        case Tucuxi::Xpert::ComputingStatus::NO_REQUESTS_SUCCEEDED:
            return CODE_NO_REQUESTS_SUCCEEDED;
    }
}
