#include "TestStage/paths.hpp"
#include "TestUtil/files.hpp"
#include "TestUtil/pybindStage.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Virtual inheritence", "[virtualInheritence]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);
	stage.keepAliveAfterTest();

	auto cppCode = R"(
#include <string>

class Animal {
public:
	virtual ~Animal() { }
	virtual std::string go(int n_times) = 0;
};

class Dog : public Animal {
public:
	std::string go(int n_times) override {
		std::string result;
		for (int i = 0; i < n_times; ++i) {
			result += "woof! ";
		}
		return result;
	}
};

std::string call_go(Animal *animal) {
	return animal->go(3);
}
)";

	auto pythonTestCode = R"(
fido = m.Dog("Fido")

# But has its new functions
// self.assertEqual(fido.bark(), "woof!")
)";

	auto errorCode = stage.runPybindTest(cppCode, pythonTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("Overriding virtual functions in python");
}
