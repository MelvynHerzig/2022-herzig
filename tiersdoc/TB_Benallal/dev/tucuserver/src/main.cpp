#include "communication/xmlreader.h"
#include "communication/query.h"

#include "pistache/endpoint.h"
#include "pistache/http.h"
#include "pistache/router.h"

#include "impl/HelloApiImpl.h"
#include "impl/ComputationApiImpl.h"
#include "impl/DrugsApiImpl.h"

#include "configuration.h"

using namespace std;
using namespace Pistache;

void init(Http::Endpoint& _pEndpoint, size_t _thr = 2) {
    auto opts = Pistache::Http::Endpoint::options()
        .threads(_thr);
    _pEndpoint.init(opts);
}

void start(Http::Endpoint& _pEndpoint, Rest::Router& _router) {
    _pEndpoint.setHandler(_router.handler());
    _pEndpoint.serve();
}

void shutdown(Http::Endpoint& _pEndpoint) {
    _pEndpoint.shutdown();
}

int main()
{
    uint16_t port = Tucuxi::Server::Configuration::getInstance()->getPort();
    Address addr(Ipv4::any(), Port(port));

    shared_ptr<Rest::Router> pRouter = make_shared<Rest::Router>();
    shared_ptr<Http::Endpoint> pEndpoint = make_shared<Http::Endpoint>(addr);
    init(*pEndpoint, 2);

    Tucuxi::Server::API::HelloApiImpl helloServer(pEndpoint, pRouter);
    helloServer.setupRoutes();
    Tucuxi::Server::API::DrugsApiImpl drugsServer(pEndpoint, pRouter);
    drugsServer.setupRoutes();
    Tucuxi::Server::API::ComputationApiImpl computationServer(pEndpoint, pRouter);
    computationServer.setupRoutes();

    start(*pEndpoint, *pRouter);

    shutdown(*pEndpoint);

    return 0;
}
