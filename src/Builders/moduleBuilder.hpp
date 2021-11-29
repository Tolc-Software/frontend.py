#pragma once

#include "Builders/functionBuilder.hpp"
#include "PybindProxy/module.hpp"
#include "PybindProxy/typeInfo.hpp"
#include <IR/ir.hpp>
#include <optional>

namespace Builders {

/**
* Build proxy module from a namespace
* NOTE: Does not traverse the structure,
*       but adds the children of ns as submodules
* Fails if any functions takes unique_ptr as an argument
*/
std::optional<PybindProxy::Module>
buildModule(IR::Namespace const& ns,
            std::string const& rootModuleName,
            PybindProxy::TypeInfo& typeInfo);
}    // namespace Builders
