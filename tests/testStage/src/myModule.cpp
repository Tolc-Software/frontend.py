#include "test.hpp"
#include <pybind11/pybind11.h>
// This headear allows automatic conversion
// of many stl containers such as vector
#include <pybind11/stl.h>
#include <vector>

namespace py = pybind11;

int add(int i, int j) {
	return i + j;
}

std::vector<int> getValues(int i, int j) {
	return {i, j};
}

enum class WorldlyThings { Money, Status, MeaningOfLife };

enum ThingsIWant { BetterChair, Abs, WarmSocks };

PYBIND11_MODULE(myModule, myModule) {
	// NOTE:    ^-- This name needs to be the same as the CMake target output name

	// optional module docstring
	myModule.doc() = "pybind11 example plugin";

	// Adding a simple function with optional help text
	// and named variables (allows python to use add(i = 5, j = 3))
	myModule.def("add",
	             &add,
	             "A function which adds two numbers",
	             py::arg("i"),
	             py::arg("j"));

	myModule.def("getValues",
	             &getValues,
	             "Get the input as an array",
	             py::arg("i"),
	             py::arg("j"));

	// NOTE: Scoped enums have 'py::arithmetic()'
	//       This adds additional convenience functions such as
	//       comparisons, and, or etc.
	py::enum_<WorldlyThings>(myModule, "WorldlyThings", py::arithmetic())
	    .value("Money", WorldlyThings::Money)
	    .value("Status", WorldlyThings::Status)
	    .value("MeaningOfLife", WorldlyThings::MeaningOfLife);

	// NOTE: Unscoped enums have 'export_values()'
	//       This exports them to the parent namespace (here myModule)
	//       This is more closely to how they are used in C++
	py::enum_<ThingsIWant>(myModule, "ThingsIWant")
	    .value("BetterChair", ThingsIWant::BetterChair)
	    .value("Abs", ThingsIWant::Abs)
	    .value("WarmSocks", ThingsIWant::WarmSocks)
	    .export_values();

	// Adding a submodule within this module
	auto myNamespace = myModule.def_submodule("MyNamespace");
	// Adding functions within a namespace
	// is done with a fully qualified name
	myNamespace.def("sayHello", &MyNamespace::sayHello);
}
