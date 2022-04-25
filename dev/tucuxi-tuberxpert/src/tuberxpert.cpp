#include <iostream>
#include <memory>

#include "tucucommon/loggerhelper.h"
#include "tucucommon/utils.h"
#include "cxxopts/include/cxxopts.hpp"

#include "language/languagemanager.h"
#include "query/xpertqueryimport.h"
#include "query/xpertquerydata.h"

#include <fstream>

using namespace std;

/// \brief Parses the program arguments argc/argv to extract drugPath, inputFileName and outputFileName.
/// \param argc Program argument count.
/// \param argv Program arguments.
/// \param drugPath String value to store parsed drug files directory.
/// \param inputFileName String value to store parsed input file path.
/// \param outputFileName String value to store parsed output file path.
/// \return true if parsing went ok otherwise false.
bool parse(int argc, char* argv[], string& drugPath, string& inputFileName, string& outputFileName)
{
    Tucuxi::Common::LoggerHelper logHelper;

    try {

        cxxopts::Options options(argv[0], " - TuberXpert command line");
        options.positional_help("[optional args]").show_positional_help();

        options.allow_unrecognised_options().add_options()
                ("d,drugpath", "Drug files directory path", cxxopts::value<std::string>())
                ("i,input", "Input request file path", cxxopts::value<std::string>())
                ("o,output", "Output response file path", cxxopts::value<std::string>())
                ("help", "Print help");


        auto result = options.parse(argc, argv);

        if (result.count("help") > 0) {
            std::cout << options.help({"", "Group"}) << std::endl;
            return EXIT_SUCCESS;
        }

        if (result.count("drugpath") > 0) {
            drugPath = result["drugpath"].as<std::string>();
        } else {
            std::cout << "The drug files directory is mandatory" << std::endl << std::endl;
            std::cout << options.help({"", "Group"}) << std::endl;
            return false;
        }

        if (result.count("input") > 0) {
            inputFileName = result["input"].as<std::string>();
        }
        else {
            std::cout << "The input file is mandatory" << std::endl << std::endl;
            std::cout << options.help({"", "Group"}) << std::endl;
            return false;
        }

        if (result.count("output") > 0) {
            outputFileName = result["output"].as<std::string>();
        }
        else {
            std::cout << "The output file is mandatory" << std::endl << std::endl;
            std::cout << options.help({"", "Group"}) << std::endl;
            return false;
        }

        logHelper.info("Drugs directory : {}", drugPath);
        logHelper.info("Input file : {}", inputFileName);
        logHelper.info("Output file name : {}", outputFileName);

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
    std::string appFolder = Tucuxi::Common::Utils::getAppFolder(argv);

    Tucuxi::Common::LoggerHelper::init(appFolder + "/tuberxpert.log");
    Tucuxi::Common::LoggerHelper logHelper;

    logHelper.info("********************************************************");
    logHelper.info("Tuberxpert console application is starting up...");

    // Parsing program arguments
    string drugPath, inputFileName, outputFileName;
    bool allGood = parse(argc, argv, drugPath, inputFileName, outputFileName);
    if(not allGood){
        return -2;
    }

    cout << drugPath << " " << inputFileName << " " << outputFileName << std::endl;

    std::ifstream ifs(inputFileName);
    if (ifs.fail()) {
        logHelper.error("Could not open the input file: " + inputFileName);

        return -1;
    }
    std::string xmlString((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    std::unique_ptr<Tucuxi::XpertQuery::XpertQueryData> query = nullptr;

    Tucuxi::XpertQuery::XpertQueryImport importer;
    Tucuxi::XpertQuery::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

    logHelper.info("Tuberxpert console application is exiting...");

    Tucuxi::Common::LoggerHelper::beforeExit();

    return EXIT_SUCCESS;
}
