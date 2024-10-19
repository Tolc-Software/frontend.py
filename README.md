![Ubuntu](https://github.com/Tolc-Software/frontend.py/workflows/Ubuntu/badge.svg) ![MacOS](https://github.com/Tolc-Software/frontend.py/workflows/MacOS/badge.svg) ![Windows](https://github.com/Tolc-Software/frontend.py/actions/workflows/windows.yml/badge.svg) ![Windows-debug-deploy](https://github.com/Tolc-Software/frontend.py/actions/workflows/windows-debug-deploy.yml/badge.svg)

# frontend.py #

`frontend.py` is a library that takes an AST defined by [`IR::IR`](https://github.com/Tolc-Software/IntermediateRepresentation) and writes [`pybind11`](https://github.com/pybind/pybind11) code. The idea is to be able to use the C++ code that corresponds to the AST from `python` via the generated `pybind11`.

`frontend.py` is a part of the [`Tolc`](https://github.com/Tolc-Software/tolc) project and is used to support creating `CPython` bindings via [`pybind11`](https://github.com/pybind/pybind11).

## Building ##

`frontend.py` can be built on Linux, MacOS, and Windows. See the [CI pipelines for more platform specific information.](./.github/workflows/).

Here is a general overview:

Requirements:

* CMake
* python3
* conan
* clang

Configure the project:

```shell
$ cmake -S. -Bbuild -G Ninja -DCMAKE_CXX_COMPILER="clang++" -DCMAKE_C_COMPILER="clang" -DCMAKE_BUILD_TYPE="Debug" -DCMAKE_PROJECT_TOP_LEVEL_INCLUDES=cmake/conan_provider.cmake -DENABLE_TESTING=ON -DENABLE_PACKAGING=ON
```

Build:

```shell
$ cmake --build build
```

Test with `ctest`:

```shell
$ cd build
$ ctest --output-on-failure
```

Install with `CPack`:

```shell
$ cpack -G TGZ --config build/CPackConfig.cmake
```


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

In some unique cases these rules may need to be broken, but you should consider them strict guidelines.

