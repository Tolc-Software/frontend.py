# frontend.py #

## Good to know ##

### The test stage contains pybind11 examples ###

The example `pybind11` module in `tests/testStage/src/myModule.cpp` contains a buildable module with example code.

Whenever there is new ways to do things, it should be added there. This will not affect any tests.

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

### `-std=c++14` ###

Right now the `pybind11` cmake interface library adds `C++14` as a flag. This should be circumvented somehow in the future.
