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
		out += fmt::format("\t.def(py::init<{}>())\n",
		                   fmt::join(init.m_arguments, ", "));
	}

	for (auto const& function : m_functions) {
		out += fmt::format("\t.{functionPybind}\n",
		                   fmt::arg("functionPybind", function.getPybind()));
	}
	// Remove the last newline
	out.pop_back();

	return out;
}
}
