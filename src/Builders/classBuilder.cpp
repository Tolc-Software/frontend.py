#include "Builders/classBuilder.hpp"
#include "Builders/enumBuilder.hpp"
#include "Builders/functionBuilder.hpp"
#include "Helpers/Pybind/extractIncludes.hpp"
#include "Helpers/combine.hpp"
#include <set>
#include <string>

namespace Builders {

PybindProxy::Class buildClass(IR::Struct const& s) {
	PybindProxy::Class c(s.m_name, s.m_representation);
	std::set<std::string> includes;

	// TODO: Remove this when IR has support for constructors
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
					includes.merge(
					    Helpers::Pybind::extractIncludes(arg.m_type));
				}
				c.addConstructor(PybindProxy::Constructor(arguments));
				continue;
			}

			// Normal function
			auto pyFunction = buildFunction(function);

			Helpers::combine(includes, pyFunction.getIncludes());
			c.addFunction(pyFunction);
		}
	}

	for (auto const& [accessModifier, variable] : s.m_memberVariables) {
		if (accessModifier == IR::AccessModifier::Public) {
			c.addMemberVariable(variable.m_name, variable.m_type.m_isConst);
		}
	}

	// Add default constructor
	if (s.m_hasImplicitDefaultConstructor) {
		c.addConstructor(PybindProxy::Constructor({}));
	}

	for (auto const& [am, e] : s.m_enums) {
		if (am == IR::AccessModifier::Public) {
			c.addEnum(buildEnum(e));
		}
	}

	for (auto const& include : includes) {
		c.addInclude(include);
	}

	return c;
}
}    // namespace Builders
