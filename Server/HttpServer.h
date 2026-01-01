#pragma once
#include <string>
#include "Router.h"
#include "ServerCore.h"

class HttpServer {
public:
    explicit HttpServer(int port);

    void get(const std::string& path, RouteHandler handler);

    void start();

private:
    int port;        // Puerto del servidor
    Router router;   // Router HTTP (vive toda la vida del server)
    Server server;   // Core TCP/HTTP (usa el router)
};