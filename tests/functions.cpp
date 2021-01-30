#include "Frontend/Python/frontend.hpp"
#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"
#include "TestUtil/runPybindTest.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Write to file functions", "[functions]") {
	std::string moduleName = "defaultModule";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <fstream>
#include <string>

void sayHello() {
	std::ofstream f("hello.txt");
	f << "Hello!";
	f.close();
}

void addYourOwn(std::string content) {
	std::ofstream f("hello.txt");
	f << content;
	f.close();
}

int calculate() {
	return 5;
}
)";

	auto pythonTestCode = fmt::format(R"(
{moduleName}.sayHello()
with open("hello.txt", "r") as f:
    self.assertEqual(f.readline(), "Hello!")

content = "This is from python!"
{moduleName}.addYourOwn(content)
with open("hello.txt", "r") as f:
    self.assertEqual(f.readline(), content)

result = {moduleName}.calculate()
self.assertEqual(result, 5)
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode =
	    TestUtil::runPybindTest(stage, cppCode, pythonTestCode, moduleName);
	REQUIRE(errorCode == 0);
}

