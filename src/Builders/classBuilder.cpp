#include "Builders/classBuilder.hpp"
#include "Builders/enumBuilder.hpp"
#include "Builders/functionBuilder.hpp"
#include "Builders/typeToStringBuilder.hpp"
#include "Helpers/Pybind/checkType.hpp"
#include "Helpers/combine.hpp"
#include "Helpers/getOverloadedFunctions.hpp"
#include "Helpers/types.hpp"
#include "PybindProxy/typeInfo.hpp"
#include <IR/ir.hpp>
#include <numeric>
#include <optional>
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

std::string
getTemplateParameterString(std::vector<IR::Type> const& parameters) {
	return std::accumulate(parameters.begin(),
	                       parameters.end(),
	                       std::string() /* Start with empty string */,
	                       [](std::string soFar, IR::Type const& current) {
		                       return std::move(soFar) + "_" +
		                              buildTypeString(current);
	                       });
}

}    // namespace

std::optional<PybindProxy::Class> buildClass(IR::Struct const& cppClass,
                                             PybindProxy::TypeInfo& typeInfo) {
	PybindProxy::Class pyClass(
	    Helpers::removeCppTemplate(cppClass.m_name) +
	        getTemplateParameterString(cppClass.m_templateArguments),
	    cppClass.m_representation);

	auto publicFunctions = getPublicFunctions(cppClass.m_functions);
	auto overloadedFunctions = Helpers::getOverloadedFunctions(publicFunctions);
	// Ignore private functions
	for (auto const& function : publicFunctions) {
		if (auto maybePyFunction = buildFunction(function, typeInfo)) {
			auto pyFunction = maybePyFunction.value();

			if (function.m_isStatic) {
				pyFunction.setAsStatic();
			}

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
		} else {
			return std::nullopt;
		}
	}

	for (auto const& [accessModifier, variable] : cppClass.m_memberVariables) {
		if (accessModifier == IR::AccessModifier::Public) {
			Helpers::Pybind::checkType(variable.m_type, typeInfo);
			pyClass.addMemberVariable(variable.m_name,
			                          variable.m_type.m_isConst,
			                          variable.m_type.m_isStatic);
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

	if (typeInfo.m_classesMarkedShared.contains(cppClass.m_representation)) {
		pyClass.setAsManagedByShared();
	}

	return pyClass;
}
}    // namespace Builders
