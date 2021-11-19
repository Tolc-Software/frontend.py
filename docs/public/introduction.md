# Python with Tolc #

In order for `C++` to be called from `python` there has to be an interface level. `tolc` generates this level from your already written `C++` interface.
To be as close to what an engineer would have written, `tolc` generates human readable [`pybind11`](https://github.com/pybind/pybind11).
This is then compiled to a `CPython` library that the `python` interpreter can understand.
