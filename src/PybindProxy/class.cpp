#include "PybindProxy/class.hpp"
#include <fmt/format.h>
#include <string>

namespace PybindProxy {

std::string Class::getPybind(std::string const& moduleName) const {
	std::string out = fmt::format(
	    "py::class_<{fullyQualifiedName}>({moduleName}, \"{name}\")\n",
	    fmt::arg("fullyQualifiedName", m_fullyQualifiedName),
	    fmt::arg("name", m_name),
	    fmt::arg("moduleName", moduleName));

	for (auto const& init : m_constructors) {
		out += fmt::format("\t.{constructorPybind}\n",
		                   fmt::arg("constructorPybind", init.getPybind()));
	}

	for (auto const& function : m_functions) {
		out += fmt::format("\t.{functionPybind}\n",
		                   fmt::arg("functionPybind", function.getPybind()));
	}
	// Remove the last newline
	out.pop_back();

	for (auto const& variable : m_memberVariables) {
		std::string accessor = variable.m_isConst ? "readonly" : "readwrite";
		out += fmt::format(
		    "\t.def_{accessor}(\"{variableName}\", &{fullyQualifiedClassName}::{variableName})\n",
		    fmt::arg("accessor", accessor),
		    fmt::arg("fullyQualifiedClassName", m_fullyQualifiedName),
		    fmt::arg("variableName", variable.m_name));
	}

	// To put the enums at the end of the class
	// we have to do some trickery since we are not allowed to insert the last ';'
	if (!m_enums.empty()) {
		// End the class statement and put in a few newlines before enums
		out += ";\n\n";

		for (auto const& e : m_enums) {
			out += fmt::format("{};\n", e.getPybind(m_name));
		}
		// The last endline
		out.pop_back();
		// The last ';'
		out.pop_back();
	}

	return out;
}

Class::Class(std::string const& name, std::string const& fullyQualifiedName)
    : m_name(name), m_fullyQualifiedName(fullyQualifiedName), m_constructors(),
      m_functions(), m_memberVariables(), m_enums(), m_includes() {}

void Class::addEnum(Enum const& e) {
	m_enums.push_back(e);
}

void Class::addFunction(Function const& function) {
	m_functions.push_back(function);
}

void Class::addConstructor(Function const& constructor) {
	m_constructors.push_back(constructor);
}

void Class::addMemberVariable(std::string const& variableName, bool isConst) {
	m_memberVariables.push_back({variableName, isConst});
}

std::string const& Class::getName() const {
	return m_name;
}

void Class::addInclude(std::string const& i) {
	m_includes.push_back(i);
}

std::vector<std::string> const& Class::getIncludes() const {
	return m_includes;
}
}
