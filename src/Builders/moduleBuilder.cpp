#include "Builders/moduleBuilder.hpp"
#include "Builders/classBuilder.hpp"
#include "Builders/functionBuilder.hpp"
#include "PybindProxy/module.hpp"
#include <IR/ir.hpp>

namespace Builders {

PybindProxy::Module buildModule(IR::Namespace const& ns) {
	PybindProxy::Module builtModule(ns.m_name);

	for (auto const& function : ns.m_functions) {
		builtModule.addFunction(Builders::buildFunction(function));
	}

	for (auto const& subNamespace : ns.m_namespaces) {
		builtModule.addSubmodule(subNamespace.m_name);
	}

	return builtModule;
}
}    // namespace Builders
