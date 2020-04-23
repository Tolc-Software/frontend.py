#include "PybindProxy/moduleFile.hpp"
#include <fmt/format.h>
#include <string>

namespace PybindProxy {

std::string ModuleFile::getPybind() const {
	std::string out;
	for (auto const& include : m_includes) {
		out += fmt::format("#include {}\n", include);
	}

	out += fmt::format(
	    R"(
namespace py = pybind11;

PYBIND11_MODULE({}, {}))",
	    m_rootModuleName,
	    m_rootModuleName);
	out += " {\n";
	for (auto const& m : m_modules) {
		out += m.getPybind();
	}
	out += "}";

	return out;
}
}
