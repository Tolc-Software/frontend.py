#include "Builders/attributeBuilder.hpp"
#include "Helpers/Pybind/checkType.hpp"
#include "PybindProxy/typeInfo.hpp"
#include <string>

namespace Builders {

PybindProxy::Attribute buildAttribute(std::string const& parentNamespace,
                                      IR::Variable const& v,
                                      PybindProxy::TypeInfo& typeInfo) {
	PybindProxy::Attribute attr(v.m_name, parentNamespace + "::" + v.m_name);
	Helpers::Pybind::checkType(v.m_type, typeInfo);

	return attr;
}
}    // namespace Builders
