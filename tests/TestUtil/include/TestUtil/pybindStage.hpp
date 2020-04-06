#pragma once

#include "TestStage/stage.hpp"
#include <IR/ir.hpp>
#include <filesystem>

namespace TestUtil {
struct PybindStage {
	PybindStage(std::filesystem::path const& baseStage,
	            std::string const& moduleName)
	    : m_stage(baseStage), m_moduleName(moduleName) {
		m_stage.setTargetName(m_moduleName);
	}

	/**
	* Adds the source file moduleName.hpp to the stage with provided content
	* Uses Tolc::Parser to parse the file and returns the global namespace
	*/
	IR::Namespace parseModuleFile(std::string const& content);

	/**
	* Adds a source file to the stage and imports the module file (moduleName.hpp)
	*/
	void addModuleFile(std::filesystem::path const& file,
	                   std::string const& content);

	/**
	*
	*/
	int runPythonUnittest(std::string const& testBody);

	TestStage::Stage m_stage;
	std::string m_moduleName;
};
}
