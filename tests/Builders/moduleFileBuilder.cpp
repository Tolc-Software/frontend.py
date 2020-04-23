#include "Builders/moduleFileBuilder.hpp"
#include "PybindProxy/module.hpp"
#include "TestUtil/string.hpp"
#include <IR/ir.hpp>
#include <catch2/catch.hpp>
#include <fmt/format.h>

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

	for (auto const& pybindCode : {"#include <pybind11/pybind11.h>",
	                               "namespace py = pybind11;",
	                               "PYBIND11_MODULE(MyModule, MyModule)"}) {
		CAPTURE(pybindCode);
		REQUIRE(TestUtil::contains(pybind, pybindCode));
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
		sub.m_parent = ns.m_name;
		ns.m_namespaces.push_back(sub);
	}

	auto file = Builders::buildModuleFile(ns);
	auto path = file.getFilepath();
	auto pybind = file.getPybind();
	CAPTURE(path);
	CAPTURE(pybind);

	REQUIRE(path == "MyModule.cpp");

	for (auto const& subNamespace : subNamespaces) {
		auto pybindCode = fmt::format(
		    R"(auto {subNamespace} = MyModule.def_submodule("{subNamespace}");)",
		    fmt::arg("subNamespace", subNamespace));
		CAPTURE(subNamespace);
		CAPTURE(pybindCode);

		REQUIRE(TestUtil::contains(pybind, pybindCode));
	}
}
