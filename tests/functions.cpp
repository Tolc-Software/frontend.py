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

	TestStage::setTargetName(tempStage, "myWonderfulModule");

	std::filesystem::remove(tempStage);
}

// namespace {
// std::filesystem::path writeFileToTestStage(std::filesystem::path const& file,
//                                            std::string const& content) {
// 	std::filesystem::path stagePath =
// 	    TestStage::getRootStagePath() / "src" / file;
// 	std::ofstream f(stagePath);
// 	f << content;
// 	f.close();
// 	return stagePath;
// }
// }    // namespace

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
