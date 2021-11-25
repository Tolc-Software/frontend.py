#include "Builders/functionBuilder.hpp"
#include <catch2/catch.hpp>

TEST_CASE("function returning containers adds the correct include", "[functionBuilder]") {
	IR::Function f;
	IR::Type t;
	IR::Type::Container c;
	c.m_container = IR::ContainerType::Vector;
	t.m_type = c;
	f.m_returnType = t;
	auto pyFunction = Builders::buildFunction(f).value();
	auto includes = pyFunction.getIncludes();
	REQUIRE(includes.size() == 1);
	for (auto const& include : includes) {
		REQUIRE(include == "<pybind11/stl.h>");
	}
}

TEST_CASE("function fails on unique_ptr argument", "[functionBuilder]") {
	IR::Function f;
	f.m_representation = "MyNamespace::f";
	IR::Type t;
	IR::Type::Container c;
	c.m_container = IR::ContainerType::UniquePtr;
	t.m_type = c;
	IR::Variable v;
	v.m_type = t;
	v.m_name = "v";
	f.m_arguments.push_back(v);
	auto pyFunction = Builders::buildFunction(f);
	REQUIRE(!pyFunction.has_value());
}
