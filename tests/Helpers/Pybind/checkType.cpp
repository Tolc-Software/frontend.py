#include "Helpers/Pybind/checkType.hpp"
#include "PybindProxy/typeInfo.hpp"
#include <IR/ir.hpp>
#include <catch2/catch.hpp>

namespace {

IR::Type getType(std::string const& representation) {
	IR::Type t;
	t.m_isConst = false;
	t.m_isReference = false;
	t.m_numPointers = 0;
	t.m_representation = representation;
	return t;
}

IR::Type constructVector() {
	auto t = getType("std::vector<>");
	IR::Type::Container c;
	c.m_container = IR::ContainerType::Vector;
	t.m_type = c;
	return t;
}

IR::Type constructFunction() {
	auto t = getType("std::function<void()>");
	IR::Type::Function f;
	f.m_representation = t.m_representation;
	t.m_type = f;
	return t;
}

IR::Type constructComplex() {
	auto t = getType("std::complex<int>");
	IR::Type::Value v;
	v.m_base = IR::BaseType::Complex;
	t.m_type = v;
	return t;
}

IR::Type constructFilesystemPath() {
	auto t = getType("std::filesystem::path");
	IR::Type::Value v;
	v.m_base = IR::BaseType::FilesystemPath;
	t.m_type = v;
	return t;
}

IR::Type insertIntoContainer(IR::Type container, IR::Type contained) {
	if (auto c = std::get_if<IR::Type::Container>(&container.m_type)) {
		c->m_containedTypes.push_back(contained);
	}
	return container;
}
}    // namespace

TEST_CASE("Can find the include for function types within conatiners",
          "[checkType]") {
	auto vectorOfFunction =
	    insertIntoContainer(constructVector(), constructFunction());
	PybindProxy::TypeInfo typeInfo;
	Helpers::Pybind::checkType(vectorOfFunction, typeInfo);
	REQUIRE(typeInfo.m_includes.size() == 2);
}

TEST_CASE("Can find the include for container types", "[checkType]") {
	auto c = constructVector();
	PybindProxy::TypeInfo typeInfo;
	Helpers::Pybind::checkType(c, typeInfo);
	REQUIRE(typeInfo.m_includes.size() == 1);
	for (auto const& include : typeInfo.m_includes) {
		REQUIRE(include == "<pybind11/stl.h>");
	}
}

TEST_CASE("Can find the include for function types", "[checkType]") {
	auto f = constructFunction();
	PybindProxy::TypeInfo typeInfo;
	Helpers::Pybind::checkType(f, typeInfo);
	REQUIRE(typeInfo.m_includes.size() == 1);
	for (auto const& include : typeInfo.m_includes) {
		REQUIRE(include == "<pybind11/functional.h>");
	}
}

TEST_CASE("Can find the include for complex types", "[checkType]") {
	auto c = constructComplex();
	PybindProxy::TypeInfo typeInfo;
	Helpers::Pybind::checkType(c, typeInfo);
	REQUIRE(typeInfo.m_includes.size() == 1);
	for (auto const& include : typeInfo.m_includes) {
		REQUIRE(include == "<pybind11/complex.h>");
	}
}

TEST_CASE("Can find the include for path types", "[checkType]") {
	auto p = constructFilesystemPath();
	PybindProxy::TypeInfo typeInfo;
	Helpers::Pybind::checkType(p, typeInfo);
	REQUIRE(typeInfo.m_includes.size() == 1);
	for (auto const& include : typeInfo.m_includes) {
		REQUIRE(include == "<pybind11/stl/filesystem.h>");
	}
}
