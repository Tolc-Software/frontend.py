#pragma once

#include "PybindProxy/function.hpp"
#include <string>
#include <vector>

namespace PybindProxy {

struct Constructor {
	explicit Constructor(std::vector<std::string> const& arguments)
	    : m_arguments(arguments) {};

	std::vector<std::string> m_arguments;
};

class Class {
public:
	explicit Class(std::string const& name) : m_name(name), m_functions() {}

	void addFunction(Function const& function) {
		m_functions.push_back(function);
	}

	void addConstructor(Constructor const& constructor) {
		m_constructors.push_back(constructor);
	}

	std::string const& getName() const {
		return m_name;
	}

	std::string getPybind(std::string const& moduleName) const;

private:
	std::string m_name;

	std::vector<Function> m_functions;
	std::vector<Constructor> m_constructors;
};
}    // namespace PybindProxy
