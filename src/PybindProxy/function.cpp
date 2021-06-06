#include "PybindProxy/function.hpp"
#include "Helpers/enumsToString.hpp"
#include <algorithm>
#include <fmt/format.h>
#include <string>

namespace PybindProxy {

std::string Function::getPybind() const {
	std::string documentation = "";

	std::string f;
	if (m_isConstructor) {
		// Get the typenames of the arguments
		std::vector<std::string> typeNames;
		typeNames.reserve(m_arguments.size());
		for (auto const& arg : m_arguments) {
			typeNames.push_back(arg.typeName);
		}

		// Results in
		// def(py::init<std::string, int, double>), "This is a constructor"
		f = fmt::format(R"(def(py::init<{}>(), "{}")",
		                fmt::join(typeNames, ", "),
		                documentation);
	} else {
		// Results in
		// def("myFunction", &MyNamespace::myFunction, "This is a function"
		f = fmt::format(R"(def("{}", &{}, "{}")",
		                m_name,
		                m_fullyQualifiedName,
		                documentation);

		if (m_returnValuePolicy) {
			f += fmt::format(", py::{returnPolicy}",
			                 fmt::arg("returnPolicy",
			                          Helpers::returnValuePolicyToString(
			                              m_returnValuePolicy.value())));
		}
	}

	// Add named arguments
	// Allows for myFun(i=5, j=2) in python
	// If all arguments names are specified in C++, we can let the python user see them
	if (std::none_of(m_arguments.begin(),
	                 m_arguments.end(),
	                 [](const auto& arg) { return arg.name.empty(); })) {
		for (auto const& arg : m_arguments) {
			f += fmt::format(R"(, py::arg("{}"))", arg.name);
		}
	}
	f += ')';

	return f;
}

Function::Function(std::string const& name,
                   std::string const& fullyQualifiedName)
    : m_name(name), m_fullyQualifiedName(fullyQualifiedName),
      m_returnValuePolicy(std::nullopt), m_arguments({}),
      m_isConstructor(false) {}

void Function::addInclude(std::string const& i) {
	m_includes.push_back(i);
}

std::vector<std::string> const& Function::getIncludes() const {
	return m_includes;
}

void Function::addArgument(std::string const& typeName,
                           std::string const& name) {
	m_arguments.push_back({typeName, name});
};

void Function::setReturnValuePolicy(return_value_policy policy) {
	m_returnValuePolicy = policy;
};

void Function::setAsConstructor() {
	m_isConstructor = true;
};

}    // namespace PybindProxy
