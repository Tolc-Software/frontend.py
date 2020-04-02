#include "Frontend/Python/frontend.hpp"
#include "TestStage/paths.hpp"
#include "TestStage/temporaryStage.hpp"
#include <IR/ir.hpp>
#include <Parser/Parse.hpp>
#include <catch2/catch.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>

TEST_CASE("Function works with default modifier", "[functions]") {
	auto rootStage = TestStage::getRootStagePath();
	std::cout << rootStage << '\n';
	auto tempStage = TestStage::createUniqueStage(rootStage);
	std::cout << tempStage << '\n';
	TestStage::populateStageFromRoot(tempStage, rootStage);

	std::string moduleName = "myWonderfulModule";
	TestStage::setTargetName(tempStage, moduleName);

	auto testFile = TestStage::addFileToStage(moduleName + ".hpp",
	                                          R"(
#include <iostream>

void sayHello() {
	std::cout << "Hello!" << '\n';
}
)",
	                                          tempStage);

	auto globalNS = Parser::parseFile(testFile.generic_string());
	for (auto [file, content] :
	     Frontend::Python::createModules(globalNS, moduleName)) {
		TestStage::addFileToStage(
		    file, "#include \"" + moduleName + ".hpp\"\n" + content, tempStage);
	}

	TestStage::runCMakeConfigure(tempStage);

	TestStage::buildCMakeProject(tempStage);

	std::filesystem::remove_all(tempStage);
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
