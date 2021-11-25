#pragma once

#include "PybindProxy/class.hpp"
#include <IR/ir.hpp>
#include <optional>

namespace Builders {

/**
* Transforms a IR::Struct to a PybindProxy::Class
* NOTE: If the struct does not have an explicit constructor,
*       the default constructor is added
* Fails if any member functions takes unique_ptr as an argument
*/
std::optional<PybindProxy::Class> buildClass(IR::Struct const& s);
}    // namespace Builders
