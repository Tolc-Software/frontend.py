#include "Frontend/Python/frontend.hpp"
#include "Builders/functionBuilder.hpp"
#include "PybindProxy/function.hpp"
#include "PybindProxy/module.hpp"
#include "PybindProxy/moduleFile.hpp"
#include <IR/ir.hpp>
#include <filesystem>
#include <string>

namespace Frontend::Python {

std::pair<std::filesystem::path, std::string>
createModules(IR::Namespace const& rootNamespace) {
	std::string rootModuleName = "pybind11Module";
	if (!rootNamespace.m_name.empty()) {
		rootModuleName = rootNamespace.m_name;
	}

	PybindProxy::Module m(rootModuleName);

	for (auto const& function : rootNamespace.m_functions) {
		m.addFunction(Builders::buildFunction(function));
	}

	PybindProxy::ModuleFile mf(m);

	return std::make_pair(mf.getFilepath(), mf.getPybind());
}

}    // namespace Frontend::Python
