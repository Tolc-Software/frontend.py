#include "Frontend/Python/frontend.hpp"
#include "PybindProxy/function.hpp"
#include "PybindProxy/module.hpp"
#include <IR/ir.hpp>
#include <filesystem>
#include <string>
#include <vector>

namespace Frontend::Python {

std::vector<std::pair<std::filesystem::path, std::string>>
createModules(IR::Namespace const& globalNamespace,
              std::string const& globalModuleName) {
	PybindProxy::Module m(globalModuleName);
	for (auto const& function : globalNamespace.m_functions) {
		PybindProxy::Function f(function.m_name, {});
		m.addFunction(f);
	}
	return {std::make_pair(
	    std::filesystem::path(fmt::format("{}.cpp", globalModuleName)),
	    m.getPybind())};
}

}    // namespace Frontend::Python
