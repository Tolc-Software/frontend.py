#include "Frontend/Python/frontend.hpp"
#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"
#include "TestUtil/runPybindTest.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Testing enums", "[enums]") {
	std::string moduleName = "defaultModule";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
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

)";

	auto pythonTestCode = fmt::format(R"(
scoped = {moduleName}.Scoped.D
enumTest = {moduleName}.EnumTest(scoped)
self.assertEqual(enumTest.e, scoped)

unscoped = {moduleName}.Unscoped.B
u = {moduleName}.f(unscoped)
self.assertEqual(u, unscoped)

deep = {moduleName}.NS.Deep.I
self.assertNotEqual(deep, {moduleName}.NS.Deep.H)
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode =
	    TestUtil::runPybindTest(stage, cppCode, pythonTestCode, moduleName);
	REQUIRE(errorCode == 0);
}
