#pragma once

#include <vector>
#include <string>
#include <sstream>

using namespace std;

namespace string_utils {

	vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);

	vector<std::string> split(const std::string &s, char delim);

	vector<string> split(const string &s, string delim);

}