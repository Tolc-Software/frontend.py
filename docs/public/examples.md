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


```


```python

# You can access static variables without instantiating class
self.assertEqual(m.WithConstructor.i, 5)

withConstructor = m.WithConstructor("Hello")
self.assertEqual(withConstructor.getS(), "Hello")
withConstructor = m.WithConstructor(s="named argument")
self.assertEqual(withConstructor.getS(), "named argument")
self.assertEqual(withConstructor.getSView(), "named argument")

withFunction = m.WithFunction()
self.assertEqual(withFunction.add(2, 5), 7)

# Test private function
with self.assertRaises(AttributeError) as error_context:
    withPrivateFunction = m.WithPrivateFunction()
    withPrivateFunction.multiply(3, 2)

self.assertEqual(len(error_context.exception.args), 1)
# print(error_context.test_case)
self.assertEqual(
    "'m.WithPrivateFunction' object has no attribute 'multiply'",
    error_context.exception.args[0],
    "Not correct exception on private functions",
)

nested = m.MyLib.Nested()
self.assertEqual(nested.divideByTwo(10), 5)

```


## Enums ##


```cpp

enum Unscoped {
	A,
	B,
	C
};

enum class Scoped {
	D,
	E,
	F
};

class EnumTest {
public:
	explicit EnumTest(Scoped s) : e(s) {};

	Scoped e;
};

Unscoped f(Unscoped s) {
	return s;
}

namespace NS {
enum class Deep {
	G,
	H,
	I
};
}


```


```python

scoped = m.Scoped.D
enumTest = m.EnumTest(scoped)
self.assertEqual(enumTest.e, scoped)

unscoped = m.Unscoped.B
u = m.f(unscoped)
self.assertEqual(u, unscoped)

deep = m.NS.Deep.I
self.assertNotEqual(deep, m.NS.Deep.H)

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

int calculate() {
	return 5;
}

int weirdArgumentsNaming(int, int i) {
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

# Without naming variables is fine
result = m.weirdArgumentsNaming(2, 5)
self.assertEqual(result, 5)

# Not possible to name any variables since they are not all known
with self.assertRaises(TypeError) as error_context:
  result = m.weirdArgumentsNaming(2, i=5)

# Getting the first letter from a string_view
result = m.firstLetter("Hello")
self.assertEqual(result, "H")

# Static functions are just normal functions in python
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

simpleMember = m.SimpleMember()
self.assertEqual(simpleMember.myString, "Hello")
simpleMember.myString = "Changed now!"
self.assertEqual(simpleMember.myString, "Changed now!")

constMember = m.ConstMember()
self.assertEqual(constMember.i, 42)

# Test changing the const member
with self.assertRaises(AttributeError) as error_context:
    constMember.i = 0

self.assertEqual(len(error_context.exception.args), 1)
self.assertEqual(
    "can't set attribute",
    error_context.exception.args[0],
    "Prohibiting changing const variables does not work!",
)

# Test getting the private member variable
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

result = m.MyLib.complexFunction()
self.assertEqual(result, 5)

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

class overloadedFunctions {
public:


};

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


# Free function overload
self.assertEqual(m.sayHello(), "Hello!")
to_me = "to me!"
self.assertEqual(m.sayHello(to_me), "Hello to me!")

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

u = m.create_unique()
self.assertEqual(u.m_hi, 5)

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

myArray = ["hi", "ho"]
withMember = m.WithMember(myArray)
self.assertEqual(withMember.getS(), myArray)

withFunction = m.WithFunction()
self.assertEqual(withFunction.sum([1, 2, 3, 4, 5]), 15)

# Test array with too many/few values
for incompatibleArray in [["too many", "too many", "too many"], ["too few"]]:
    with self.assertRaises(TypeError) as error_context:
        withMember = m.WithMember(incompatibleArray)

    self.assertEqual(len(error_context.exception.args), 1)
    self.assertTrue(
        "incompatible constructor arguments" in error_context.exception.args[0],
        "Error msg does not mention incompatible arguments: "
        + str(error_context.exception.args[0]),
    )
    self.assertTrue(
        "Invoked with: " + str(incompatibleArray)
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

myArray = ["hi", "ho"]
withMember = m.WithMember(myArray)
self.assertEqual(withMember.getS(), myArray)

withFunction = m.WithFunction()
self.assertEqual(withFunction.sum([1, 2, 3]), 6)

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

result0 = m.takingFunction(callback)
self.assertEqual(result0, 5.0)

inc_by_one = m.returnFunction(callback)
self.assertEqual(inc_by_one(5), 6)

def fiver():
  return 5

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

myArray = ["hi", "ho"]
withMember = m.WithMember(myArray)
self.assertEqual(withMember.getS(), myArray)

withFunction = m.WithFunction()
self.assertEqual(withFunction.sum([1, 2, 3]), 6)

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

myMap = {"hi": 4, "ho": 5}
c = m.MyClass(myMap)
self.assertEqual(c.getS(), myMap)

# Test map of the wrong type
for incompatiblemap in [{"key": "value"}, {5: 2}]:
    with self.assertRaises(TypeError) as error_context:
        c = m.MyClass(incompatiblemap)
        c.getValue(incompatiblemap, 5)

    self.assertEqual(len(error_context.exception.args), 1)
    self.assertTrue(
        "incompatible function arguments" in error_context.exception.args[0]
        or "incompatible constructor arguments"
        in error_context.exception.args[0],
        "Error msg does not mention incompatible arguments: \n\t"
        + str(error_context.exception.args[0]),
    )
    self.assertTrue(
        str(incompatiblemap) in error_context.exception.args[0],
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

greeting = "hello"
withMember = m.WithMember(greeting)
self.assertEqual(withMember.getS(), greeting)

withFunction = m.WithFunction()
self.assertEqual(withFunction.getNullopt(), None)

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
myArray = ["hi", 4]
for t in [myArray, tuple(myArray)]:
    withMember = m.MyClass(t)
    self.assertEqual(withMember.getS(), tuple(t))

withFunction = m.WithFunction()
self.assertEqual(withFunction.sum((1, 2)), 3)

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
myArray = ["hi", 4]
for t in [myArray, tuple(myArray)]:
    withMember = m.MyClass(t)
    self.assertEqual(withMember.getS(), tuple(t))

withFunction = m.WithFunction()
self.assertAlmostEqual(withFunction.sum((1, 2, 3.3, 2.0)), 8.3, delta=0.0001)

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

myunordered_set = {"hi", "this is a unordered_set"}
c = m.MyClass(myunordered_set)
self.assertEqual(c.getS(), myunordered_set)

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

number = 6
withNumber = m.WithMember(number)
self.assertEqual(withNumber.getS(), number)

withBool = m.WithMember(True)
self.assertEqual(withBool.getS(), True)

withFunction = m.WithFunction()
self.assertEqual(withFunction.getFive(), 5)
self.assertEqual(withFunction.getHello(), "Hello")
self.assertEqual(withFunction.getTrue(), True)

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

myArray = ["hi", "ho"]
withMember = m.WithMember(myArray)
self.assertEqual(withMember.getS(), myArray)

withFunction = m.WithFunction()
self.assertEqual(withFunction.sum([1, 2, 3]), 6)

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

