#include "PybindProxy/module.hpp"
#include <fmt/format.h>
#include <string>

namespace PybindProxy {

std::string Module::getPybind() const {
	std::string out;
	for (auto const& function : m_functions) {
		out += fmt::format("\t{}.{};\n", m_name, function.getPybind());
	}

	return out;
}
}
