#include <string>
#include "HttpParser.h"
#include <iostream>

HttpRequest parseHttpRequest(const std::string& raw) {
    HttpRequest req;

    auto endLine = raw.find("\r\n");
    auto firstLine = raw.substr(0, endLine);


    auto p1 = firstLine.find(' ');
    auto p2 = firstLine.find(' ', p1 + 1);

    req.method = firstLine.substr(0, p1);
    req.path = firstLine.substr(p1 + 1, p2 - p1 - 1);

    return req;
}