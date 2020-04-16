#pragma once

#include <IR/ir.hpp>
#include <filesystem>
#include <string>

namespace Frontend::Python {
/**
  * Takes a representation of some C++ code.
  * The name of the rootNamespace becomes the name of the root module.
  * Returns a file and the corresponding file content.
  * The file content is the converted C++ into pybind11 bindings.
  * NOTE: If the rootNamespace name is an empty string, a default "pybind11Module" will be added.
  */
std::pair<std::filesystem::path, std::string>
createModules(IR::Namespace const& rootNamespace);
}
