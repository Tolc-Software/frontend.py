#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace TestStage {

/**
* Represents a CMake project that can be used to build and run tests.
* It inherits from a base project that it copies files from.
*
* It is not strictly necessary,
* but to enable some functionality assumptions about the baseStage are made:
* To enable setTargetName:
*     - The target in the base project CMakeLists.txt includes cmake/TargetName.cmake
*     - The target is set from the variable 'target_name'
* Ex: (CMakeLists.txt)
*     include(cmake/TargetName.cmake)
*     add_executable(${target_name} src/main.cpp)
* To get the source file from addSourceFile into a target:
*
*/
struct Stage {
	/**
	* Create a unique stage in baseStage/stages
	* Copy files from baseStage to stage
	* Inherits the files:
	*     CMakeLists.txt
	*     Anything in the cmake directory
	*     Anything in the build/_deps directory
	*/
	Stage(std::filesystem::path const& baseStage);

	/**
	* Removes the stage and everything in it from the filesystem
	*/
	~Stage();

	/**
	* Set the name of the CMake target that will be built in the stage
	* Sets the variable 'target_name' in cmake/TargetName.cmake
	*/
	void setTargetName(std::string const& target);

	/**
	* Adds a file to the source directory in the stage
	* Returns the path to the file
	* Sets the variable 'sources' in cmake/Sources.cmake
	* Ex:
	*     addSourceFile("myFile.cpp", "int f() { return 5; }");
	* Results in:
	*     File m_stage/src/myfile.cpp
	*     with corresponding content
	*/
	std::filesystem::path addSourceFile(std::filesystem::path const& file,
	                                    std::string const& content);

	/**
	* Adds a file to the stage
	* Returns the path to the file
	*/
	std::filesystem::path addFile(std::filesystem::path const& file,
	                              std::string const& content);

	/**
	* Configure CMake project in stage
	* Provides reasonable defaults
	*/
	void runCMakeConfigure(std::string const& compiler = "clang++",
	                       std::string const& generator = "Ninja",
	                       std::string const& buildType = "Release");

	/**
	* Build CMake target
	*/
	void buildCMakeProject();

	/**
	* Runs a command within the test stage
	* workingDirectory is relative the stage root
	* Returns the exit status of the command
	*/
	int runCommand(
	    std::string const& cmd,
	    std::optional<std::filesystem::path> workingDirectory = std::nullopt);

	// Path to the generated stage
	std::filesystem::path m_stage;

	// Source files
	std::vector<std::filesystem::path> m_sources = {};

	// If the stage should be removed when this object is destroyed
	bool m_removeOnDestruction = true;
};
}
