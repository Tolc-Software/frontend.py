#include "Frontend/Python/frontend.hpp"
#include <IR/ir.hpp>
#include <Parser/Parse.hpp>
#include <string>
#include <vector>

std::vector<std::string> createModules(std::string const& filename) {
	return {filename};
}
