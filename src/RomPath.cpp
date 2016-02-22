#include "RomPath.hpp"
#include "StringUtils.hpp"

using namespace std;

namespace gbemu {

	string getRomPathFromArgvOrFail(int argc, char* argv[]) {
		if (argc < 2) {
			return "";
		}
		return string(argv[1]);
	}

	string getRomFileNameFromPath(string path) {
		auto elements = string_utils::split(path, "\\");
		return elements.back();
	}
}