#pragma once

#include <IR/ir.hpp>
#include <optional>
#include <variant>

namespace Helpers {
IR::Type::Container const* getContainer(IR::Type const& type) {
	return std::get_if<IR::Type::Container>(&type.m_type);
}

bool requiresPybind11STLHeader(IR::Type::Container const& container) {
	using IR::ContainerType;
	switch (container.m_container) {
		case ContainerType::Array:
		case ContainerType::Allocator:
		case ContainerType::Map:
		case ContainerType::Set:
		case ContainerType::Unordered_map:
		case ContainerType::Unordered_set:
		case ContainerType::Vector: return true; return false;
	};
}
}    // namespace Helpers
