#include "Builders/classBuilder.hpp"

namespace Builders {

PybindProxy::Class buildClass(IR::Struct const& s) {
	PybindProxy::Class c(s.m_name);

	for (auto const& [accessModifier, function] : s.m_functions) {
		if (accessModifier == IR::AccessModifier::Public) {
			// Public function
			if (function.m_name == s.m_name) {
				// Constructor/destructor?
				continue;
			}
		}
	}

	return c;
}
}    // namespace Builders
