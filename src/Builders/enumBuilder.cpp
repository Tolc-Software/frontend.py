#include "Builders/enumBuilder.hpp"
#include <string>

namespace Builders {

PybindProxy::Enum buildEnum(IR::Enum const& e) {
	PybindProxy::Enum proxyEnum(e.m_name, e.m_representation);

	proxyEnum.setScoped(e.m_isScoped);

	for (auto const& value : e.m_values) {
		proxyEnum.addValue(value);
	}

	return proxyEnum;
}
}    // namespace Builders
