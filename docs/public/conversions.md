This page shows what is automatically translated and to what.
On the left is the `C++` and to the right what the corresponding interface in `python` will be.
Not converted means there will be no automatic translation to a corresponding `python` object (for example `std::queue` does not automatically translate to `queue.Queue`).
Note that any restriction this poses only applies to the public interface of your code (e.g. your public headers).

## Conversion tables ##

| C++                                 | Python translation             |
|:----------------------------------- |:------------------------------ |
| Namespace                           | Module                         |
| Nested namespace                    | Submodule                      |
| Class                               | Class                          |
| Public function                     | Class function                 |
| Private function                    | Not converted                  |
| Static member function              | Static class function          |
| Static member variable              | Static class variable          |
| Public const member variable        | Read only property             |
| Public non const member variable    | Read write property            |
| Private member variable             | Not converted                  |
| Global variable                     | Module variable                |
| Global static variable              | Static module variable         |
| Free function                       | Module function                |
| Overloaded function                 | Module function*               |
| Enum                                | Enum                           |
| Scoped enum                         | Enum                           |
| Templated class/function            | Not converted**                |
| Specialized class template          | Class***                       |
| Specialized function template       | Function****                   |

\* Will try to match arguments to types provided.

\*\* No direct translation to `python`. Will not emit warning.

\*\*\* The naming convention for these classes can be found under the [Template Naming Convention page](template_naming_convention.md).

\*\*\*\* Functions with different template arguments will behave as overloaded functions.

| C++ Standard library class      | Python translation                                                |
|:------------------------------- |:----------------------------------------------------------------- |
| std::array                      | array                                                             |
| std::complex                    | [complex](https://docs.python.org/3/library/cmath.html)           |
| std::deque                      | array                                                             |
| std::filesystem::path           | [pathlib.Path](https://docs.python.org/3/library/pathlib.html)    |
| std::forward\_list              | Not converted                                                     |
| std::function                   | function                                                          |
| std::list                       | array                                                             |
| std::map                        | dict                                                              |
| std::multimap                   | Not converted                                                     |
| std::multiset                   | Not converted                                                     |
| std::optional                   | (None \|\| value)                                                 |
| std::pair                       | tuple                                                             |
| std::priority\_queue            | Not converted                                                     |
| std::queue                      | Not converted                                                     |
| std::set                        | set                                                               |
| std::shared_ptr                 | value                                                             |
| std::stack                      | Not converted                                                     |
| std::tuple                      | tuple                                                             |
| std::unique_ptr                 | value*                                                            |
| std::shared_ptr                 | value**                                                           |
| std::unordered\_map             | dict                                                              |
| std::unordered\_multimap        | Not converted                                                     |
| std::unordered\_multiset        | Not converted                                                     |
| std::unordered\_set             | set                                                               |
| std::valarray                   | array                                                             |
| std::variant                    | value                                                             |
| std::vector                     | array                                                             |

\* Note that due to how python works internally, function argument of type `std::unique_ptr` is not allowed. For more info see [the pybind11 documentation on smart pointers](https://pybind11.readthedocs.io/en/stable/advanced/smart_ptrs.html).

\*\* When a user defined struct/class is managed by `std::shared_ptr`, it is marked as such in the resulting pybind11. For more info see [the pybind11 documentation on smart pointers](https://pybind11.readthedocs.io/en/stable/advanced/smart_ptrs.html).

| C++ builtin type           | Python translation       |
|:-------------------------- |:------------------------ |
| bool                       | bool                     |
| char                       | str                      |
| char16\_t                  | str                      |
| char32\_t                  | str                      |
| double                     | float                    |
| float                      | float                    |
| int                        | int                      |
| int8_t                     | int                      |
| int16_t                    | int                      |
| int32_t                    | int                      |
| int64_t                    | int                      |
| long double                | int                      |
| long int                   | int                      |
| long long int              | int                      |
| short int                  | int                      |
| signed char                | str                      |
| string                     | str                      |
| string\_view               | str                      |
| uint8_t                    | int                      |
| uint16_t                   | int                      |
| uint32_t                   | int                      |
| uint64_t                   | int                      |
| unsigned char              | str                      |
| unsigned int               | int                      |
| unsigned long int          | int                      |
| unsigned long long int     | int                      |
| unsigned short int         | int                      |
| wchar\_t                   | str                      |

