#pragma once

#include <set>
#include <string>

namespace Pybind::Proxy {

struct TypeInfo {
	// E.g. vector conversion requires inclusion of <pybind11/stl.h>
	// E.g: '<string>' for '#include <string>'
	// NOTE: Use <> or "" explicitly
	std::set<std::string> m_includes;

	// If a class is managed by a sharded_ptr, its fully qualified name will be here
	// A class will be marked shared if it is handled by a sharded_ptr
	std::set<std::string> m_classesMarkedShared;
};

}    // namespace Pybind::Proxy
