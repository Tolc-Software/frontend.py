#pragma once

#include "PybindProxy/class.hpp"
#include "PybindProxy/enum.hpp"
#include "PybindProxy/function.hpp"
#include <string>
#include <vector>

namespace PybindProxy {

struct Module {
	explicit Module(std::string const& name,
	                std::string const& fullyQualifiedName);

	void addFunction(Function const& function);

	void addClass(Class const& c);

	void addEnum(Enum const& e);

	std::string const& getName() const;

	void addSubmodule(std::string const& child);

	void addInclude(std::string const& i);

	// Return a unique variable name that can be used in the generated code for this module
	std::string getVariableName() const;

	std::vector<std::string> const& getIncludes() const;

	std::string getPybind() const;

private:
	// User defined name of the module
	std::string m_name;

	std::string m_fullyQualifiedName;

	// If this is non empty,
	// then this module is a submodule
	std::vector<std::string> m_submodules;

	std::vector<Function> m_functions;
	std::vector<Class> m_classes;
	std::vector<Enum> m_enums;
	std::vector<std::string> m_includes;
};
}
