#include "Builders/attributeBuilder.hpp"
#include "Helpers/Pybind/extractIncludes.hpp"
#include <string>

namespace Builders {

PybindProxy::Attribute buildAttribute(std::string const& parentNamespace,
                                      IR::Variable const& v) {
	PybindProxy::Attribute attr(v.m_name, parentNamespace + "::" + v.m_name);

	for (auto i : Helpers::Pybind::extractIncludes(v.m_type)) {
		attr.addInclude(i);
	}

	return attr;
}
}    // namespace Builders
