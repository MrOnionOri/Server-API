#include "Json.h"

Json::Json() : first(true) {
	ss << "{";
}

void Json::addComma() {
	if(!first) ss << ",";
	first = false;
}

Json& Json::add(const std::string& key, const std::string& value) {
	addComma();
	ss << "\"" << key << "\":\"" << value << "\"";
	return *this;
}

Json& Json::add(const std::string& key, int value) {
	addComma();
	ss << "\"" << key << "\":" << value;
	return *this;
}

Json& Json::add(const std::string& key, bool value) {
	addComma();
	ss << "\"" << key << "\":" << (value ? "true" : "false");
	return *this;
}

Json& Json::add(const std::string& key, double value) {
	addComma();
	ss << "\"" << key << "\":" << value;
	return *this;
}

Json& Json::addArray(const std::string& key, const std::vector<int>& value) {
	addComma();
	ss << "\"" << key << "\":[";
	for(size_t i = 0; i < value.size(); ++i) {
		if(i > 0) ss << ",";
		ss << value[i];
	}
	ss << "]";
	return *this;
}

Json& Json::addArray(const std::string& key, const std::vector<std::string>& value) {
	addComma();
	ss << "\"" << key << "\":[";
	for(size_t i = 0; i < value.size(); ++i) {
		if(i > 0) ss << ",";
		ss << "\"" << value[i] << "\"";
	}
	ss << "]";
	return *this;
}

Json& Json::addArray(const std::string& key, const std::vector<bool>& value) {
	addComma();
	ss << "\"" << key << "\":[";
	for(size_t i = 0; i < value.size(); ++i) {
		if(i > 0) ss << ",";
		ss << (value[i] ? "true" : "false");
	}
	ss << "]";
	return *this;
}

Json& Json::addArray(const std::string& key, const std::vector<double>& value) {
	addComma();
	ss << "\"" << key << "\":[";
	for(size_t i = 0; i < value.size(); ++i) {
		if(i > 0) ss << ",";
		ss << value[i];
	}
	ss << "]";
	return *this;
}	

Json& Json::addObject(const std::string& key, const Json& value) {
	addComma();
	ss << "\"" << key << "\":" << value.str();
	return *this;
}

Json& Json::add(const std::string& key, size_t value) {
	return add(key, static_cast<int>(value));
}

std::string Json::str() const {
	return ss.str() + "}";
}