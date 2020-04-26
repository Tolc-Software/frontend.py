#include "Builders/classBuilder.hpp"
#include "Builders/functionBuilder.hpp"

namespace Builders {

PybindProxy::Class buildClass(IR::Struct const& s) {
	PybindProxy::Class c(s.m_name, s.m_representation);

	// TODO: Remove this when IR has support for constructors
	auto addedConstructor = false;
	for (auto const& [accessModifier, function] : s.m_functions) {
		// Ignore private functions
		if (accessModifier == IR::AccessModifier::Public) {
			if (function.m_name == s.m_name) {
				// Constructor/destructor?
				// Only support constructor (?)
				// TODO: Add better support for constructor/destructor
				std::vector<std::string> arguments;
				for (auto const& arg : function.m_arguments) {
					arguments.push_back(arg.m_type.m_representation);
				}
				c.addConstructor(PybindProxy::Constructor(arguments));
				addedConstructor = true;
				continue;
			}

			// Normal function
			c.addFunction(buildFunction(function));
		}
	}

	// No constructor -> add default constructor
	if (!addedConstructor) {
		c.addConstructor(PybindProxy::Constructor({}));
	}

	return c;
}
}    // namespace Builders