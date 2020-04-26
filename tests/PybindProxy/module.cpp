#include "PybindProxy/module.hpp"
#include "TestUtil/string.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Modules defines their functions", "[module]") {
	std::string moduleName = "myModule";
	PybindProxy::Module m(moduleName);

	std::vector<std::string> functions = {"f", "calculate", "foo"};
	for (auto const& function : functions) {
		m.addFunction(PybindProxy::Function(function, function));
	}

	auto pybindCode = m.getPybind();
	CAPTURE(pybindCode);

	using TestUtil::contains;
	for (auto const& function : functions) {
		auto expectedContains =
		    fmt::format(R"({module}.def("{function}", &{function})",
		                fmt::arg("module", moduleName),
		                fmt::arg("function", function));
		CAPTURE(function);
		CAPTURE(expectedContains);
		REQUIRE(contains(pybindCode, expectedContains));
	}
}

TEST_CASE("Modules defines their submodules", "[module]") {
	std::string moduleName = "myTestModule";
	PybindProxy::Module m(moduleName);

	std::vector<std::string> submodules = {"sub1", "mySub", "child"};
	for (auto const& submodule : submodules) {
		m.addSubmodule(submodule);
	}

	auto pybindCode = m.getPybind();
	CAPTURE(pybindCode);

	using TestUtil::contains;
	for (auto const& submodule : submodules) {
		auto expectedContains = fmt::format(
		    R"(auto {submodule} = {module}.def_submodule("{submodule}");)",
		    fmt::arg("module", moduleName),
		    fmt::arg("submodule", submodule));
		CAPTURE(submodule);
		CAPTURE(expectedContains);
		REQUIRE(contains(pybindCode, expectedContains));
	}
}

TEST_CASE("Modules defines their classes", "[module]") {
	std::string moduleName = "myTestModule";
	PybindProxy::Module m(moduleName);

	std::vector<std::string> classes = {"Cl", "MyClass", "OtherClass"};
	for (auto const& cls : classes) {
		PybindProxy::Class c(cls, cls);
		m.addClass(c);
	}

	auto pybindCode = m.getPybind();
	CAPTURE(pybindCode);

	using TestUtil::contains;
	for (auto const& cls : classes) {
		auto expectedContains =
		    fmt::format("\tpy::class_<{cls}>({moduleName}, \"{cls}\");\n",
		                fmt::arg("cls", cls),
		                fmt::arg("moduleName", moduleName));
		CAPTURE(cls);
		CAPTURE(expectedContains);
		REQUIRE(contains(pybindCode, expectedContains));
	}
}
