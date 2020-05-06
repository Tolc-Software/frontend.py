#include "Builders/functionBuilder.hpp"
#include "PybindProxy/function.hpp"

namespace Builders {

PybindProxy::Function buildFunction(IR::Function const& fun) {
	PybindProxy::Function f(fun.m_name, fun.m_representation);

	for (auto const& arg : fun.m_arguments) {
		f.addArgument(arg.m_name);
	}

	if (fun.m_returnType.m_numPointers > 0) {
		f.setReturnValuePolicy(
		    PybindProxy::Function::return_value_policy::reference);
	}

	return f;
}
}    // namespace Builders
