#pragma once
#include <string>
#include <sstream>
#include <vector>

class Json {
public:
	Json();

	Json& add(const std::string& key, const std::string& value);
	Json& add(const std::string& key, int value);
	Json& add(const std::string& key, bool value);
	Json& add(const std::string& key, double value);

	Json& addArray(const std::string& key, const std::vector<int>& value);
	Json& addArray(const std::string&, const std::vector<std::string>& value);
	Json& addArray(const std::string&, const std::vector<bool>& value);
	Json& addArray(const std::string&, const std::vector<double>& value);

	Json& addObject(const std::string& key, const Json& value);

	std::string str() const;

private:
	std::ostringstream ss;
	bool first;
	
	void addComma();
};