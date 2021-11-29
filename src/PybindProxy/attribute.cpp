#include "PybindProxy/attribute.hpp"
#include <fmt/format.h>
#include <string>

namespace PybindProxy {

Attribute::Attribute(std::string const& name,
                     std::string const& fullyQualifiedName)
    : m_name(name), m_fullyQualifiedName(fullyQualifiedName) {}

std::string Attribute::getPybind() const {
	return fmt::format(R"(attr("{name}") = &{fullyQualifiedName})",
	                   fmt::arg("name", m_name),
	                   fmt::arg("fullyQualifiedName", m_fullyQualifiedName));
}
}    // namespace PybindProxy
