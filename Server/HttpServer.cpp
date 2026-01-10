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

ThreadPoolSnapshot HttpServer::getStats() const {
    return server.getPoolSnapshot();
}

void HttpServer::start() {
    server.start();
}
