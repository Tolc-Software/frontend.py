#include "PybindProxy/function.hpp"
#include <fmt/format.h>
#include <string>

namespace PybindProxy {

std::string Function::getPybind() const {
	return fmt::format(R"(def("{}", &{}, ""))", m_name, m_name);
}
}
