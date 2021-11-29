#pragma once

#include "PybindProxy/typeInfo.hpp"
#include <IR/ir.hpp>
#include <queue>
#include <set>
#include <string>

namespace Helpers::Pybind {

// Fill TypeInfo related to input type
// Should be called once per type in the whole interface
void checkType(IR::Type const& type, PybindProxy::TypeInfo& info);

}    // namespace Helpers::Pybind
