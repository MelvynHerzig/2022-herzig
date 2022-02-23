/*
* Copyright (C) 2017 Tucuxi SA
*/

#include "tucucommon/utils.h"
#include "tucucommon/loggerhelper.h"
#include "tucucommon/general.h"
#include "tucucore/definitions.h"
#include "tucucore/intakeevent.h"
#include "tucucore/parameter.h"
#include "tucucore/pkmodels/onecompartmentextra.h"
#include "tucucore/drugmodelrepository.h"

#include "tucuxperthandler.h"
#include <QApplication>

#include "cxxopts/include/cxxopts.hpp"

using namespace std::chrono_literals;

cxxopts::ParseResult
parse(int _argc, char* _argv[])
{
    // Get application folder
    std::string appFolder = Tucuxi::Common::Utils::getAppFolder(_argv);

    Tucuxi::Common::LoggerHelper logHelper;

    try
    {

        cxxopts::Options options(_argv[0], " - Tucuxi Expert System");
        options
                .positional_help("[optional args]")
                .show_positional_help();

        options
                .allow_unrecognised_options()
                .add_options()
                ("d,drugpath", "Drug files path", cxxopts::value<std::string>())
                ("i,input", "Input XML file containing the patient's data", cxxopts::value<std::string>())
                ("o,output", "Path to write the output HTML file containing the results", cxxopts::value<std::string>())
                ("h,help", "Print help")
                ;


        auto result = options.parse(_argc, _argv);

        if (result.count("help") > 0)
        {
            std::cout << options.help({"", "Group"}) << std::endl;
            exit(EXIT_SUCCESS);
        }

        std::string drugPath = appFolder + "/drugfiles/";
        if (result.count("drugpath") > 0) {
            drugPath = result["drugpath"].as<std::string>();
        }

        std::string inputFileName;
        if (result.count("input") > 0) {
            inputFileName = result["input"].as<std::string>();
        }
        else {
            std::cout << "The input file is mandatory!\n\n" ;
            std::cout << options.help({"", "Group"}) << std::endl;
            exit(EXIT_FAILURE);
        }
        std::string outputFileName;
        if (result.count("output") > 0) {
            outputFileName = result["output"].as<std::string>();
        }
        else {
            std::cout << "The output file is mandatory!\n\n" ;
            std::cout << options.help({"", "Group"}) << std::endl;
            exit(EXIT_FAILURE);
        }

        logHelper.info("Input file : {} , output path: {}", inputFileName, outputFileName);


        Tucuxi::Xpert::TucuxpertHandler tucuxpertHandler;
        int exitCode = tucuxpertHandler.handle(inputFileName, outputFileName, drugPath);


        if (exitCode != EXIT_SUCCESS) {
            exit(exitCode);
        }

        return result;

    } catch (const cxxopts::OptionException& e)
    {
        logHelper.error("error parsing options: {}", e.what());
        exit(1);
    }
}

/// \defgroup TucuXpert Tucuxi autonomous application
/// \brief The Tucuxi expert application
///
/// The Tucuxi expert application offers a simple command line interface to
/// generate a report based on the available data on the patient.

int main(int _argc, char** _argv)
{
    QApplication app(_argc, _argv);
    // Get application folder
    std::string appFolder = Tucuxi::Common::Utils::getAppFolder(_argv);

    Tucuxi::Common::LoggerHelper::init(appFolder + "/tucuXpert.log");
    Tucuxi::Common::LoggerHelper logHelper;

    logHelper.info("********************************************************");
    logHelper.info("Tucuxi Expert System is starting up...");

    auto result = parse(_argc, _argv);

    logHelper.info("Tucuxi Expert System is exiting...");

    Tucuxi::Common::LoggerHelper::beforeExit();

    return 0;
}
