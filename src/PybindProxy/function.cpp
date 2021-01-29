#include "PybindProxy/function.hpp"
#include "Helpers/enumsToString.hpp"
#include <fmt/format.h>
#include <string>

namespace PybindProxy {

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

Function::Function(std::string const& name,
                   std::string const& fullyQualifiedName)
    : m_name(name), m_fullyQualifiedName(fullyQualifiedName),
      m_returnValuePolicy(std::nullopt), m_arguments({}) {}

void Function::addInclude(std::string const& i) {
	m_includes.push_back(i);
}

std::vector<std::string> const& Function::getIncludes() const {
	return m_includes;
}

void Function::addArgument(std::string const& argument) {
	m_arguments.push_back(argument);
};

void Function::setReturnValuePolicy(return_value_policy policy) {
	m_returnValuePolicy = policy;
};

}    // namespace PybindProxy
