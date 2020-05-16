#pragma once

#include <IR/ir.hpp>
#include <queue>
#include <set>
#include <string>

namespace Helpers::Pybind {

/**
* Go through type and check if it requires any extra pybind11 includes
* E.g. vector conversion requires inclusion of <pybind11/stl.h>
*/
std::set<std::string> extractIncludes(IR::Type const& type);

}    // namespace Helpers::Pybind
