#pragma once

#include "Frontend/Python/frontend.hpp"
#include "TestUtil/pybindStage.hpp"

namespace TestUtil {

int runPybindTest(TestUtil::PybindStage& stage,
                  std::string const& cppCode,
                  std::string const& pythonUnittestCode) {
	auto globalNS = stage.parseModuleFile(cppCode);

	auto [file, content] = Frontend::Python::createModule(globalNS);

	stage.addModuleFile(file, content);

	return stage.runPythonUnittest(pythonUnittestCode);
}
}    // namespace TestUtil
