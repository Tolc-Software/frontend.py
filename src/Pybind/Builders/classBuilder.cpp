#include "Pybind/Builders/classBuilder.hpp"
#include "Pybind/Builders/enumBuilder.hpp"
#include "Pybind/Builders/functionBuilder.hpp"
#include "Pybind/Builders/typeToStringBuilder.hpp"
#include "Pybind/Helpers/combine.hpp"
#include "Pybind/Helpers/types.hpp"
#include "Pybind/Proxy/function.hpp"
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

std::optional<std::string> getOperatorName(IR::Operator op) {
	using IR::Operator;
	switch (op) {
		case Operator::Addition:
			// +
			return "__add__";
		case Operator::Subtraction:
			// -
			return "__sub__";
		case Operator::Multiplication:
			// *
			return "__mul__";
		case Operator::Division:
			// /
			return "__truediv__";
		case Operator::Modulus:
			// %
			return "__mod__";
		case Operator::Assignment:
			// =
			// No translation
			return std::nullopt;
		case Operator::Equal:
			// ==
			return "__eq__";
		case Operator::NotEqual:
			// !=
			return "__ne__";
		case Operator::GreaterThan:
			// >
			return "__gt__";
		case Operator::GreaterThanOrEqualTo:
			// >=
			return "__ge__";
		case Operator::LessThan:
			// <
			return "__lt__";
		case Operator::LessThanOrEqualTo:
			// <=
			return "__le__";
		case Operator::Subscript:
			// []
			return "__getitem__";
		case Operator::Call:
			// ()
			return "__call__";
	}
	return "";
}

void buildMemberFunction(Pybind::Proxy::Function& pyFunction,
                         IR::Function const& cppFunction,
                         std::set<std::string> const& overloadedFunctions) {
	if (cppFunction.m_isStatic) {
		pyFunction.setAsStatic();
	}

	if (overloadedFunctions.find(cppFunction.m_representation) !=
	    overloadedFunctions.end()) {
		pyFunction.setAsOverloaded();
	}
}

}    // namespace

std::optional<Pybind::Proxy::Class>
buildClass(IR::Struct const& cppClass, Pybind::Proxy::TypeInfo& typeInfo) {
	Pybind::Proxy::Class pyClass(
	    Pybind::Helpers::removeCppTemplate(cppClass.m_name) +
	        getTemplateParameterString(cppClass.m_templateArguments),
	    cppClass.m_representation);

	pyClass.setDocumentation(cppClass.m_documentation);
	auto overloadedFunctions =
	    Pybind::getOverloadedFunctions(cppClass.m_public.m_functions);
	// Ignore private functions
	for (auto const& function : cppClass.m_public.m_functions) {
		if (auto maybePyFunction = buildFunction(function, typeInfo)) {
			auto& pyFunction = maybePyFunction.value();

			buildMemberFunction(pyFunction, function, overloadedFunctions);

			pyClass.addFunction(pyFunction);
		} else {
			return std::nullopt;
		}
	}

	auto overloadedOperators =
	    Pybind::getOverloadedFunctions(cppClass.m_public.m_operators);
	for (auto const& [op, function] : cppClass.m_public.m_operators) {
		if (auto maybePyFunction = buildFunction(function, typeInfo)) {
			if (auto maybeName = getOperatorName(op)) {
				auto& pyFunction = maybePyFunction.value();

				// The python operators have special names
				pyFunction.setPythonName(maybeName.value());

				buildMemberFunction(pyFunction, function, overloadedOperators);

				pyClass.addFunction(pyFunction);
			}
		} else {
			return std::nullopt;
		}
	}

	for (auto const& constructor : cppClass.m_public.m_constructors) {
		if (auto maybePyFunction = buildFunction(constructor, typeInfo)) {
			auto& pyFunction = maybePyFunction.value();

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
		                          variable.m_documentation,
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
