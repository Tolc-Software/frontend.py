#include "Builders/moduleBuilder.hpp"
#include "Builders/attributeBuilder.hpp"
#include "Builders/classBuilder.hpp"
#include "Builders/enumBuilder.hpp"
#include "Builders/functionBuilder.hpp"
#include "Helpers/combine.hpp"
#include "Helpers/getOverloadedFunctions.hpp"
#include "Helpers/split.hpp"
#include "PybindProxy/module.hpp"
#include <IR/ir.hpp>
#include <algorithm>
#include <fmt/format.h>
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
	// MyNS::Math, rootModule -> rootModule_MyNs_Math
	// This is to avoid naming conflicts when defining namespaces with the
	// same name as the root module
	// This happens if you call your module tensorflow and have a namespace with tensorflow
	auto splitted = Helpers::split(qualifiedName, "::");

	// If we only have the global namespace (empty name), drop it
	// This will result in module variable name of "MyModule" instead of "MyModule_"
	if (splitted.size() == 1 && splitted[0] == "") {
		splitted.pop_back();
	}

	splitted.push_front(rootModuleName);
	// If qualifiedName is the root name (global namespace has no name)
	// This will return rootModuleName
	return fmt::format("{}", fmt::join(splitted, "_"));
}

std::optional<PybindProxy::Module>
buildModule(IR::Namespace const& ns, std::string const& rootModuleName) {
	PybindProxy::Module builtModule(
	    getVariableName(ns.m_representation, rootModuleName));
	std::set<std::string> includes;

	auto overloadedFunctions = Helpers::getOverloadedFunctions(ns.m_functions);
	for (auto const& function : ns.m_functions) {
		if (auto maybeF = Builders::buildFunction(function)) {
			auto f = maybeF.value();
			if (overloadedFunctions.find(function.m_representation) !=
			    overloadedFunctions.end()) {
				f.setAsOverloaded();
			}
			combineIncludes(includes, f);
			builtModule.addFunction(f);
		} else {
			return std::nullopt;
		}
	}

	for (auto const& variable : ns.m_variables) {
		auto v = Builders::buildAttribute(ns.m_representation, variable);
		combineIncludes(includes, v);
		builtModule.addAttribute(v);
	}

	for (auto const& cls : ns.m_structs) {
		if (auto maybeC = Builders::buildClass(cls)) {
			auto c = maybeC.value();
			combineIncludes(includes, c);
			builtModule.addClass(c);
		} else {
			return std::nullopt;
		}
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
