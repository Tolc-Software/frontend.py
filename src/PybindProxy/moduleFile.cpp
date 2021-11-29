#include "PybindProxy/moduleFile.hpp"
#include <fmt/format.h>
#include <string>

namespace PybindProxy {

ModuleFile::ModuleFile(Module const& rootModule, std::string const& libraryName)
    : m_rootModuleName(rootModule.getVariableName()),
      m_libraryName(libraryName), m_modules({rootModule}), m_typeInfo() {}

void ModuleFile::addModule(Module const& m) {
	m_modules.push_back(m);
}

std::filesystem::path ModuleFile::getFilepath() const {
	return m_libraryName + ".cpp";
}

void ModuleFile::setTypeInfo(PybindProxy::TypeInfo const& info) {
	m_typeInfo = info;
	m_typeInfo.m_includes.insert("<pybind11/pybind11.h>");
}

std::string ModuleFile::getPybind() const {
	std::vector<std::string> includes;
	std::transform(m_typeInfo.m_includes.begin(),
	               m_typeInfo.m_includes.end(),
	               std::back_inserter(includes),
	               [](auto const& i) { return fmt::format("#include {}", i); });

	std::string out = fmt::format(
	    R"(
{includes}

namespace py = pybind11;

PYBIND11_MODULE({libraryName}, {rootModuleName}))",
	    fmt::arg("includes", fmt::join(includes, "\n")),
	    fmt::arg("libraryName", m_libraryName),
	    fmt::arg("rootModuleName", m_rootModuleName));

	out += " {\n";
	for (auto const& m : m_modules) {
		out += m.getPybind();
	}
	out += '}';

	return out;
}
}
