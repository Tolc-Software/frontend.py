#include "test.hpp"
#include <pybind11/pybind11.h>

namespace py = pybind11;

int add(int i, int j) {
	return i + j;
}

PYBIND11_MODULE(myModule, m) {
	// NOTE:    ^-- This name needs to be the same as the CMake target

	// optional module docstring
	m.doc() = "pybind11 example plugin";

	// Adding a simple function with optional help text
	m.def("add", &add, "A function which adds two numbers");

	// Adding a submodule within this module
	auto myNamespace = m.def_submodule("MyNamespace");
	// Adding functions within a namespace
	// is done with a fully qualified name
	myNamespace.def("sayHello", &MyNamespace::sayHello);
}
