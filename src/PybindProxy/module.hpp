#pragma once

#include "PybindProxy/function.hpp"
#include <string>
#include <vector>

namespace PybindProxy {
	struct Module {
	    Module(std::string const& name)
	        : m_name(name), m_includes({"pybind11/pybind11.h"}) {}

	    void addFunction(Function const& function) {
		    for (auto const& include : function.getIncludes()) {
			    // TODO: Make these unique on write
			    m_includes.push_back(include);
		    }

		    m_functions.push_back(function);
	    }

	    std::string getPybind() const {
		    std::string out;
		    for (auto const& include : m_includes) {
			    out += fmt::format("#include <{}>\n", include);
		    }

		    out += fmt::format("PYBIND11_MODULE({}, m)", m_name);
		    out += " {\n";
		    for (auto const& function : m_functions) {
			    out += fmt::format("\t{}\n", function.getPybind());
		    }
		    out += "}";

		    return out;
	    }

	private:
	    std::string m_name;
	    std::vector<Function> m_functions;
	    // What will be included
	    // Ex: 'string' for '#include <string>'
	    std::vector<std::string> m_includes;
    };
}
