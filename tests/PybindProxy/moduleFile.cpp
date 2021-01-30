#include "PybindProxy/moduleFile.hpp"
#include "PybindProxy/function.hpp"
#include "PybindProxy/module.hpp"
#include "TestUtil/string.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("ModuleFile can take a module", "[moduleFile]") {
	std::string moduleName = "myModule";
	PybindProxy::Module m(moduleName);
	PybindProxy::Function f("f", "f");
	m.addFunction(f);
	PybindProxy::ModuleFile mf(m, moduleName);

	auto pybindCode = mf.getPybind();
	CAPTURE(pybindCode);

	// The necessary include
	REQUIRE(TestUtil::contains(pybindCode, "#include <pybind11/pybind11.h>"));
	// The module
	REQUIRE(TestUtil::contains(pybindCode,
	                           fmt::format("PYBIND11_MODULE({}, {})",
	                                       moduleName,
	                                       m.getVariableName())));
	// The module contains the function
	REQUIRE(TestUtil::contains(pybindCode,
	                           m.getVariableName() + R"(.def("f", &f)"));
}
