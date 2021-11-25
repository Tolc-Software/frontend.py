#pragma once

#include "PybindProxy/attribute.hpp"
#include "PybindProxy/class.hpp"
#include "PybindProxy/enum.hpp"
#include "PybindProxy/function.hpp"
#include <string>
#include <vector>

namespace PybindProxy {

struct Module {
	explicit Module(std::string const& variableName);

	void addFunction(Function const& function);

	void addClass(Class const& c);

	void addEnum(Enum const& e);

	void addAttribute(Attribute const& a);

	std::string const& getVariableName() const;

	void addSubmodule(std::string const& name, std::string const& variableName);

	void addInclude(std::string const& i);

	std::vector<std::string> const& getIncludes() const;

	std::string getPybind() const;

private:
	std::string m_variableName;

	// If this is non empty,
	// then this module is a submodule
	// {Simple name, Variable name}
	// E.g.
	// {"Sub", "NS_Nested_Sub"}
	std::vector<std::pair<std::string, std::string>> m_submodules;

	std::vector<Function> m_functions;
	std::vector<Class> m_classes;
	std::vector<Enum> m_enums;
	std::vector<Attribute> m_attributes;
	std::vector<std::string> m_includes;
};
}
