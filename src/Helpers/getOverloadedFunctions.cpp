#include "Helpers/getOverloadedFunctions.hpp"
#include <IR/ir.hpp>
#include <set>
#include <string>
#include <vector>

namespace Helpers {

// Go through functions and return the representations of the functions that are overloaded
// i.e. exists twice in functions
std::set<std::string>
getOverloadedFunctions(std::vector<IR::Function> const& functions) {
	// Will contain all function names
	std::set<std::string> visitedFunctions;
	std::set<std::string> overloadedFunctions;
	for (auto const& function : functions) {
		if (auto [it, isNewFunction] =
		        visitedFunctions.insert(function.m_representation);
		    !isNewFunction) {
			overloadedFunctions.insert(function.m_representation);
		}
	}
	return overloadedFunctions;
}
}    // namespace
