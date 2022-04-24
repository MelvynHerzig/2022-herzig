#ifndef XPERTQUERYIMPORT_H
#define XPERTQUERYIMPORT_H

#include "tucuquery/queryimport.h"

#include "xpertquerydata.h"

namespace Tucuxi {
namespace XpertQuery {

/// \brief This class extends the tucucore queryimporter in order to load administrative information
/// and the custom request "requestXpert"
/// \date 23/04/2022
/// \author Herzig Melvyn
class XpertQueryImport : public Query::QueryImport
{
public:

    /// \brief Xpert query import empty constructor.
    XpertQueryImport();

    /// \brief Destructor.
    virtual ~XpertQueryImport();

protected:

    /// \brief Import a query from an xml document.
    /// \param _query A reference to a query pointer that will be allocated within the function
    /// \param _document An XmlDocument in which the query is stored
    /// \return Result::Ok if the import went well, another Result else.
    /// This function is reentrant.
    Status importDocument(std::unique_ptr<Tucuxi::Query::QueryData>& _query, Tucuxi::Common::XmlDocument& _document);
};

} // namespace XpertQuery
} // namespace Tucuxi

#endif // XPERTQUERYIMPORT_H
