#include "Builders/attributeBuilder.hpp"
#include "TestUtil/string.hpp"
#include <IR/ir.hpp>
#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <string>

TEST_CASE("Can build simple attributes", "[attributeBuilder]") {
	IR::Variable v;

	std::string variableName = "i";
	v.m_name = variableName;

	std::string moduleName = "Module";
	auto fullyQualifiedName = moduleName + "::" + variableName;

	PybindProxy::TypeInfo typeInfo;
	auto proxyAttribute = Builders::buildAttribute(moduleName, v, typeInfo);
	auto pybind = proxyAttribute.getPybind();
	CAPTURE(pybind);

	auto expectedContains =
	    fmt::format(R"(attr("{name}") = &{fullyQualifiedName})",
	                fmt::arg("fullyQualifiedName", fullyQualifiedName),
	                fmt::arg("name", v.m_name));
	CAPTURE(expectedContains);
	REQUIRE(TestUtil::contains(pybind, expectedContains));
}
