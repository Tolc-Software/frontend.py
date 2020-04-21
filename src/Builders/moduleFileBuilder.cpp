#include "Builders/moduleFileBuilder.hpp"
#include "Builders/functionBuilder.hpp"
#include "PybindProxy/moduleFile.hpp"
#include <IR/ir.hpp>

namespace Builders {

PybindProxy::ModuleFile buildModuleFile(IR::Namespace const& rootNamespace) {
	std::string rootModuleName = "pybind11Module";
	if (!rootNamespace.m_name.empty()) {
		rootModuleName = rootNamespace.m_name;
	}

	PybindProxy::Module m(rootModuleName);

	for (auto const& function : rootNamespace.m_functions) {
		m.addFunction(Builders::buildFunction(function));
	}

	PybindProxy::ModuleFile mf(m);
	return mf;
}
}    // namespace Builders

