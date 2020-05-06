#include "PybindProxy/function.hpp"
#include "Helpers/enumsToString.hpp"
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

TEST_CASE("Return value policy", "[function]") {
	using ReturnValue = PybindProxy::Function::return_value_policy;
	std::vector<ReturnValue> policies = {ReturnValue::automatic,
	                                     ReturnValue::take_ownership,
	                                     ReturnValue::copy,
	                                     ReturnValue::move,
	                                     ReturnValue::reference,
	                                     ReturnValue::reference_internal,
	                                     ReturnValue::automatic,
	                                     ReturnValue::automatic_reference};

	for (auto const& policy : policies) {
		PybindProxy::Function f("f", "f");
		f.setReturnValuePolicy(policy);

		auto pybindCode = f.getPybind();
		auto containsString = fmt::format(
		    R"(, py::{})", Helpers::returnValuePolicyToString(policy));

		CAPTURE(pybindCode);
		CAPTURE(containsString);

		REQUIRE(TestUtil::contains(pybindCode, containsString));
	}
}
