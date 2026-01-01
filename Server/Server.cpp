#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "HttpServer.h"
#include "HttpRequest.h"
//#include "ServerCore.h";

#pragma comment(lib, "Ws2_32.lib")

int main() {
    HttpServer server(8080);
    server.get("/ping", [](const HttpRequest&) {
		HttpResponse response;
		response.body = "pong";
		return response;
    });
    server.start();
}
