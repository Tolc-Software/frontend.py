# Architecture #

### Builders are smart, they are building dumb objects ###

You should see a strong connection between the `Builders` and the `PybindProxy` objects they are building. You should consider the objects as data aggregations, with builders making decisions on how they are stored the best. **The `PybindProxy` classes should not make decisions.**

## Good to know ##

### `frontend.py` assumes that `C++` code does not let users take care of memory management ###

This means that functions returning a pointer are not treated as transfers of ownership. `frontend.py` just makes `python` see a reference of the pointer and it is assumed that the `C++` code cleans up after the object.

### The test stage contains `pybind11` examples ###

The example `pybind11` module in `tests/testStage/src/myModule.cpp` contains a buildable module with example code.

Whenever there are discoveries of new ways to do things in `pybind11`, they should be added there. This will not affect any tests.

### `ImportError: dynamic module does not define init function (PyInit_\*)` ###

The module name must be **exactly** the same as the library name.

Example:

```cmake
# CMakeLists.txt
pybind11_add_module(myModule src/module.cpp SYSTEM)
```

```cpp
// src/module.cpp
#include <pybind11/pybind11.h>

namespace py = pybind11;

int add(int i, int j) {
	return i + j;
}

PYBIND11_MODULE(myModule, m) {
//              ^ Must be the same as in CMakeLists
//
	m.def("add", &add, "A function which adds two numbers");
}
```

Otherwise you will get:

```python
>>> import myModule
ImportError: dynamic module does not define init function (PyInit_whateverYouCalledYourModule)
```
