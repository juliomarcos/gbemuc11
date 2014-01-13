#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>

using namespace std;

namespace gbemu {
	string getRomFileNameFromPath(string path);
	string getRomPathFromArgvOrFail(int argc, char* argv[]);
}
