#include "Frontend/Python/frontend.hpp"
#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"
#include "TestUtil/runPybindTest.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Classes", "[classes]") {
	std::string moduleName = "defaultModule";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <string>

class WithConstructor {
public:
	explicit WithConstructor(std::string s) : m_s(s) {}

	std::string getS() { return m_s; }

private:
	std::string m_s;
};

class WithFunction {
public:
	int add(int i, int j) {
		return i + j;
	}
};

class WithPrivateFunction {
	double multiply(double i, double j) {
		return i * j;
	}
};

namespace MyLib {

	class Nested {
	public:
		double divideByTwo(double d) {
			return d / 2;
		}
	};
}

)";

	auto pythonTestCode = fmt::format(R"(
withConstructor = {moduleName}.WithConstructor("Hello")
self.assertEqual(withConstructor.getS(), "Hello")

withFunction = {moduleName}.WithFunction()
self.assertEqual(withFunction.add(2, 5), 7)

# Test private function
with self.assertRaises(AttributeError) as error_context:
    withPrivateFunction = {moduleName}.WithPrivateFunction()
    withPrivateFunction.multiply(3, 2)

self.assertEqual(len(error_context.exception.args), 1)
# print(error_context.test_case)
self.assertEqual(
    "'{moduleName}.WithPrivateFunction' object has no attribute 'multiply'",
    error_context.exception.args[0],
    "Not correct exception on private functions",
)

nested = {moduleName}.MyLib.Nested()
self.assertEqual(nested.divideByTwo(10), 5)
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode = TestUtil::runPybindTest(stage, cppCode, pythonTestCode);
	REQUIRE(errorCode == 0);
}
