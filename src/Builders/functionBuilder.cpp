#include "Builders/functionBuilder.hpp"

namespace Builders {

PybindProxy::Function buildFunction(IR::Function const& fun) {
	PybindProxy::Function f(fun.m_name, fun.m_representation);

	for (auto const& arg : fun.m_arguments) {
		f.addArgument(arg.m_name);
	}
	return f;
}
}    // namespace Builders
