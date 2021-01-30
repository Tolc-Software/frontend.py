#pragma once

#include "Builders/functionBuilder.hpp"
#include "PybindProxy/module.hpp"
#include <IR/ir.hpp>

namespace Builders {

/**
* Build proxy module from a namespace
* NOTE: Does not traverse the structure,
*       but adds the children of ns as submodules
*/
PybindProxy::Module buildModule(IR::Namespace const& ns,
                                std::string const& rootModuleName);
}    // namespace Builders
