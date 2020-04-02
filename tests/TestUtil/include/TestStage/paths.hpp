#pragma once

#include <filesystem>

namespace TestStage {

/**
* Get the path to the root of test stage
* NOTE: This is populated at configure time by cmake
*/
std::filesystem::path getRootStagePath() {
	return "/home/simon/external/code/cpp/frontend.py/tests/testStage";
}
}
