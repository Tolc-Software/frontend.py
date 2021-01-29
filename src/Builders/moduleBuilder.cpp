#include "Builders/moduleBuilder.hpp"
#include "Builders/classBuilder.hpp"
#include "Builders/enumBuilder.hpp"
#include "Builders/functionBuilder.hpp"
#include "Helpers/combine.hpp"
#include "PybindProxy/module.hpp"
#include <IR/ir.hpp>
#include <set>
#include <string>

namespace Builders {

namespace {
template <typename PybindProxyType>
void combineIncludes(std::set<std::string>& includes,
                     PybindProxyType const& pyType) {
	Helpers::combine(includes, pyType.getIncludes());
}
}    // namespace

PybindProxy::Module buildModule(IR::Namespace const& ns) {
	PybindProxy::Module builtModule(ns.m_name, ns.m_representation);
	std::set<std::string> includes;

	for (auto const& function : ns.m_functions) {
		auto f = Builders::buildFunction(function);
		combineIncludes(includes, f);
		builtModule.addFunction(f);
	}

	for (auto const& cls : ns.m_structs) {
		auto c = Builders::buildClass(cls);
		combineIncludes(includes, c);
		builtModule.addClass(c);
	}

	for (auto const& e : ns.m_enums) {
		builtModule.addEnum(Builders::buildEnum(e));
	}

	for (auto const& subNamespace : ns.m_namespaces) {
		builtModule.addSubmodule(subNamespace.m_name);
	}

	for (auto const& include : includes) {
		builtModule.addInclude(include);
	}

	return builtModule;
}
}    // namespace Builders
