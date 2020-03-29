#include <IR/ir.hpp>
#include <catch2/catch.hpp>

TEST_CASE("Function works with default modifier", "[functions]") {
	// using IR::AccessModifier;
	// for (auto [accessModifier, structure] :
	//      {std::make_pair(AccessModifier::Private, std::string("class")),
	//       std::make_pair(AccessModifier::Public, std::string("struct"))}) {
	// 	auto globalNS =
	// 	    TestUtil::parseString(structure + " MyStructure { void fun(); };");

	// 	SECTION("Parser finds the function") {
	// 		REQUIRE(globalNS.m_functions.size() == 0);
	// 		REQUIRE(globalNS.m_structs.size() == 1);
	// 		auto structure = globalNS.m_structs[0];
	// 		REQUIRE(structure.m_functions.size() == 1);
	// 		auto& [access, fun] = structure.m_functions.back();
	// 		CHECK(fun.m_name == "fun");
	// 		CHECK(access == accessModifier);
	// 	}
	// }
}
