#pragma once

#include "PybindProxy/moduleFile.hpp"
#include <IR/ir.hpp>

namespace Builders {

/**
* Goes through the given namespace downwards and creates a moduleFile from it
* NOTE: If the rootNamespace name is an empty string, a default "pybind11Module" will be added.
*/
PybindProxy::ModuleFile buildModuleFile(IR::Namespace const& rootNamespace);
}    // namespace Builders

