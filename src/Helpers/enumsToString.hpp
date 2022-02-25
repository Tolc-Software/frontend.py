#pragma once

#include "Pybind/Proxy/function.hpp"
#include <string>

namespace Helpers {
std::string
returnValuePolicyToString(Pybind::Proxy::Function::return_value_policy p);
}
