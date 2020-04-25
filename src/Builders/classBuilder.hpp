#pragma once

#include "PybindProxy/class.hpp"
#include <IR/ir.hpp>

namespace Builders {

/**
* Transforms a IR::Struct to a PybindProxy::Class
*/
PybindProxy::Class buildClass(IR::Struct const& s);
}    // namespace Builders
