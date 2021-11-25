#include "PybindProxy/attribute.hpp"
#include <fmt/format.h>
#include <string>

namespace PybindProxy {

Attribute::Attribute(std::string const& name,
                     std::string const& fullyQualifiedName)
    : m_name(name), m_fullyQualifiedName(fullyQualifiedName), m_includes() {}

std::string Attribute::getPybind() const {
	return fmt::format(R"(attr("{name}") = &{fullyQualifiedName})",
	                   fmt::arg("name", m_name),
	                   fmt::arg("fullyQualifiedName", m_fullyQualifiedName));
}

void Attribute::addInclude(std::string const& i) {
	m_includes.push_back(i);
}

std::vector<std::string> const& Attribute::getIncludes() const {
	return m_includes;
}
}    // namespace PybindProxy
