#pragma once

#include "Pybind/Proxy/attribute.hpp"
#include "Pybind/Proxy/class.hpp"
#include "Pybind/Proxy/enum.hpp"
#include "Pybind/Proxy/function.hpp"
#include "Pybind/Proxy/typeInfo.hpp"
#include <string>
#include <vector>

namespace Pybind::Proxy {

struct Module {
	explicit Module(std::string const& variableName);

	void addFunction(Function const& function);

	void addClass(Class const& c);

	void addEnum(Enum const& e);

	void addAttribute(Attribute const& a);

	std::string const& getVariableName() const;

	void addSubmodule(std::string const& name, std::string const& variableName);

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
};
}    // namespace Pybind::Proxy
