#pragma once

#include <IR/ir.hpp>
#include <filesystem>
#include <string>
#include <vector>

namespace Frontend::Python {
/**
  * Takes a representation of C++ code and
  * returns a set of files and corresponding file content.
  * The file content is the converted C++ into pybind11 bindings.
  */
std::vector<std::pair<std::filesystem::path, std::string>>
createModules(IR::Namespace const& globalNamespace);
}
