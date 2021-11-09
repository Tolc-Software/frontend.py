#include "Helpers/Pybind/extractIncludes.hpp"
#include <catch2/catch.hpp>
#include <IR/ir.hpp>

namespace {
IR::Type constructVector() {
	IR::Type t;
	t.m_isConst = false;
	t.m_isReference = false;
	t.m_numPointers = 0;
	t.m_representation = "std::vector<>";
	IR::Type::Container c;
	c.m_container = IR::ContainerType::Vector;
	t.m_type = c;
	return t;
}

IR::Type constructFunction() {
	IR::Type t;
	t.m_isConst = false;
	t.m_isReference = false;
	t.m_numPointers = 0;
	t.m_representation = "std::function<void()>";
	IR::Type::Function f;
	f.m_representation = t.m_representation;
	t.m_type = f;
	return t;
}

IR::Type insertIntoContainer(IR::Type container, IR::Type contained) {
	if (auto c = std::get_if<IR::Type::Container>(&container.m_type)) {
		c->m_containedTypes.push_back(contained);
	}
	return container;
}
}

TEST_CASE("Can find the include for function types within conatiners",
          "[extractIncludes]") {
	auto vectorOfFunction =
	    insertIntoContainer(constructVector(), constructFunction());
	auto includes = Helpers::Pybind::extractIncludes(vectorOfFunction);
	REQUIRE(includes.size() == 2);
}

TEST_CASE("Can find the include for container types", "[extractIncludes]") {
	auto c = constructVector();
	auto includes = Helpers::Pybind::extractIncludes(c);
	REQUIRE(includes.size() == 1);
	for (auto const& include : includes) {
		REQUIRE(include == "<pybind11/stl.h>");
	}
}

TEST_CASE("Can find the include for function types", "[extractIncludes]") {
	auto f = constructFunction();
	auto includes = Helpers::Pybind::extractIncludes(f);
	REQUIRE(includes.size() == 1);
	for (auto const& include : includes) {
		REQUIRE(include == "<pybind11/functional.h>");
	}
}
