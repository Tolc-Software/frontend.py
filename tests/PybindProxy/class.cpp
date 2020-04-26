#include "PybindProxy/class.hpp"
#include "TestUtil/string.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Empty class", "[class]") {
	std::string moduleName = "myModule";
	std::string className = "myClass";
	PybindProxy::Class c(className, className);
	auto pybindCode = c.getPybind(moduleName);
	CAPTURE(pybindCode);

	using TestUtil::contains;
	auto containsString =
	    fmt::format(R"(py::class_<{className}>({moduleName}, "{className}"))",
	                fmt::arg("className", className),
	                fmt::arg("moduleName", moduleName));
	CAPTURE(containsString);
	REQUIRE(contains(pybindCode, containsString));
}

TEST_CASE("Class with functions", "[class]") {
	std::string moduleName = "myModule";
	std::string className = "myOtherClass";
	PybindProxy::Class c(className, className);

	std::vector<std::string> functions = {"f", "calculate", "foo"};
	for (auto const& function : functions) {
		c.addFunction(PybindProxy::Function(function, function));
	}

	auto pybindCode = c.getPybind(moduleName);
	CAPTURE(pybindCode);

	using TestUtil::contains;
	for (auto const& function : functions) {
		auto containsString = fmt::format("\t.def(\"{function}\", &{function}",
		                                  fmt::arg("function", function));
		CAPTURE(function);
		CAPTURE(containsString);
		REQUIRE(contains(pybindCode, containsString));
	}
}

TEST_CASE("Class with constructor", "[class]") {
	std::string moduleName = "myModule";
	std::string className = "myFreshClass";
	PybindProxy::Class c(className, className);

	std::vector<std::string> arguments = {"const std::string&", "int"};
	c.addConstructor(PybindProxy::Constructor(arguments));

	auto pybindCode = c.getPybind(moduleName);
	CAPTURE(pybindCode);

	using TestUtil::contains;
	auto containsString =
	    fmt::format("\t.def(py::init<{arguments}>())",
	                fmt::arg("arguments", fmt::join(arguments, ", ")));
	CAPTURE(containsString);
	REQUIRE(contains(pybindCode, containsString));
}

TEST_CASE("Class with member variables", "[class]") {
	std::string moduleName = "NewlyMadeModule";
	std::string className = "SuperbClass";
	PybindProxy::Class c(className, className);

	std::vector<std::string> constVariables = {"myInt", "var", "yes"};
	for (auto const& variable : constVariables) {
		c.addMemberVariable(variable, true);
	}

	std::vector<std::string> nonConstVariables = {"myOtherInt", "var2", "no"};
	for (auto const& variable : nonConstVariables) {
		c.addMemberVariable(variable, false);
	}

	auto pybindCode = c.getPybind(moduleName);
	CAPTURE(pybindCode);

	using TestUtil::contains;
	for (auto const& variable : constVariables) {
		auto containsString = fmt::format(
		    "\t.def_readonly(\"{variable}\", &{className}::{variable})",
		    fmt::arg("variable", variable),
		    fmt::arg("className", className));

		CAPTURE(containsString);
		REQUIRE(contains(pybindCode, containsString));
	}

	for (auto const& variable : nonConstVariables) {
		auto containsString = fmt::format(
		    "\t.def_readwrite(\"{variable}\", &{className}::{variable})",
		    fmt::arg("variable", variable),
		    fmt::arg("className", className));

		CAPTURE(containsString);
		REQUIRE(contains(pybindCode, containsString));
	}
}
