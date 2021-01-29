#include "PybindProxy/moduleFile.hpp"
#include "PybindProxy/function.hpp"
#include "PybindProxy/module.hpp"
#include "TestUtil/string.hpp"
#include <catch2/catch.hpp>

TEST_CASE("ModuleFile can take a module", "[moduleFile]") {
	PybindProxy::Module m("myModule", "myModule");
	PybindProxy::Function f("f", "f");
	m.addFunction(f);
	PybindProxy::ModuleFile mf(m);

	auto pybindCode = mf.getPybind();
	CAPTURE(pybindCode);

	// The necessary include
	REQUIRE(TestUtil::contains(pybindCode, "#include <pybind11/pybind11.h>"));
	// The module
	REQUIRE(
	    TestUtil::contains(pybindCode, "PYBIND11_MODULE(myModule, myModule)"));
	// The module contains the function
	REQUIRE(TestUtil::contains(pybindCode, R"(myModule.def("f", &f)"));
}
