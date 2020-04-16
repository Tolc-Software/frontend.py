#pragma once

#include "PybindProxy/module.hpp"
#include <filesystem>
#include <string>
#include <vector>

namespace PybindProxy {

class ModuleFile {
public:
	explicit ModuleFile(Module const& rootModule)
	    : m_rootModuleName(rootModule.getName()),
	      m_includes({"<pybind11/pybind11.h>"}), m_modules({rootModule}) {}

	/**
	* Add a module to the file.
	* NOTE: The order these are added is the order they will be put in the file
	*/
	void addSubmodule(Module const& m) {
		m_modules.push_back(m);
	}

	std::filesystem::path getFilepath() const {
		return m_rootModuleName + ".cpp";
	}

	std::string getPybind() const;

private:
	std::string m_rootModuleName;

	// What will be included
	// Ex: '<string>' for '#include <string>'
	// NOTE: Use <> or "" explicitly
	std::vector<std::string> m_includes;

	// Will be put one after each other
	// The modules are responsible for which of them are submodules
	std::vector<Module> m_modules;
};
}    // namespace PybindProxy
