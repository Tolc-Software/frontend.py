#include "Frontend/Python/frontend.hpp"
#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"
#include "TestUtil/runPybindTest.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Write to file functions", "[functions]") {
	std::string moduleName = "defaultModule";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <functional>

double takingFunction(std::function<double(int)> callMe) {
	return callMe(5);
}

std::function<int(int)> func_ret(const std::function<int(int)> &f) {
	return [f](int i) {
		return f(i) + 1;
	};
}
)";

	auto pythonTestCode = fmt::format(R"(
def callback(i):
  return i

result0 = {moduleName}.takingFunction(callback)
self.assertEqual(result0, 5.0)

inc_by_one = {moduleName}.func_ret(callback)
self.assertEqual(inc_by_one(5), 6)
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode =
	    TestUtil::runPybindTest(stage, cppCode, pythonTestCode, moduleName);
	REQUIRE(errorCode == 0);
}

