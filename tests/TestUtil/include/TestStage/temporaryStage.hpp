#pragma once

#include <filesystem>
#include <fstream>
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
}    // namespace TestStage
