#include "Frontend/Python/frontend.h"
#include <IR/ir.hpp>
#include <Parser/Parse.h>
#include <string>
#include <vector>

std::vector<std::string> createModules(std::string const& filename) {
	return {filename};
}
