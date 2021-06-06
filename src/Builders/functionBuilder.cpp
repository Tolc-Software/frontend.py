#include "Builders/functionBuilder.hpp"
#include "Helpers/Pybind/extractIncludes.hpp"
#include "PybindProxy/function.hpp"

namespace Builders {

PybindProxy::Function buildFunction(IR::Function const& fun,
                                    bool isConstructor) {
	PybindProxy::Function f(fun.m_name, fun.m_representation);

	if (isConstructor) {
		f.setAsConstructor();
	}

	std::set<std::string> includes;

	for (auto const& arg : fun.m_arguments) {
		f.addArgument(arg.m_type.m_representation, arg.m_name);

		includes.merge(Helpers::Pybind::extractIncludes(arg.m_type));
	}

	includes.merge(Helpers::Pybind::extractIncludes(fun.m_returnType));
	if (fun.m_returnType.m_numPointers > 0) {
		f.setReturnValuePolicy(
		    PybindProxy::Function::return_value_policy::reference);
	}

	for (auto const& include : includes) {
		f.addInclude(include);
	}

	return f;
}
}    // namespace Builders
