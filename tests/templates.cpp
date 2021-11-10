#include "Frontend/Python/frontend.hpp"
#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"
#include "TestUtil/runPybindTest.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Specialized templates", "[templates]") {
	std::string moduleName = "defaultModule";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);
	stage.keepAliveAfterTest();

	auto cppCode = R"(
#include <string>

template <typename T>
T getSomething(T something) {
	return something;
}

template <>
std::string getSomething(std::string something);
)";

	auto pythonTestCode = fmt::format(R"(
hi = {moduleName}.getSomething("hi")
self.assertEqual(hi, "hi")
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode =
	    TestUtil::runPybindTest(stage, cppCode, pythonTestCode, moduleName);
	REQUIRE(errorCode == 0);
}
