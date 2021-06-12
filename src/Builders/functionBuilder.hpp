#pragma once

#include "PybindProxy/function.hpp"
#include <IR/ir.hpp>

namespace Builders {

/**
* Transforms a IR::Function to a PybindProxy::Function
*/
PybindProxy::Function buildFunction(IR::Function const& fun);
}    // namespace Builders
