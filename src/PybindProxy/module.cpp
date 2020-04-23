#include "PybindProxy/module.hpp"
#include <fmt/format.h>
#include <string>

namespace PybindProxy {

std::string Module::getPybind() const {
	std::string out;
	for (auto const& function : m_functions) {
		out += fmt::format("\t{}.{};\n", m_name, function.getPybind());
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
}
