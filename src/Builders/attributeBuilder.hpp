#pragma once

#include "PybindProxy/attribute.hpp"
#include <IR/ir.hpp>

namespace Builders {

/**
* Transforms a IR::Variable to a PybindProxy::Attribute
*/
PybindProxy::Attribute buildAttribute(std::string const& parentNamespace,
                                      IR::Variable const& v);
}    // namespace Builders
