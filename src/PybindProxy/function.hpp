#pragma once

#include <string>

namespace PybindProxy {
	struct Function {
	    Function(std::string const& name, std::string const& fullyQualifiedName)
	        : m_name(name), m_fullyQualifiedName(fullyQualifiedName) {}

	    std::string getPybind() const;

	private:
	    std::string m_name;
	    std::string m_fullyQualifiedName;
    };
}
