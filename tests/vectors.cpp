#include "Frontend/Python/frontend.hpp"
#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"
#include "TestUtil/runPybindTest.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Using std::vectors", "[vectors]") {
	std::string moduleName = "defaultModule";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <string>
#include <vector>

class WithMember {
public:
	explicit WithMember(std::vector<std::string> s) : m_s(s) {}

	std::vector<std::string> getS() { return m_s; }

private:
	std::vector<std::string> m_s;
};

class WithFunction {
public:
	int sum(std::vector<int> v) {
		int s = 0;
		for (auto i : v) {
			s += i;
		}
		return s;
	}
};

)";

	auto pythonTestCode = fmt::format(R"(
myArray = ["hi", "ho"]
withMember = {moduleName}.WithMember(myArray)
self.assertEqual(withMember.getS(), myArray)

withFunction = {moduleName}.WithFunction()
self.assertEqual(withFunction.sum([1, 2, 3]), 6)
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode =
	    TestUtil::runPybindTest(stage, cppCode, pythonTestCode, moduleName);
	REQUIRE(errorCode == 0);
}
