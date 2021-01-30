#pragma once

#include <IR/ir.hpp>
#include <filesystem>
#include <string>

namespace Frontend::Python {
/**
  * Takes a representation of some C++ code.
  * Returns a file and the corresponding file content.
  * The file content is the converted IR into pybind11 bindings.
  */
std::pair<std::filesystem::path, std::string>
createModule(IR::Namespace const& rootNamespace, std::string const& moduleName);
}
