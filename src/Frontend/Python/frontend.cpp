#include "Frontend/Python/frontend.hpp"
#include "Builders/moduleFileBuilder.hpp"
#include "PybindProxy/moduleFile.hpp"
#include <IR/ir.hpp>
#include <filesystem>
#include <string>

namespace Frontend::Python {

std::pair<std::filesystem::path, std::string>
createModule(IR::Namespace const& rootNamespace) {
	auto moduleFile = Builders::buildModuleFile(rootNamespace);

	return std::make_pair(moduleFile.getFilepath(), moduleFile.getPybind());
}

}    // namespace Frontend::Python
