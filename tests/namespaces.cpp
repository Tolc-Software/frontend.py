#include "Frontend/Python/frontend.hpp"
#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"
#include "TestUtil/runPybindTest.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Namespaces turn into modules", "[namespaces]") {
	std::string moduleName = "defaultModule";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
namespace MyLib {

int complexFunction() {
	return 5;
}
}

)";

	auto pythonTestCode = fmt::format(R"(
result = {moduleName}.MyLib.complexFunction()
self.assertEqual(result, 5)
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode = TestUtil::runPybindTest(stage, cppCode, pythonTestCode);
	REQUIRE(errorCode == 0);
}
