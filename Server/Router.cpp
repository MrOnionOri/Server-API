#include "Router.h"

void Router::get(const std::string& path, RouteHandler handler) {
	getRoutes[path] = handler;
}
HttpResponse Router::route(const HttpRequest& req) {
	auto it = getRoutes.find(req.path);
	if (it != getRoutes.end()) {
		return it->second(req);
	}
	HttpResponse res;
	res.status = 404;
	res.statusText = "Not Found";
	res.body = "404 Not Found";
	return res;
}