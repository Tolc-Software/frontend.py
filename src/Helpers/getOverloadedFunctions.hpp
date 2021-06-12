#pragma once
#include <IR/ir.hpp>
#include <set>
#include <string>
#include <vector>

namespace Helpers {
std::set<std::string>
getOverloadedFunctions(std::vector<IR::Function> const& functions);
}
