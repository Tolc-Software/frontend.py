#include "Builders/classBuilder.hpp"
#include "PybindProxy/class.hpp"
#include "TestUtil/string.hpp"
#include <IR/ir.hpp>
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Builds a simple class", "[classBuilder]") {
	std::string moduleName = "MyModule";
	IR::Struct s;
	s.m_name = "MyStruct";
	s.m_representation = fmt::format("MyNamespace::{className}",
	                                 fmt::arg("className", s.m_name));

	auto myStruct = Builders::buildClass(s);
	auto pybind = myStruct.getPybind(moduleName);
	CAPTURE(pybind);

	auto pybindCode = fmt::format(
	    R"(py::class_<{fullyQualifiedClassName}>({moduleName}, "{className}");)",
	    fmt::arg("fullyQualifiedClassName", s.m_representation),
	    fmt::arg("moduleName", moduleName),
	    fmt::arg("className", s.m_name));
	CAPTURE(pybindCode);
	REQUIRE(TestUtil::contains(pybind, pybindCode));
}

