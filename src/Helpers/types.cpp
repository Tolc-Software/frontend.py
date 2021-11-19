#include "Helpers/types.hpp"
#include <IR/ir.hpp>
#include <optional>
#include <variant>

namespace Helpers {
IR::Type::Container const* getContainer(IR::Type const& type) {
	if (auto container = std::get_if<IR::Type::Container>(&type.m_type)) {
		return container;
	}
	return nullptr;
}

bool isFunctionType(IR::Type const& type) {
	return std::holds_alternative<IR::Type::Function>(type.m_type);
}

std::string removeCppTemplate(std::string const& name) {
	// MyClass<int> -> MyClass
	return name.substr(0, name.find('<'));
}

}    // namespace Helpers