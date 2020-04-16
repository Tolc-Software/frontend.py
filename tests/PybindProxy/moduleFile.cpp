#include "PybindProxy/moduleFile.hpp"
#include "PybindProxy/function.hpp"
#include "PybindProxy/module.hpp"
#include <catch2/catch.hpp>

namespace {
bool contains(std::string const& str, std::string const& substr) {
	return str.find(substr) != std::string::npos;
}
}    // namespace

TEST_CASE("ModuleFile can take a module", "[moduleFile]") {
	PybindProxy::Module m("myModule");
	PybindProxy::Function f("f", {});
	m.addFunction(f);
	PybindProxy::ModuleFile mf(m);

	auto pybindCode = mf.getPybind();
	CAPTURE(pybindCode);

	// The necessary include
	REQUIRE(contains(pybindCode, "#include <pybind11/pybind11.h>"));
	// The module
	REQUIRE(contains(pybindCode, "PYBIND11_MODULE(myModule, myModule)"));
	// The module contains the function
	REQUIRE(contains(pybindCode, R"(myModule.def("f", &f)"));
}
