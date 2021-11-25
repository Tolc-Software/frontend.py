#include "Builders/functionBuilder.hpp"
#include "Helpers/Pybind/extractIncludes.hpp"
#include "Helpers/types.hpp"
#include "PybindProxy/function.hpp"
#include <optional>
#include <spdlog/spdlog.h>

namespace Builders {

std::optional<PybindProxy::Function>
buildFunction(IR::Function const& cppFunction) {
	PybindProxy::Function pyFunction(
	    Helpers::removeCppTemplate(cppFunction.m_name),
	    cppFunction.m_representation);

	std::set<std::string> includes;

	for (auto const& arg : cppFunction.m_arguments) {
		if (!Helpers::isContainerType(arg.m_type,
		                              IR::ContainerType::UniquePtr)) {
			pyFunction.addArgument(arg.m_type.m_representation, arg.m_name);
			includes.merge(Helpers::Pybind::extractIncludes(arg.m_type));
		} else {
			spdlog::error(
			    R"(The function {} takes a std::unique_ptr as one of its argument. Python cannot give up ownership of an object to a function. See https://pybind11.readthedocs.io/en/stable/advanced/smart_ptrs.html for more info.)",
			    cppFunction.m_representation);
			return std::nullopt;
		}
	}

	includes.merge(Helpers::Pybind::extractIncludes(cppFunction.m_returnType));
	pyFunction.setReturnType(cppFunction.m_returnType.m_representation);

	if (cppFunction.m_returnType.m_numPointers > 0) {
		pyFunction.setReturnValuePolicy(
		    PybindProxy::Function::return_value_policy::reference);
	}

	for (auto const& include : includes) {
		pyFunction.addInclude(include);
	}

	return pyFunction;
}
}    // namespace Builders
