/**
* Tucuxi TDM server
* This is the Tucuxi TDM server.
*
* OpenAPI spec version: 1.0.0
* Contact: nadir.benallal@heig-vd.ch
*
* NOTE: This class is auto generated by the swagger code generator program.
* https://github.com/swagger-api/swagger-codegen.git
* Do not edit the class manually.
*/

#include "ComputationApi.h"

namespace Tucuxi {
namespace Server {
namespace API {

ComputationApi::ComputationApi(std::shared_ptr<Pistache::Http::Endpoint> _pEndpoint, std::shared_ptr<Pistache::Rest::Router> _pRouter)
    : m_httpEndpoint(_pEndpoint), m_router(_pRouter)
{ };

void ComputationApi::setupRoutes() {
    using namespace Pistache::Rest;

    Routes::Post(*m_router, m_base + "/computation", Routes::bind(&ComputationApi::compute_requests_handler, this));
}

void ComputationApi::compute_requests_handler(const Pistache::Rest::Request& _request, Pistache::Http::ResponseWriter _response) {
    
    try {
        XMLReader xmlReader(_request.body());
        this->compute_requests(xmlReader.getQuery(), _response);
    } catch (std::runtime_error& e) {
      //send a 400 error
      _response.send(Pistache::Http::Code::Bad_Request, e.what());
      return;
    }

}

void ComputationApi::computation_api_default_handler(const Pistache::Rest::Request& _request, Pistache::Http::ResponseWriter _response) {
    UNUSED(_request);
    _response.send(Pistache::Http::Code::Not_Found, "The requested method does not exist (ComputationApi)");
}

} // namespace API
} // namespace Server
} // namespace Tucuxi

