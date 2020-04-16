#include "Frontend/Python/frontend.hpp"
#include "PybindProxy/function.hpp"
#include "PybindProxy/module.hpp"
#include "PybindProxy/moduleFile.hpp"
#include <IR/ir.hpp>
#include <filesystem>
#include <string>

namespace Frontend::Python {

std::pair<std::filesystem::path, std::string>
createModules(IR::Namespace const& rootNamespace) {
	std::string rootModuleName = "globalModule";
	if (!rootNamespace.m_name.empty()) {
		rootModuleName = rootNamespace.m_name;
	}

	PybindProxy::Module m(rootModuleName);

	for (auto const& function : rootNamespace.m_functions) {
		PybindProxy::Function f(function.m_name, {});
		m.addFunction(f);
	}

	PybindProxy::ModuleFile mf(m);

	return std::make_pair(
	    std::filesystem::path(fmt::format("{}.cpp", rootModuleName)),
	    mf.getPybind());
}

}    // namespace Frontend::Python
