#include "Builders/moduleFileBuilder.hpp"
#include "Builders/moduleBuilder.hpp"
#include "Helpers/combine.hpp"
#include "PybindProxy/moduleFile.hpp"
#include <IR/ir.hpp>
#include <queue>
#include <set>
#include <string>

namespace {
struct ModulePair {
	IR::Namespace const& m_namespace;
	PybindProxy::Module m_module;
};
}    // namespace

namespace Builders {

PybindProxy::ModuleFile buildModuleFile(IR::Namespace const& rootNamespace,
                                        std::string const& rootModuleName) {
	PybindProxy::Module rootModule =
	    Builders::buildModule(rootNamespace, rootModuleName);
	PybindProxy::ModuleFile moduleFile(rootModule, rootModuleName);
	std::set<std::string> includes;
	Helpers::combine(includes, rootModule.getIncludes());

	std::queue<ModulePair> namespaces;
	for (auto const& subNamespace : rootNamespace.m_namespaces) {
		namespaces.push({subNamespace,
		                 Builders::buildModule(subNamespace, rootModuleName)});
	}

	while (!namespaces.empty()) {
		auto const& [currentNamespace, currentModule] = namespaces.front();

		moduleFile.addModule(currentModule);

		Helpers::combine(includes, currentModule.getIncludes());

		// Go deeper into the nested namespaces
		for (auto const& subNamespace : currentNamespace.m_namespaces) {
			namespaces.push(
			    {subNamespace,
			     Builders::buildModule(subNamespace, rootModuleName)});
		}

		// Need currentNamespace and currentModule to live this far
		namespaces.pop();
	}

	// All unique includes from each module
	for (auto const& include : includes) {
		moduleFile.addInclude(include);
	}

	return moduleFile;
}
}    // namespace Builders

