#include "Pybind/Proxy/function.hpp"
#include "Pybind/Helpers/getDocumentationParameter.hpp"
#include "Pybind/Helpers/string.hpp"
#include "Pybind/returnValuePolicy.hpp"
#include <algorithm>
#include <fmt/format.h>
#include <string>

namespace Pybind::Proxy {

std::string Function::getPybind() const {
	std::string f;
	if (m_isConstructor) {
		// Results in
		// def(py::init<std::string, int, double>), "This is a constructor"
		f = fmt::format(
		    "def(py::init<{}>(), {}",
		    getArgumentTypes(),
		    Pybind::Helpers::getDocumentationParameter(m_documentation));
	} else {
		// Results in
		// def("myFunction", (void (*)(int, double))&MyNamespace::myFunction, "This is a function"
		f = fmt::format(
		    "def{}(\"{}\", {}&{}, {}",
		    m_isStatic ? "_static" : "",
		    m_name,
		    m_isOverloaded ? getSignature() : "",
		    m_fullyQualifiedName,
		    Pybind::Helpers::getDocumentationParameter(m_documentation));

		if (m_returnValuePolicy) {
			f += fmt::format(", py::{returnPolicy}",
			                 fmt::arg("returnPolicy",
			                          Pybind::returnValuePolicyToString(
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
      m_returnType("void"), m_returnValuePolicy(std::nullopt), m_arguments({}),
      m_isConstructor(false), m_isOverloaded(false), m_isStatic(false) {}

void Function::addArgument(std::string const& typeName,
                           std::string const& name) {
	m_arguments.push_back({typeName, name});
};

void Function::setReturnType(std::string const& typeName) {
	m_returnType = typeName;
}

void Function::setReturnValuePolicy(return_value_policy policy) {
	m_returnValuePolicy = policy;
};

void Function::setAsConstructor() {
	m_isConstructor = true;
};

void Function::setAsStatic() {
	m_isStatic = true;
};

void Function::setAsOverloaded() {
	m_isOverloaded = true;
};

void Function::setDocumentation(std::string const& documentation) {
	m_documentation = documentation;
}

std::string Function::getArgumentTypes() const {
	// Get the typenames of the arguments
	std::vector<std::string> typeNames;
	std::transform(m_arguments.begin(),
	               m_arguments.end(),
	               std::back_inserter(typeNames),
	               [](auto const& argument) { return argument.typeName; });
	return fmt::format("{}", fmt::join(typeNames, ", "));
}

std::string Function::getSignature() const {
	return fmt::format(R"(({returnType}({namespace}*)({arguments})))",
	                   fmt::arg("returnType", m_returnType),
	                   fmt::arg("namespace",
	                            Pybind::Helpers::removeSubString(
	                                m_fullyQualifiedName, m_name)),
	                   fmt::arg("arguments", getArgumentTypes()));
}

}    // namespace Pybind::Proxy
