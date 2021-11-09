#include "Helpers/Pybind/extractIncludes.hpp"
#include <IR/ir.hpp>
#include <queue>
#include <set>
#include <spdlog/spdlog.h>
#include <string>
#include <variant>

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

/**
* Return true iff type is a function
*/
constexpr bool isFunctionType(IR::Type const& type) {
	return std::holds_alternative<IR::Type::Function>(type.m_type);
}
}    // namespace

namespace Helpers::Pybind {

std::set<std::string> extractIncludes(IR::Type const& type) {
	std::set<std::string> includes;

	std::queue<IR::Type::Container> containersToCheck;
	if (auto container = getContainer(type)) {
		containersToCheck.push(*container);
	} else if (isFunctionType(type)) {
		// The type is std::function
		includes.insert("<pybind11/functional.h>");
	}

	while (!containersToCheck.empty()) {
		IR::Type::Container const& current = containersToCheck.front();

		// See https://pybind11.readthedocs.io/en/stable/advanced/cast/overview.html
		switch (current.m_container) {
			case IR::ContainerType::Array:
			case IR::ContainerType::Deque:
			case IR::ContainerType::List:
			case IR::ContainerType::Map:
			case IR::ContainerType::Optional:
			case IR::ContainerType::Set:
			case IR::ContainerType::UnorderedMap:
			case IR::ContainerType::UnorderedSet:
			case IR::ContainerType::Variant:
			case IR::ContainerType::Vector:
				includes.insert("<pybind11/stl.h>");
				break;

			case IR::ContainerType::ForwardList:
			case IR::ContainerType::MultiMap:
			case IR::ContainerType::MultiSet:
			case IR::ContainerType::PriorityQueue:
			case IR::ContainerType::Queue:
			case IR::ContainerType::Stack:
			case IR::ContainerType::UnorderedMultiMap:
			case IR::ContainerType::UnorderedMultiSet:
				spdlog::error(
				    "Container type {} does not currently have a direct translation via pybind11. The translation might not work.",
				    type.m_representation);
				break;

			case IR::ContainerType::Allocator:
			case IR::ContainerType::EqualTo:
			case IR::ContainerType::Greater:
			case IR::ContainerType::Hash:
			case IR::ContainerType::Less:
			case IR::ContainerType::Pair:
			case IR::ContainerType::Tuple: break;
		}

		for (auto const& containedType : current.m_containedTypes) {
			if (auto container = getContainer(containedType)) {
				containersToCheck.push(*container);
			} else if (isFunctionType(type)) {
				// The type is std::function
				includes.insert("<pybind11/functional.h>");
			}
		}

		containersToCheck.pop();
	}
	return includes;
}

}    // namespace Helpers::Pybind
