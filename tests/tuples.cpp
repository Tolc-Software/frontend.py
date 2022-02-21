#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Using std::tuples", "[tuples]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <string>
#include <tuple>

class MyClass {
public:
	explicit MyClass(std::tuple<std::string, int> s) : m_s(s) {}

	std::tuple<std::string, int> getS() { return m_s; }

	std::tuple<std::string, int> m_s;
};

class WithFunction {
public:
	double sum(std::tuple<int, int, float, double> t) {
		return std::get<0>(t)
			   + std::get<1>(t)
			   + std::get<2>(t)
			   + std::get<3>(t);
	}
};

)";

	auto pythonTestCode = fmt::format(R"(
# Converts to a tuple, but is convertible from array aswell
myArray = ["hi", 4]
for t in [myArray, tuple(myArray)]:
    withMember = {moduleName}.MyClass(t)
    self.assertEqual(withMember.getS(), tuple(t))

withFunction = {moduleName}.WithFunction()
self.assertAlmostEqual(withFunction.sum((1, 2, 3.3, 2.0)), 8.3, delta=0.0001)
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode = stage.runPybindTest(cppCode, pythonTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("std::tuple");
}
