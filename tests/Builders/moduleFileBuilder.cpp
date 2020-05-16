#include "Builders/moduleFileBuilder.hpp"
#include "PybindProxy/module.hpp"
#include "TestUtil/string.hpp"
#include <IR/ir.hpp>
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE(
    "Adding a function with a vector for argument automaticall includes stl.h",
    "[moduleFileBuilder]") {
	IR::Namespace ns;
	ns.m_name = "MyModule";
	ns.m_representation = ns.m_name;
	IR::Function f;
	f.m_name = 'f';
	f.m_representation = f.m_name;
	IR::Type returnType;
	IR::Type::Container c;
	c.m_container = IR::ContainerType::Vector;
	returnType.m_type = c;
	f.m_returnType = returnType;
	ns.m_functions.push_back(f);

	auto file = Builders::buildModuleFile(ns);
	auto path = file.getFilepath();
	auto pybind = file.getPybind();
	CAPTURE(path);
	CAPTURE(pybind);

	REQUIRE(path == "MyModule.cpp");

	for (auto const& expectedContains :
	     {"#include <pybind11/pybind11.h>",
	      "#include <pybind11/stl.h>",
	      "namespace py = pybind11;",
	      "PYBIND11_MODULE(MyModule, MyModule)"}) {
		CAPTURE(expectedContains);
		REQUIRE(TestUtil::contains(pybind, expectedContains));
	}
}

TEST_CASE("One level namespace", "[moduleFileBuilder]") {
	IR::Namespace ns;
	ns.m_name = "MyModule";
	ns.m_representation = ns.m_name;

	auto file = Builders::buildModuleFile(ns);
	auto path = file.getFilepath();
	auto pybind = file.getPybind();
	CAPTURE(path);
	CAPTURE(pybind);

	REQUIRE(path == "MyModule.cpp");

	for (auto const& expectedContains :
	     {"#include <pybind11/pybind11.h>",
	      "namespace py = pybind11;",
	      "PYBIND11_MODULE(MyModule, MyModule)"}) {
		CAPTURE(expectedContains);
		REQUIRE(TestUtil::contains(pybind, expectedContains));
	}
}

TEST_CASE("Two level namespace", "[moduleFileBuilder]") {
	IR::Namespace ns;
	ns.m_name = "MyModule";
	ns.m_representation = ns.m_name;
	std::vector subNamespaces = {"sub1", "sub2", "sub3"};
	for (auto const& subNs : subNamespaces) {
		IR::Namespace sub;
		sub.m_name = subNs;
		sub.m_representation = sub.m_name;
		ns.m_namespaces.push_back(sub);
	}

	auto file = Builders::buildModuleFile(ns);
	auto path = file.getFilepath();
	auto pybind = file.getPybind();
	CAPTURE(path);
	CAPTURE(pybind);

	REQUIRE(path == "MyModule.cpp");

	for (auto const& subNamespace : subNamespaces) {
		auto expectedContains = fmt::format(
		    R"(auto {subNamespace} = MyModule.def_submodule("{subNamespace}");)",
		    fmt::arg("subNamespace", subNamespace));
		CAPTURE(subNamespace);
		CAPTURE(expectedContains);

		REQUIRE(TestUtil::contains(pybind, expectedContains));
	}
}
