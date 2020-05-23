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
	s.m_hasImplicitDefaultConstructor = true;

	auto myStruct = Builders::buildClass(s);
	auto pybind = myStruct.getPybind(moduleName);
	CAPTURE(pybind);

	auto expectedContains = fmt::format(
	    R"(py::class_<{fullyQualifiedClassName}>({moduleName}, "{className}"))",
	    fmt::arg("fullyQualifiedClassName", s.m_representation),
	    fmt::arg("moduleName", moduleName),
	    fmt::arg("className", s.m_name));
	CAPTURE(expectedContains);
	REQUIRE(TestUtil::contains(pybind, expectedContains));
}

TEST_CASE("Empty class gets default constructor", "[classBuilder]") {
	std::string moduleName = "MyModule";
	IR::Struct s;
	s.m_name = "MyStruct";
	s.m_representation = s.m_name;
	s.m_hasImplicitDefaultConstructor = true;

	auto myStruct = Builders::buildClass(s);
	auto pybind = myStruct.getPybind(moduleName);
	CAPTURE(pybind);

	auto expectedContains = "def(py::init<>())";
	CAPTURE(expectedContains);
	REQUIRE(TestUtil::contains(pybind, expectedContains));
}

TEST_CASE("Class with a constructor", "[classBuilder]") {
	std::string moduleName = "MyModule";
	IR::Struct s;
	s.m_name = "MyStruct";
	s.m_representation = s.m_name;
	s.m_hasImplicitDefaultConstructor = true;

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

	auto expectedContains = fmt::format("\t.def(py::init<{type}>())",
	                                    fmt::arg("type", t.m_representation));
	CAPTURE(expectedContains);
	REQUIRE(TestUtil::contains(pybind, expectedContains));
}

TEST_CASE("Class with functions", "[classBuilder]") {
	std::string moduleName = "MyModule";
	IR::Struct s;
	s.m_name = "MyStruct";
	s.m_representation = s.m_name;
	s.m_hasImplicitDefaultConstructor = true;

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
		auto expectedContains =
		    fmt::format("\t.def(\"{function}\", &{function}",
		                fmt::arg("function", function));
		CAPTURE(expectedContains);
		REQUIRE(TestUtil::contains(pybind, expectedContains));
	}
}

TEST_CASE("Class with member variables", "[classBuilder]") {
	struct Var {
		std::string name;
		std::string type;
		bool isConst;
	};
	std::vector<Var> variables = {Var({"v0", "int", true}),
	                              Var({"s", "const std::string&", false}),
	                              Var({"myVar", "double", false})};

	std::string moduleName = "MyModule";
	IR::Struct s;
	s.m_name = "MyStruct";
	s.m_representation = s.m_name;
	s.m_hasImplicitDefaultConstructor = true;

	for (auto const& var : variables) {
		IR::Variable v;
		v.m_name = var.name;
		IR::Type t;
		t.m_representation = var.type;
		t.m_isConst = var.isConst;
		v.m_type = t;
		s.m_memberVariables.push_back({IR::AccessModifier::Public, v});
	}

	auto myStruct = Builders::buildClass(s);
	auto pybind = myStruct.getPybind(moduleName);
	CAPTURE(pybind);

	for (auto const& var : variables) {
		auto accessor = var.isConst ? "readonly" : "readwrite";
		auto expectedContains = fmt::format(
		    R"(def_{accessor}("{variableName}", &{className}::{variableName}))",
		    fmt::arg("accessor", accessor),
		    fmt::arg("variableName", var.name),
		    fmt::arg("className", s.m_name));
		CAPTURE(expectedContains);
		REQUIRE(TestUtil::contains(pybind, expectedContains));
	}
}

TEST_CASE("Class with vector in constructor gives the correct include",
          "[classBuilder]") {
	std::string moduleName = "MyModule";
	IR::Struct s;
	s.m_name = "MyStruct";
	s.m_hasImplicitDefaultConstructor = true;
	IR::Function constructor;
	constructor.m_name = s.m_name;
	IR::Type arg;
	IR::Type::Container c;
	c.m_container = IR::ContainerType::Vector;
	arg.m_type = c;
	constructor.m_arguments.push_back({"myVar", arg});
	s.m_functions.push_back({IR::AccessModifier::Public, constructor});

	auto myStruct = Builders::buildClass(s);
	auto includes = myStruct.getIncludes();
	REQUIRE(includes.size() == 1);
	for (auto const& include : includes) {
		REQUIRE(include == "<pybind11/stl.h>");
	}
}

TEST_CASE("Class with vector in member function gives the correct include",
          "[classBuilder]") {
	std::string moduleName = "MyModule";
	IR::Struct s;
	s.m_name = "SomeFunction";
	s.m_hasImplicitDefaultConstructor = true;
	IR::Function constructor;
	constructor.m_name = s.m_name;
	IR::Type arg;
	IR::Type::Container c;
	c.m_container = IR::ContainerType::Vector;
	arg.m_type = c;
	constructor.m_arguments.push_back({"myVar", arg});
	s.m_functions.push_back({IR::AccessModifier::Public, constructor});

	auto myStruct = Builders::buildClass(s);
	auto includes = myStruct.getIncludes();
	REQUIRE(includes.size() == 1);
	for (auto const& include : includes) {
		REQUIRE(include == "<pybind11/stl.h>");
	}
}
