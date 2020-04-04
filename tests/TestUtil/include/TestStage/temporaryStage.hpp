#pragma once

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>

namespace TestStage {
/**
* Create a unique stage in rootStage/stages
* Returns the path to the newly created stage
*/
std::filesystem::path
createUniqueStage(std::filesystem::path const& rootStage) {
	// Use to build a random string
	std::random_device r;
	std::default_random_engine rEngine(r());
	std::uniform_int_distribution<> uniform(0, std::numeric_limits<int>::max());
	auto stages = rootStage / "stages";

	// Ensure that the new stage does not exist before
	std::filesystem::path newStage = stages / std::to_string(uniform(rEngine));
	while (std::filesystem::exists(newStage)) {
		newStage = stages / std::to_string(uniform(rEngine));
	}

	std::filesystem::create_directories(newStage);

	return newStage;
}

/**
* Copy files from rootStage to stage
* Inherits the files:
*     CMakeLists.txt
*     Anything in the cmake directory
*     Anything in the build/_deps directory
*/
void populateStageFromRoot(std::filesystem::path const& stage,
                           std::filesystem::path const& rootStage) {
	namespace fs = std::filesystem;
	std::vector<fs::path> pathsToCopy = {
	    "cmake", "CMakeLists.txt", fs::path("build") / "_deps"};
	for (auto const& p : pathsToCopy) {
		if (fs::exists(rootStage / p)) {
			if (fs::is_directory(rootStage / p)) {
				fs::create_directories(stage / p);
			}
			fs::copy(rootStage / p, stage / p);
		}
	}
}

/**
* Set the name of the target that will be built in the stage
*/
void setTargetName(std::filesystem::path const& stage,
                   std::string const& target) {
	// Make sure it exists
	std::filesystem::create_directories(stage / "cmake");

	std::ofstream file(stage / "cmake" / "TargetName.cmake");
	file << "set(target_name \"" + target + "\")";
	file.close();
}

/**
* Adds a file to the source directory in the stage
* Returns the path to the file
*/
std::filesystem::path addFileToStage(std::filesystem::path const& file,
                                     std::string const& content,
                                     std::filesystem::path const& stage) {
	auto src = stage / "src";
	std::filesystem::create_directories(src);
	std::ofstream f(src / file);
	f << content;
	f.close();
	return src / file;
}

/**
* Configure cmake project in stage
* Provides reasonable defaults
*/
void runCMakeConfigure(std::filesystem::path const& stage,
                       std::string const& compiler = "clang++",
                       std::string const& generator = "Ninja",
                       std::string const& buildType = "Release") {
	// Save to restore after function call
	auto originalDirectory = std::filesystem::current_path();

	// Switch to stage
	std::filesystem::current_path(stage);
	auto cmd = "cmake -S. -Bbuild -G " + generator +
	           " -DCMAKE_CXX_COMPILER=" + compiler +
	           " -DCMAKE_BUILD_TYPE=" + buildType;
	std::cout << cmd << '\n';
	std::system(cmd.c_str());

	std::filesystem::current_path(originalDirectory);
}

/**
* Build cmake project in stage
*/
void buildCMakeProject(std::filesystem::path const& stage) {
	// Save to restore after function call
	auto originalDirectory = std::filesystem::current_path();

	// Switch to stage
	std::filesystem::current_path(stage);
	auto cmd = "cmake --build build -j1";
	std::cout << cmd << '\n';
	std::system(cmd);

	std::filesystem::current_path(originalDirectory);
}

/**
 * Adds a unittest to build directory in the stage
 */
void addPythonUnittest(std::filesystem::path const& stage,
                       std::string const& moduleName,
                       std::string const& testName,
                       std::vector<std::string> const& testBody,
                       std::vector<std::string> const& includes) {
	std::string test = "import unittest\n";
	for (auto const& include : includes) {
		test += "import " + include + '\n';
	}

	// Add the test class
	test += R"(


class Test)" +
	        moduleName + R"((unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        pass
)";

	test += R"(
    def test_)" +
	        testName + R"((self):
)";

	for (auto const& line : testBody) {
		test += R"(
        )" + line +
		        '\n';
	}

	// Make it easily runnable
	test += R"(
if __name__ == "__main__":
    unittest.main()
)";

	std::filesystem::create_directories(stage / "build");

	std::string filename = "test_" + moduleName + ".py";
	std::ofstream testFile(stage / "build" / filename);
	testFile << test;
	testFile.close();
}

int runPythonUnittest(std::filesystem::path const& stage,
                      std::string const& moduleName) {
	// Save to restore after function call
	auto originalDirectory = std::filesystem::current_path();

	// Switch to stage
	std::filesystem::current_path(stage / "build");
	auto cmd = "python3 test_" + moduleName + ".py";
	std::cout << cmd << '\n';
	auto errorCode = std::system(cmd.c_str());

	std::filesystem::current_path(originalDirectory);
	return errorCode;
}
}    // namespace TestStage
