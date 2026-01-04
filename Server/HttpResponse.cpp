#include "HttpResponse.h"

std::string HttpResponse::toString() const {
	std::string response;
	response += "HTTP/1.1 " + std::to_string(status) + " " + statusText + "\r\n";
	response += "Content-Length: " + std::to_string(body.size()) + "\r\n";
	response += "Connection: close\r\n";
	response += "\r\n";
	response += body;
	return response;
}

std::string text(std::string text) {
	return text;
}