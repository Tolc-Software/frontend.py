This page shows what is automatically translated and to what. On the left is the `C++` and to the right what the corresponding interface in `python` will be. Not converted means there will be no automatic translation to a corresponding `python` object (for example `std::queue` does not automatically translate to `queue.Queue`).

## Conversion tables ##

| C++                                 | Python translation             |
|:----------------------------------- |:------------------------------ |
| Namespace                           | Module                         |
| Nested namespace                    | Submodule                      |
| Class                               | Class                          |
| Nested class                        | ???                            |
| Public Function                     | Class function                 |
| Private Function                    | Not converted                  |
| Public const member                 | Read only property             |
| Public non const member             | Read write property            |
| Private member variables            | Not converted                  |
| Free function                       | Module function                |
| Overloaded function                 | Module function*               |
| Enum                                | Enum                           |
| Scoped enum                         | Enum                           |
| Templated classes/functions         | Not converted**                |
| Specialized class templates         | Class***                       |
| Specialized function templates      | Function****                   |

\* Will try to match arguments to types provided.

\*\* No direct translation to `python`. Will not emit warning.

\*\*\* The naming convention for these classes can be found under the [Template Naming Convention page](template_naming_convention.md).

\*\*\*\* Functions with different template arguments will behave as overloaded functions.

| C++ Standard library class      | Python translation       |
|:------------------------------- |:------------------------ |
| std::array                      | array                    |
| std::deque                      | array                    |
| std::forward\_list              | Not converted            |
| std::function                   | function                 |
| std::list                       | array                    |
| std::map                        | dict                     |
| std::multimap                   | Not converted            |
| std::multiset                   | Not converted            |
| std::optional                   | (None \|\| value)        |
| std::pair                       | tuple                    |
| std::priority\_queue            | Not converted            |
| std::queue                      | Not converted            |
| std::set                        | Set                      |
| std::stack                      | Not converted            |
| std::tuple                      | tuple                    |
| std::unordered\_map             | dict                     |
| std::unordered\_multimap        | Not converted            |
| std::unordered\_multiset        | Not converted            |
| std::unordered\_set             | set                      |
| std::variant                    | value                    |
| std::vector                     | array                    |


| C++ builtin type           | Python translation       |
|:-------------------------- |:------------------------ |
| bool                       | bool                     |
| char                       | str                      |
| char16\_t                  | str                      |
| char32\_t                  | str                      |
| double                     | float                    |
| float                      | float                    |
| int                        | int                      |
| long double                | int                      |
| long int                   | int                      |
| long long int              | int                      |
| short int                  | int                      |
| signed char                | str                      |
| string                     | str                      |
| string\_view               | str                      |
| unsigned char              | str                      |
| unsigned int               | int                      |
| unsigned long int          | int                      |
| unsigned long long int     | int                      |
| unsigned short int         | int                      |
| wchar\_t                   | str                      |

