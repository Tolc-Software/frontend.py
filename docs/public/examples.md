# Examples #

Each example is taken from the test suite for `Tolc` and, given that you use the latest version, you can expect them all to work.

Each `C++` library named `MyLib` exports as a `python module` called `MyLib`, in every test the module name is simply `m` for brevity. All tests use the `python` builtin [`unittest`](https://docs.python.org/3/library/unittest.html) library. The examples that follow contains a bit of `C++` code, and the respective `python` code using it. Each `python` example is wrapped in the following boilerplate that is removed to make the examples more readable:

```python
import unittest
import m

class TestMyLib(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        pass

    def test_m(self):
        # The actual python example body goes here
        self.assertEqual(m.sayTen(), 10)

if __name__ == "__main__":
    unittest.main()
```


## Classes ##


```cpp

#include <string>
#include <string_view>

class WithConstructor {
public:
	explicit WithConstructor(std::string s) : m_s(s) {}

	static int const i = 5;

	// This class has a readwrite variable
	int readwrite = 10;

	std::string getS() { return m_s; }
	std::string_view getSView() { return m_s; }

private:
	std::string m_s;
};

class WithFunction {
public:
	int add(int i, int j) {
		return i + j;
	}
};

class WithPrivateFunction {
	double multiply(double i, double j) {
		return i * j;
	}
};

namespace MyLib {

	class Nested {
	public:
		double divideByTwo(double d) {
			return d / 2;
		}
	};
}

/** Documentation carries over */
struct Documentation {};

/***********************************************************
* JavaDoc Style
* is
* boxy
**********************************************************/
struct JavaDoc {};

```


```python

# You can access static variables without instantiating class
self.assertEqual(m.WithConstructor.i, 5)

# Creating classes via their constructor
with_constructor = m.WithConstructor("Hello")
self.assertEqual(with_constructor.getS(), "Hello")

# Documentation for variables carries over aswell
self.assertIn("This class has a readwrite variable", m.WithConstructor.readwrite.__doc__)

# Named arguments in constructors
with_constructor = m.WithConstructor(s="named argument")
self.assertEqual(with_constructor.getS(), "named argument")
self.assertEqual(with_constructor.getSView(), "named argument")

# Member functions are available after construction
with_function = m.WithFunction()
self.assertEqual(with_function.add(2, 5), 7)

# Private functions have no bindings
with self.assertRaises(AttributeError) as error_context:
    with_private_function = m.WithPrivateFunction()
    with_private_function.multiply(3, 2)

self.assertEqual(len(error_context.exception.args), 1)
# print(error_context.test_case)
self.assertEqual(
    "'m.WithPrivateFunction' object has no attribute 'multiply'",
    error_context.exception.args[0],
    "Not correct exception on private functions",
)

# Classes under namespaces are available under the corresponding submodule
nested = m.MyLib.Nested()
self.assertEqual(nested.divideByTwo(10), 5)

# Different styles of documentation on classes carries over
self.assertIn("Documentation carries over", m.Documentation.__doc__)
self.assertIn("JavaDoc Style", m.JavaDoc.__doc__)


```


## Documentation Styles ##


```cpp

// One line comment
class OneLiner {};

/** Single multi line comment */
class SingleMulti {};

/**
* Multi
* line
* comment
*/
class Multi {};

/**
Bare multi
Another line
*/
class BareMulti {};

/*!
* Qt style
*/
class QtStyle {};

/*****************************
* JavaDoc Style
* is
* boxy
****************************/
class JavaDoc {};

///
/// Triplets is a commitment
///
class Triplets {};

//!
//! This is one of the doxy styles
//!
class DoxyBang {};

```


```python

# These types of documentations are supported for:
#   Classes
#   Member variables
#   Enums
#   Functions

self.assertIn("One line comment", m.OneLiner.__doc__)

self.assertIn("Single multi line", m.SingleMulti.__doc__)

self.assertIn("Multi", m.Multi.__doc__)

self.assertIn("Bare multi", m.BareMulti.__doc__)

self.assertIn("Qt style", m.QtStyle.__doc__)

self.assertIn("JavaDoc Style", m.JavaDoc.__doc__)

self.assertIn("Triplets", m.Triplets.__doc__)

self.assertIn("one of the doxy styles", m.DoxyBang.__doc__)

```


## Enums ##


```cpp

enum Unscoped {
	Under,
	Uboat,
};

enum class Scoped {
	Sacred,
	Snail,
};

class EnumTest {
public:
	explicit EnumTest(Scoped _s) : s(_s) {};

	Scoped s;
};

Unscoped f(Unscoped u) {
	return u;
}

namespace NS {
	// Documentation describing the enum
	enum class Deep {
		Double,
		Down,
	};
}


```


```python

# C++11 enums work
scoped = m.Scoped.Snail
enumTest = m.EnumTest(scoped)
self.assertEqual(enumTest.s, scoped)

# Aswell as legacy enums
unscoped = m.Unscoped.Uboat
u = m.f(unscoped)
self.assertEqual(u, unscoped)

# Enums under namespaces are available under the corresponding submodule
deep = m.NS.Deep.Down
self.assertNotEqual(deep, m.NS.Deep.Double)

# Documentation carries over from C++
self.assertIn("Documentation describing the enum", m.NS.Deep.__doc__)

```


## Functions ##


```cpp

#include <fstream>
#include <string>

void sayHello() {
	std::ofstream f("hello.txt");
	f << "Hello!";
	f.close();
}

void addYourOwn(std::string content) {
	std::ofstream f("hello.txt");
	f << content;
	f.close();
}

/**
* Documentation carries over
*/
int calculate() {
	return 5;
}

// Different documentation styles are supported
int missingArgumentsNaming(int, int i) {
	return i;
}

char firstLetter(std::string_view s) {
	return s[0];
}

int static getZero() {
	return 0;
}

```


```python

m.sayHello()
with open("hello.txt", "r") as f:
    self.assertEqual(f.readline(), "Hello!")

content = "This is from python!"
m.addYourOwn(content)
with open("hello.txt", "r") as f:
    self.assertEqual(f.readline(), content)

result = m.calculate()
self.assertEqual(result, 5)
self.assertIn("Documentation carries over", m.calculate.__doc__)

# Without naming variables is fine
result = m.missingArgumentsNaming(2, 5)
self.assertEqual(result, 5)
self.assertIn("Different documentation styles are supported", \
  m.missingArgumentsNaming.__doc__)

# Not possible to name any variables unless they are all known
with self.assertRaises(TypeError) as error_context:
  result = m.missingArgumentsNaming(2, i=5)

# std::string_view works fine
result = m.firstLetter("Hello")
self.assertEqual(result, "H")

# Static functions are just normal module functions in python
self.assertEqual(m.getZero(), 0)

```


## Global Variables ##


```cpp

#include <string>

static int i = 0;
namespace Nested {
	int i = 0;
	std::string s = "Hello world";
}

```


```python

# Starts at 0 and can be changed
self.assertEqual(m.i, 0)
m.i = 5
self.assertEqual(m.i, 5)

# Nested with the same name
self.assertEqual(m.Nested.i, 0)

# More complex variables are available aswell
self.assertEqual(m.Nested.s, "Hello world")

```


## Member Variables ##


```cpp

#include <string>

class SimpleMember {
public:
	explicit SimpleMember() : myString("Hello") {}

	std::string myString;
};

class ConstMember {
public:
	const int i = 42;
};

class PrivateMember {
public:
	explicit PrivateMember(std::string s) : myString(s) {}

private:
	std::string myString;
};

namespace MyLib {

	class Nested {
	public:
		double d = 4.3;
	};
}


```


```python

# Mutable member variables can be changed
simpleMember = m.SimpleMember()
self.assertEqual(simpleMember.myString, "Hello")
simpleMember.myString = "Changed now!"
self.assertEqual(simpleMember.myString, "Changed now!")

constMember = m.ConstMember()
self.assertEqual(constMember.i, 42)

# Const member variables cannot be changed
with self.assertRaises(AttributeError) as error_context:
    constMember.i = 0

self.assertEqual(len(error_context.exception.args), 1)
self.assertEqual(
    "can't set attribute",
    error_context.exception.args[0],
    "Prohibiting changing const variables does not work!",
)

# Private member variables are not available
with self.assertRaises(AttributeError) as error_context:
    privateMember = m.PrivateMember("Hello")
    print(privateMember.myString)

self.assertEqual(len(error_context.exception.args), 1)
self.assertEqual(
    "'m.PrivateMember' object has no attribute 'myString'",
    error_context.exception.args[0],
    "Prohibiting changing const variables does not work!",
)

nested = m.MyLib.Nested()
self.assertEqual(nested.d, 4.3)

```


## Namespaces ##


```cpp

#include <string>

namespace MyLib {

int complexFunction() {
	return 5;
}

	namespace We {
		namespace Are {
			namespace Going {
				namespace Pretty {
					namespace Deep {
						std::string meaningOfLife() {
							return "42";
						}
					}
				}
			}
		}
	}
}


```


```python

# Namespaces corresponds to submodules
result = m.MyLib.complexFunction()
self.assertEqual(result, 5)

# You can nest namespaces arbitrarily deep
lifeProTips = m.MyLib.We.Are.Going.Pretty.Deep.meaningOfLife()
self.assertEqual(lifeProTips, "42")

```


## Overloaded Functions ##


```cpp

#include <string>

// Overloaded free functions
std::string sayHello() {
	return "Hello!";
}
std::string sayHello(std::string to) {
	return std::string("Hello ") + to;
}

std::string safety() { return "Safe!"; }

class Overload {
public:
	// Overloaded constructor
	Overload() {};
	Overload(std::string) {};

	// Overloaded class functions
	std::string getStuff() { return "Stuff"; }
	std::string getStuff(std::string customStuff) { return customStuff; }

	std::string safety() { return "Safe!"; }
};


```


```python

# Overloaded functions work the same as in C++
# Free function overload
self.assertEqual(m.sayHello(), "Hello!")
self.assertEqual(m.sayHello("to me!"), "Hello to me!")

# Class function overload
overload = m.Overload()
overload = m.Overload("Overloaded!")
self.assertEqual(overload.getStuff(), "Stuff")
self.assertEqual(overload.getStuff("My stuff"), "My stuff")

self.assertEqual(overload.safety(), "Safe!")
self.assertEqual(overload.safety(), m.safety())

```


## Smart Pointers ##


```cpp

#include <memory>

struct Example {
	int m_hi = 5;
};

struct ExampleShared {
	int m_hi = 10;
};

std::unique_ptr<Example> create_unique() {
	return std::make_unique<Example>();
}

std::shared_ptr<ExampleShared> create_shared() {
	return std::make_shared<ExampleShared>();
}

```


```python

# std::unique_ptr acts as a normal value
# Note that passing a std::unique_ptr as an argument gives an error
#   See https://pybind11.readthedocs.io/en/stable/advanced/smart_ptrs.html
u = m.create_unique()
self.assertEqual(u.m_hi, 5)

# std::shared_ptr acts as a normal value
s = m.create_shared()
self.assertEqual(s.m_hi, 10)

```


## std::array ##


```cpp

#include <array>
#include <string>

class WithMember {
public:
	explicit WithMember(std::array<std::string, 2> s) : m_s(s) {}

	std::array<std::string, 2> getS() { return m_s; }

private:
	std::array<std::string, 2> m_s;
};

class WithFunction {
public:
	int sum(std::array<int, 5> v) {
		int s = 0;
		for (auto i : v) {
			s += i;
		}
		return s;
	}
};


```


```python

# std::array translates to a normal array in python
my_array = ["hi", "ho"]
with_member = m.WithMember(my_array)
self.assertEqual(with_member.getS(), my_array)

with_function = m.WithFunction()
self.assertEqual(with_function.sum([1, 2, 3, 4, 5]), 15)

# It still corresponds to a fixed amount of elements
for incompatible_array in [["too many", "too many", "too many"], ["too few"]]:
    with self.assertRaises(TypeError) as error_context:
        with_member = m.WithMember(incompatible_array)

    self.assertEqual(len(error_context.exception.args), 1)
    self.assertTrue(
        "incompatible constructor arguments" in error_context.exception.args[0],
        "Error msg does not mention incompatible arguments: "
        + str(error_context.exception.args[0]),
    )
    self.assertTrue(
        "Invoked with: " + str(incompatible_array)
        in error_context.exception.args[0],
        "Error msg does not mention the given arguments: " + str(error_context.exception.args[0]),
    )

```


## std::complex ##


```cpp

#include <complex>

using namespace std::complex_literals;

std::complex<int> i() {
	return 5;
}

std::complex<double> d() {
	return 1. + 2i;
}

std::complex<float> f() {
	return 0.f + 5if;
}

std::complex<double> r(std::complex<double> d) {
	return d;
}

```


```python

# std::complex translates to a complex in python
i = m.i()
self.assertEqual(i.real, 5)
self.assertEqual(i.imag, 0)

d = m.d()
self.assertEqual(d.real, 1)
self.assertEqual(d.imag, 2)

f = m.f()
self.assertEqual(f.real, 0)
self.assertEqual(f.imag, 5)

# Using python builtin complex class
r = m.r(complex(1, 2))
self.assertEqual(r.real, 1)
self.assertEqual(r.imag, 2)

```


## std::deque ##


```cpp

#include <string>
#include <deque>

class WithMember {
public:
	explicit WithMember(std::deque<std::string> s) : m_s(s) {}

	std::deque<std::string> getS() { return m_s; }

private:
	std::deque<std::string> m_s;
};

class WithFunction {
public:
	int sum(std::deque<int> v) {
		int s = 0;
		for (auto i : v) {
			s += i;
		}
		return s;
	}
};


```


```python

# std::deque translates to a normal array in python
my_array = ["hi", "ho"]
with_member = m.WithMember(my_array)
self.assertEqual(with_member.getS(), my_array)

with_function = m.WithFunction()
self.assertEqual(with_function.sum([1, 2, 3]), 6)

```


## std::filesystem::path ##


```cpp

#include <filesystem>
#include <vector>

std::filesystem::path takingPath(std::filesystem::path p) {
	return p;
}

std::string toString(std::filesystem::path p) {
	return p.string();
}

std::filesystem::path joinPaths(std::vector<std::filesystem::path> arrayToSum) {
	std::filesystem::path sum;
	for (auto f : arrayToSum) {
		sum /= f;
	}
	return sum;
}

```


```python

# std::filesystem::path translates to pathlib.Path in python
from pathlib import Path

p0 = Path("Hello")
result0 = m.takingPath(p0)
self.assertEqual(result0, p0)

p1 = Path("Something")
toString = m.toString(p1)
self.assertEqual(toString, p1.name)

result1 = m.joinPaths([p0, p1])
self.assertEqual(result1, p0 / p1)

```


## std::function ##


```cpp

#include <functional>
#include <vector>

double takingFunction(std::function<double(int)> callMe) {
	return callMe(5);
}

std::function<int(int)> returnFunction(const std::function<int(int)> &f) {
	return [f](int i) {
		return f(i) + 1;
	};
}

int accumulateArrayOfFunctions(std::vector<std::function<int()>> arrayToSum) {
	int sum = 0;
	for (auto f : arrayToSum) {
		sum += f();
	}
	return sum;
}

```


```python

def callback(i):
  return i

# You can send a python function as a C++ callback
result0 = m.takingFunction(callback)
self.assertEqual(result0, 5.0)

# Or in the other direction
inc_by_one = m.returnFunction(callback)
self.assertEqual(inc_by_one(5), 6)

def fiver():
  return 5

# Or a vector of functions
result1 = m.accumulateArrayOfFunctions([fiver, fiver])
self.assertEqual(result1, 10)

```


## std::list ##


```cpp

#include <string>
#include <list>

class WithMember {
public:
	explicit WithMember(std::list<std::string> s) : m_s(s) {}

	std::list<std::string> getS() { return m_s; }

private:
	std::list<std::string> m_s;
};

class WithFunction {
public:
	int sum(std::list<int> v) {
		int s = 0;
		for (auto i : v) {
			s += i;
		}
		return s;
	}
};


```


```python

# std::list translates to a normal array in python
my_array = ["hi", "ho"]
with_member = m.WithMember(my_array)
self.assertEqual(with_member.getS(), my_array)

with_function = m.WithFunction()
self.assertEqual(with_function.sum([1, 2, 3]), 6)

```


## std::map ##


```cpp

#include <map>
#include <string>

class MyClass {
public:
	explicit MyClass(std::map<std::string, int> s) : m_s(s) {}

	std::map<std::string, int> getS() { return m_s; }

	std::string getValue(std::map<int, std::string> const& m, int key) {
		auto it = m.find(key);
		if (it != m.end()) {
			return it->second;
		}
		return "";
	}

private:
	std::map<std::string, int> m_s;
};


```


```python

# std::map translates to a normal dictionary in python
my_map = {"hi": 4, "ho": 5}
c = m.MyClass(my_map)
self.assertEqual(c.getS(), my_map)

# The maps are typed on the C++ side
for incopatible_map in [{"key": "value"}, {5: 2}]:
    with self.assertRaises(TypeError) as error_context:
        c = m.MyClass(incopatible_map)
        c.getValue(incopatible_map, 5)

    self.assertEqual(len(error_context.exception.args), 1)
    self.assertTrue(
        "incompatible function arguments" in error_context.exception.args[0]
        or "incompatible constructor arguments"
        in error_context.exception.args[0],
        "Error msg does not mention incompatible arguments: \n\t"
        + str(error_context.exception.args[0]),
    )
    self.assertTrue(
        str(incopatible_map) in error_context.exception.args[0],
        "Error msg does not mention the given arguments: \n\t"
        + str(error_context.exception.args[0]),
    )

```


## std::optional ##


```cpp

#include <optional>
#include <string>

class WithMember {
public:
	explicit WithMember(std::optional<std::string> s) : m_s(s) {}

	std::optional<std::string> getS() { return m_s; }

private:
	std::optional<std::string> m_s;
};

class WithFunction {
public:
	std::optional<int> getNullopt() {
		return std::nullopt;
	}
};


```


```python

# std::optional is either the value or None in python
greeting = "hello"
with_member = m.WithMember(greeting)
self.assertEqual(with_member.getS(), greeting)

with_function = m.WithFunction()
self.assertEqual(with_function.getNullopt(), None)

```


## std::pair ##


```cpp

#include <string>

class MyClass {
public:
	explicit MyClass(std::pair<std::string, int> s) : m_s(s) {}

	std::pair<std::string, int> getS() { return m_s; }

private:
	std::pair<std::string, int> m_s;
};

class WithFunction {
public:
	int sum(std::pair<int, int> v) {
		return v.first + v.second;
	}
};


```


```python

# Converts to a tuple, but is convertible from array aswell
my_array = ["hi", 4]
for t in [my_array, tuple(my_array)]:
    with_member = m.MyClass(t)
    self.assertEqual(with_member.getS(), tuple(t))

with_function = m.WithFunction()
self.assertEqual(with_function.sum((1, 2)), 3)

```


## std::set ##


```cpp

#include <set>
#include <string>

class MyClass {
public:
	explicit MyClass(std::set<std::string> s) : m_s(s) {}

	std::set<std::string> getS() { return m_s; }

	int getValue(std::set<int> const& m, int key) {
		auto it = m.find(key);
		if (it != m.end()) {
			return *it;
		}
		return -1;
	}

private:
	std::set<std::string> m_s;
};


```


```python

# std::set translates to a normal array or a set in python
mySet = {"hi", "this is a set"}
c = m.MyClass(mySet)
self.assertEqual(c.getS(), mySet)

self.assertEqual(c.getValue({1, 2, 3}, 3), 3)
self.assertEqual(c.getValue({1, 2, 3}, 4), -1)

# Test set of the wrong type
for incompatibleset in [{"key": "value"}, (5, 2)]:
    with self.assertRaises(TypeError) as error_context:
        c = m.MyClass(incompatibleset)
        c.getValue(incompatibleset, 5)

self.assertEqual(len(error_context.exception.args), 1)
self.assertTrue(
    "incompatible function arguments" in error_context.exception.args[0]
    or "incompatible constructor arguments" in error_context.exception.args[0],
    "Error msg does not mention incompatible arguments: \n\t"
    + str(error_context.exception.args[0]),
)
self.assertTrue(
    str(incompatibleset) in error_context.exception.args[0],
    "Error msg does not mention the given arguments: \n\t"
    + str(error_context.exception.args[0]),
)



```


## std::tuple ##


```cpp

#include <string>
#include <tuple>

class MyClass {
public:
	explicit MyClass(std::tuple<std::string, int> s) : m_s(s) {}

	std::tuple<std::string, int> getS() { return m_s; }

	std::tuple<std::string, int> m_s;
};

class WithFunction {
public:
	double sum(std::tuple<int, int, float, double> t) {
		return std::get<0>(t)
			   + std::get<1>(t)
			   + std::get<2>(t)
			   + std::get<3>(t);
	}
};


```


```python

# Converts to a tuple, but is convertible from array aswell
my_array = ["hi", 4]
for t in [my_array, tuple(my_array)]:
    with_member = m.MyClass(t)
    self.assertEqual(with_member.getS(), tuple(t))

with_function = m.WithFunction()
self.assertAlmostEqual(with_function.sum((1, 2, 3.3, 2.0)), 8.3, delta=0.0001)

```


## std::unordered_map ##


```cpp

#include <string>
#include <unordered_map>

class MyClass {
public:
	explicit MyClass(std::unordered_map<std::string, int> s) : m_s(s) {}

	std::unordered_map<std::string, int> getS() { return m_s; }

	std::string getValue(std::unordered_map<int, std::string> const& m, int key) {
		auto it = m.find(key);
		if (it != m.end()) {
			return it->second;
		}
		return "";
	}

private:
	std::unordered_map<std::string, int> m_s;
};


```


```python

# std::unordered_map translates to a normal dictionary in python
myunordered_map = {"hi": 4, "ho": 5}
c = m.MyClass(myunordered_map)
self.assertEqual(c.getS(), myunordered_map)

# Test unordered_map of the wrong type
for incompatible_map in [{"key": "value"}, {5: 2}]:
    with self.assertRaises(TypeError) as error_context:
        c = m.MyClass(incompatible_map)
        c.getValue(incompatible_map, 5)

    self.assertEqual(len(error_context.exception.args), 1)
    self.assertTrue(
        "incompatible function arguments" in error_context.exception.args[0]
        or "incompatible constructor arguments"
        in error_context.exception.args[0],
        "Error msg does not mention incompatible arguments: \n\t"
        + str(error_context.exception.args[0]),
    )
    self.assertTrue(
        str(incompatible_map) in error_context.exception.args[0],
        "Error msg does not mention the given arguments: \n\t"
        + str(error_context.exception.args[0]),
    )


```


## std::unordered_set ##


```cpp

#include <string>
#include <unordered_set>

class MyClass {
public:
	explicit MyClass(std::unordered_set<std::string> s) : m_s(s) {}

	std::unordered_set<std::string> getS() { return m_s; }

	int getValue(std::unordered_set<int> const& m, int key) {
		auto it = m.find(key);
		if (it != m.end()) {
			return *it;
		}
		return -1;
	}

private:
	std::unordered_set<std::string> m_s;
};


```


```python

# std::unordered_set translates to a normal array or a set in python
my_unordered_set = {"hi", "this is a unordered_set"}
c = m.MyClass(my_unordered_set)
self.assertEqual(c.getS(), my_unordered_set)

self.assertEqual(c.getValue({1, 2, 3}, 3), 3)
self.assertEqual(c.getValue({1, 2, 3}, 4), -1)

# Test unordered_set of the wrong type
for incompatible_set in [{"key": "value"}, (5, 2)]:
    with self.assertRaises(TypeError) as error_context:
        c = m.MyClass(incompatible_set)
        c.getValue(incompatible_set, 5)

self.assertEqual(len(error_context.exception.args), 1)
self.assertTrue(
    "incompatible function arguments" in error_context.exception.args[0]
    or "incompatible constructor arguments" in error_context.exception.args[0],
    "Error msg does not mention incompatible arguments: \n\t"
    + str(error_context.exception.args[0]),
)
self.assertTrue(
    str(incompatible_set) in error_context.exception.args[0],
    "Error msg does not mention the given arguments: \n\t"
    + str(error_context.exception.args[0]),
)

```


## std::valarray ##


```cpp

#include <valarray>

std::valarray<int> get() {
	return {1, 2, 3};
}

```


```python

v = m.get()
self.assertEqual(v, [1, 2, 3])

```


## std::variant ##


```cpp

#include <string>
#include <variant>

class WithMember {
public:
	explicit WithMember(std::variant<int, bool> s) : m_s(s) {}

	std::variant<int, bool> getS() { return m_s; }

private:
	std::variant<int, bool> m_s;
};

class WithFunction {
public:
	std::variant<int, std::string, bool> getFive() {
		return 5;
	}

	std::variant<int, std::string, bool> getHello() {
		return std::string("Hello");
	}

	std::variant<int, std::string, bool> getTrue() {
		return true;
	}
};


```


```python

# std::variant translates to one of the values in python
number = 6
withNumber = m.WithMember(number)
self.assertEqual(withNumber.getS(), number)

withBool = m.WithMember(True)
self.assertEqual(withBool.getS(), True)

with_function = m.WithFunction()
self.assertEqual(with_function.getFive(), 5)
self.assertEqual(with_function.getHello(), "Hello")
self.assertEqual(with_function.getTrue(), True)

```


## std::vector ##


```cpp

#include <string>
#include <vector>

class WithMember {
public:
	explicit WithMember(std::vector<std::string> s) : m_s(s) {}

	std::vector<std::string> getS() { return m_s; }

private:
	std::vector<std::string> m_s;
};

class WithFunction {
public:
	int sum(std::vector<int> v) {
		int s = 0;
		for (auto i : v) {
			s += i;
		}
		return s;
	}
};


```


```python

# std::vector translates to a normal array in python
my_array = ["hi", "ho"]
with_member = m.WithMember(my_array)
self.assertEqual(with_member.getS(), my_array)

with_function = m.WithFunction()
self.assertEqual(with_function.sum([1, 2, 3]), 6)

```


## Templates ##


```cpp

#include <array>
#include <map>
#include <string>
#include <vector>

template <typename T>
T getSomething(T something) {
  return something;
}

template std::string getSomething(std::string something);
template int getSomething(int);
template std::vector<std::string> getSomething(std::vector<std::string>);

template <typename T>
class MyClass {
public:
T myFun(T type) {
	return type;
}
};

template class MyClass<int>;
template class MyClass<std::map<char, std::vector<int>>>;
template class MyClass<std::array<int, 3>>;

```


```python

# getSomething<std::string>
hi = m.getSomething("hi")
self.assertEqual(hi, "hi")

# getSomething<int>
five = m.getSomething(5)
self.assertEqual(five, 5)

# getSomething<std::vector<std::string>>
l = m.getSomething(["hi"])
self.assertEqual(l, ["hi"])

# MyClass<int>
my_class_int = m.MyClass_int()
self.assertEqual(my_class_int.myFun(25), 25)

# MyClass<std::map<char, std::vector<int>>>
my_class_map = m.MyClass_map_char_vector_int()
self.assertEqual(my_class_map.myFun({'h': [1]}), {'h': [1]})

# MyClass<std::array<int, 3>>
my_class_array = m.MyClass_array_int_3()
self.assertEqual(my_class_array.myFun([1, 2, 3]), [1, 2, 3])

```

