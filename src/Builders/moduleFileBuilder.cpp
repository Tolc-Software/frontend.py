#include "Builders/moduleFileBuilder.hpp"
#include "Builders/functionBuilder.hpp"
#include "Builders/moduleBuilder.hpp"
#include "PybindProxy/moduleFile.hpp"
#include <IR/ir.hpp>
#include <queue>

namespace {
struct ModulePair {
	IR::Namespace const& m_namespace;
	PybindProxy::Module m_module;
};
}    // namespace

namespace Builders {

PybindProxy::ModuleFile buildModuleFile(IR::Namespace const& rootNamespace) {
	PybindProxy::Module rootModule = Builders::buildModule(rootNamespace);
	PybindProxy::ModuleFile moduleFile(rootModule);

	std::queue<ModulePair> namespaces;
	for (auto const& subNamespace : rootNamespace.m_namespaces) {
		namespaces.push({subNamespace, Builders::buildModule(subNamespace)});
	}

	while (!namespaces.empty()) {
		auto const& [currentNamespace, currentModule] = namespaces.front();

		moduleFile.addModule(currentModule);

		// Go deeper into the nested namespaces
		for (auto const& subNamespace : currentNamespace.m_namespaces) {
			namespaces.push(
			    {subNamespace, Builders::buildModule(subNamespace)});
		}

		// Need currentNamespace and currentModule to live this far
		namespaces.pop();
	}

	return moduleFile;
}
}    // namespace Builders

