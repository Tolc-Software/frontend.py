#pragma once

#include "PybindProxy/function.hpp"
#include <string>
#include <vector>

namespace PybindProxy {

    struct Module {
	    explicit Module(std::string const& name, std::string const& parent = "")
	        : m_name(name), m_parentModule(parent), m_functions() {}

	    void addFunction(Function const& function) {
		    m_functions.push_back(function);
	    }

	    std::string const& getName() const {
		    return m_name;
	    }

	    std::string getPybind() const {
		    std::string out;
		    for (auto const& function : m_functions) {
			    out += fmt::format("\t{}.{};\n", m_name, function.getPybind());
		    }

		    return out;
	    }

	private:
	    std::string m_name;

	    // If this is non empty,
	    // then this module is a submodule
	    std::string m_parentModule;

	    std::vector<Function> m_functions;
    };

}
