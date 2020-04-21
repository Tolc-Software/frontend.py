#include "PybindProxy/function.hpp"
#include <fmt/format.h>
#include <string>

namespace PybindProxy {

std::string Function::getPybind() const {
	std::string documentation = "";
	auto f = fmt::format(
	    R"(def("{}", &{}, "{}")", m_name, m_fullyQualifiedName, documentation);

	// Add named arguments
	// Allows for myFun(i=5, j=2) in python
	for (auto const& arg : m_arguments) {
		f += fmt::format(R"(, py::arg("{}"))", arg);
	}
	f += ')';

	return f;
}
}
