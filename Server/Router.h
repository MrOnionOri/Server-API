#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include "HttpRequest.h"
#include "HttpResponse.h"

using RouteHandler = std::function<HttpResponse(const HttpRequest&)>;

class Router {
public:
	void get(const std::string& path, RouteHandler  handler);
	HttpResponse route(const HttpRequest& req);

private:
	std::unordered_map<std::string, RouteHandler> getRoutes;
};