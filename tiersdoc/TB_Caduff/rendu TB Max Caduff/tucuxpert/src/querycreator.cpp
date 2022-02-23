#include "querycreator.h"
#include "requestcreator.h"
#include "drugmodelselector.h"



namespace Tucuxi {
namespace Xpert {


QueryCreator::QueryCreator()
{

}

Common::IImport::Status QueryCreator::importFromFile(XpertData& _xpertData, const string &_filePath)
{
    // Ensure the function is reentrant
    std::lock_guard<std::mutex> lock(m_mutex);

    setStatus(Status::Ok);

    Common::XmlDocument document;
    if (!document.open(_filePath)) {
        setStatus(Status::CantCreateXmlDocument, "file could not be opened");
        return Status::CantOpenFile;
    }

    return createQuery(_xpertData, document);
}

Common::IImport::Status QueryCreator::importFromString(XpertData& _xpertData, const string &_xmlStr)
{
    // Ensure the function is reentrant
    lock_guard<mutex> lock(m_mutex);

    setStatus(Status::Ok);

    Common::XmlDocument document;
    if (!document.fromString(_xmlStr)) {
        setStatus(Status::CantCreateXmlDocument, "xml document could not be created. The tags must be controlled");
        return Status::CantCreateXmlDocument;
    }

    return createQuery(_xpertData, document);
}


QueryCreator::Status QueryCreator::createQuery (XpertData& _xpertData, Common::XmlDocument & _document)
{

    if ( ! _document.isValid ()) {
        setStatus (Status::Error, "XML document is not loaded or not valid!");
        return Status::Error;
    }

    Query::QueryData* tempQuery;

    if (importDocument (tempQuery, _document) != Status::Ok ) {
        return getStatus ();
    }

    _xpertData.getQueries().m_queryId = tempQuery->getQueryID();
    _xpertData.setLang (tempQuery->getLanguage() );

    if ( RequestCreator().createRequests (_xpertData, tempQuery) == EXIT_FAILURE) {
        setStatus (Status::Error, "Requests creation failed.");
        return Status::Error;
    }



    // TODO: parse and set AdminData !!



    delete tempQuery;

    return getStatus();

}



}
}
