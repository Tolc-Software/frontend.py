#include "Frontend/Python/frontend.hpp"
#include <IR/ir.hpp>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace Frontend::Python {

std::vector<std::pair<std::filesystem::path, std::string>>
createModules(IR::Namespace const& /* globalNamespace*/,
              std::string_view globalModuleName) {
	return {std::make_pair(std::filesystem::path("hi"), "ho")};
}

}    // namespace Frontend::Python
