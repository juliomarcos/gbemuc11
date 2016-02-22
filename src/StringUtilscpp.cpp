#include "StringUtils.hpp"

using namespace std;

namespace string_utils {
	vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems) {
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}

	vector<std::string> split(const std::string &s, char delim) {
		std::vector<std::string> elems;
		split(s, delim, elems);
		return elems;
	}

	vector<string> split(const string &s, string delim) {
		vector<string> elems;
		size_t delimLength = delim.length();

		size_t start = 0;
		size_t end = 0;
		while ((end = s.find(delim, start)) != string::npos) {
			elems.push_back(string(s, start, (end - start)));
			start = end + delimLength;
		}
		elems.push_back(string(s, start));
		return elems;
	}
}