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
	* Copy files/directories in pathsToCopy from baseStage to stage
	*/
	Stage(std::filesystem::path const& baseStage,
	      std::vector<std::filesystem::path> const& pathsToCopy);

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

	/*
	* Configure CMake project in stage
	* then build with reasonable defaults
	* Returns the exit code of the first command that fails or 0
	*/
	int configureAndBuild();

	/**
	* Runs a command within the test stage
	* workingDirectory is relative the stage root
	* Returns the exit code of the command
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

private:
	/**
	* NOTE: This is not a cross platform function
	* Configure CMake project in stage
	* Provides reasonable defaults
	* Returns the exit code of the command
	*/
	int runCMakeConfigure(std::string const& compiler);

	/**
	* NOTE: This is not a cross platform function
	* Build CMake target
	* Returns the exit code of the command
	*/
	int buildCMakeProject();
};
}    // namespace TestStage
