#include "Builders/classBuilder.hpp"
#include "Builders/enumBuilder.hpp"
#include "Builders/functionBuilder.hpp"
#include "Helpers/Pybind/extractIncludes.hpp"
#include "Helpers/combine.hpp"
#include "Helpers/getOverloadedFunctions.hpp"
#include <IR/ir.hpp>
#include <set>
#include <string>

namespace Builders {

namespace {
std::vector<IR::Function> getPublicFunctions(
    std::vector<std::pair<IR::AccessModifier, IR::Function>> const& functions) {
	std::vector<IR::Function> publicFunctions;
	for (auto const& [am, function] : functions) {
		if (am == IR::AccessModifier::Public) {
			publicFunctions.push_back(function);
		}
	}
	return publicFunctions;
}
}    // namespace

PybindProxy::Class buildClass(IR::Struct const& cppClass) {
	PybindProxy::Class pyClass(cppClass.m_name, cppClass.m_representation);
	std::set<std::string> includes;

	auto publicFunctions = getPublicFunctions(cppClass.m_functions);
	auto overloadedFunctions = Helpers::getOverloadedFunctions(publicFunctions);
	for (auto const& function : publicFunctions) {
		// Ignore private functions
		auto pyFunction = buildFunction(function);
		Helpers::combine(includes, pyFunction.getIncludes());

		if (overloadedFunctions.find(function.m_representation) !=
		    overloadedFunctions.end()) {
			pyFunction.setAsOverloaded();
		}

		if (function.m_name == cppClass.m_name) {
			pyFunction.setAsConstructor();
			pyClass.addConstructor(pyFunction);
		} else {
			pyClass.addFunction(pyFunction);
		}
	}

	for (auto const& [accessModifier, variable] : cppClass.m_memberVariables) {
		if (accessModifier == IR::AccessModifier::Public) {
			pyClass.addMemberVariable(variable.m_name,
			                          variable.m_type.m_isConst);
		}
	}

	// Add default constructor
	if (cppClass.m_hasImplicitDefaultConstructor) {
		auto constructor =
		    PybindProxy::Function(pyClass.getName(), pyClass.getName());
		constructor.setAsConstructor();
		pyClass.addConstructor(constructor);
	}

	for (auto const& [am, e] : cppClass.m_enums) {
		if (am == IR::AccessModifier::Public) {
			pyClass.addEnum(buildEnum(e));
		}
	}

	for (auto const& include : includes) {
		pyClass.addInclude(include);
	}

	return pyClass;
}
}    // namespace Builders
