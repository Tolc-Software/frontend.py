#include "Builders/moduleFileBuilder.hpp"
#include "Builders/moduleBuilder.hpp"
#include "Helpers/combine.hpp"
#include "PybindProxy/moduleFile.hpp"
#include <IR/ir.hpp>
#include <optional>
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

std::optional<PybindProxy::ModuleFile>
buildModuleFile(IR::Namespace const& rootNamespace,
                std::string const& rootModuleName) {
	if (auto maybeRootModule =
	        Builders::buildModule(rootNamespace, rootModuleName)) {
		auto rootModule = maybeRootModule.value();
		PybindProxy::ModuleFile moduleFile(rootModule, rootModuleName);
		std::set<std::string> includes;
		Helpers::combine(includes, rootModule.getIncludes());

		std::queue<ModulePair> namespaces;
		for (auto const& subNamespace : rootNamespace.m_namespaces) {
			if (auto m = Builders::buildModule(subNamespace, rootModuleName)) {
				namespaces.push({subNamespace, m.value()});
			} else {
				return std::nullopt;
			}
		}

		while (!namespaces.empty()) {
			auto const& [currentNamespace, currentModule] = namespaces.front();

			moduleFile.addModule(currentModule);

			Helpers::combine(includes, currentModule.getIncludes());

			// Go deeper into the nested namespaces
			for (auto const& subNamespace : currentNamespace.m_namespaces) {
				if (auto m =
				        Builders::buildModule(subNamespace, rootModuleName)) {
					namespaces.push({subNamespace, m.value()});
				} else {
					return std::nullopt;
				}
			}

			// Need currentNamespace and currentModule to live this far
			namespaces.pop();
		}

		// All unique includes from each module
		for (auto const& include : includes) {
			moduleFile.addInclude(include);
		}

		return moduleFile;
	} else {
		return std::nullopt;
	}
}
}    // namespace Builders
