#pragma once

#include "PybindProxy/class.hpp"
#include <IR/ir.hpp>

namespace Builders {

/**
* Transforms a IR::Struct to a PybindProxy::Class
* NOTE: If the struct does not have an explicit constructor,
*       the default constructor is added
*/
PybindProxy::Class buildClass(IR::Struct const& s);
}    // namespace Builders
