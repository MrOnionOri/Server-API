#include "HttpResponse.h"

std::string HttpResponse::toString() const {
	std::string response;
	response += "HTTP/1.1 " + std::to_string(status) + " " + statusText + "\r\n";
	response += "Content-Type: " + contentType + "\r\n";
	response += "Content-Length: " + std::to_string(body.size()) + "\r\n";
	response += "Connection: close\r\n";
	response += "\r\n";
	response += body;
	return response;
}

HttpResponse HttpResponse::text(const std::string& text) {
	HttpResponse res;
	res.contentType = "text/plain";
	res.body = text;
	return res;
}

HttpResponse HttpResponse::json(const std::string& json) {
	HttpResponse res;
	res.contentType = "application/json";
	res.body = json;
	return res;
}

HttpResponse HttpResponse::internalError() {
	HttpResponse res;
	res.status = 500;
	res.statusText = "Internal Server Error";
	res.body = "500 Internal Server Error";
	return res;
}