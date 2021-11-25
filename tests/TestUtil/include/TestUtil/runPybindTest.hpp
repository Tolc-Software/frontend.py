#pragma once

#include "Frontend/Python/frontend.hpp"
#include "TestUtil/pybindStage.hpp"

namespace TestUtil {

int runPybindTest(TestUtil::PybindStage& stage,
                  std::string const& cppCode,
                  std::string const& pythonUnittestCode,
                  std::string const& moduleName) {
	auto globalNS = stage.parseModuleFile(cppCode);

	if (auto m = Frontend::Python::createModule(globalNS, moduleName)) {
		auto [file, content] = m.value();

		stage.addModuleFile(file, content);

		return stage.runPythonUnittest(pythonUnittestCode);
	}

	return 1;
}
}    // namespace TestUtil
