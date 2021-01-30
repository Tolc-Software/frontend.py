#include "Builders/moduleBuilder.hpp"
#include "Builders/classBuilder.hpp"
#include "Builders/enumBuilder.hpp"
#include "Builders/functionBuilder.hpp"
#include "Helpers/combine.hpp"
#include "PybindProxy/module.hpp"
#include <IR/ir.hpp>
#include <algorithm>
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

// Return a unique variable name that can be used in the generated code for this module
std::string getVariableName(std::string qualifiedName,
                            std::string const& rootModuleName) {
	// MyNS::Math, rootModule -> rootModule__MyNs__Math
	// This is to avoid naming conflicts when defining namespaces with the
	// same name as the root module
	// This happens if you call your module tensorflow and have a namespace with tensorflow
	std::replace(qualifiedName.begin(), qualifiedName.end(), ':', '_');
	// Check if qualifiedName is the root name (global namespace has no name)
	return qualifiedName.empty() ? rootModuleName :
                                   rootModuleName + "__" + qualifiedName;
}

PybindProxy::Module buildModule(IR::Namespace const& ns,
                                std::string const& rootModuleName) {
	// Check if trying to build from the global namespace or a subnamespace
	auto moduleName = ns.m_name.empty() ? rootModuleName : ns.m_name;

	PybindProxy::Module builtModule(
	    getVariableName(ns.m_representation, rootModuleName));
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
		builtModule.addSubmodule(
		    subNamespace.m_name,
		    getVariableName(subNamespace.m_representation, rootModuleName));
	}

	for (auto const& include : includes) {
		builtModule.addInclude(include);
	}

	return builtModule;
}
}    // namespace Builders
