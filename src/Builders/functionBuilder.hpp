#pragma once

#include "PybindProxy/function.hpp"
#include <IR/ir.hpp>
#include <optional>

namespace Builders {

/**
* Transforms a IR::Function to a PybindProxy::Function
* Fails if any of the arguments are std::unique_ptr (https://pybind11.readthedocs.io/en/stable/advanced/smart_ptrs.html)
*/
std::optional<PybindProxy::Function> buildFunction(IR::Function const& fun);
}    // namespace Builders
