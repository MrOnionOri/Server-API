#include "HttpServer.h"

HttpServer::HttpServer(int port)
    : port(port),
    router(),
    server(port, &router)
{
}

void HttpServer::get(const std::string& path, RouteHandler handler) {
    router.get(path, handler);
}

void HttpServer::start() {
    server.start();
}
