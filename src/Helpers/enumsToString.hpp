#pragma once

#include "PybindProxy/function.hpp"
#include <string>

namespace Helpers {
std::string
returnValuePolicyToString(PybindProxy::Function::return_value_policy p);
}
