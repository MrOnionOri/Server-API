#pragma once
#include <string>

struct HttpResponse {
	int status = 200;
	std::string statusText = "OK";
	std::string body;
	std::string contentType = "text/plain";
	std::string header;
	
	std::string toString() const;

	// Helpers (factory methods)
	static HttpResponse text(const std::string& text);
	static HttpResponse json(const std::string& json);
	static HttpResponse notFound();
	static HttpResponse internalError();
};