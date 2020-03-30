#pragma once

#include <fmt/format.h>
#include <string>

namespace PybindProxy {
	struct Function {
	    Function(std::string const& name,
	             std::vector<std::string> const& includes)
	        : m_name(name), m_includes(includes) {}

	    std::string getPybind() const {
		    return fmt::format(
		        "m.def(\"{}\", &{}, \"A happy function!\");", m_name, m_name);
	    }

	    std::vector<std::string> const& getIncludes() const {
		    return m_includes;
	    }

	private:
	    std::string m_name;
	    // What will be included
	    // Ex: 'string' for '#include <string>'
	    std::vector<std::string> m_includes;
    };
}
