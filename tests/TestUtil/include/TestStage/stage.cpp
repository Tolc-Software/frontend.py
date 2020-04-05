#include "TestStage/stage.hpp"
#include <fmt/format.h>
#include <fstream>
#include <limits>
#include <optional>
#include <random>

namespace {
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

}    // namespace

namespace TestStage {
Stage::Stage(std::filesystem::path const& baseStage)
    : m_stage(createUniqueStage(baseStage)) {
	// Copy the inherited files from base to this stage
	// Inherits the files:
	//      * CMakeLists.txt
	//      * Anything in the cmake directory
	//      * Anything in the build/_deps directory
	namespace fs = std::filesystem;
	std::vector<fs::path> pathsToCopy = {
	    "cmake", "CMakeLists.txt", fs::path("build") / "_deps"};
	for (auto const& p : pathsToCopy) {
		if (fs::exists(baseStage / p)) {
			if (fs::is_directory(baseStage / p)) {
				fs::create_directories(m_stage / p);
			}
			fs::copy(baseStage / p, m_stage / p);
		}
	}
}

Stage::~Stage() {
	std::filesystem::remove_all(m_stage);
}

void Stage::setTargetName(std::string const& target) {
	// Make sure it exists
	std::filesystem::create_directories(m_stage / "cmake");

	std::ofstream file(m_stage / "cmake" / "TargetName.cmake");
	file << "set(target_name \"" + target + "\")";
	file.close();
}

std::filesystem::path Stage::addSourceFile(std::filesystem::path const& file,
                                           std::string const& content) {
	auto src = m_stage / "src";
	std::filesystem::create_directories(src);
	auto filepath = src / file;
	addFile(filepath, content);
	m_sources.push_back(filepath);

	// Add the sources variable
	std::filesystem::create_directories(m_stage / "cmake");
	std::ofstream cmakeFile(m_stage / "cmake" / "Sources.cmake");
	cmakeFile << "set(sources ";
	for (auto const& source : m_sources) {
		cmakeFile << source.c_str() << '\n';
	}
	cmakeFile << ")";
	cmakeFile.close();

	return filepath;
}

std::filesystem::path Stage::addFile(std::filesystem::path const& file,
                                     std::string const& content) {
	auto filepath = m_stage / file;
	std::ofstream f(filepath);
	f << content;
	f.close();
	return filepath;
}

void Stage::runCMakeConfigure(std::string const& compiler,
                              std::string const& generator,
                              std::string const& buildType) {
	runCommand(fmt::format(
	    "cmake -S. -Bbuild -G {} -DCMAKE_CXX_COMPILER={} -DCMAKE_BUILD_TYPE={}",
	    generator,
	    compiler,
	    buildType));
}

void Stage::buildCMakeProject() {
	runCommand("cmake --build build -j1");
}

int Stage::runCommand(std::string const& cmd,
                      std::optional<std::filesystem::path> workingDirectory) {
	// Save to restore after function call
	auto originalDirectory = std::filesystem::current_path();

	auto cwd = m_stage;
	if (workingDirectory) {
		cwd = m_stage / workingDirectory.value();
		std::filesystem::create_directories(cwd);
	}
	std::filesystem::current_path(cwd);

	fmt::print(cmd);
	auto errorCode = std::system(cmd.c_str());

	std::filesystem::current_path(originalDirectory);
	return errorCode;
}
}

