#include "Frontend/Python/frontend.hpp"
#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"
#include "TestUtil/runPybindTest.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Smart pointers of custom types work", "[smartPointers]") {
	// NOTE: Smart pointers only work with custom defined types.
	//       This makes unique_ptr<int> not computable.
	//       If this is desirable in the future,
	//       wrap the int in a lambda and copy the value
	// Ex:
	//
	//   std::unique_ptr<int> create_int() {
	//      return std::unique_ptr<int>(new int(53));
	//   }
	//
	//   m.def("create_int", []() { return *create_int().get(); });
	std::string moduleName = "defaultModule";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <memory>

struct Example {
	int m_hi = 5;
};

std::unique_ptr<Example> create_unique() {
	return std::make_unique<Example>();
}

std::shared_ptr<Example> create_shared() {
	return std::make_shared<Example>();
}
)";

	auto pythonTestCode = fmt::format(R"(
u = {moduleName}.create_unique()
self.assertEqual(u.m_hi, 5)

s = {moduleName}.create_shared()
self.assertEqual(s.m_hi, 5)
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode =
	    TestUtil::runPybindTest(stage, cppCode, pythonTestCode, moduleName);
	REQUIRE(errorCode == 0);
}
