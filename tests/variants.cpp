#include "Frontend/Python/frontend.hpp"
#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"
#include "TestUtil/runPybindTest.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Using std::variants", "[variants]") {
	std::string moduleName = "defaultModule";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <string>
#include <variant>

class WithMember {
public:
	explicit WithMember(std::variant<int, bool> s) : m_s(s) {}

	auto getS() { return m_s; }

private:
	std::variant<int, bool> m_s;
};

class WithFunction {
public:
	std::variant<int, std::string, bool> getFive() {
		return 5;
	}

	std::variant<int, std::string, bool> getHello() {
		return std::string("Hello");
	}

	std::variant<int, std::string, bool> getTrue() {
		return true;
	}
};

)";

	auto pythonTestCode = fmt::format(R"(
number = 6
withNumber = {moduleName}.WithMember(number)
self.assertEqual(withNumber.getS(), number)

withBool = {moduleName}.WithMember(True)
self.assertEqual(withBool.getS(), True)

withFunction = {moduleName}.WithFunction()
self.assertEqual(withFunction.getFive(), 5)
self.assertEqual(withFunction.getHello(), "Hello")
self.assertEqual(withFunction.getTrue(), True)
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode =
	    TestUtil::runPybindTest(stage, cppCode, pythonTestCode, moduleName);
	REQUIRE(errorCode == 0);
}
