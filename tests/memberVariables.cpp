#include "Frontend/Python/frontend.hpp"
#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"
#include "TestUtil/runPybindTest.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Classes", "[namespaces]") {
	std::string moduleName = "MyLib";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
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

)";

	auto pythonTestCode = fmt::format(R"(
simpleMember = {moduleName}.SimpleMember()
self.assertEqual(simpleMember.myString, "Hello")
simpleMember.myString = "Changed now!"
self.assertEqual(simpleMember.myString, "Changed now!")

constMember = {moduleName}.ConstMember()
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
    privateMember = {moduleName}.PrivateMember("Hello")
    print(privateMember.myString)

self.assertEqual(len(error_context.exception.args), 1)
self.assertEqual(
    "'{moduleName}.PrivateMember' object has no attribute 'myString'",
    error_context.exception.args[0],
    "Prohibiting changing const variables does not work!",
)

nested = {moduleName}.MyLib.Nested()
self.assertEqual(nested.d, 4.3)
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode =
	    TestUtil::runPybindTest(stage, cppCode, pythonTestCode, moduleName);
	REQUIRE(errorCode == 0);
}
