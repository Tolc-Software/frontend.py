#pragma once

#include "PybindProxy/attribute.hpp"
#include "PybindProxy/typeInfo.hpp"
#include <IR/ir.hpp>

namespace Builders {

/**
* Transforms a IR::Variable to a PybindProxy::Attribute
*/
PybindProxy::Attribute buildAttribute(std::string const& parentNamespace,
                                      IR::Variable const& v,
                                      PybindProxy::TypeInfo& typeInfo);
}    // namespace Builders
