#include "Frontend/Python/frontend.hpp"
#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"
#include "TestUtil/runPybindTest.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Using std::maps", "[maps]") {
	std::string moduleName = "MyLib";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <map>
#include <string>

class MyClass {
public:
	explicit MyClass(std::map<std::string, int> s) : m_s(s) {}

	std::map<std::string, int> getS() { return m_s; }

	std::string getValue(std::map<int, std::string> const& m, int key) {
		auto it = m.find(key);
		if (it != m.end()) {
			return it->second;
		}
		return "";
	}

private:
	std::map<std::string, int> m_s;
};

)";

	auto pythonTestCode = fmt::format(R"(
myMap = {{"hi": 4, "ho": 5}}
c = {moduleName}.MyClass(myMap)
self.assertEqual(c.getS(), myMap)

# Test map of the wrong type
for incompatiblemap in [{{"key": "value"}}, {{5: 2}}]:
    with self.assertRaises(TypeError) as error_context:
        c = {moduleName}.MyClass(incompatiblemap)
        c.getValue(incompatiblemap, 5)

    self.assertEqual(len(error_context.exception.args), 1)
    self.assertTrue(
        "incompatible function arguments" in error_context.exception.args[0]
        or "incompatible constructor arguments"
        in error_context.exception.args[0],
        "Error msg does not mention incompatible arguments: \n\t"
        + str(error_context.exception.args[0]),
    )
    self.assertTrue(
        str(incompatiblemap) in error_context.exception.args[0],
        "Error msg does not mention the given arguments: \n\t"
        + str(error_context.exception.args[0]),
    )

)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode =
	    TestUtil::runPybindTest(stage, cppCode, pythonTestCode, moduleName);
	REQUIRE(errorCode == 0);
}
