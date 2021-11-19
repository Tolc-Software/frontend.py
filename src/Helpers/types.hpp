#pragma once

#include <IR/ir.hpp>

namespace Helpers {
/**
* Return container if type is a container, otherwise nullptr
*/
IR::Type::Container const* getContainer(IR::Type const& type);

/**
* Return true iff type is a function
*/
bool isFunctionType(IR::Type const& type);

/**
* Return substring without the template parameters
* Ex:
*   MyClass<int> -> MyClass
*   myFunction<int> -> myFunction
*/
std::string removeCppTemplate(std::string const& name);
}    // namespace Helpers
