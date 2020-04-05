#include "Frontend/Python/frontend.hpp"
#include "TestStage/paths.hpp"
#include "TestStage/stage.hpp"
#include "TestStage/temporaryStage.hpp"
#include <IR/ir.hpp>
#include <Parser/Parse.hpp>
#include <catch2/catch.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>

TEST_CASE("Function works with default modifier", "[functions]") {
	// Create a mock project
	auto stage = TestStage::Stage(TestStage::getRootStagePath());
	stage.setTargetName("myWonderfulModule");
	std::string moduleName = "myWonderfulModule";
	auto testFile = stage.addSourceFile(moduleName + ".hpp",
	                                    R"(
#include <fstream>

void sayHello() {
	std::ofstream f("hello.txt");
	f << "Hello!";
	f.close();
}
)");
	auto globalNS = Parser::parseFile(testFile.generic_string());
	for (auto [file, content] :
	     Frontend::Python::createModules(globalNS, moduleName)) {
		stage.addSourceFile(file,
		                    "#include \"" + moduleName + ".hpp\"\n" + content);
	}

	stage.runCMakeConfigure();
	stage.buildCMakeProject();

	std::vector<std::string> includes = {"myWonderfulModule"};
	std::string testName = "sayHello";
	std::vector<std::string> testBody = {
	    R"(myWonderfulModule.sayHello())",
	    R"(with open("hello.txt", "r") as f:)",
	    R"(    self.assertEqual(f.readline(), "Hello!"))"};

	TestStage::addPythonUnittest(
	    stage.m_stage, moduleName, testName, testBody, includes);

	auto errorCode = TestStage::runPythonUnittest(stage.m_stage, moduleName);
	REQUIRE(errorCode == 0);
}

// TEST_CASE("Function works with default modifier", "[functions]") {
// 	auto cppFile = writeFileToTestStage("test.hpp", R"(
// #include <iostream>

// void sayHello() {
// 	std::cout << "Hello!" << '\n';
// }
// )");
// 	auto globalNS = Parser::parseFile(cppFile.generic_string());
// 	std::string globalModuleName = "myModule";
// 	for (auto [file, content] :
// 	     Frontend::Python::createModules(globalNS, globalModuleName)) {
// 		writeFileToTestStage(file, content);
// 	}
// }
