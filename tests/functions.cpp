#include "Frontend/Python/frontend.hpp"
#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"
#include <catch2/catch.hpp>

TEST_CASE("Simple function without arguments", "[functions]") {
	std::string moduleName = "myWonderfulModule";
	auto pybindStage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto globalNSpy = pybindStage.parseModuleFile(R"(
#include <fstream>

void sayHello() {
	std::ofstream f("hello.txt");
	f << "Hello!";
	f.close();
}
)");
	for (auto [file, content] :
	     Frontend::Python::createModules(globalNSpy, moduleName)) {
		pybindStage.addModuleFile(file, content);
	}

	std::string pythonTestBody = R"(
myWonderfulModule.sayHello()
with open("hello.txt", "r") as f:
    self.assertEqual(f.readline(), "Hello!")
)";

	auto errorCode = pybindStage.runPythonUnittest(pythonTestBody);
	REQUIRE(errorCode == 0);
}
