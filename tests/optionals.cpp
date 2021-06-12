#include "Frontend/Python/frontend.hpp"
#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"
#include "TestUtil/runPybindTest.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Using std::optionals", "[optionals]") {
	std::string moduleName = "defaultModule";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <optional>
#include <string>

class WithMember {
public:
	explicit WithMember(std::optional<std::string> s) : m_s(s) {}

	std::optional<std::string> getS() { return m_s; }

private:
	std::optional<std::string> m_s;
};

class WithFunction {
public:
	std::optional<int> getNullopt() {
		return std::nullopt;
	}
};

)";

	auto pythonTestCode = fmt::format(R"(
greeting = "hello"
withMember = {moduleName}.WithMember(greeting)
self.assertEqual(withMember.getS(), greeting)

withFunction = {moduleName}.WithFunction()
self.assertEqual(withFunction.getNullopt(), None)
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode =
	    TestUtil::runPybindTest(stage, cppCode, pythonTestCode, moduleName);
	REQUIRE(errorCode == 0);
}
