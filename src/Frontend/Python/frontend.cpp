#include "Frontend/Python/frontend.hpp"
#include "Builders/moduleFileBuilder.hpp"
#include "PybindProxy/moduleFile.hpp"
#include <IR/ir.hpp>
#include <filesystem>
#include <optional>
#include <string>

namespace Frontend::Python {

std::optional<std::pair<std::filesystem::path, std::string>>
createModule(IR::Namespace const& rootNamespace,
             std::string const& moduleName) {
	if (auto moduleFile =
	        Builders::buildModuleFile(rootNamespace, moduleName)) {
		return std::make_pair(moduleFile.value().getFilepath(),
		                      moduleFile.value().getPybind());
	}
	return std::nullopt;
}

}    // namespace Frontend::Python
