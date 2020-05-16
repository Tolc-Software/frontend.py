#include "Helpers/Pybind/extractIncludes.hpp"
#include <catch2/catch.hpp>
#include <IR/ir.hpp>

namespace {
IR::Type constructContainer(IR::ContainerType container) {
	IR::Type t;
	t.m_isConst = false;
	t.m_isReference = false;
	t.m_numPointers = 0;
	t.m_representation = "";
	IR::Type::Container c;
	c.m_container = container;
	t.m_type = c;
	return t;
}
}

TEST_CASE("Can find the include for container types", "[extractIncludes]") {
	auto c = constructContainer(IR::ContainerType::Vector);
	auto includes = Helpers::Pybind::extractIncludes(c);
	REQUIRE(includes.size() == 1);
	for (auto const& include : includes) {
		REQUIRE(include == "<pybind11/stl.h>");
	}
}
