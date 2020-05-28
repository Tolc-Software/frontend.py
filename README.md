# frontend.py #

## Design goals ##

### Sane (easily understandable) defaults ###

`C++` uses many 'valid' styles when naming things. A function with a `camelCase` name such as `myFunction` would be considered fine in `C++` but unconventional in `python` where `snake_case` is more 'pythonic (see [PEP8](https://www.python.org/dev/peps/pep-0008/#function-and-variable-names) for more information). To avoid confusion when converting existing `C++` these case transformations should be opt in.

### Screws are not glued in ###

You should be able to completely change the `python` interface as long as it does not change the `C++`. This means that each logical module in `frontend.py` must have an internal interface, for things like the `C++` fully qualified names, *and* an exposed interface that is public to change.

### `frontend.py` should write readable code ###

It is highly likely that when converting a large library, you may want to tweak things. It should then feel natural to add some lines to the output without getting a heart attack scrolling through obfuscated code. This practically means:

* Format the output the same way the current code is formatted (see [.clang-format](./.clang-format)).
    * Note that the user can post format. Focus on *readability*.
* We should not output more code than what is needed.

In some cases these may need to be broken, but you should consider them a strict guideline.

## Internal code design goals ##

### Builders are smart, they are building dumb objects ###

You should see a strong connection between the `Builders` and the `PybindProxy` objects they are building. You should consider the objects as data aggregations, with builders making decisions on how they are stored the best. **The `PybindProxy` classes should not make decisions.**

## What is supported ##

### Conversion tables ###

| C++ concept                | Python translation      |
| --------------------------:|------------------------:|
| Namespace                  | Module                  |
| Nested namespace           | Submodule               |
| Class                      | Class                   |
| Nested class               | ???                     |
| Public Function            | Class function          |
| Private Function           | Not converted           |
| Public const member        | Read only property      |
| Public non const member    | Read write property     |
| Private member variables   | Not converted           |
| Free function              | Function in module      |
| Overloaded function        | ???                     |
| Enum                       | Python enum             |
| Templated functions        | ???                     |
| Specialized templates      | ???                     |
| STL containers             | Default: See table      |
| STL containers             | With mod: ???           |
|                            |                         |

| C++ STL container          | Python translation      |
| --------------------------:|------------------------:|
| std::array                 | array                   |
| std::map                   | dict                    |
| std::pair                  | tuple                   |
| std::set                   | set                     |
| std::tuple                 | tuple                   |
| std::unordered\_map        | dict                    |
| std::unordered\_set        | dict                    |
| std::vector                | array                   |
|                            |                         |

**NOTE**: The term 'Not converted' means that the code can still be called from within `C++`, but is not callable from `python`.
**NOTE**: '???' means that `frontend.py` does not support it *yet*.

## Good to know ##

### `frontend.py` assumes that `C++` code does not let users take care of memory management ###

This means that functions returning a pointer are not treated as transfers of ownership. `frontend.py` just makes `python` see a reference of the pointer and it is assumed that the `C++` code cleans up after the object.

### Frontend.py does not support `IR::Namespace`s with an empty name ###

By default the `Parser` library does emit a `IR::Namespace` corresponding to the global namespace of whatever code it was parsing. To make `frontend.py` as general as possible, there is no default 'global module' made from a nameless `IR::Namespace`. This makes `frontend.py` take a general `namespace`, and recursively creates modules from it. If a parsed `C++` header is used as an input, then it is assumed that the global `namespace` is given a reasonable name.

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

### `-std=c++14` ###

Right now the `pybind11` cmake interface library adds `C++14` as a flag. This should be circumvented somehow in the future.
