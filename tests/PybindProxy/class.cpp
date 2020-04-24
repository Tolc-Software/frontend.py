#include "PybindProxy/class.hpp"
#include "TestUtil/string.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

// TEST_CASE("Modules defines their functions", "[module]") {
// 	std::string moduleName = "myModule";
// 	PybindProxy::Module m(moduleName);

// 	std::vector<std::string> functions = {"f", "calculate", "foo"};
// 	for (auto const& function : functions) {
// 		m.addFunction(PybindProxy::Function(function, function));
// 	}

// 	auto pybindCode = m.getPybind();
// 	CAPTURE(pybindCode);

// 	using TestUtil::contains;
// 	for (auto const& function : functions) {
// 		auto containsString =
// 		    fmt::format(R"({module}.def("{function}", &{function})",
// 		                fmt::arg("module", moduleName),
// 		                fmt::arg("function", function));
// 		CAPTURE(function);
// 		CAPTURE(containsString);
// 		REQUIRE(contains(pybindCode, containsString));
// 	}
// }

TEST_CASE("Empty class", "[class]") {
	std::string moduleName = "myModule";
	std::string className = "myClass";
	PybindProxy::Class c(className);
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
	PybindProxy::Class c(className);

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
	PybindProxy::Class c(className);

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
