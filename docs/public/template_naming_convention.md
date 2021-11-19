When creating bindings for a templated `class` `tolc` will choose a name based on the template parameters.
For example:

```cpp
template <typename T>
class Example {
public:
T f(T type) {
	return type;
}
};

template class Example<int>;
```

The specialized `class` `Example<int>` will be available from `python` as `Example_int`:

```python
import MyLib

example = MyLib.Example_int
# Prints 5
print(example.f(5))
```

Multiple template parameters are separated with an underscore (_).

## Type to string conversions ##

| C++ type                        | Resulting name           |
|:------------------------------- |:------------------------ |
| std::array                      | array                    |
| std::deque                      | deque                    |
| std::forward\_list              | forwardlist converted    |
| std::function                   | function                 |
| std::list                       | list                     |
| std::map                        | map                      |
| std::multimap                   | multimap                 |
| std::multiset                   | multiset                 |
| std::optional                   | optional                 |
| std::pair                       | pair                     |
| std::priority\_queue            | priorityqueue            |
| std::queue                      | queue                    |
| std::set                        | set                      |
| std::stack                      | stack                    |
| std::tuple                      | tuple                    |
| std::unordered\_map             | unorderedmap             |
| std::unordered\_multimap        | unorderedmultimap        |
| std::unordered\_multiset        | unorderedmultiset        |
| std::unordered\_set             | unorderedset             |
| std::variant                    | variant                  |
| std::vector                     | vector                   |
| bool                            | bool                     |
| char                            | char                     |
| char16\_t                       | char16t                  |
| char32\_t                       | char32t                  |
| double                          | double                   |
| float                           | float                    |
| int                             | int                      |
| long double                     | longdouble               |
| long int                        | longint                  |
| long long int                   | longlongint              |
| short int                       | shortint                 |
| signed char                     | signedchar               |
| string                          | string                   |
| string\_view                    | stringview               |
| unsigned char                   | unsignedchar             |
| unsigned int                    | unsignedint              |
| unsigned long int               | unsignedlongint          |
| unsigned long long int          | unsignedlonglongint      |
| unsigned short int              | unsignedshortint         |
| wchar\_t                        | wchart                   |
