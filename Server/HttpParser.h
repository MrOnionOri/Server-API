#pragma once
#include "HttpRequest.h"

//HttpRequest parserHttpRequest(const std::string& rawRequest) {
//	HttpRequest request;
//	size_t pos = 0;
//	size_t endLine = rawRequest.find("\r\n");
//	std::string requestLine = rawRequest.substr(0, endLine);
//	size_t methodEnd = requestLine.find(' ');
//	request.method = requestLine.substr(0, methodEnd);
//	size_t pathEnd = requestLine.find(' ', methodEnd + 1);
//	request.path = requestLine.substr(methodEnd + 1, pathEnd - methodEnd - 1);
//	pos = endLine + 2;
//	while (true) {
//		endLine = rawRequest.find("\r\n", pos);
//		if (endLine == pos) {
//			pos += 2;
//			break;
//		}
//		std::string headerLine = rawRequest.substr(pos, endLine - pos);
//		size_t colonPos = headerLine.find(':');
//		if (colonPos != std::string::npos) {
//			std::string headerName = headerLine.substr(0, colonPos);
//			std::string headerValue = headerLine.substr(colonPos + 2);
//			request.headers[headerName] = headerValue;
//		}
//		pos = endLine + 2;
//	}
//	request.body = rawRequest.substr(pos);
//	return request;
//}

HttpRequest parseHttpRequest(const std::string & raw);

