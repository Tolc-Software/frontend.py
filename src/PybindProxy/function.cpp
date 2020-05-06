#include "PybindProxy/function.hpp"
#include "Helpers/enumsToString.hpp"
#include <fmt/format.h>
#include <string>

namespace PybindProxy {

void Function::addArgument(std::string const& argument) {
	m_arguments.push_back(argument);
};

void Function::setReturnValuePolicy(return_value_policy policy) {
	m_returnValuePolicy = policy;
};

std::string Function::getPybind() const {
	std::string documentation = "";
	auto f = fmt::format(
	    R"(def("{}", &{}, "{}")", m_name, m_fullyQualifiedName, documentation);

	if (m_returnValuePolicy) {
		f += fmt::format(", py::{returnPolicy}",
		                 fmt::arg("returnPolicy",
		                          Helpers::returnValuePolicyToString(
		                              m_returnValuePolicy.value())));
	}

	// Add named arguments
	// Allows for myFun(i=5, j=2) in python
	for (auto const& arg : m_arguments) {
		f += fmt::format(R"(, py::arg("{}"))", arg);
	}
	f += ')';

	return f;
}
}    // namespace PybindProxy
