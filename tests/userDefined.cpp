#include "Frontend/Python/frontend.hpp"
#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"
#include "TestUtil/runPybindTest.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("User defined classes", "[userDefined]") {
	std::string moduleName = "MyLib";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <string>

class MyClass {
public:
	explicit MyClass(std::string s) : m_s(s) {}

	std::string* getS() { return &m_s; }

private:
	std::string m_s;
};

MyClass buildMyClass(std::string const& s) {
	return MyClass(s);
}

class Owner {
public:
	explicit Owner(MyClass m) : m_myClass(m) {};

	MyClass getMyClass() const { return m_myClass; }

private:
	MyClass m_myClass;
};
)";

	auto pythonTestCode = fmt::format(R"(
phrase = "Hello from py"
myClass = {moduleName}.buildMyClass(phrase);

self.assertEqual(myClass.getS(), phrase)

owner = {moduleName}.Owner(myClass)
self.assertEqual(owner.getMyClass().getS(), phrase)
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode =
	    TestUtil::runPybindTest(stage, cppCode, pythonTestCode, moduleName);
	REQUIRE(errorCode == 0);
}
