#pragma once

#include "PybindProxy/enum.hpp"
#include <IR/ir.hpp>

namespace Builders {

/**
* Transforms a IR::Enum to a PybindProxy::Enum
*/
PybindProxy::Enum buildEnum(IR::Enum const& s);
}    // namespace Builders
