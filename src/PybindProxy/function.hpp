#pragma once

#include <string>
#include <vector>

namespace PybindProxy {
	struct Function {
	    Function(std::string const& name, std::string const& fullyQualifiedName)
	        : m_name(name), m_fullyQualifiedName(fullyQualifiedName),
	          m_arguments({}) {}

	    std::string getPybind() const;

	    void addArgument(std::string const& argument) {
		    m_arguments.push_back(argument);
	    };

	private:
	    std::string m_name;
	    std::string m_fullyQualifiedName;
	    std::vector<std::string> m_arguments;
    };
}
