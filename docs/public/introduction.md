# Python with Tolc #

In order for `C++` to be called from `python` there has to be an interface level. `tolc` generates this level from your already written `C++` interface.
To be as close to what an engineer would have written, `tolc` generates human readable [`pybind11`](https://github.com/pybind/pybind11).
This is then compiled to a `CPython` library that the `python` interpreter can understand.

## Using a `C++` library from `python` ##

This is a quick guide to using a `C++` library (here called `MyLib`) from `python`. We will:

1. Download and use `Tolc`
2. Use the resulting `CPython` library from `python`

The following works on all supported platforms. On all platforms you need `git` available in your `path`. Commands that should be run from a terminal starts with `$ `, while comments starts with `# `.

### Downloading and Using `Tolc` ###

Just add the following in a `CMakeLists.txt` below where the library you intend to use from `javascript` is defined:

```cmake
# Download Tolc
# Can be ["latest", "v0.2.0", ...]
set(tolc_version latest)
include(FetchContent)
FetchContent_Declare(
  tolc_entry
  URL https://github.com/Tolc-Software/tolc/releases/download/${tolc_version}/tolc-${CMAKE_HOST_SYSTEM_NAME}.tar.xz
)
FetchContent_Populate(tolc_entry)

set(tolc_DIR ${tolc_entry_SOURCE_DIR}/lib/cmake/tolc)
find_package(
  tolc
  CONFIG
  REQUIRED
)

tolc_create_bindings(
  TARGET MyLib
  LANGUAGE python
  OUTPUT python-bindings
)
```

Assuming your library is called `MyLib`, and the bindings should be generated to the directory `python-bindings`.

Now you can configure your project as normal (in Visual Studio, this is typically done automatically on save):

```shell
$ cmake -S. -Bbuild
```

And finally build it:

```shell
$ cmake --build build
```

This will produce a `CPython` library under `build/tolc` (with `MSVC` it will be under for example `build/tolc/Debug`), and you can use it as:

```shell
# With MSVC build\tolc\Debug
$ cd build/tolc
$ python
>>> import MyLib
```

If you want to see what more is supported you can take a look at [the Examples section](./examples.md).

