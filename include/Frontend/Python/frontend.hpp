#pragma once

#include <IR/ir.hpp>
#include <filesystem>
#include <string>

namespace Frontend::Python {
/**
  * Takes a representation of C++ code and the name of the global module.
  * Returns a file and the corresponding file content.
  * The file content is the converted C++ into pybind11 bindings.
  */
std::pair<std::filesystem::path, std::string>
createModules(IR::Namespace const& rootNamespace);
}
