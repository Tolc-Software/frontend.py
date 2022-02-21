#include "Frontend/Python/frontend.hpp"
#include "TestStage/paths.hpp"
#include "TestUtil/files.hpp"
#include "TestUtil/pybindStage.hpp"
#include "TestUtil/runPybindTest.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Classes", "[classes]") {
	std::string moduleName = "MyLib";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);
	// Add instantiation in a source file.
	// This cannot be just declared, must be instantiated
	// See https://github.com/pybind/pybind11/issues/682
	// And https://en.cppreference.com/w/cpp/language/static
	//
	// Instantiation (must be in a source file):
	stage.addModuleFile("test.cpp", "int const WithConstructor::i;");

	auto cppCode = R"(
#include <string>
#include <string_view>

class WithConstructor {
public:
	explicit WithConstructor(std::string s) : m_s(s) {}

	static int const i = 5;

	std::string getS() { return m_s; }
	std::string_view getSView() { return m_s; }

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
# You can access static variables without instantiating class
self.assertEqual({moduleName}.WithConstructor.i, 5)

withConstructor = {moduleName}.WithConstructor("Hello")
self.assertEqual(withConstructor.getS(), "Hello")
withConstructor = {moduleName}.WithConstructor(s="named argument")
self.assertEqual(withConstructor.getS(), "named argument")
self.assertEqual(withConstructor.getSView(), "named argument")

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

	auto errorCode =
	    TestUtil::runPybindTest(stage, cppCode, pythonTestCode, moduleName);
	REQUIRE(errorCode == 0);
}
