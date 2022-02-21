#include "Frontend/Python/frontend.hpp"
#include "Builders/moduleFileBuilder.hpp"
#include "PybindProxy/moduleFile.hpp"
#include <IR/ir.hpp>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace Frontend::Python {

std::optional<std::vector<std::pair<std::filesystem::path, std::string>>>
createModule(IR::Namespace const& rootNamespace,
             std::string const& moduleName) {
	if (auto maybeModuleFile =
	        Builders::buildModuleFile(rootNamespace, moduleName)) {
		auto& moduleFile = maybeModuleFile.value();

		return std::vector {
		    std::make_pair(moduleFile.getFilepath(), moduleFile.getPybind())};
	}
	return std::nullopt;
}

}    // namespace Frontend::Python
