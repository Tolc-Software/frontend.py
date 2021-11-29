#include "Builders/moduleFileBuilder.hpp"
#include "Builders/moduleBuilder.hpp"
#include "Helpers/combine.hpp"
#include "PybindProxy/moduleFile.hpp"
#include "PybindProxy/typeInfo.hpp"
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
	PybindProxy::TypeInfo typeInfo;
	if (auto maybeRootModule =
	        Builders::buildModule(rootNamespace, rootModuleName, typeInfo)) {
		auto rootModule = maybeRootModule.value();
		PybindProxy::ModuleFile moduleFile(rootModule, rootModuleName);

		std::queue<ModulePair> namespaces;
		for (auto const& subNamespace : rootNamespace.m_namespaces) {
			if (auto m = Builders::buildModule(
			        subNamespace, rootModuleName, typeInfo)) {
				namespaces.push({subNamespace, m.value()});
			} else {
				return std::nullopt;
			}
		}

		while (!namespaces.empty()) {
			auto const& [currentNamespace, currentModule] = namespaces.front();

			moduleFile.addModule(currentModule);

			// Go deeper into the nested namespaces
			for (auto const& subNamespace : currentNamespace.m_namespaces) {
				if (auto m = Builders::buildModule(
				        subNamespace, rootModuleName, typeInfo)) {
					namespaces.push({subNamespace, m.value()});
				} else {
					return std::nullopt;
				}
			}

			// Need currentNamespace and currentModule to live this far
			namespaces.pop();
		}

		moduleFile.setTypeInfo(typeInfo);
		return moduleFile;
	}

	return std::nullopt;
}
}    // namespace Builders
