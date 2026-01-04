#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "HttpServer.h"
#include "HttpRequest.h"

#pragma comment(lib, "Ws2_32.lib")

int main() {
    HttpServer server(8080);
    server.get("/ping", [](const HttpRequest&) {
		HttpResponse response;
		response.body = "Hola Mundo";
		return response;
    });
    server.get("/adio", [](const HttpRequest&) {
        HttpResponse response;
        response.body = "<html><body><h1>ADIOS MUNDO</h1></body></html>";
        return response;
    });
    server.start();
}
