#include "TestUtil/pybindStage.hpp"
#include "TestStage/stage.hpp"
#include "TestStage/temporaryStage.hpp"
#include "TestUtil/parse.hpp"
#include <IR/ir.hpp>
#include <Parser/Parse.hpp>
#include <filesystem>
#include <string>

namespace TestUtil {

void PybindStage::addModuleFile(std::filesystem::path const& file,
                                std::string const& content) {
	m_stage.addSourceFile(file,
	                      "#include \"" + m_moduleName + ".hpp\"\n" + content);
}

IR::Namespace PybindStage::parseModuleFile(std::string const& content) {
	auto testFile = m_stage.addSourceFile(m_moduleName + ".hpp", content);
	return TestUtil::parseFile(testFile.generic_string());
}

int PybindStage::runPythonUnittest(std::string const& testBody) {
	// Make sure the module is built
	m_stage.runCMakeConfigure();
	m_stage.buildCMakeProject();

	std::vector<std::string> includes = {m_moduleName};
	std::string testName = "default";

	std::vector<std::string> body;

	std::string::size_type pos = 0;
	std::string::size_type prev = 0;
	while ((pos = testBody.find('\n', prev)) != std::string::npos) {
		body.push_back(testBody.substr(prev, pos - prev));
		prev = pos + 1;
	}

	// To get the last substring (or only, if delimiter is not found)
	body.push_back(testBody.substr(prev));

	TestStage::addPythonUnittest(
	    m_stage.m_stage, m_moduleName, testName, body, includes);

	return TestStage::runPythonUnittest(m_stage.m_stage, m_moduleName);
}
}

