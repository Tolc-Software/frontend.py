#pragma once

#include "PybindProxy/moduleFile.hpp"
#include <IR/ir.hpp>

namespace Builders {

/**
* Goes through the given namespace downwards and creates a moduleFile from it
*/
PybindProxy::ModuleFile buildModuleFile(IR::Namespace const& rootNamespace,
                                        std::string const& moduleName);
}    // namespace Builders

