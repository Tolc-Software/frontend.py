#pragma once

#include "PybindProxy/function.hpp"
#include <string>
#include <vector>

namespace PybindProxy {

struct Module {
	explicit Module(std::string const& name)
	    : m_name(name), m_submodules({}), m_functions() {}

	void addFunction(Function const& function) {
		m_functions.push_back(function);
	}

	std::string const& getName() const {
		return m_name;
	}

	void addSubmodule(std::string const& child) {
		m_submodules.push_back(child);
	}

	std::string getPybind() const;

private:
	std::string m_name;

	// If this is non empty,
	// then this module is a submodule
	std::vector<std::string> m_submodules;

	std::vector<Function> m_functions;
};
}
