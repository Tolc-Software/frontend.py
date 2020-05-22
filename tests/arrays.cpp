#include "Frontend/Python/frontend.hpp"
#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"
#include "TestUtil/runPybindTest.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Using std::arrays", "[arrays]") {
	std::string moduleName = "defaultModule";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <array>
#include <string>

class WithMember {
public:
	explicit WithMember(std::array<std::string, 2> s) : m_s(s) {}

	auto getS() { return m_s; }

private:
	std::array<std::string, 2> m_s;
};

class WithFunction {
public:
	int sum(std::array<int, 5> v) {
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
self.assertEqual(withFunction.sum([1, 2, 3, 4, 5]), 15)

# Test array with too many/few values
for incompatibleArray in [["too many", "too many", "too many"], ["too few"]]:
    with self.assertRaises(TypeError) as error_context:
        withMember = defaultModule.WithMember(incompatibleArray)

    self.assertEqual(len(error_context.exception.args), 1)
    self.assertTrue(
        "incompatible constructor arguments" in error_context.exception.args[0],
        "Error msg does not mention incompatible arguments: "
        + str(error_context.exception.args[0]),
    )
    self.assertTrue(
        "Invoked with: " + str(incompatibleArray)
        in error_context.exception.args[0],
        "Error msg does not mention the given arguments: " + str(error_context.exception.args[0]),
    )

)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode = TestUtil::runPybindTest(stage, cppCode, pythonTestCode);
	REQUIRE(errorCode == 0);
}
