#include "tucuxperthandler.h"
#include "tucucommon/componentmanager.h"
#include "tucucore/drugmodelrepository.h"
#include "tucuquery/querydata.h"
#include "querycreator.h"
#include "htmlexporter.h"
#include "tucuquery/querytocoreextractor.h"
#include "tucuquery/querycomputer.h"

#include "tucuquery/computingqueryresponsexmlexport.h"

namespace Tucuxi {
namespace Xpert {


TucuxpertHandler::TucuxpertHandler()
{

}

// TODO define status codes to return
int TucuxpertHandler::handle (const std::string& _inputFile, const std::string& _outputFile, const std::string& _drugPath)
{
    Common::LoggerHelper log;

    // loading the drug folder in DrugModelRepository
    Common::ComponentManager* pCmpMgr = Common::ComponentManager::getInstance();

    Core::DrugModelRepository *drugModelRepository = dynamic_cast<Core::DrugModelRepository*>(
                Core::DrugModelRepository::createComponent());
    pCmpMgr->registerComponent("DrugModelRepository", drugModelRepository);
    drugModelRepository->addFolderPath (_drugPath);


    // this fills an XpertData object from the input XML's content, with the help of a RequestsCreator to generate requests.
    XpertData xpertData;
    QueryCreator queryCreator;

    if (queryCreator.importFromFile (xpertData, _inputFile) != Query::QueryImport::Status::Ok) {
        log.error("Error in input processing: " + queryCreator.getErrorMessage ());
        return EXIT_FAILURE;
    }


    log.info ("selected drug model: {}" , xpertData.getDrugModelResult().drugModel->getDrugModelId());

    // launching computations with the core

    Query::QueryComputer computer;
    computer.compute(xpertData.getQueries(), xpertData.getResponses());


    // TEMP CHECK, REMOVE!
//    Query::ComputingQueryResponseXmlExport xmlExporter;
//    xmlExporter.exportToFile(xpertData.getResponses(), "../../../tests/TEMP_OUTPUT.xml");


    HtmlExporter htmlExporter;
    htmlExporter.exportToHtml(xpertData, _outputFile);


    // unregister Component
    pCmpMgr->unregisterComponent("DrugModelRepository");
    delete drugModelRepository;


    return EXIT_SUCCESS;
}


}
}

