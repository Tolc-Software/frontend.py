#include "Frontend/Python/frontend.hpp"
#include <IR/ir.hpp>
#include <Parser/Parse.hpp>
#include <catch2/catch.hpp>
#include <filesystem>
#include <fstream>

namespace {
void writeFileToTestStage(std::filesystem::path const& file,
                          std::string const& content) {
	std::filesystem::path stagePath =
	    "/home/simon/external/code/cpp/frontend.py/tests/testStage/src/" / file;
	std::ofstream f(stagePath);
	f << content;
	f.close();
}
}    // namespace

TEST_CASE("Function works with default modifier", "[functions]") {
	auto globalNS = Parser::parseString("class MyStructure { void fun(); };");
	std::string globalModuleName = "myModule";
	for (auto& [file, content] :
	     Frontend::Python::createModules(globalNS, globalModuleName)) {
		writeFileToTestStage(file, content);
	}
}
