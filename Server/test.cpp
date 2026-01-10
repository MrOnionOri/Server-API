#include <iostream>
#include "Json.h"
#include "HttpResponse.h"
#include "HttpRequest.h"

inline HttpResponse testPlace(const HttpRequest& req) {
	Json json;
	json.add("name", "John Doe")
		.add("age", 30)
		.add("isStudent", false)
		.add("height", 5.9);
	std::vector<int> scores = {85, 90, 78};
	json.addArray("scores", scores);
	std::vector<std::string> hobbies = {"reading", "gaming", "hiking"};
	json.addArray("hobbies", hobbies);
	Json address;
	address.add("street", "123 Main St")
		   .add("city", "Anytown")
		   .add("zip", "12345");
	json.addObject("address", address);
	HttpResponse pendejo = HttpResponse::json(json.str());

	return pendejo;
}