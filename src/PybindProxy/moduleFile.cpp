#include "PybindProxy/moduleFile.hpp"
#include <fmt/format.h>
#include <string>

namespace PybindProxy {

ModuleFile::ModuleFile(Module const& rootModule)
    : m_rootModuleName(rootModule.getName()),
      m_includes({"<pybind11/pybind11.h>"}), m_modules({rootModule}) {}

void ModuleFile::addModule(Module const& m) {
	m_modules.push_back(m);
}

void ModuleFile::addInclude(std::string const& i) {
	m_includes.push_back(i);
}

std::filesystem::path ModuleFile::getFilepath() const {
	return m_rootModuleName + ".cpp";
}

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
	out += '}';

	return out;
}
}
