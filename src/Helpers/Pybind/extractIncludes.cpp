#include "Helpers/Pybind/extractIncludes.hpp"
#include <IR/ir.hpp>
#include <queue>
#include <set>
#include <string>

namespace {
/**
* Return container if type is a container, otherwise nullptr
*/
IR::Type::Container const* getContainer(IR::Type const& type) {
	if (auto container = std::get_if<IR::Type::Container>(&type.m_type)) {
		return container;
	}
	return nullptr;
}
}    // namespace

namespace Helpers::Pybind {

std::set<std::string> extractIncludes(IR::Type const& type) {
	std::set<std::string> includes;

	std::queue<IR::Type::Container> containersToCheck;
	if (auto container = getContainer(type)) {
		containersToCheck.push(*container);
	}

	while (!containersToCheck.empty()) {
		IR::Type::Container const& current = containersToCheck.front();

		switch (current.m_container) {
			case IR::ContainerType::Array:
			case IR::ContainerType::Map:
			case IR::ContainerType::Set:
			case IR::ContainerType::Unordered_map:
			case IR::ContainerType::Unordered_set:
			case IR::ContainerType::Vector:
				includes.insert("<pybind11/stl.h>");
				break;
			case IR::ContainerType::Allocator:
			case IR::ContainerType::Greater:
			case IR::ContainerType::Hash:
			case IR::ContainerType::Less:
			case IR::ContainerType::Pair:
			case IR::ContainerType::Tuple:
			case IR::ContainerType::Equal_to: break;
		}

		for (auto const& containedType : current.m_containedTypes) {
			if (auto container = getContainer(containedType)) {
				containersToCheck.push(*container);
			}
		}

		containersToCheck.pop();
	}
	return includes;
}

}    // namespace Helpers::Pybind
