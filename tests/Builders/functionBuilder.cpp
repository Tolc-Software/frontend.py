#include "Builders/functionBuilder.hpp"
#include <catch2/catch.hpp>

TEST_CASE("function returning containers adds the correct include", "[functionBuilder]") {
	IR::Function f;
	IR::Type t;
	IR::Type::Container c;
	c.m_container = IR::ContainerType::Vector;
	t.m_type = c;
	f.m_returnType = t;
	auto pyFunction = Builders::buildFunction(f);
	auto includes = pyFunction.getIncludes();
	REQUIRE(includes.size() == 1);
	for (auto const& include : includes) {
		REQUIRE(include == "<pybind11/stl.h>");
	}
}
