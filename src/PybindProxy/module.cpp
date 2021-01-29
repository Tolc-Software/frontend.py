#include "PybindProxy/module.hpp"
#include <fmt/format.h>
#include <string>

namespace PybindProxy {

std::string Module::getPybind() const {
	std::string out;
	for (auto const& function : m_functions) {
		out += fmt::format("\t{}.{};\n", m_name, function.getPybind());
	}

	for (auto const& cls : m_classes) {
		out += fmt::format("\t{};\n", cls.getPybind(m_name));
	}

	for (auto const& e : m_enums) {
		out += fmt::format("\t{};\n", e.getPybind(m_name));
	}

	// Define all the children
	for (auto const& submodule : m_submodules) {
		out += fmt::format(
		    "\tauto {submodule} = {name}.def_submodule(\"{submodule}\");\n",
		    fmt::arg("submodule", submodule),
		    fmt::arg("name", m_name));
	}

	return out;
}

Module::Module(std::string const& name)
    : m_name(name), m_submodules({}), m_functions(), m_enums(), m_includes() {}

void Module::addFunction(Function const& function) {
	m_functions.push_back(function);
}

void Module::addClass(Class const& c) {
	m_classes.push_back(c);
}

void Module::addEnum(Enum const& e) {
	m_enums.push_back(e);
}

std::string const& Module::getName() const {
	return m_name;
}

void Module::addSubmodule(std::string const& child) {
	m_submodules.push_back(child);
}

void Module::addInclude(std::string const& i) {
	m_includes.push_back(i);
}

std::vector<std::string> const& Module::getIncludes() const {
	return m_includes;
}
}
