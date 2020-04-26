#include "Builders/classBuilder.hpp"
#include "PybindProxy/class.hpp"
#include "TestUtil/string.hpp"
#include <IR/ir.hpp>
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Class within namespace", "[classBuilder]") {
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

TEST_CASE("Class with a constructor", "[classBuilder]") {
	std::string moduleName = "MyModule";
	IR::Struct s;
	s.m_name = "MyStruct";
	s.m_representation = s.m_name;

	IR::Function f;
	f.m_name = s.m_name;
	f.m_representation = f.m_name;
	IR::Variable v;
	v.m_name = 's';
	IR::Type t;
	t.m_representation = "const std::string&";
	v.m_type = t;
	f.m_arguments.push_back(v);
	s.m_functions.push_back({IR::AccessModifier::Public, f});

	auto myStruct = Builders::buildClass(s);
	auto pybind = myStruct.getPybind(moduleName);
	CAPTURE(pybind);

	auto pybindCode = fmt::format("\t.def(py::init<{type}>());",
	                              fmt::arg("type", t.m_representation));
	CAPTURE(pybindCode);
	REQUIRE(TestUtil::contains(pybind, pybindCode));
}

TEST_CASE("Class with functions", "[classBuilder]") {
	std::string moduleName = "MyModule";
	IR::Struct s;
	s.m_name = "MyStruct";
	s.m_representation = s.m_name;

	std::vector<std::pair<std::string, std::string>> functions = {
	    std::make_pair("f", "int"),
	    std::make_pair("another_func", "std::string"),
	    std::make_pair("fun", "double")};
	for (auto const& [function, type] : functions) {
		IR::Function f;

		f.m_name = function;
		f.m_representation = f.m_name;
		IR::Variable v;
		v.m_name = "myVar";
		IR::Type t;
		t.m_representation = type;
		v.m_type = t;
		f.m_arguments.push_back(v);
		s.m_functions.push_back({IR::AccessModifier::Public, f});
	}

	auto myStruct = Builders::buildClass(s);
	auto pybind = myStruct.getPybind(moduleName);
	CAPTURE(pybind);

	for (auto const& [function, type] : functions) {
		auto pybindCode = fmt::format("\t.def(\"{function}\", &{function}",
		                              fmt::arg("function", function));
		CAPTURE(pybindCode);
		REQUIRE(TestUtil::contains(pybind, pybindCode));
	}
}
