#include "xpertqueryimport.h"

namespace Tucuxi {
namespace XpertQuery {

XpertQueryImport::XpertQueryImport() = default;


XpertQueryImport::~XpertQueryImport() = default;


Common::IImport::Status XpertQueryImport::importDocument(std::unique_ptr<Query::QueryData>& _query, Common::XmlDocument& _document)
{
    return Tucuxi::Query::QueryImport::importDocument(_query, _document);
}


} // namespace XpertQuery
} // namespace Tucuxi
