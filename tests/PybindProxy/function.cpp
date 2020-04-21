#include "PybindProxy/function.hpp"
#include "TestUtil/string.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Simple function within a namespace", "[function]") {
	PybindProxy::Function f("f", "MyNamespace::f");

	auto pybindCode = f.getPybind();
	CAPTURE(pybindCode);

	using TestUtil::contains;
	// The function bind
	REQUIRE(contains(pybindCode, R"(def("f", &MyNamespace::f)"));
}

TEST_CASE("Function with arguments", "[function]") {
	PybindProxy::Function f("f", "f");
	std::vector<std::string> args = {"i", "j", "k"};
	for (auto const& arg : args) {
		f.addArgument(arg);
	}

	auto pybindCode = f.getPybind();
	CAPTURE(pybindCode);

	using TestUtil::contains;
	// The function bind
	REQUIRE(contains(pybindCode, R"(def("f", &f)"));

	for (auto const& arg : args) {
		REQUIRE(contains(pybindCode, fmt::format(R"(py::arg("{}"))", arg)));
	}
}
