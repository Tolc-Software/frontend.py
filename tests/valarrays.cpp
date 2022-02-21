#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Simple valarrays", "[valarrays]") {
	std::string moduleName = "MyLib";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <valarray>

std::valarray<int> get() {
	return {1, 2, 3};
}
)";

	auto pythonTestCode = fmt::format(R"(
v = {moduleName}.get()
self.assertEqual(v, [1, 2, 3])
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode = stage.runPybindTest(cppCode, pythonTestCode);
	REQUIRE(errorCode == 0);
}
