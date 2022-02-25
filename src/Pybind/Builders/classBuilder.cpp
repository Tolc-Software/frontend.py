#include "Pybind/Builders/classBuilder.hpp"
#include "Pybind/Builders/enumBuilder.hpp"
#include "Pybind/Builders/functionBuilder.hpp"
#include "Pybind/Builders/typeToStringBuilder.hpp"
#include "Pybind/Helpers/combine.hpp"
#include "Pybind/Helpers/types.hpp"
#include "Pybind/Proxy/typeInfo.hpp"
#include "Pybind/checkType.hpp"
#include "Pybind/getOverloadedFunctions.hpp"
#include <IR/ir.hpp>
#include <numeric>
#include <optional>
#include <set>
#include <string>

namespace Pybind::Builders {

namespace {

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

std::optional<Pybind::Proxy::Class>
buildClass(IR::Struct const& cppClass, Pybind::Proxy::TypeInfo& typeInfo) {
	Pybind::Proxy::Class pyClass(
	    Pybind::Helpers::removeCppTemplate(cppClass.m_name) +
	        getTemplateParameterString(cppClass.m_templateArguments),
	    cppClass.m_representation);

	auto overloadedFunctions =
	    Pybind::getOverloadedFunctions(cppClass.m_public.m_functions);
	// Ignore private functions
	for (auto const& function : cppClass.m_public.m_functions) {
		if (auto maybePyFunction = buildFunction(function, typeInfo)) {
			auto pyFunction = maybePyFunction.value();

			if (function.m_isStatic) {
				pyFunction.setAsStatic();
			}

			if (overloadedFunctions.find(function.m_representation) !=
			    overloadedFunctions.end()) {
				pyFunction.setAsOverloaded();
			}

			pyClass.addFunction(pyFunction);
		} else {
			return std::nullopt;
		}
	}

	for (auto const& constructor : cppClass.m_public.m_constructors) {
		if (auto maybePyFunction = buildFunction(constructor, typeInfo)) {
			auto pyFunction = maybePyFunction.value();

			if (constructor.m_isStatic) {
				pyFunction.setAsStatic();
			}

			if (cppClass.m_public.m_constructors.size() > 1) {
				pyFunction.setAsOverloaded();
			}

			pyFunction.setAsConstructor();
			pyClass.addConstructor(pyFunction);
		}
	}

	for (auto const& variable : cppClass.m_public.m_memberVariables) {
		Pybind::checkType(variable.m_type, typeInfo);
		pyClass.addMemberVariable(variable.m_name,
		                          variable.m_type.m_isConst,
		                          variable.m_type.m_isStatic);
	}

	// Add default constructor
	if (cppClass.m_hasImplicitDefaultConstructor) {
		auto constructor =
		    Pybind::Proxy::Function(pyClass.getName(), pyClass.getName());
		constructor.setAsConstructor();
		pyClass.addConstructor(constructor);
	}

	for (auto const& e : cppClass.m_public.m_enums) {
		pyClass.addEnum(buildEnum(e));
	}

	if (typeInfo.m_classesMarkedShared.contains(cppClass.m_representation)) {
		pyClass.setAsManagedByShared();
	}

	return pyClass;
}
}    // namespace Pybind::Builders
