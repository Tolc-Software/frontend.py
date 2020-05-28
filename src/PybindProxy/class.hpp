#pragma once

#include "PybindProxy/enum.hpp"
#include "PybindProxy/function.hpp"
#include <string>
#include <vector>

namespace PybindProxy {

struct Constructor {
	explicit Constructor(std::vector<std::string> const& arguments)
	    : m_arguments(arguments) {};

	std::vector<std::string> m_arguments;
};

class Class {
public:
	explicit Class(std::string const& name,
	               std::string const& fullyQualifiedName)
	    : m_name(name), m_fullyQualifiedName(fullyQualifiedName),
	      m_constructors(), m_functions(), m_memberVariables(), m_enums(),
	      m_includes() {}

	void addEnum(Enum const& e) {
		m_enums.push_back(e);
	}

	void addFunction(Function const& function) {
		m_functions.push_back(function);
	}

	void addConstructor(Constructor const& constructor) {
		m_constructors.push_back(constructor);
	}

	void addMemberVariable(std::string const& variableName, bool isConst) {
		m_memberVariables.push_back({variableName, isConst});
	}

	std::string const& getName() const {
		return m_name;
	}

	void addInclude(std::string const& i) {
		m_includes.push_back(i);
	}

	std::vector<std::string> const& getIncludes() const {
		return m_includes;
	}

	std::string getPybind(std::string const& moduleName) const;

private:
	struct MemberVariable {
		std::string m_name;
		bool m_isConst;
	};

	std::string m_name;
	std::string m_fullyQualifiedName;

	std::vector<Constructor> m_constructors;
	std::vector<Function> m_functions;
	std::vector<MemberVariable> m_memberVariables;
	std::vector<Enum> m_enums;

	std::vector<std::string> m_includes;
};
}    // namespace PybindProxy
