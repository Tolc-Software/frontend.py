#pragma once

#include "PybindProxy/enum.hpp"
#include "PybindProxy/function.hpp"
#include <string>
#include <vector>

namespace PybindProxy {

struct Constructor {
	explicit Constructor(std::vector<std::string> const& arguments);

	std::vector<std::string> m_arguments;
};

class Class {
public:
	explicit Class(std::string const& name,
	               std::string const& fullyQualifiedName);

	void addEnum(Enum const& e);

	void addFunction(Function const& function);

	void addConstructor(Constructor const& constructor);

	void addMemberVariable(std::string const& variableName, bool isConst);

	std::string const& getName() const;

	void addInclude(std::string const& i);

	std::vector<std::string> const& getIncludes() const;

	std::string getPybind(std::string const& moduleName) const;

private:
	struct MemberVariable {
		// User defined name of the member variable
		std::string m_name;
		bool m_isConst;
	};

	// User defined name of the class
	std::string m_name;
	std::string m_fullyQualifiedName;

	std::vector<Constructor> m_constructors;
	std::vector<Function> m_functions;
	std::vector<MemberVariable> m_memberVariables;
	std::vector<Enum> m_enums;

	std::vector<std::string> m_includes;
};
}    // namespace PybindProxy
