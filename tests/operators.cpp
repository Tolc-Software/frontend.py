#include "TestStage/paths.hpp"
#include "TestUtil/files.hpp"
#include "TestUtil/pybindStage.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Operators", "[operators]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <string>

class Operator {
public:
  explicit Operator(int v) : value(v) {}

  // +-*/&
  Operator operator+(int i) { return Operator(value + i); }
  Operator operator-(int i) { return Operator(value - i); }
  Operator operator*(int i) { return Operator(value * i); }
  Operator operator/(int i) { return Operator(value / i); }
  Operator operator%(int i) { return Operator(value % i); }

  // Comparisons
  bool operator==(const Operator &rhs) { return value == rhs.value; }
  bool operator!=(const Operator &rhs) { return value != rhs.value; }
  bool operator<(const Operator &rhs) { return value < rhs.value; }
  bool operator>(const Operator &rhs) { return value > rhs.value; }
  bool operator<=(const Operator &rhs) { return value <= rhs.value; }
  bool operator>=(const Operator &rhs) { return value >= rhs.value; }

  // Subscript
  Operator operator[](unsigned idx) { return Operator(static_cast<int>(idx)); }

  // Call
  int operator()(int x) { return value + x; }
  std::string operator()(std::string const& x) { return x + std::to_string(value); }

  int value;
};
)";

	auto pythonTestCode = R"(
operator = m.Operator(10)
self.assertEqual(operator.value, 10)

# Normal operators translate as expected
self.assertEqual((operator + 5).value, 15)
self.assertEqual((operator - 5).value, 5)
self.assertEqual((operator * 5).value, 50)
self.assertEqual((operator / 5).value, 2)
self.assertEqual((operator % 3).value, 1)

other = m.Operator(5)
# Comparison operators
self.assertTrue(operator != other)
self.assertTrue(operator > other)
self.assertTrue(operator >= other)

self.assertFalse(operator == other)
self.assertFalse(operator < other)
self.assertFalse(operator <= other)

# Subscript []
self.assertEqual(operator[100].value, 100)

# Call ()
self.assertEqual(operator(100), 110)
# Overloading works
self.assertEqual(operator("The inner value is: "), "The inner value is: 10")
)";

	auto errorCode = stage.runPybindTest(cppCode, pythonTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("Operators");
}
