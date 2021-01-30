#pragma once

#include "PybindProxy/module.hpp"
#include <filesystem>
#include <string>
#include <vector>

namespace PybindProxy {

class ModuleFile {
public:
	explicit ModuleFile(Module const& rootModule,
	                    std::string const& libraryName);

	/**
	* Add a module to the file.
	* NOTE: The order these are added is the order they will be put in the file
	*/
	void addModule(Module const& m);

	void addInclude(std::string const& i);

	std::filesystem::path getFilepath() const;

	std::string getPybind() const;

private:
	std::string m_rootModuleName;
	std::string m_libraryName;

	// What will be included
	// Ex: '<string>' for '#include <string>'
	// NOTE: Use <> or "" explicitly
	std::vector<std::string> m_includes;

	// Will be put one after each other
	// The modules are responsible for which of them are submodules
	std::vector<Module> m_modules;
};
}    // namespace PybindProxy
