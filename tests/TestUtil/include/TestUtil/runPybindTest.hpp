#pragma once

#include "Frontend/Python/frontend.hpp"
#include "TestUtil/pybindStage.hpp"

namespace TestUtil {

int runPybindTest(TestUtil::PybindStage& stage,
                  std::string const& cppCode,
                  std::string const& pythonUnittestCode,
                  std::string const& moduleName) {
	auto globalNS = stage.parseModuleFile(cppCode);

	auto [file, content] = Frontend::Python::createModule(globalNS, moduleName);

	stage.addModuleFile(file, content);

	return stage.runPythonUnittest(pythonUnittestCode);
}
}    // namespace TestUtil
