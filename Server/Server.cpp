#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "HttpServer.h"
#include "HttpRequest.h"
#include "test.cpp"

#pragma comment(lib, "Ws2_32.lib")

int main() {
    HttpServer server(8080);
    server.get("/ping", [](const HttpRequest&) {
		HttpResponse response;
		response.body = "Hola Mundo";
		return response;
    });
    server.get("/adio", testPlace);
    server.get("/stats", [&server](const HttpRequest&) { //implementar unas rutas por default 
        auto s = server.getStats();

        Json json;
        json.add("totalThreads", s.totalThreads)
            .add("busyThreads", s.busyThreads)
            .add("idleThreads", s.idleThreads)
            .add("queuedTasks", s.queuedTasks);

        return HttpResponse::json(json.str());
    });
    server.start();
}