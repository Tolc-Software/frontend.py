#include "test.hpp"
#include <pybind11/pybind11.h>

namespace py = pybind11;

int add(int i, int j) {
	return i + j;
}

PYBIND11_MODULE(myModule, myModule) {
	// NOTE:    ^-- This name needs to be the same as the CMake target

	// optional module docstring
	myModule.doc() = "pybind11 example plugin";

	// Adding a simple function with optional help text
	// and named variables (allows python to use add(i = 5, j = 3))
	myModule.def("add",
	             &add,
	             "A function which adds two numbers",
	             py::arg("i"),
	             py::arg("j"));

	// Adding a submodule within this module
	auto myNamespace = myModule.def_submodule("MyNamespace");
	// Adding functions within a namespace
	// is done with a fully qualified name
	myNamespace.def("sayHello", &MyNamespace::sayHello);
}
