#include "PybindProxy/class.hpp"
#include <fmt/format.h>
#include <string>

namespace PybindProxy {

std::string Class::getPybind(std::string const& moduleName) const {
	std::string out =
	    fmt::format("\tpy::class_<{name}>({moduleName}, \"{name}\")\n",
	                fmt::arg("name", m_name),
	                fmt::arg("moduleName", moduleName));

	for (auto const& init : m_constructors) {
		out += fmt::format("\t\t.def(py::init<{}>())\n",
		                   fmt::join(init.m_arguments, ", "));
	}

	for (auto const& function : m_functions) {
		out += fmt::format("\t\t.{functionPybind}\n",
		                   fmt::arg("functionPybind", function.getPybind()));
	}
	// Remove the last newline
	out.pop_back();
	out += ';';

	return out;
}
}
